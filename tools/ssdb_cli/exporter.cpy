import util.*;

fp = null;
progress = 0;
read_size = 0;
total_size = 0;

function write_line(params){
	gs = globals();
	foreach(params as k=>v){
		params[k] = str(v).encode('string-escape');
	}
	line = str('\t').join(params) + '\n';
	gs['read_size'] += len(line);
	gs['fp'].write(line);
}

function show_progress(){
	gs = globals();
	progress = gs['progress'];
	read_size = gs['read_size'];
	total_size = gs['total_size'];

	progress_2 = int(float(read_size)/total_size * 100);
	if(progress_2 - progress >= 5 || read_size == total_size){
		gs['progress'] = progress_2;
		printf("%2d%%\n", progress_2);
	}
}

function my_readline(c){
	if(c == null){
		c = '';
	}
	try{
		return raw_input(c);
	}catch(Exception e){
	}
	return '';
}

function run(link, args){
	gs = globals();

	kstart = '';
	kend = '';
	hstart = '';
	hend = '';
	zstart = '';
	zend = '';
	qstart = '';
	qend = '';

	output_file = false;
	interactive = false;
	foreach(args as arg){
		if(arg == '-i'){
			interactive = true;
		}else{
			output_file = arg;
		}
	}
	if(output_file == false){
		sys.stderr.write('Usage: export [-i] out_file\n');
		return;
	}
	if(os.path.exists(output_file)){
		print 'Error: ' + output_file + ' already exists!';
		return;
	}

	if(interactive){
		printf("input KV range[start, end]: \n");
		kstart = my_readline('  start(inclusive, default none): ');
		kend   = my_readline('    end(inclusive, default none): ');
		printf("input HASH range: \n");
		hstart = my_readline('  start(inclusive, default none): ');
		hend   = my_readline('    end(inclusive, default none): ');
		printf("input ZSET range: \n");
		zstart = my_readline('  start(inclusive, default none): ');
		zend   = my_readline('    end(inclusive, default none): ');
		printf("input QUEUE range: \n");
		qstart = my_readline('  start(inclusive, default none): ');
		qend   = my_readline('    end(inclusive, default none): ');
	}
	
	gs['fp'] = open(output_file, 'w');
	
	gs = globals();
	gs['total_size'] = dbsize(link);

	if(gs['total_size'] <= 0){
		gs['total_size'] = 1;
	}
	gs['total_size'] *= 1024 * 1024;

	// KV
	ls = new SSDB_kv_scan(link);
	ls.set_range(kstart, kend);
	// by default, ssdb's iterator is start-exclusive,
	r = link.request('get', [ls.key]);
	if(r.ok()){
		write_line(['set', ls.key, r.data]);
	}
	while(ls.next()){
		show_progress();
		write_line(['set', ls.key, ls.val]);
	}

	// HASH
	ls = new SSDB_hash_list(link);
	ls.set_range(hstart, hend);
	scan = new SSDB_hash_scan(link);
	scan.name = ls.key;
	while(scan.next()){
		show_progress();
		write_line(['hset', ls.key, scan.key, scan.val]);
	}
	while(ls.next()){
		scan = new SSDB_hash_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(['hset', ls.key, scan.key, scan.val]);
		}
	}

	// ZSET
	ls = new SSDB_zset_list(link);
	ls.set_range(zstart, zend);
	scan = new SSDB_zset_scan(link);
	scan.name = ls.key;
	while(scan.next()){
		show_progress();
		write_line(['zset', ls.key, scan.key, scan.val]);
	}
	while(ls.next()){
		scan = new SSDB_zset_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(['zset', ls.key, scan.key, scan.val]);
		}
	}

	// QUEUE
	ls = new SSDB_queue_list(link);
	ls.set_range(qstart, qend);
	scan = new SSDB_queue_scan(link);
	scan.name = ls.key;
	while(scan.next()){
		show_progress();
		write_line(['qpush', ls.key, scan.val]);
	}
	while(ls.next()){
		scan = new SSDB_queue_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(['qpush', ls.key, scan.val]);
		}
	}
	
	if(gs['fp']){
		gs['fp'].close();
	}

	gs['read_size'] = gs['total_size'];
	show_progress();
	print 'done.';
}
