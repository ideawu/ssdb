<?php
/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB PHP client SDK.
 */

class SSDB_Response
{
	public $code;
	public $data = null;
	public $message;

	function __construct($code='ok', $data_or_message=null){
		$this->code = $code;
		if($code == 'ok'){
			$this->data = $data_or_message;
		}else{
			$this->message = $data_or_message;
		}
	}

	function __toString(){
		if($this->code == 'ok'){
			$s = $this->data === null? '' : json_encode($this->data);
			return $this->code . ' ' . $s;
		}else{
			return $this->code . ' ' . $this->message;
		}
	}

	function ok(){
		return $this->code == 'ok';
	}

	function not_found(){
		return $this->code == 'not_found';
	}
}

class SSDB
{
	private $debug = false;
	public $sock = null;
	private $_closed = false;
	private $recv_buf = '';

	function __construct($host, $port, $timeout_ms=200){
		$this->sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if(!$this->sock){
			throw new Exception(socket_strerror(socket_last_error()));
		}
		$timeout_sec = intval($timeout_ms/1000);
		$timeout_ms = $timeout_ms - $timeout_sec * 1000;
		@socket_set_timeout($this->sock, $timeout_sec, $timeout_ms);
		$ret = @socket_connect($this->sock, $host, $port);
		if(!$ret){
			throw new Exception(socket_strerror(socket_last_error()));
		}
		if(is_int(TCP_NODELAY)){
			socket_set_option($this->sock, SOL_TCP, TCP_NODELAY, 1);
		}
	}

	function close(){
		if(!$this->_closed){
			socket_close($this->sock);
			$this->_closed = true;
			$this->sock = null;
		}
	}

	function closed(){
		return $this->_closed;
	}

	// all supported are listed, for documentation purpose

	function set($key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_set($pairs=array()){
		$args = array();
		foreach($pairs as $k=>$v){
			$args[] = $k;
			$args[] = $v;
		}
		return $this->__call(__FUNCTION__, $args);
	}

	function incr($key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function decr($key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function get($key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_get($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function del($key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_del($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function scan($key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function rscan($key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function keys($key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	/* zset */

	function zset($name, $item, $score){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zget($name, $item){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zdel($name, $item){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zscan($name, $key_start, $score_start, $score_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zrscan($name, $key_start, $score_start, $score_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zkeys($name, $key_start, $score_start, $score_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zincr($name, $item, $score){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zdecr($name, $item, $score){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zsize($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zlist($name_start, $name_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	/* hashmap */

	function hset($name, $key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hget($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hdel($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hscan($name, $key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hrscan($name, $key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hkeys($name, $key_start, $key_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hincr($name, $key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hdecr($name, $key, $val){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hsize($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hlist($name_start, $name_end, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function __call($cmd, $params=array()){
		$req = array($cmd);
		foreach($params as $p){
			if(is_array($p)){
				$req = array_merge($req, $p);
			}else{
				$req[] = $p;
			}
		}
		$this->send($req);

		$resp = $this->recv();

		if($resp === false){
			return new SSDB_Response('error', 'Unknown error');
		}else if(!$resp){
			return new SSDB_Response('disconnected', 'Connection closed');
		}
		switch($cmd){
			case 'set':
			case 'zset':
			case 'hset':
			case 'del':
			case 'zdel':
			case 'hdel':
			case 'multi_set':
			case 'multi_del':
				return new SSDB_Response($resp[0], $resp[1]);
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
			case 'get':
			case 'zget':
			case 'hget':
			case 'hsize':
			case 'zsize':
				if($resp[0] == 'ok'){
					if(count($resp) == 2){
						return new SSDB_Response('ok', $resp[1]);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response($resp[0], $resp[1]);
				}
				break;
			case 'keys':
			case 'zkeys':
			case 'hkeys':
			case 'hlist':
			case 'zlist':
				$data = array();
				if($resp[0] == 'ok'){
					for($i=1; $i<count($resp); $i++){
						$data[] = $resp[$i];
					}
				}
				return new SSDB_Response($resp[0], $data);
			case 'scan':
			case 'rscan':
			case 'zscan':
			case 'zrscan':
			case 'hscan':
			case 'hrscan':
			case 'multi_get':
				if($resp[0] == 'ok'){
					if(count($resp) % 2 == 1){
						$data = array();
						for($i=1; $i<count($resp); $i+=2){
							$data[$resp[$i]] = $resp[$i + 1];
						}
						return new SSDB_Response('ok', $data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response($resp[0]);
				}
				break;
			default:
				return new SSDB_Response($resp[0], array_slice($resp, 1));
		}
		return new SSDB_Response('error', 'Unknown command: $cmd');
	}

	private function send($data){
		$ps = array();
		foreach($data as $p){
			$ps[] = strlen($p);
			$ps[] = $p;
		}
		$s = join("\n", $ps) . "\n\n";
		if($this->debug){
			echo '> ' . str_replace(array("\r", "\n"), array('\r', '\n'), $s) . "\n";
		}
		try{
			while(true){
				$ret = socket_write($this->sock, $s);
				if($ret == 0){
					return false;
				}
				$s = substr($s, $ret);
				if(strlen($s) == 0){
					break;
				}
			}
		}catch(Exception $e){
			return false;
		}
		return $ret;
	}

	private function recv(){
		while(true){
			$ret = $this->parse();
			if($ret === null){
				try{
					$data = socket_read($this->sock, 1024*128);
					if($this->debug){
						echo '< ' . str_replace(array("\r", "\n"), array('\r', '\n'), $data) . "\n";
					}
				}catch(Exception $e){
					$data = '';
				}
				if(strlen($data) == 0){
					$this->close();
					return array();
				}
				$this->recv_buf .= $data;
			}else{
				return $ret;
			}
		}
	}

	private function parse(){
		//if(len($this->recv_buf)){print 'recv_buf: ' + repr($this->recv_buf);}
		$ret = array();
		$spos = 0;
		$epos = 0;
		$this->recv_buf = ltrim($this->recv_buf);
		while(true){
			$spos = $epos;
			$epos = strpos($this->recv_buf, "\n", $spos);
			if($epos === false){
				break;
			}
			$epos += 1;
			$line = substr($this->recv_buf, $spos, $epos - $spos);
			$spos = $epos;

			$line = trim($line);
			if(strlen($line) == 0){ // head end
				$this->recv_buf = substr($this->recv_buf, $spos);
				return $ret;
			}

			$num = intval($line);
			$epos = $spos + $num;
			if($epos > strlen($this->recv_buf)){
				break;
			}
			$data = substr($this->recv_buf, $spos, $epos - $spos);
			$ret[] = $data;

			$spos = $epos;
			$epos = strpos($this->recv_buf, "\n", $spos);
			if($epos === false){
				break;
			}
			$epos += 1;
		}

		return null;
	}
}
