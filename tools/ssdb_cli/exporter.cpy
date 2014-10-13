import util.*;

progress = 0;
read_size = 0;
total_size = 0;

function write_line(fp, params){
	gs = globals();
	foreach(params as k=>v){
		params[k] = str(v).encode('string-escape');
	}
	line = str('\t').join(params) + '\n';
	gs['read_size'] += len(line);
	fp.write(line);
}

function show_progress(){
	gs = globals();
	progress = gs['progress'];
	read_size = gs['read_size'];
	total_size = gs['total_size'];

	progress_2 = int(float(read_size)/total_size * 100);
	if(progress_2 - progress >= 5 || read_size == total_size){
		gs['progress'] = progress_2;
		printf("%02d%%\n", progress_2);
	}
}


function run(link, output_file){
	if(os.path.exists(output_file)){
		print 'Error: ' + output_file + ' already exists!';
		return;
	}
	fp = open(output_file, 'w');
	
	gs = globals();
	gs['total_size'] = dbsize(link);

	if(gs['total_size'] <= 0){
		gs['total_size'] = 1;
	}
	gs['total_size'] *= 1024 * 1024;

	// KV
	scan = new SSDB_kv_scan(link);
	while(scan.next()){
		show_progress();
		write_line(fp, ['set', scan.key, scan.val]);
	}

	// HASH
	ls = new SSDB_hash_list(link);
	while(ls.next()){
		scan = new SSDB_hash_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(fp, ['hset', ls.key, scan.key, scan.val]);
		}
	}

	// ZSET
	ls = new SSDB_zset_list(link);
	while(ls.next()){
		scan = new SSDB_zset_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(fp, ['zset', ls.key, scan.key, scan.val]);
		}
	}

	ls = new SSDB_queue_list(link);
	while(ls.next()){
		scan = new SSDB_queue_scan(link);
		scan.name = ls.key;
		while(scan.next()){
			show_progress();
			write_line(fp, ['qpush_back', ls.key, scan.key]);
		}
	}

	read_size = total_size;
	show_progress();
}
