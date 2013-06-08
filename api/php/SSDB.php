<?php
/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB PHP client SDK.
 */

class SimpleSSDB extends SSDB
{
	function __construct($host, $port, $timeout_ms=200){
		parent::__construct($host, $port, $timeout_ms);
		$this->easy();
	}
}

class SSDB_Response
{
	public $cmd;
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
		}else{
			$s = $this->message;
		}
		return sprintf('%-13s %12s %s', $this->cmd, $this->code, $s);
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
	private $_easy = false;
	public $last_resp = null;

	function __construct($host, $port, $timeout_ms=200){
		$this->sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if(!$this->sock){
			throw new Exception(socket_strerror(socket_last_error()));
		}
		$timeout_sec = intval($timeout_ms/1000);
		$timeout_ms = $timeout_ms - $timeout_sec * 1000;
        @socket_set_option($this->sock, SOL_SOCKET, SO_RCVTIMEO, array('sec'=>$timeout_sec, 'usec'=>$timeout_ms));
        @socket_set_option($this->sock, SOL_SOCKET, SO_SNDTIMEO, array('sec'=>$timeout_sec, 'usec'=>$timeout_ms));
		$ret = @socket_connect($this->sock, $host, $port);
		if(!$ret){
			throw new Exception(socket_strerror(socket_last_error()));
		}
		if(is_int(TCP_NODELAY)){
			socket_set_option($this->sock, SOL_TCP, TCP_NODELAY, 1);
		}
	}
	
	/**
	 * After this method invoked with yesno=true, all requesting methods
	 * will not return a SSDB_Response object.
	 * And some certain methods like get/zget will return false
	 * when response is not ok(not_found, etc)
	 */
	function easy(){
		$this->_easy = true;
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

	private $batch_mode = false;
	private $batch_cmds = array();

	function batch(){
		$this->batch_mode = true;
		$this->batch_cmds = array();
		return $this;
	}

	function multi(){
		return $this->batch();
	}

	function exec(){
		$ret = array();
		foreach($this->batch_cmds as $op){
			list($cmd, $params) = $op;
			$this->send_req($cmd, $params);
		}
		foreach($this->batch_cmds as $op){
			list($cmd, $params) = $op;
			$resp = $this->recv_resp($cmd);
			$resp = $this->check_easy_resp($cmd, $resp);
			$ret[] = $resp;
		}
		$this->batch_mode = false;
		$this->batch_cmds = array();
		return $ret;
	}

	function __call($cmd, $params=array()){
		$cmd = strtolower($cmd);
		// act like Redis::zAdd($key, $score, $value);
		if($cmd == 'zadd'){
			$cmd = 'zset';
			$t = $params[1];
			$params[1] = $params[2];
			$params[2] = $t;
		}

		if($this->batch_mode){
			$this->batch_cmds[] = array($cmd, $params);
			return $this;
		}

		if($this->send_req($cmd, $params) === false){
			$resp = new SSDB_Response('error', 'send error');
		}else{
			$resp = $this->recv_resp($cmd);
		}
		$resp = $this->check_easy_resp($cmd, $resp);
		return $resp;
	}

	private function check_easy_resp($cmd, $resp){
		$this->last_resp = $resp;
		if($this->_easy){
			if(!$resp->ok() && !is_array($resp->data)){
				return false;
			}else{
				return $resp->data;
			}
		}else{
			$resp->cmd = $cmd;
			return $resp;
		}
	}

	// all supported are listed, for documentation purpose

	function multi_set($kvs=array()){
		$args = array();
		foreach($kvs as $k=>$v){
			$args[] = $k;
			$args[] = $v;
		}
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_get($args=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_del($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_exists($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_hexists($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zexists($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_hsize($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zsize($keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_hget($name, $keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zget($name, $keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_hdel($name, $keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zdel($name, $keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_hset($name, $kvs=array()){
		$args = array($name);
		foreach($kvs as $k=>$v){
			$args[] = $k;
			$args[] = $v;
		}
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zset($name, $kvs=array()){
		$args = array($name);
		foreach($kvs as $k=>$v){
			$args[] = $k;
			$args[] = $v;
		}
		return $this->__call(__FUNCTION__, $args);
	}

	/**/

	function set($key, $val){
		$args = func_get_args();
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

	function exists($key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function get($key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function del($key){
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

	function zset($name, $key, $score){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	// for migrate from Redis::zAdd()
	function zadd($key, $score, $value){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zget($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zexists($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zdel($name, $key){
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

	function zincr($name, $key, $score){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zdecr($name, $key, $score){
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

	function hexists($name){
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
	
	private function send_req($cmd, $params){
		$req = array($cmd);
		foreach($params as $p){
			if(is_array($p)){
				$req = array_merge($req, $p);
			}else{
				$req[] = $p;
			}
		}
		return $this->send($req);
	}

	private function recv_resp($cmd){
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
			case 'hsize':
			case 'zsize':
			case 'exists':
			case 'hexists':
			case 'zexists':
			case 'multi_set':
			case 'multi_del':
			case 'multi_hset':
			case 'multi_hdel':
			case 'multi_zset':
			case 'multi_zdel':
			case 'incr':
			case 'decr':
			case 'zincr':
			case 'zdecr':
			case 'hincr':
			case 'hdecr':
			case 'zget':
				$val = isset($resp[1])? intval($resp[1]) : 0;
				return new SSDB_Response($resp[0], $val);
			case 'get':
			case 'hget':
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
			case 'multi_hsize':
			case 'multi_zsize':
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
			case 'multi_exists':
			case 'multi_hexists':
			case 'multi_zexists':
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
				$ret = @socket_write($this->sock, $s);
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
					$data = @socket_read($this->sock, 1024*128);
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
