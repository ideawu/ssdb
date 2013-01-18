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
	print '* ssdb-cli commands *';
	print '    set a 1';
	print '    get a';
	print '    scan key_start key_end limit';
	print '        scan a z 10';
	print '    zset n a 1';
	print '    zget n a';
	print '    zscan setname key score_start score_end limit';
	print '        zscan n a 1 3 10';
	print '';
}

function usage(){
	print '';
	print 'Usage:';
	print '    ssdb-cli [-h HOST -p PORT]';
	print '';
	print 'Options:';
	print '    -h 127.0.0.1';
	print '        ssdb server hostname/ip address';
	print '    -p 8888';
	print '        ssdb server port';
	print '';
	print 'Examples:';
	print '    ssdb-cli';
	print '    ssdb-cli -p 8888';
	print '    ssdb-cli -h 127.0.0.1 -p 8888';
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
	return ret;
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

	retry = 0;
	max_retry = 5;
	while(true){
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

	if(!resp.ok()){
		print 'error: ' + resp.code;
	}else{
		switch(cmd){
			case 'get':
			case 'zget':
			case 'hget':
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
				print repr_data(resp.data);
				break;
			case 'set':
			case 'zset':
			case 'hset':
			case 'del':
			case 'zdel':
			case 'hdel':
				print resp.code;
				break;
			case 'scan':
			case 'rscan':
			case 'hscan':
			case 'hrscan':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 22);
				foreach(resp.data['index'] as k){
					printf('  %-15s : %s\n', repr_data(k), repr_data(resp.data['items'][k]));
				}
				printf('%d result(s)\n', len(resp.data['index']));
				break;
			case 'zscan':
			case 'zrscan':
				printf('%-15s %s\n', 'key', 'score');
				print ('-' * 22);
				foreach(resp.data['index'] as k){
					score = resp.data['items'][k];
					printf('  %-15s: %s\n', repr_data(repr_data(k)), score);
				}
				printf('%d result(s)\n', len(resp.data['index']));
				break;
			case 'keys':
			case 'zkeys':
			case 'hkeys':
				printf('  %15s\n', 'key');
				print ('-' * 17);
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				printf('%d result(s)\n', len(resp.data));
				break;
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
				printf('%-15s %s\n', 'key', 'value');
				print ('-' * 22);
				foreach(resp.data as k=>v){
					printf('  %-15s : %s\n', repr_data(k), repr_data(v));
				}
				printf('%d result(s)\n', len(resp.data));
				break;
			default:
				print repr_data(resp.code), repr_data(resp.data);
				break;
		}
	}
}

