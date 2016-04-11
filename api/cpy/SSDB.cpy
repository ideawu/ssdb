/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB Cpy client SDK.
 */

import socket;

class SSDB_Response{
	function init(code='', data_or_message=null){
		this.type = 'none';
		this.code = code;
		this.data = null;
		this.message = null;
		this.set(code, data_or_message);
	}
	
	function set(code, data_or_message=null){
		this.code = code;
		if(code == 'ok'){
			this.data = data_or_message;
		}else{
			if(isinstance(data_or_message, list)){
				if(len(data_or_message) > 0){
					this.message = data_or_message[0];
				}
			}else{
				this.message = data_or_message;
			}
		}
	}

	function __repr__(){
		return str(this.code) + ' ' + str(this.message) + ' ' + str(this.data);
	}

	function ok(){
		return this.code == 'ok';
	}

	function not_found(){
		return this.code == 'not_found';
	}
	
	function str_resp(resp){
		this.type = 'val';
		if(resp[0] == 'ok'){
			if(len(resp) == 2){
				this.set('ok', resp[1]);
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
	
	function str_resp(resp){
		this.type = 'val';
		if(resp[0] == 'ok'){
			if(len(resp) == 2){
				this.set('ok', resp[1]);
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
	
	function int_resp(resp){
		this.type = 'val';
		if(resp[0] == 'ok'){
			if(len(resp) == 2){
				try{
					val = int(resp[1]);
					this.set('ok', val);
				}catch(Exception e){
					this.set('server_error', 'Invalid response');
				}
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
	
	function float_resp(resp){
		this.type = 'val';
		if(resp[0] == 'ok'){
			if(len(resp) == 2){
				try{
					val = float(resp[1]);
					this.set('ok', val);
				}catch(Exception e){
					this.set('server_error', 'Invalid response');
				}
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
	
	function list_resp(resp){
		this.type = 'list';
		this.set(resp[0], resp[1 ..]);
		return this;
	}
	
	function int_map_resp(resp){
		this.type = 'map';
		if(resp[0] == 'ok'){
			if(len(resp) % 2 == 1){
				data = {'index':[], 'items':{}};
				for(i=1; i<len(resp); i+=2){
					k = resp[i];
					v = resp[i + 1];
					try{
						v = int(v);
					}catch(Exception e){
						v = -1;
					}
					data['index'].append(k);
					data['items'][k] = v;
				}
				this.set('ok', data);
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
	
	function str_map_resp(resp){
		this.type = 'map';
		if(resp[0] == 'ok'){
			if(len(resp) % 2 == 1){
				data = {'index':[], 'items':{}};
				for(i=1; i<len(resp); i+=2){
					k = resp[i];
					v = resp[i + 1];
					data['index'].append(k);
					data['items'][k] = v;
				}
				this.set('ok', data);
			}else{
				this.set('server_error', 'Invalid response');
			}
		}else{
			this.set(resp[0], resp[1 .. ]);
		}
		return this;
	}
}

class SSDB{
	function init(host, port){
		this.recv_buf = '';
		this._closed = false;
		this.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
		this.sock.connect(tuple([host, port]));
		this.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1);
	}

	function close(){
		if(!this._closed){
			this.sock.close();
			this._closed = True;
		}
	}

	function closed(){
		return this._closed;
	}

	function request(cmd, params=null){        
		if(params == null){
			params = [];
		}
		params = [cmd] + params;
		this.send(params);

		resp = this.recv();
		if(resp == null){
			return new SSDB_Response('error', 'Unknown error');
		}
		if(len(resp) == 0){
			return new SSDB_Response('disconnected', 'Connection closed');
		}
		
		ret = new SSDB_Response();
		switch(cmd){
			case 'ping':
			case 'set':
			case 'del':
			case 'qset':
			case 'zset':
			case 'hset':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
			case 'zdel':
			case 'hdel':
			case 'multi_set':
			case 'multi_del':
			case 'multi_hset':
			case 'multi_hdel':
			case 'multi_zset':
			case 'multi_zdel':
				if(len(resp) > 1){
					return ret.int_resp(resp);
				}else{
					return new SSDB_Response(resp[0], null);
				}
				break;
			case 'version':
			case 'substr':
			case 'get':
			case 'getset':
			case 'hget':
			case 'qfront':
			case 'qback':
			case 'qget':
				return ret.str_resp(resp);
				break;
			case 'qpop':
			case 'qpop_front':
			case 'qpop_back':
				size = 1;
				try{
					size = int(params[2]);
				}catch(Exception e){
				}
				if(size == 1){
					return ret.str_resp(resp);
				}else{
					return ret.list_resp(resp);
				}
				break;
			case 'dbsize':
			case 'getbit':
			case 'setbit':
			case 'countbit':
			case 'bitcount':
			case 'strlen':
			case 'ttl':
			case 'expire':
			case 'setnx':
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
			case 'hsize':
			case 'zsize':
			case 'qsize':
			case 'zget':
			case 'zrank':
			case 'zrrank':
			case 'zsum':
			case 'zcount':
			case 'zremrangebyrank':
			case 'zremrangebyscore':
			case 'hclear':
			case 'zclear':
			case 'qclear':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
			case 'qtrim_front':
			case 'qtrim_back':
				return ret.int_resp(resp);
				break;
			case 'zavg':
				return ret.float_resp(resp);
				break;
			case 'keys':
			case 'rkeys':
			case 'zkeys':
			case 'zrkeys':
			case 'hkeys':
			case 'hrkeys':
			case 'list':
			case 'hlist':
			case 'hrlist':
			case 'zlist':
			case 'zrlist':
				return ret.list_resp(resp);
				break;
			case 'scan':
			case 'rscan':
			case 'hgetall':
			case 'hscan':
			case 'hrscan':
				return ret.str_map_resp(resp);
				break;
			case 'zscan':
			case 'zrscan':
			case 'zrange':
			case 'zrrange':
			case 'zpop_front':
			case 'zpop_back':
				return ret.int_map_resp(resp);
				break;
			case 'auth':
            case 'exists':
            case 'hexists':
            case 'zexists':
				return ret.int_resp(resp);
                break;
            case 'multi_exists':
            case 'multi_hexists':
            case 'multi_zexists':
				return ret.int_map_resp(resp);
				break;
			case 'multi_get':
			case 'multi_hget':
				return ret.str_map_resp(resp);
				break;
			case 'multi_hsize':
			case 'multi_zsize':
			case 'multi_zget':
				return ret.int_map_resp(resp);
				break;
			default:
				return ret.list_resp(resp);
				break;
		}
		return new SSDB_Response('error', 'Unknown error');
	}

	function send(data){
		ps = [];
		foreach(data as p){
			p = str(p);
			ps.append(str(len(p)));
			ps.append(p);
		}
		nl = '\n';
		s = nl.join(ps) + '\n\n';
		#print '> ' + repr(s);
		try{
			while(true){
				ret = this.sock.send(s);
				if(ret == 0){
					return -1;
				}
				s = s[ret .. ];
				if(len(s) == 0){
					break;
				}
			}
		}catch(socket.error e){
			return -1;
		}
		//except socket.error as (val, msg):
		return ret;
	}

	function net_read(){
		try{
			data = this.sock.recv(1024*8);
			#print '< ' + repr(data);
		}catch(Exception e){
			data = '';
		}
		if(data == ''){
			this.close();
			return 0;
		}
		this.recv_buf += data;
		return len(data);
	}

	function recv(){
		while(true){
			ret = this.parse();
			if(ret == null){
				if(this.net_read() == 0){
					return [];
				}
			}else{
				return ret;
			}
		}
	}

	function parse(){
		//if(len(this.recv_buf)){print 'recv_buf: ' + repr(this.recv_buf);}
		ret = [];
		spos = 0;
		epos = 0;
		while(true){
			spos = epos;
			epos = this.recv_buf.find('\n', spos);
			if(epos == -1){
				break;
			}
			epos += 1;
			line = this.recv_buf[spos .. epos];
			spos = epos;

			if(line.strip() == ''){ // head end
				if(len(ret) == 0){
					continue;
				}else{
					this.recv_buf = this.recv_buf[spos .. ];
					return ret;
				}
			}

			try{
				num = int(line);
			}catch(Exception e){
				// error
				return [];
			}
			epos = spos + num;
			if(epos > len(this.recv_buf)){
				break;
			}
			data = this.recv_buf[spos .. epos];
			ret.append(data);

			spos = epos;
			epos = this.recv_buf.find('\n', spos);
			if(epos == -1){
				break;
			}
			epos += 1;
		}

		return null;
	}
}
