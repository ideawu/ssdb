import thread, re, time, socket;
import getopt, shlex;
import datetime;

try{
	import readline;
}catch(Exception e){
}

escape_data = false;

function welcome(){
	sys.stderr.write('ssdb (cli) - ssdb command line tool.\n');
	sys.stderr.write('Copyright (c) 2012-2014 ssdb.io\n');
	sys.stderr.write('\n');
	sys.stderr.write("'h' or 'help' for help, 'q' to quit.\n");
	sys.stderr.write('\n');
}

function show_command_help(){
	print '';
	print '# Display ssdb-server status';
	print '	info';
	print '# Escape/Do not escape response data';
	print '	: escape yes|no';
	print '# KEY-VALUE COMMANDS';
	print '	set key value';
	print '	setx key value ttl';
	print '	get key';
	print '	del key';
	print '	list key_start key_end limit';
	print '	keys key_start key_end limit';
	print '	scan key_start key_end limit';
	print '# MAP(HASHMAP) COMMANDS';
	print '	hset name key value';
	print '	hget name key';
	print '	hdel name key';
	print '	hclear name';
	print '	hlist name_start name_end limit';
	print '	hkeys name key_start key_end limit';
	print '	hscan name key_start key_end limit';
	print '# ZSET(SORTED SET) COMMANDS';
	print '	zset name key score';
	print '	zget name key';
	print '	zdel name key';
	print '	zclear name';
	print '	zlist name_start name_end limit';
	print '	zkeys name key_start score_start score_end limit';
	print '	zscan name key_start score_start score_end limit';
	print '# FLUSH DATABASE';
	print '	flushdb';
	print '	flushdb kv';
	print '	flushdb hash';
	print '	flushdb zset';
	print '';
	print '# EXAMPLES';
	print '	scan "" "" 10';
	print '	scan aa "" 10';
	print '	hlist "" "" 10';
	print '	hscan h "" "" 10';
	print '	hscan h aa "" 10';
	print '	zlist "" "" 10';
	print '	zscan z "" "" "" 10';
	print '	zscan z "" 1 100 10';
	print '';
	print 'press \'q\' and Enter to quit.';
	print '';
}

function usage(){
	print '';
	print 'Usage:';
	print '	ssdb-cli [-h] [HOST] [-p] [PORT]';
	print '';
	print 'Options:';
	print '	-h 127.0.0.1';
	print '		ssdb server hostname/ip address';
	print '	-p 8888';
	print '		ssdb server port';
	print '';
	print 'Examples:';
	print '	ssdb-cli';
	print '	ssdb-cli 8888';
	print '	ssdb-cli 127.0.0.1 8888';
	print '	ssdb-cli -h 127.0.0.1 -p 8888';
}

function repr_data(str){
	gs = globals();
	if(gs['escape_data'] == false){
		return str;
	}
	ret = repr(str);
	if(len(ret) > 0){
		if(ret[0] == '\''){
			ret = ret.replace("\\'", "'");
			ret = ret[1 .. -1];
		}else if(ret[0] == '"'){
			ret = ret.replace('\\"', '"');
			ret = ret[1 .. -1];
		}else{
		}
	}
	ret = ret.replace("\\\\", "\\");
	return ret;
}

