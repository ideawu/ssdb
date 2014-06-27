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
		this.code = code;
		this.data = null;
		this.message = null;
		if(code == 'ok'){
			this.data = data_or_message;
		}else{
			this.message = data_or_message;
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
		switch(cmd){
			case 'set':
			case 'zset':
			case 'hset':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
			case 'del':
			case 'zdel':
			case 'hdel':
			case 'multi_set':
			case 'multi_del':
			case 'multi_hset':
			case 'multi_hdel':
			case 'multi_zset':
			case 'multi_zdel':
				if(len(resp) > 1){
					return new SSDB_Response(resp[0], int(resp[1]));
				}else{
					return new SSDB_Response(resp[0], 1);
				}
				break;
			case 'substr':
			case 'get':
			case 'getset':
			case 'hget':
			case 'qfront':
			case 'qback':
			case 'qget':
			case 'qpop':
			case 'qpop_front':
			case 'qpop_back':
				if(resp[0] == 'ok'){
					if(len(resp) == 2){
						return new SSDB_Response('ok', resp[1]);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
			case 'getbit':
			case 'setbit':
			case 'countbit':
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
			case 'zavg':
			case 'zremrangebyrank':
			case 'zremrangebyscore':
			case 'hclear':
			case 'zclear':
			case 'qclear':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
				if(resp[0] == 'ok'){
					if(len(resp) == 2){
						try{
							if(cmd == 'zavg'){
								val = float(resp[1]);
							}else{
								val = int(resp[1]);
							}
							return new SSDB_Response('ok', val);
						}catch(Exception e){
							return new SSDB_Response('server_error', 'Invalid response');
						}
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
			case 'keys':
			case 'zkeys':
			case 'hkeys':
			case 'list':
			case 'hlist':
			case 'zlist':
				data = [];
				if(resp[0] == 'ok'){
					for(i=1; i<len(resp); i++){
						data.append(resp[i]);
					}
				}
				return new SSDB_Response(resp[0], data);
				break;
			case 'scan':
			case 'rscan':
			case 'hgetall':
			case 'hscan':
			case 'hrscan':
				if(resp[0] == 'ok'){
					if(len(resp) % 2 == 1){
						data = {'index':[], 'items':{}};
						for(i=1; i<len(resp); i+=2){
							k = resp[i];
							v = resp[i + 1];
							data['index'].append(k);
							data['items'][k] = v;
						}
						return new SSDB_Response('ok', data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
			case 'zscan':
			case 'zrscan':
			case 'zrange':
			case 'zrrange':
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
						return new SSDB_Response('ok', data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
            case 'exists':
            case 'hexists':
            case 'zexists':
                data = false;
				if(resp[0] == 'ok'){
                    if(len(resp) >= 2){
                        if(resp[1] == '1'){
                            data = true;
                        }
                    }
                }
				return new SSDB_Response(resp[0], data);
                break;
            case 'multi_exists':
            case 'multi_hexists':
            case 'multi_zexists':
				data = {};
				if(len(resp) % 2 == 1){
					for(i=1; i<len(resp); i+=2){
						k = resp[i];
						if(resp[i + 1] == '1'){
                            v = true;
                        }else{
                            v = false;
                        }
						data[k] = v;
					}
                }
				return new SSDB_Response('ok', data);
                break;
			case 'multi_get':
			case 'multi_hget':
				if(resp[0] == 'ok'){
					if(len(resp) % 2 == 1){
						data = {};
						for(i=1; i<len(resp); i+=2){
							k = resp[i];
							v = resp[i + 1];
							data[k] = v;
						}
						return new SSDB_Response('ok', data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
			case 'multi_hsize':
			case 'multi_zsize':
			case 'multi_zget':
				if(resp[0] == 'ok'){
					if(len(resp) % 2 == 1){
						data = {};
						for(i=1; i<len(resp); i+=2){
							k = resp[i];
							v = int(resp[i + 1]);
							data[k] = v;
						}
						return new SSDB_Response('ok', data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response(resp[0]);
				}
				break;
			default:
				if(len(resp) > 1){
					data = [];
					for(i=1; i<len(resp); i++){
						data.append(resp[i]);
					}
				}else{
					data = '';
				}
				return new SSDB_Response(resp[0], data);
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