nagios_probe = '';
nagios_warn = 85;
nagios_critical = 95;

function run(link, cli_args){
	gs = globals();
	opt = '';
	foreach(cli_args as arg){
		if(opt == '' && arg.startswith('-')){
			opt = arg;
		}else{
			switch(opt){
				case '-n':
					opt = '';
					gs['nagios_probe'] = arg;
					break;
				case '-w':
					gs['nagios_warn'] = arg;
					opt = '';
					break;
				case '-c':
					gs['nagios_critical'] = arg;
					opt = '';
					break;
				default: 
					# ignore args '-h host -p port'
					opt = '';
					break;
			}
		}
	}
	try{
		resp = link.request('info', []);
		if(nagios_probe == 'info'){
			nagios_info(resp);
		}
		if(nagios_probe == 'dbsize'){
			nagios_dbsize(resp);
		}
		if(nagios_probe == 'replication'){
			nagios_replication(resp);
		}
		if(nagios_probe == 'write_read'){
			nagios_write_read(link);
		}
		# Possible future checks:
		# - check if binlogs.max_seq == replication.client.last_seq
		# - does total_calls is growing
	}catch(Exception e){
		sys.stderr.write(str(e) + '\n');
	}
	#sys.stderr.write('exit\n');
	exit(0);
}

function nagios_info(resp){
	is_val = false;
	for(i=1; i<len(resp.data); i++){
		s = resp.data[i];
		if(is_val){
			s = '	' + s.replace('\n', '\n	');
		}
		print s;
		is_val = !is_val;
	}
}

function nagios_probe_check(resp){
	next_val = false;
	ret = '';
	for(i=1; i<len(resp.data); i++){
		s = resp.data[i];
		if(next_val){
			s = s.replace('\n', '\n	');
			next_val = !next_val;
			#print s;
			ret += s;
		}
		if(s == nagios_probe){
			next_val = !next_val;
		}
	}
	return ret;
}

function nagios_dbsize(resp){
	dbsize = nagios_probe_check(resp);
	if(long(dbsize) > long(nagios_critical)){
		print 'CRITICAL: dbsize ' + str(dbsize) + ' larger than ' + str(nagios_critical);
		exit(2);
	}else if(long(dbsize) > long(nagios_warn)){
		print 'WARN: dbsize ' + str(dbsize) + ' larger than ' + str(nagios_warn);
		exit(1);
	}else{
		print 'OK: dbsize ' + str(dbsize) + ' less than ' + str(nagios_critical);
		exit(0);
	}
}

function nagios_replication(resp){
	replication = nagios_probe_check(resp);
	replication = replication.replace('slaveof', '\nslaveof');
	if(replication.find('DISCONNECTED') > 0 ){
		print 'CRITICAL: ' + replication;
		exit(2);
	}else if(replication.find('COPY') > 0 || replication.find('INIT') > 0 || replication.find('OUT_OF_SYNC') > 0){
		print 'WARN: ' + replication;
		exit(1);
	}else if(replication.find('SYNC') > 0){
		print 'OK: ' + replication;
		exit(0);
	}else{
		print 'WARN, is replication configured? Status: ' + replication;
		exit(1);
	}
}

function nagios_write_read(link){
	test_key = 'write_read_test_key';
	resp = link.request('set', ['nagiostest', test_key]);
	#print resp;
	resp = link.request('get', ['nagiostest']);
	#print resp;
	if (resp.data == test_key){
		print 'OK: ' + resp.data;
		exit(0);
	}else{
		print 'WRITE_READ failed: ' + resp.data;
		exit(2);
	}
}
