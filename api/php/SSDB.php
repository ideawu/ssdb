<?php
/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB PHP client SDK.
 */

class SSDBException extends Exception
{
}

/**
 * All methods(except *exists) returns false on error,
 * so one should use Identical(if($ret === false)) to test the return value.
 */
class SimpleSSDB extends SSDB
{
	function __construct($host, $port, $timeout_ms=2000){
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

	function __construct($host, $port, $timeout_ms=2000){
		$timeout_f = (float)$timeout_ms/1000;
		$this->sock = @stream_socket_client("$host:$port", $errno, $errstr, $timeout_f);
		if(!$this->sock){
			throw new SSDBException("$errno: $errstr");
		}
		$timeout_sec = intval($timeout_ms/1000);
		$timeout_usec = ($timeout_ms - $timeout_sec * 1000) * 1000;
		@stream_set_timeout($this->sock, $timeout_sec, $timeout_usec);
		if(function_exists('stream_set_chunk_size')){
			@stream_set_chunk_size($this->sock, 1024 * 1024);
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
			@fclose($this->sock);
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
	
	function request(){
		$args = func_get_args();
		$cmd = array_shift($args);
		return $this->__call($cmd, $args);
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

		try{
			if($this->send_req($cmd, $params) === false){
				$resp = new SSDB_Response('error', 'send error');
			}else{
				$resp = $this->recv_resp($cmd);
			}
		}catch(SSDBException $e){
			if($this->_easy){
				throw $e;
			}else{
				$resp = new SSDB_Response('error', $e->getMessage());
			}
		}
		$resp = $this->check_easy_resp($cmd, $resp);
		return $resp;
	}

	private function check_easy_resp($cmd, $resp){
		$this->last_resp = $resp;
		if($this->_easy){
			if($resp->not_found()){
				return NULL;
			}else if(!$resp->ok() && !is_array($resp->data)){
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

	function multi_hexists($name, $keys=array()){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function multi_zexists($name, $keys=array()){
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

	function setx($key, $val, $ttl){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function incr($key, $val=1){
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

	function zexists($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zdel($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zrange($name, $offset, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zclear($name){
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

	function zincr($name, $key, $score=1){
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

	function zrank($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zrrank($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zRevRank($name, $key){
		$args = func_get_args();
		return $this->__call("zrrank", $args);
	}

	function zrrange($name, $offset, $limit){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function zRevRange($name, $offset, $limit){
		$args = func_get_args();
		return $this->__call("zrrange", $args);
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

	function hexists($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hdel($name, $key){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hclear($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function hgetall($name){
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

	function hincr($name, $key, $val=1){
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
	
	/*****/

	function qfront($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function qback($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function qpop($name){
		$args = func_get_args();
		return $this->__call(__FUNCTION__, $args);
	}

	function qpush($name, $item){
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
			case 'getbit':
			case 'setbit':
			case 'countbit':
			case 'strlen':
			case 'set':
			case 'setx':
			case 'setnx':
			case 'zset':
			case 'hset':
			case 'qpush':
			case 'qpush_front':
			case 'qpush_back':
			case 'del':
			case 'zdel':
			case 'hdel':
			case 'hsize':
			case 'zsize':
			case 'qsize':
			case 'hclear':
			case 'zclear':
			case 'qclear':
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
			case 'zrank':
			case 'zrrank':
			case 'zcount':
			case 'zsum':
			case 'zremrangebyrank':
			case 'zremrangebyscore':
				$val = isset($resp[1])? intval($resp[1]) : 0;
				return new SSDB_Response($resp[0], $val);
			case 'zavg':
				$val = isset($resp[1])? floatval($resp[1]) : (float)0;
				return new SSDB_Response($resp[0], $val);
			case 'get':
			case 'substr':
			case 'getset':
			case 'hget':
			case 'qget':
			case 'qfront':
			case 'qback':
			case 'qpop':
			case 'qpop_front':
			case 'qpop_back':
				if($resp[0] == 'ok'){
					if(count($resp) == 2){
						return new SSDB_Response('ok', $resp[1]);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					$errmsg = isset($resp[1])? $resp[1] : '';
					return new SSDB_Response($resp[0], $errmsg);
				}
				break;
			case 'keys':
			case 'zkeys':
			case 'hkeys':
			case 'hlist':
			case 'zlist':
			case 'qslice':
				$data = array();
				if($resp[0] == 'ok'){
					for($i=1; $i<count($resp); $i++){
						$data[] = $resp[$i];
					}
				}
				return new SSDB_Response($resp[0], $data);
			case 'exists':
			case 'hexists':
			case 'zexists':
				if($resp[0] == 'ok'){
					if(count($resp) == 2){
						return new SSDB_Response('ok', (bool)$resp[1]);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					$errmsg = isset($resp[1])? $resp[1] : '';
					return new SSDB_Response($resp[0], $errmsg);
				}
				break;
			case 'multi_exists':
			case 'multi_hexists':
			case 'multi_zexists':
				if($resp[0] == 'ok'){
					if(count($resp) % 2 == 1){
						$data = array();
						for($i=1; $i<count($resp); $i+=2){
							$data[$resp[$i]] = (bool)$resp[$i + 1];
						}
						return new SSDB_Response('ok', $data);
					}else{
						return new SSDB_Response('server_error', 'Invalid response');
					}
				}else{
					return new SSDB_Response($resp[0]);
				}
				break;
			case 'scan':
			case 'rscan':
			case 'zscan':
			case 'zrscan':
			case 'zrange':
			case 'zrrange':
			case 'hscan':
			case 'hrscan':
			case 'hgetall':
			case 'multi_hsize':
			case 'multi_zsize':
			case 'multi_get':
			case 'multi_hget':
			case 'multi_zget':
				if($resp[0] == 'ok'){
					if(count($resp) % 2 == 1){
						$data = array();
						for($i=1; $i<count($resp); $i+=2){
							if($cmd[0] == 'z'){
								$data[$resp[$i]] = intval($resp[$i + 1]);
							}else{
								$data[$resp[$i]] = $resp[$i + 1];
							}
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
				$ret = @fwrite($this->sock, $s);
				if($ret == false){
					$this->close();
					throw new SSDBException('Connection lost');
				}
				$s = substr($s, $ret);
				if(strlen($s) == 0){
					break;
				}
				@fflush($this->sock);
			}
		}catch(Exception $e){
			$this->close();
			throw new SSDBException($e->getMessage());
		}
		return $ret;
	}

	private function recv(){
		while(true){
			$ret = $this->parse();
			if($ret === null){
				try{
					$data = @fread($this->sock, 1024 * 1024);
					if($this->debug){
						echo '< ' . str_replace(array("\r", "\n"), array('\r', '\n'), $data) . "\n";
					}
				}catch(Exception $e){
					$data = '';
				}
				if($data == false){
					$this->close();
					throw new SSDBException('Connection lost');
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
		// performance issue for large reponse
		//$this->recv_buf = ltrim($this->recv_buf);
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
