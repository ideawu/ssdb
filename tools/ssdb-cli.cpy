import thread, re, time, socket;
import getopt, shlex;

try{
	import readline;
}catch(Exception e){
}


function welcome(){
	print ('ssdb (cli) - ssdb command line tool.');
	print ('Copyright (c) 2012 ideawu.com');
	print ('');
	print "'h' or 'help' for help, 'q' to quit.";
	print ('');
}
welcome();

function show_command_help(){
	print '';
	print '# KEY-VALUE COMMANDS';
	print '	set key value';
	print '	get key';
	print '	del key';
	print '	list key_start key_end limit';
	print '	keys key_start key_end limit';
	print '	scan key_start key_end limit';
	print '# MAP(HASHMAP) COMMANDS';
	print '	hset name key value';
	print '	hget name key';
	print '	hdel name key';
	print '	hlist name_start name_end limit';
	print '	hkeys name key_start key_end limit';
	print '	hscan name key_start key_end limit';
	print '# ZSET(SORTED SET) COMMANDS';
	print '	zset name key score';
	print '	zget name key';
	print '	zdel name key';
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
	print '	ssdb-cli [-h HOST -p PORT]';
	print '';
	print 'Options:';
	print '	-h 127.0.0.1';
	print '		ssdb server hostname/ip address';
	print '	-p 8888';
	print '		ssdb server port';
	print '';
	print 'Examples:';
	print '	ssdb-cli';
	print '	ssdb-cli -p 8888';
	print '	ssdb-cli -h 127.0.0.1 -p 8888';
}

function repr_data(str){
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

function flushdb(link, data_type){
	printf('\n');
	printf('					DANGEROUS!\n');
	printf('\n');
	printf('This operation is DANGEROUS and is not recoverable, if you\n');
	printf('realy want to flush the whole db(delete ALL data in ssdb server),\n');
	printf('input \'yes\' and press Enter, or just press Enter to cancel\n');
	printf('\n');
	printf('flushdb? ');
	
	line = sys.stdin.readline().strip();
	if(line != 'yes'){
		printf('Operation cancelled.\n\n');
		return;
	}

	print 'Begin to flushdb...';
	
	batch = 1000;
	
	d_kv = 0;
	if(data_type == '' || data_type == 'kv'){
		while(true){
			resp = link.request('keys', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			d_kv += len(resp.data);
			foreach(resp.data as key){
				link.request('del', [key]);
			}
			printf('delete[kv] %d key(s).\n', d_kv);
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
			foreach(resp.data as hname){
				d_hash += 1;
				while(true){
					r2 = link.request('hkeys', [hname, '', '', batch]);
					if(len(r2.data) == 0){
						break;
					}
					foreach(r2.data as key){
						d_hkeys += 1;
						link.request('hdel', [hname, key]);
					}
					printf('delete[hash] %d hash(s), %d key(s).\n', d_hash, d_hkeys);
				}
			}
			printf('delete[hash] %d hash(s), %d key(s).\n', d_hash, d_hkeys);
		}
	}
	
	d_zset = 0;
	d_zkeys = 0;
	if(data_type == '' || data_type == 'zset'){
		while(true){
			resp = link.request('zlist', ['', '', batch]);
			if(len(resp.data) == 0){
				break;
			}
			foreach(resp.data as zname){
				d_zset += 1;
				while(true){
					r2 = link.request('zkeys', [zname, '', '', '', batch]);
					if(len(r2.data) == 0){
						break;
					}
					foreach(r2.data as key){
						d_zkeys += 1;
						link.request('zdel', [zname, key]);
					}
					printf('delete[zset] %d zset(s), %d key(s).\n', d_zset, d_zkeys);
				}
			}
			printf('delete[zset] %d zset(s), %d key(s).\n', d_zset, d_zkeys);
		}
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
	printf('\n');
}



default_opts = {
	'-h' : '127.0.0.1',
	'-p' : '8888',
};

opt_err = false;
try{
	opts, args = getopt.getopt(sys.argv[1 .. ], 'h:p:');
	opts = dict(opts);
}catch(getopt.GetoptError e){
	opts = {};
	opt_err = true;
}
foreach(default_opts as k=>v){
	if(!opts.has_key(k)){
		opts[k] = v;
	}
}

if(opt_err){
	usage();
	sys.exit(0);
}


host = opts['-h'];
port = int(opts['-p']);

sys.path.append('./api/python');
sys.path.append('../api/python');
import SSDB.SSDB;

try{
	link = new SSDB(host, port);
}catch(socket.error e){
	print 'Connection error: ', str(e);
	sys.exit(0);
}


while(true){
	line = '';
	c = sprintf('ssdb %s:%s> ', host, str(port));
	line = raw_input(c);
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
	cmd = ps[0];
	args = ps[1 .. ];
	
	if(cmd == 'flushdb'){
		if(len(args) == 0){
			flushdb(link, '');
		}else{
			flushdb(link, args[0]);
		}
		continue;
	}

	retry = 0;
	max_retry = 5;
	import datetime;
	stime = datetime.datetime.now();
	while(true){
		stime = datetime.datetime.now();
		resp = link.request(cmd, args);
		etime = datetime.datetime.now();
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

	ts = etime - stime;
	time_consume = ts.seconds + ts.microseconds/1000000.;
	if(!resp.ok()){
		print 'error: ' + resp.code;
		printf('(%.3f sec)\n', time_consume);
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
				printf('(%.3f sec)\n', time_consume);
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
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
				break;
			case 'get':
			case 'zget':
			case 'hget':
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
			case 'hsize':
			case 'zsize':
			case 'multi_del':
			case 'multi_hdel':
			case 'multi_zdel':
				print repr_data(resp.data);
				printf('(%.3f sec)\n', time_consume);
				break;
			case 'set':
			case 'zset':
			case 'hset':
			case 'del':
			case 'zdel':
			case 'hdel':
				print resp.code;
				printf('(%.3f sec)\n', time_consume);
				break;
			case 'scan':
			case 'rscan':
			case 'hscan':
			case 'hrscan':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 25);
				foreach(resp.data['index'] as k){
					printf('  %-15s : %s\n', repr_data(k), repr_data(resp.data['items'][k]));
				}
				printf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume);
				break;
			case 'zscan':
			case 'zrscan':
				printf('%-15s %s\n', 'key', 'score');
				print ('-' * 25);
				foreach(resp.data['index'] as k){
					score = resp.data['items'][k];
					printf('  %-15s: %s\n', repr_data(repr_data(k)), score);
				}
				printf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume);
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
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
				break;
			case 'hlist':
			case 'zlist':
				printf('  %15s\n', 'name');
				print ('-' * 17);
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
				break;
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 25);
				foreach(resp.data as k=>v){
					printf('  %-15s : %s\n', repr_data(k), repr_data(v));
				}
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
				break;
			case 'info':
				is_val = false;
				for(i=1; i<len(resp.data); i++){
					s = resp.data[i];
					if(is_val){
						s = '	' + s.replace('\n', '\n	');
					}
					print s;
					is_val = !is_val;
				}
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
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
				printf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume);
				break;
			default:
				print repr_data(resp.code), repr_data(resp.data);
				printf('(%.3f sec)\n', time_consume);
				break;
		}
	}
}

