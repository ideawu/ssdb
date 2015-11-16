function hclear(link, hname, verbose=true){
	ret = 0;
	r = link.request('hclear', [hname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}

function zclear(link, zname, verbose=true){
	ret = 0;
	r = link.request('zclear', [zname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}

function qclear(link, zname, verbose=true){
	ret = 0;
	r = link.request('qclear', [zname]);
	try{
		ret = r.data;
	}catch(Exception e){
	}
	return ret;
}
function flushdb(link, data_type){
	resp = link.request('info');
	for(i=1; i<len(resp.data); i+=2){
		if(resp.data[i] == 'replication'){
			throw new Exception('flushdb is not allowed when replication is in use!');
		}
	}

	printf('\n');
	printf('============================ DANGER! ============================\n');
	printf('This operation is DANGEROUS and is not recoverable, if you\n');
	printf('really want to flush the whole db(delete ALL data in ssdb server),\n');
	printf('input \'yes\' and press Enter, or just press Enter to cancel\n');
	printf('\n');
	printf('flushdb will break replication states, you must fully understand\n');
	printf('the RISK before you doing this!\n');
	printf('\n');
	printf('> flushdb? ');
	
	line = sys.stdin.readline().strip();
	if(line != 'yes'){
		printf('Operation cancelled.\n\n');
		return;
	}

	print 'Begin to flushdb...\n';

	if(data_type == ''){
		resp = link.request('flushdb', []);
		if(resp.code != 'ok' && resp.code != 'client_error'){
			throw new Exception(resp.message);
		}
	}
	
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
