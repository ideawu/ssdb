import thread, re, time, socket;
import getopt, shlex;
import datetime;
import ssdb_cli.*;
sys.path.append('./api/python');
sys.path.append('../api/python');
sys.path.append('/usr/local/ssdb/api/python');
import SSDB.*;

try{
	import readline;
}catch(Exception e){
}

escape_data = false;

function welcome(){
	sys.stderr.write('ssdb (cli) - ssdb command line tool.\n');
	sys.stderr.write('Copyright (c) 2012-2016 ssdb.io\n');
	sys.stderr.write('\n');
	sys.stderr.write("'h' or 'help' for help, 'q' to quit.\n");
	sys.stderr.write('\n');
}

function show_command_help(){
	print '';
	print '# display ssdb-server status';
	print '	info';
	print '# escape/do not escape response data';
	print '	: escape yes|no';
	print '# export/import';
	print '	export [-i] out_file';
	print '		-i	interactive mode';
	print '	import in_file';
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
	print '	-v --help';
	print '		show this message';
	print '	-n [info, dbsize, replication, write_read]';
	print '		choose nagios probe';
	print '	-w INT';
	print '		set nagios WARN level';
	print '	-c INT';
	print '		set nagios CRITICAL level';
	print '';
	print 'Examples:';
	print '	ssdb-cli';
	print '	ssdb-cli 8888';
	print '	ssdb-cli 127.0.0.1 8888';
	print '	ssdb-cli -h 127.0.0.1 -p 8888';
	print '	ssdb-cli -h 127.0.0.1 -p 8888 -n dbsize -w 500000 -c 600000';
	print '	ssdb-cli -h 127.0.0.1 -p 8888 -n replication';
	print '	ssdb-cli -h 127.0.0.1 -p 8888 -n write_read';
	print '	ssdb-cli -n info';
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

host = '';
port = '';
opt = '';
args = [];
run_nagios = false;

foreach(sys.argv[1 ..] as arg){
	if(opt == '' && arg.startswith('-')){
		opt = arg;
		if(arg == '--help' || arg == '--h' || arg == '-v'){
			usage();
			exit(0);
		}
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
			// nagios
			case '-n':
			case '-w':
			case '-c':
				run_nagios = true;
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
		if(!re.match('^[0-9]+$', arg)){
			host = arg;
			break;
		}
	}
}
if(port == ''){
	port = '8888';
	foreach(args as arg){
		if(re.match('^[0-9]+$', arg)){
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

try{
	link = new SSDB(host, port);
}catch(socket.error e){
	sys.stderr.write(sprintf('Failed to connect to: %s:%d\n', host, port));
	sys.stderr.write(sprintf('Connection error: %s\n', str(e)));
	sys.exit(0);
}

if(run_nagios){
	nagios.run(link, sys.argv[1 ..]);
	exit(0);
}

welcome();
if(sys.stdin.isatty()){
	util.show_version(link);
}


password = false;

function request_with_retry(cmd, args=null){
	gs = globals();
	link = gs['link'];
	password = gs['password'];
	
	if(!args){
		args = [];
	}
	
	retry = 0;
	max_retry = 5;
	while(true){
		resp = link.request(cmd, args);
		if(resp.code == 'disconnected'){
			link.close();
			sleep = retry;
			if(sleep > 3){
				sleep = 3;
			}
			time.sleep(sleep);
			retry ++;
			if(retry > max_retry){
				sys.stderr.write('cannot connect to server, give up...\n');
				break;
			}
			sys.stderr.write(sprintf('[%d/%d] reconnecting to server... ', retry, max_retry));
			try{
				link = new SSDB(host, port);
				gs['link'] = link;
				sys.stderr.write('done.\n');
			}catch(socket.error e){
				sys.stderr.write(sprintf('Connect error: %s\n', str(e)));
				continue;
			}
			if(password){
				ret = link.request('auth', [password]);
			}
		}else{
			return resp;
		}
	}
	return null;
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

	for(i=0; i<len(ps); i++){
		ps[i] = ps[i].decode('string-escape');
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
		util.show_version(link);
		continue;
	}
	if(cmd == 'auth'){
		if(len(args) == 0){
			sys.stderr.write('Usage: auth password\n');
			continue;
		}
		password = args[0];
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
			resp = request_with_retry('ping');
			if(!resp){
				throw new Exception('error');
			}
			if(resp.code != 'ok'){
				throw new Exception(resp.message);
			}
			
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

	stime = datetime.datetime.now();
	resp = request_with_retry(cmd, args);
	if(resp == null){
		sys.stderr.write("error!\n");
		continue;
	}

	time_consume = timespan(stime);

	if(!resp.ok()){
		if(resp.not_found()){
			sys.stderr.write('not_found\n');
		}else{
			s = resp.code;
			if(resp.message){
				s += ': ' + str(resp.message);
			}
			sys.stderr.write(str(s) + '\n');
		}
		sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
	}else{
		skip = false;
		switch(cmd){
			case 'ping':
			case 'qset':
			case 'compact':
			case 'auth':
			case 'set':
			case 'setx':
			case 'zset':
			case 'hset':
			case 'del':
			case 'zdel':
				skip = true;
				printf(str(resp.code) + '\n');
				break;
			case 'info':
				skip = true;
				is_val = false;
				for(i=1; i<len(resp.data); i++){
					s = resp.data[i];
					if(is_val){
						s = '	' + s.replace('\n', '\n	');
					}
					print s;
					is_val = !is_val;
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
		}
		if(skip){
			sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
			continue;
		}

		switch(resp.type){
			case 'none':
				printf(str(resp.data) + '\n');
				break;
			case 'val':
				if(resp.code == 'ok'){
					printf(str(resp.data) + '\n');
				}else{
					if(resp.data){
						print repr_data(resp.code), repr_data(resp.data);
					}else{
						print repr_data(resp.code);
					}
				}
				break;
			case 'list':
				sys.stderr.write(sprintf('  %15s\n', 'key'));
				sys.stderr.write('-' * 17 + '\n');
				foreach(resp.data as k){
					printf('  %15s\n', repr_data(k));
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data), time_consume));
				break;
			case 'map':
				sys.stderr.write(sprintf('%-15s %s\n', 'key', 'value'));
				sys.stderr.write('-' * 25 + '\n');
				foreach(resp.data['index'] as k){
					v = resp.data['items'][k];
					printf('  %-15s: %s\n', repr_data(repr_data(k)), v);
				}
				sys.stderr.write(sprintf('%d result(s) (%.3f sec)\n', len(resp.data['index']), time_consume));
				break;
		}
		sys.stderr.write(sprintf('(%.3f sec)\n', time_consume));
	}
}