function hclear(link, hname, verbose=true){
	ret = 0;
	num = 0;
	batch = 1000;
	last_count = 0;
	
	r = link.request('hclear', [hname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}

function zclear(link, zname, verbose=true){
	ret = 0;
	num = 0;
	batch = 1000;
	last_count = 0;

	r = link.request('zclear', [zname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}

function qclear(link, zname, verbose=true){
	ret = 0;
	num = 0;
	batch = 1000;
	last_count = 0;

	r = link.request('qclear', [zname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}

function flushdb(link, data_type){
	printf('\n');
	printf('============================ DANGER! ============================\n');
	printf('This operation is DANGEROUS and is not recoverable, if you\n');
	printf('really want to flush the whole db(delete ALL data in ssdb server),\n');
	printf('input \'yes\' and press Enter, or just press Enter to cancel\n');
	printf('=================================================================\n');
	printf('\n');
	printf('> flushdb? ');
	
	line = sys.stdin.readline().strip();
	if(line != 'yes'){
		printf('Operation cancelled.\n\n');
		return;
	}

	print 'Begin to flushdb...\n';
	
	batch = 1000;
	
	d_kv = 0;
	if(data_type == '' || data_type == 'kv'){
		while(true){
			resp = link.request('keys', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			d_kv += len(resp.data);
			link.request('multi_del', resp.data);
			printf('delete[kv  ] %d key(s).\n', d_kv);
		}
	}
	
	d_hash = 0;
	d_hkeys = 0;
	if(data_type == '' || data_type == 'hash'){
		while(true){
			resp = link.request('hlist', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			last_num = 0;
			foreach(resp.data as hname){
				d_hash += 1;
				deleted_num = hclear(link, hname, false);
				d_hkeys += deleted_num;
				if(d_hkeys - last_num >= batch){
					last_num = d_hkeys;
					printf('delete[hash] %d hash(s), %d key(s).\n', d_hash, d_hkeys);
				}
			}
			if(d_hkeys - last_num >= batch){
				printf('delete[hash] %d hash(s), %d key(s).\n', d_hash, d_hkeys);
			}
		}
		printf('delete[hash] %d hash(s), %d key(s).\n', d_hash, d_hkeys);
	}
	
	d_zset = 0;
	d_zkeys = 0;
	if(data_type == '' || data_type == 'zset'){
		while(true){
			resp = link.request('zlist', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			last_num = 0;
			foreach(resp.data as zname){
				d_zset += 1;
				deleted_num = zclear(link, zname, false);
				d_zkeys += deleted_num;
				if(d_zkeys - last_num >= batch){
					last_num = d_zkeys;
					printf('delete[zset] %d zset(s), %d key(s).\n', d_zset, d_zkeys);
				}
			}
			if(d_zkeys - last_num >= batch){
				printf('delete[zset] %d zset(s), %d key(s).\n', d_zset, d_zkeys);
			}
		}
		printf('delete[zset] %d zset(s), %d key(s).\n', d_zset, d_zkeys);
	}
	
	d_list = 0;
	d_lkeys = 0;
	if(data_type == '' || data_type == 'list'){
		while(true){
			resp = link.request('qlist', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			last_num = 0;
			foreach(resp.data as zname){
				d_list += 1;
				deleted_num = qclear(link, zname, false);
				d_lkeys += deleted_num;
				if(d_zkeys - last_num >= batch){
					last_num = d_lkeys;
					printf('delete[list] %d list(s), %d key(s).\n', d_list, d_lkeys);
				}
			}
			if(d_lkeys - last_num >= batch){
				printf('delete[list] %d list(s), %d key(s).\n', d_list, d_lkeys);
			}
		}
		printf('delete[list] %d list(s), %d key(s).\n', d_list, d_lkeys);
	}

	printf('\n');
	printf('===== flushdb stats =====\n');
	if(data_type == '' || data_type == 'kv'){
		printf('[kv]   %8d key(s).\n', d_kv);
	}
	if(data_type == '' || data_type == 'hash'){
		printf('[hash] %8d hash(s), %8d key(s).\n', d_hash, d_hkeys);
	}
	if(data_type == '' || data_type == 'zset'){
		printf('[zset] %8d zset(s), %8d key(s).\n', d_zset, d_zkeys);
	}
	if(data_type == '' || data_type == 'list'){
		printf('[list] %8d list(s), %8d key(s).\n', d_list, d_lkeys);
	}
	printf('\n');
	
	printf('clear binlog\n');
	link.request('clear_binlog');
	printf('\n');

	printf('compacting...\n');
	link.request('compact');
	printf('done.\n');
	printf('\n');
}

function timespan(stime){
	etime = datetime.datetime.now();
	ts = etime - stime;
	time_consume = ts.seconds + ts.microseconds/1000000.;
	return time_consume;
}


host = '';
port = '';
opt = '';
args = [];
foreach(sys.argv[1 ..] as arg){
	if(opt == '' && arg.startswith('-')){
		opt = arg;
	}else{
		switch(opt){
			case '-h':
				host = arg;
				opt = '';
				break;
			case '-p':
				port = arg;
				opt = '';
				break;
			default:
				args.append(arg);
				break;
		}
	}
}

if(host == ''){
	host = '127.0.0.1';
	foreach(args as arg){
		if(!re.match('^[0-9]+$', args[0])){
			host = arg;
			break;
		}
	}
}
if(port == ''){
	port = '8888';
	foreach(args as arg){
		if(re.match('^[0-9]+$', args[0])){
			port = arg;
			break;
		}
	}
}

try{
	port = int(port);
}catch(Exception e){
	print 'Invalid argument port: ', port;
	usage();
	sys.exit(0);
}

sys.path.append('./api/python');
sys.path.append('../api/python');
import SSDB.SSDB;

try{
	link = new SSDB(host, port);
}catch(socket.error e){
	printf('Failed to connect to: %s:%d\n', host, port);
	print 'Connection error: ', str(e);
	sys.exit(0);
}
welcome();

try{
	resp = link.request('info', []);
	print 'ssdb-server version: ' + resp.data[2] + '\n';
}catch(Exception e){
}

while(true){
	line = '';
	c = sprintf('ssdb %s:%s> ', host, str(port));
	b = sys.stdout;
	sys.stdout = sys.stderr;
	try{
		line = raw_input(c);
	}catch(Exception e){
		break;
	}
	sys.stdout = b;
	
	if(line == ''){
		continue;
	}
	line = line.strip();
	if(line == 'q' || line == 'quit'){
		print 'bye.';
		break;
	}
	if(line == 'h' || line == 'help'){
		show_command_help();
		continue;
	}

	try{
		ps = shlex.split(line);
	}catch(Exception e){
		print 'error: ', e;
		continue;
	}
	if(len(ps) == 0){
		continue;
	}
	cmd = ps[0].lower();
	args = ps[1 .. ];
	
	if(cmd == ':'){
		op = '';
		if(len(args) > 0){
			op = args[0];
		}
		if(op != 'escape'){
			print "Bad setting!";
			continue;
		}
		yn = 'yes';
		if(len(args) > 1){
			yn = args[1];
		}
		gs = globals();
		if(yn == 'yes'){
			gs['escape_data'] = true;
			print "  Escape response";
		}else if(yn == 'no' || yn == 'none'){
			gs['escape_data'] = false;
			print "  No escape response";
		}else{
			print "  Usage: escape yes|no";
		}
		continue;
	}
	
	try{
		if(cmd == 'flushdb'){
			stime = datetime.datetime.now();
			if(len(args) == 0){
				flushdb(link, '');
			}else{
				flushdb(link, args[0]);
			}
			sys.stderr.write(sprintf('(%.3f sec)\n', timespan(stime)));
			continue;
		}
	}catch(Exception e){
		sys.stderr.write("error! - " + str(e) + "\n");
		continue;
	}

	retry = 0;
	max_retry = 5;
	stime = datetime.datetime.now();
	while(true){
		stime = datetime.datetime.now();
		resp = link.request(cmd, args);
		if(resp.code == 'disconnected'){
			link.close();
			time.sleep(retry);
			retry ++;
			if(retry > max_retry){
				print 'cannot connect to server, give up...';
				break;
			}
			printf('[%d/%d] reconnecting to server... ', retry, max_retry);
			try{
				link = new SSDB(host, port);
				print 'done.';
			}catch(socket.error e){
				print 'Connect error: ', str(e);
				continue;
			}
			print '';
		}else{
			break;
		}
	}

	time_consume = timespan(stime);
	if(!resp.ok()){
		if(resp.not_found()){
			print 'not_found';
		}else{
			print 'error: ' + resp.code;
		}
		sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
	}else{
		switch(cmd){
			case 'exists':
			case 'hexists':
			case 'zexists':
				if(resp.data == true){
					printf('true\n');
				}else{
					printf('false\n');
				}
				sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
				break;
			case 'multi_exists':
			case 'multi_hexists':
			case 'multi_zexists':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 25);
				foreach(resp.data as k=>v){
					if(v == true){
						s = 'true';
					}else{
						s = 'false';
					}
					printf('  %-15s : %s\n', repr_data(k), s);
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'getbit':
			case 'setbit':
			case 'countbit':
			case 'strlen':
			case 'getset':
			case 'setnx':
			case 'get':
			case 'substr':
			case 'ttl':
			case 'expire':
			case 'zget':
			case 'hget':
			case 'qfront':
			case 'qback':
			case 'qget':
			case 'qpop':
			case 'qpop_front':
			case 'qpop_back':
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
			case 'hsize':
			case 'zsize':
			case 'qsize':
			case 'zrank':
			case 'zrrank':
			case 'zsum':
			case 'zcount':
			case 'zavg':
			case 'zremrangebyrank':
			case 'zremrangebyscore':
			case 'zavg':
			case 'multi_del':
			case 'multi_hdel':
			case 'multi_zdel':
			case 'hclear':
			case 'zclear':
			case 'qclear':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
				print repr_data(resp.data);
				sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
				break;
			case 'set':
			case 'setx':
			case 'zset':
			case 'hset':
			case 'del':
			case 'zdel':
			case 'hdel':
				print resp.code;
				sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
				break;
			case 'scan':
			case 'rscan':
			case 'hgetall':
			case 'hscan':
			case 'hrscan':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 25);
				foreach(resp.data['index'] as k){
					printf('  %-15s : %s\n', repr_data(k), repr_data(resp.data['items'][k]));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume));
				break;
			case 'zscan':
			case 'zrscan':
			case 'zrange':
			case 'zrrange':
				printf('%-15s %s\n', 'key', 'score');
				print ('-' * 25);
				foreach(resp.data['index'] as k){
					score = resp.data['items'][k];
					printf('  %-15s: %s\n', repr_data(repr_data(k)), score);
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume));
				break;
			case 'keys':
			case 'list':
			case 'zkeys':
			case 'hkeys':
				printf('  %15s\n', 'key');
				print ('-' * 17);
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'hvals':
				printf('  %15s\n', 'value');
				print ('-' * 17);
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'hlist':
			case 'hrlist':
			case 'zlist':
			case 'zrlist':
			case 'qlist':
			case 'qrlist':
			case 'qslice':
			case 'qrange':
				foreach(resp.data as k){
					printf('  %s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 25);
				foreach(resp.data as k=>v){
					printf('  %-15s : %s\n', repr_data(k), repr_data(v));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'info':
				is_val = false;
				for(i=1; i<len(resp.data); i++){
					s = resp.data[i];
					if(is_val){
						s = '    ' + s.replace('\n', '\n    ');
					}
					print s;
					is_val = !is_val;
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'key_range':
				if(len(resp.data) != 6){
					print 'error!';
				}else{
					for(i=0; i<len(resp.data); i++){
						resp.data[i] = repr_data(resp.data[i]);
						if(resp.data[i] == ''){
							resp.data[i] = '""';
						}
					}
					klen = 0;
					vlen = 0;
					for(i=0; i<len(resp.data); i+=2){
						klen = max(len(resp.data[i]), klen);
						vlen = max(len(resp.data[i+1]), vlen);
					}
					printf('	kv   :  %-*s  -  %-*s\n', klen, resp.data[0], vlen, resp.data[1]);
					printf('	hash :  %-*s  -  %-*s\n', klen, resp.data[2], vlen, resp.data[3]);
					printf('	zset :  %-*s  -  %-*s\n', klen, resp.data[4], vlen, resp.data[5]);
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			default:
				print repr_data(resp.code), repr_data(resp.data);
				sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
				break;
		}
	}
}

