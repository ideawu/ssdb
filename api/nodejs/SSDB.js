/**
 * Copyright (c) 2013, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB nodejs client SDK.
 */

var net = require('net');

// timeout: microseconds, if ommitted, it will be considered as listener
// callback(err, ssdb)
exports.connect = function(host, port, timeout, listener){
	var self = this;
	var recv_buf = new Buffer(0);
	var callbacks = [];
	var connected = false;

	if(typeof(timeout) == 'function'){
		listener = timeout;
		timeout = 0;
	}
	listener = listener || function(){};

	var sock = new net.Socket();
	sock.on('error', function(e){
		if(!connected){
			listener('connect_failed', e);
		}else{
			var callback = callbacks.shift();
			callback(['error']);
		}
	});
	sock.connect(port, host, function(){
		connected = true;
		sock.setNoDelay(true);
		sock.setKeepAlive(true);
		sock.setTimeout(timeout);
		listener(0, self);
	});

	self.close = function(){
		sock.end();
	}

	self.request = function(cmd, params, callback){
		var arr = [cmd].concat(params);
		self.send_request(arr);
		callbacks.push(callback || function(){});
	}

	function build_buffer(arr){
		var bs = [];
		var size = 0;
		for(var i = 0; i < arr.length; i++){
			var arg = arr[i];
			if(arg instanceof Buffer){
				//
			}else{
				arg = new Buffer(arg.toString());
			}
			bs.push(arg);
			size += arg.length;
		}
		var ret = new Buffer(size);
		var offset = 0;
		for(var i=0; i<bs.length; i++){
			bs[i].copy(ret, offset);
			offset += bs[i].length;
		}
		return ret;
	}

	self.send_request = function(params){
		var bs = [];
		for(var i in params){
			var p = params[i];
			var len = 0;
			if(!(p instanceof Buffer)){
				p = p.toString();
			}
			bs.push(p.length);
			bs.push('\n');
			bs.push(p);
			bs.push('\n');
		}
		bs.push('\n');
		var req = build_buffer(bs);
		sock.write(req);
		//console.log('write ' + req.length + ' bytes');
	}

	sock.on('data', function(data){
		recv_buf = build_buffer([recv_buf, data]);
		while(recv_buf.length > 0){
			var resp = parse();
			if(!resp){
				break;
			}
			resp[0] = resp[0].toString();
			var callback = callbacks.shift();
			callback(resp);
		}
	});

	function memchr(buf, ch, start){
		start = start || 0;
		ch = typeof(ch) == 'string'? ch.charCodeAt(0) : ch;
		for(var i=start; i<buf.length; i++){
			if(buf[i] == ch){
				return i;
			}
		}
		return -1;
	}

	function parse(){
		var ret = [];
		var spos = 0;
		var pos;
		//console.log('parse: ' + recv_buf.length + ' bytes');
		while(true){
			//pos = recv_buf.indexOf('\n', spos);
			pos = memchr(recv_buf, '\n', spos);
			if(pos == -1){
				// not finished
				return null;
			}
			var line = recv_buf.slice(spos, pos).toString();
			spos = pos + 1;
			line = line.replace(/^\s+(.*)\s+$/, '\1');
			if(line.length == 0){
				// parse end
				//recv_buf = recv_buf.substr(spos);
				recv_buf = recv_buf.slice(spos);
				break;
			}
			var len = parseInt(line);
			if(isNaN(len)){
				// error
				console.log('error 1');
				return null;
			}
			if(spos + len > recv_buf.length){
				// not finished
				//console.log(spos + len, recv_buf.length);
				//console.log('not finish');
				return null;
			}
			//var data = recv_buf.substr(spos, len);
			var data = recv_buf.slice(spos, spos + len);
			spos += len;
			ret.push(data);

			//pos = recv_buf.indexOf('\n', spos);
			pos = memchr(recv_buf, '\n', spos);
			if(pos == -1){
				// not finished
				console.log('error 3');
				return null;
			}
			// '\n', or '\r\n'
			//if(recv_buf.charAt(spos) != '\n' && recv_buf.charAt(spos) != '\r' && recv_buf.charAt(spos+1) != '\n'){
			var cr = '\r'.charCodeAt(0);
			var lf = '\n'.charCodeAt(0);
			if(recv_buf[spos] != lf && recv_buf[spos] != cr && recv_buf[spos+1] != lf){
				// error
				console.log('error 4 ' + recv_buf[spos]);
				return null;
			}
			spos = pos + 1;
		}
		return ret;
	}

	// callback(err, val);
	// err: 0 on sucess, or error_code(string) on error
	self.get = function(key, callback){
		self.request('get', [key], function(resp){
			if(callback){
				var err = resp[0] == 'ok'? 0 : resp[0];
				var val = resp[1];
				callback(err, val);
			}
		});
	}

	// callback(err);
	self.set = function(key, val, callback){
		self.request('set', [key, val], function(resp){
			if(callback){
				var err = resp[0] == 'ok'? 0 : resp[0];
				callback(err);
			}
		});
	}

	// callback(err);
	self.del = function(key, callback){
		self.request('del', [key], function(resp){
			if(callback){
				var err = resp[0] == 'ok'? 0 : resp[0];
				callback(err);
			}
		});
	}

	return self;
}


/*
example:
var SSDB = require('./SSDB.js');
var ssdb = SSDB.connect(host, port, function(err){
	if(err){
		return;
	}
	ssdb.set('a', new Date(), function(){
		console.log('set a');
	});
});
*/

