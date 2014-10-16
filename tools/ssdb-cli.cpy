import thread, re, time, socket;
import getopt, shlex;
import datetime;
import ssdb_cli.*;

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
	print '# display ssdb-server status';
	print '    info';
	print '# escape/do not escape response data';
	print '    : escape yes|no';
	print '# export/import';
	print '    export [-i] out_file';
	print '        -i    interactive mode';
	print '    import in_file';
	print '';
	print 'see http://ssdb.io/docs/php/ for commands details';
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

function repr_data(s){
	gs = globals();
	if(gs['escape_data'] == false){
		return s;
	}
	ret = str(s).encode('string-escape');
	return ret;
}

function timespan(stime){
	etime = datetime.datetime.now();
	ts = etime - stime;
	time_consume = ts.seconds + ts.microseconds/1000000.;
	return time_consume;
}

function show_version(){
	try{
		resp = link.request('info', []);
		sys.stderr.write('server version: ' + resp.data[2] + '\n\n');
	}catch(Exception e){
	}
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
	sys.stderr.write(sprintf('Invalid argument port: ', port));
	usage();
	sys.exit(0);
}

sys.path.append('./api/python');
sys.path.append('../api/python');
sys.path.append('/usr/local/ssdb/api/python');
import SSDB.SSDB;

try{
	link = new SSDB(host, port);
}catch(socket.error e){
	sys.stderr.write(sprintf('Failed to connect to: %s:%d\n', host, port));
	sys.stderr.write(sprintf('Connection error: %s\n', str(e)));
	sys.exit(0);
}

welcome();
if(sys.stdin.isatty()){
	show_version();
}


password = false;

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
		sys.stderr.write('bye.\n');
		break;
	}
	if(line == 'h' || line == 'help'){
		show_command_help();
		continue;
	}

	try{
		ps = shlex.split(line);
	}catch(Exception e){
		sys.stderr.write(sprintf('error: %s\n', str(e)));
		continue;
	}
	if(len(ps) == 0){
		continue;
	}
	cmd = ps[0].lower();
	if(cmd.startswith(':')){
		ps[0] = cmd[1 ..];
		cmd = ':';
		args = ps;
	}else{
		args = ps[1 .. ];
	}
	if(cmd == ':'){
		op = '';
		if(len(args) > 0){
			op = args[0];
		}
		if(op != 'escape'){
			sys.stderr.write("Bad setting!\n");
			continue;
		}
		yn = 'yes';
		if(len(args) > 1){
			yn = args[1];
		}
		gs = globals();
		if(yn == 'yes'){
			gs['escape_data'] = true;
			sys.stderr.write("  Escape response\n");
		}else if(yn == 'no' || yn == 'none'){
			gs['escape_data'] = false;
			sys.stderr.write("  No escape response\n");
		}else{
			sys.stderr.write("  Usage: escape yes|no\n");
		}
		continue;
	}
	if(cmd == 'v'){
		show_version();
		continue;
	}
	if(cmd == 'auth'){
		if(len(args) == 0){
			sys.stderr.write('Usage: auth password\n');
		}
		password = args[0];
		continue;
	}
	if(cmd == 'export'){
		exporter.run(link, args);
		continue;
	}
	if(cmd == 'import'){
		if(len(args) < 1){
			sys.stderr.write('Usage: import in_file\n');
			continue;
		}
		filename = args[0];
		importer.run(link, filename);
		continue;
	}
	
	try{
		if(cmd == 'flushdb'){
			stime = datetime.datetime.now();
			if(len(args) == 0){
				flushdb.flushdb(link, '');
			}else{
				flushdb.flushdb(link, args[0]);
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
				sys.stderr.write('cannot connect to server, give up...\n');
				break;
			}
			sys.stderr.write(sprintf('[%d/%d] reconnecting to server... ', retry, max_retry));
			try{
				link = new SSDB(host, port);
				sys.stderr.write('done.\n');
			}catch(socket.error e){
				sys.stderr.write(sprintf('Connect error: %s\n', str(e)));
				continue;
			}
			sys.stderr.write('\n');
			if(password){
				ret = link.request('auth', [password]);
			}
		}else{
			break;
		}
	}

	time_consume = timespan(stime);
	if(!resp.ok()){
		if(resp.not_found()){
			sys.stderr.write('not_found\n');
		}else{
			s = resp.code;
			if(resp.message){
				s += ': ' + resp.message;
			}
			sys.stderr.write(str(s) + '\n');
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
				sys.stderr.write(sprintf('%-15s %s\n', 'key', 'value'));
				sys.stderr.write('-' * 25 + '\n');
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
			case 'qtrim_front':
			case 'qtrim_back':
				print repr_data(resp.data);
				sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
				break;
			case 'compact':
			case 'auth':
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
				sys.stderr.write(sprintf('%-15s %s\n', 'key', 'value'));
				sys.stderr.write('-' * 25 + '\n');
				foreach(resp.data['index'] as k){
					printf('  %-15s : %s\n', repr_data(k), repr_data(resp.data['items'][k]));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume));
				break;
			case 'zscan':
			case 'zrscan':
			case 'zrange':
			case 'zrrange':
				sys.stderr.write(sprintf('%-15s %s\n', 'key', 'score'));
				sys.stderr.write('-' * 25 + '\n');
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
				sys.stderr.write(sprintf('  %15s\n', 'key'));
				sys.stderr.write('-' * 17 + '\n');
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'hvals':
				sys.stderr.write(sprintf('  %15s\n', 'value'));
				sys.stderr.write('-' * 17 + '\n');
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
			case 'qpop':
			case 'qpop_front':
			case 'qpop_back':
				foreach(resp.data as k){
					printf('  %s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
				sys.stderr.write(sprintf('%-15s %s\n', 'key', 'value'));
				sys.stderr.write('-' * 25 + '\n');
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
					sys.stderr.write('error!\n');
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

