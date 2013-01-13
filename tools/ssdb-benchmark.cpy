import thread, re, time, datetime, select, socket;
import getopt, shlex;
import random;

try{
	import readline;
}catch(Exception e){
}

function welcome(){
	print ('ssdb-benchmark.');
	print ('Copyright (c) 2012 ideawu.com');
	print ('');
}

function usage(){
	print '';
	print 'Usage:';
	print '    ssdb-benchmark -n NUM_REQS [-h HOST -p PORT]';
	print 'Options:';
	print '    -h 127.0.0.1';
	print '        ssdb server hostname/ip address';
	print '    -p 8888';
	print '        ssdb server port';
	print '    -n NUM_REQS';
	print '        Number of requests';
	print '    -c 10';
	print '        Number of concurrent connections';
	print 'Examples:';
	print '    ssdb-benchmark -n 1000';
	print '    ssdb-benchmark -n 1000 -p 8888';
	print '    ssdb-benchmark -n 1000 -h 127.0.0.1 -p 8888';
}
welcome();


default_opts = {
	'-h' : '127.0.0.1',
	'-p' : '8888',
	'-n' : '',
	'-c' : '10',
};

opt_err = false;
try{
	opts, args = getopt.getopt(sys.argv[1 .. ], 'h:p:n:c:');
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

if(!opts['-p'].isdigit()){
	print "Error: invalid option '-p'!";
	opt_err = true;
}
if(!opts['-n'].isdigit()){
	print "Error: invalid option '-n'!";
	opt_err = true;
}
if(!opts['-c'].isdigit()){
	print "Error: invalid option '-c'!";
	opt_err = true;
}


if(opt_err){
	usage();
	sys.exit(0);
}


host = opts['-h'];
port = int(opts['-p']);
num_req = int(opts['-n']);
num_conn = int(opts['-c']);

function prepare_data(num){
	random.seed();

	rand_s = 100000;
	rand_e = rand_s * 10 - 1;

	ret = [];
	for(i=0; i<num; i++){
		k = random.randint(0, rand_e);
		v = random.randint(rand_s, rand_e);
		s = random.randint(0, rand_e);
		ret.append([k, v, s]);
	}
	return ret;
}

function make_connections(num, host, port){
	sys.path.append(os.path.dirname(__file__) + '/../../api/python');
	import SSDB.SSDB;

	links = {};
	try{
		for(i=0; i<num; i++){
			link = new SSDB(host, port);
			links[link.sock.fileno()] = link;
		}
	}catch(socket.error e){
		print 'Connection error: ', str(e);
		sys.exit(0);
	}
	return links;
}

function benchmark(cmd, prepared_data, links){
	reqs = [];
	foreach(prepared_data as i=>k,v,s){
		ok = k;
		k = str(k);
		v = str(v);
		if(cmd == 'set'){
			reqs.append(['set', k, v]);
		}
		if(cmd == 'get'){
			reqs.append(['get', k]);
		}
		if(cmd == 'scan'){
			reqs.append(['scan', ok - 1, k + '0', 10]);
			#reqs.append(['scan', '0', 'z', 10]);
		}
		if(cmd == 'del'){
			reqs.append(['del', k]);
		}

		if(cmd == 'zset'){
			reqs.append(['zset', 'z', k, s]);
		}
		if(cmd == 'zget'){
			reqs.append(['zget', 'z', k]);
		}
		if(cmd == 'zdel'){
			reqs.append(['zdel', 'z', k]);
		}
		if(cmd == 'zscan'){
			reqs.append(['zscan', 'z', k, s-1, s*2, 10]);
		}

		if(cmd == 'hset'){
			reqs.append(['hset', 'h', k, v]);
		}
		if(cmd == 'hget'){
			reqs.append(['hget', 'h', k]);
		}
		if(cmd == 'hscan'){
			reqs.append(['hscan', 'h', ok - 1, k + '0', 10]);
		}
		if(cmd == 'hdel'){
			reqs.append(['hdel', 'h', k]);
		}
	}
	random.shuffle(reqs);

	rfds = [];
	foreach(links as link){
		rfds.append(link.sock.fileno());
	}

	stime = datetime.datetime.now();

	send_count = 0;
	foreach(links as link){
		if(send_count < len(reqs)){
			link.last_req = reqs[send_count];
			link.send(reqs[send_count]);
			send_count ++;
		}
	}

	recv_count = 0;
	quit = false;
	while(!quit){
		r, w, e = select.select(rfds, [], [], 1);
		foreach(r as fd){
			link = links[fd];
			link.net_read();
			resp = link.parse();
			// we won't send another request before we get the response to previouse request
			if(resp == null){
				continue;
			}
			if(resp[0] != 'ok' && resp[0] != 'not_found'){
				print 'error';
				print resp;
			}
			if((cmd == 'zscan' || cmd == 'scan') && len(resp) == 1){
				print resp;
				print cmd + ' error ' + str(link.last_req);
			}
			if(send_count % 10 == 1){
				#time.sleep(0.001);
			}
			#print resp;
			recv_count ++;
			if(recv_count == len(reqs)){
				quit = true;
				break;
			}
			if(send_count < len(reqs)){
				link.last_req = reqs[send_count];
				link.send(reqs[send_count]);
				send_count ++;
			}
		}
	}

	etime = datetime.datetime.now();
	ts = etime - stime;
	ts = ts.seconds + ts.microseconds/1000000.;
	time_ = ts;
	if(ts == 0){
		ts = 1;
	}
	speed = len(reqs)/ts;
	print '';
	printf('======= %s =======\n', cmd);
	printf('qps: %d, time: %.3f s\n', speed, time_);
}


printf('preparing %d request(s)...\n', num_req);
prepared_data = prepare_data(num_req);

printf('making %d connection(s) to server...\n', num_conn);
links = make_connections(num_conn, host, port);

benchmark('set', prepared_data, links);
benchmark('get', prepared_data, links);
benchmark('scan', prepared_data, links);
benchmark('del', prepared_data, links);

benchmark('zset', prepared_data, links);
benchmark('zget', prepared_data, links);
benchmark('zscan', prepared_data, links);
benchmark('zdel', prepared_data, links);

benchmark('hset', prepared_data, links);
benchmark('hget', prepared_data, links);
benchmark('hscan', prepared_data, links);
benchmark('hdel', prepared_data, links);

foreach(links as link){
	link.close();
}
