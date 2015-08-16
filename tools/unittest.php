<?php
/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * unit test.
 */

include(dirname(__FILE__) . '/../api/php/SSDB.php');

class SSDBTest extends UnitTest{
	private $ssdb;

	function __construct(){
		$host = '127.0.0.1';
		$port = 8888;
		$this->ssdb = new SimpleSSDB($host, $port);
		$this->ssdb->auth('very-strong-password-11111111111111111');
		$this->clear();
	}

	function clear(){
		$ssdb = $this->ssdb;
		$deleted = 0;
		while(1){
			$ret = $ssdb->scan('TEST_', 'TEST_'.pack('C', 255), 1000);
			if(!$ret){
				break;
			}
			foreach($ret as $k=>$v){
				$ssdb->del($k);
				$deleted += 1;
			}
		}
		while(1){
			$names = $ssdb->hlist('TEST_', 'TEST_'.pack('C', 255), 1000);
			if(!$names){
				break;
			}
			foreach($names as $name){
				$deleted += $ssdb->hclear($name);
				$ret = $ssdb->hsize($name);
				$this->assert($ret == 0);
			}
		}
		while(1){
			$names = $ssdb->zlist('TEST_', 'TEST_'.pack('C', 255), 1000);
			if(!$names){
				break;
			}
			foreach($names as $name){
				$deleted += $ssdb->zclear($name);
				$ret = $ssdb->zsize($name);
				$this->assert($ret == 0);
			}
		}
		while(1){
			$names = $ssdb->qlist('TEST_', 'TEST_'.pack('C', 255), 1000);
			if(!$names){
				break;
			}
			foreach($names as $name){
				$deleted += $ssdb->qclear($name);
				$ret = $ssdb->qsize($name);
				$this->assert($ret == 0);
			}
		}
		if($deleted > 0){
			echo "clear $deleted\n";
		}
	}

	function test_kv(){
		$ssdb = $this->ssdb;
		$val = str_repeat(mt_rand(), mt_rand(1, 100));
		
		$ssdb->del('TEST_a');
		$ret = $ssdb->ttl('TEST_a');
		$this->assert($ret === -1);
		$ret = $ssdb->expire('TEST_a', 10);
		$this->assert($ret === 0);
		$ssdb->set('TEST_a', $val);
		$ret = $ssdb->expire('TEST_a', 10);
		$this->assert($ret === 1);
		
		$ssdb->setx('TEST_a', $val, 1);
		$ret = $this->ssdb->get('TEST_a');
		$this->assert($ret === $val);
		usleep(1.5 * 1000 * 1000);
		$ret = $this->ssdb->get('TEST_a');
		$this->assert($ret === null);

		$ssdb->set('TEST_a', $val);
		$ssdb->set('TEST_b', $val);
		
		$ret = $this->ssdb->get('TEST_a');
		$this->assert($ret === $val);

		$ret = $ssdb->scan('TEST_', 'TEST_'.pack('C', 255), 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->scan('TEST_a', 'TEST_'.pack('C', 255), 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->scan('TEST_b', 'TEST_'.pack('C', 255), 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->scan('TEST_', 'TEST_a', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->scan('TEST_', 'TEST_b', 10);
		$this->assert(count($ret) == 2);

		$ret = $ssdb->rscan('TEST_'.pack('C', 255), 'TEST_', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->rscan('TEST_b', 'TEST_'.pack('C', 0), 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->rscan('TEST_a', 'TEST_'.pack('C', 0), 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->rscan('TEST_'.pack('C', 255), 'TEST_a', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->rscan('TEST_'.pack('C', 255), 'TEST_b', 10);
		$this->assert(count($ret) == 1);

		$ret = $ssdb->keys('TEST_', 'TEST_'.pack('C', 255), 10);
		$this->assert(count($ret) == 2);

		$kvs = array();
		for($i=0; $i<5; $i++){
			$kvs['TEST_' . $i] = $i;
			$ssdb->multi_set($kvs);
			$ret = $ssdb->multi_get(array_keys($kvs));
			$this->assert(count($ret) == count($kvs));
			$ret = $ssdb->multi_del(array_keys($kvs));
			$ret = $ssdb->multi_get(array_keys($kvs));
			$this->assert(count($ret) == 0);
		}

		$ret = $ssdb->exists('TEST_a');
		$this->assert($ret === true);
		$ssdb->del('TEST_a');
		$ret = $ssdb->exists('TEST_a');
		$this->assert($ret === false);
		$ret = $ssdb->get('TEST_a');
		$this->assert($ret === null);
		$ssdb->del('TEST_b');
		
		$ssdb->del('TEST_a');
		$ret = $ssdb->setnx('TEST_a', 'a');
		$this->assert($ret === 1);
		$ret = $ssdb->setnx('TEST_a', 't');
		$this->assert($ret === 0);
		$ret = $ssdb->get('TEST_a');
		$this->assert($ret === 'a');
		
		$ssdb->del('TEST_a');
		$ret = $ssdb->getset('TEST_a', 'a');
		$this->assert($ret === null);
		$ret = $ssdb->getset('TEST_a', 'b');
		$this->assert($ret === 'a');
		$ret = $ssdb->get('TEST_a');
		$this->assert($ret === 'b');

		$key = 'TEST_a';
		$ssdb->del($key);
		$ret = $ssdb->setbit($key, 8, 1);
		$this->assert($ret === 0);
		$ret = $ssdb->setbit($key, 8, 1);
		$this->assert($ret === 1);
		$ret = $ssdb->countbit($key, 0, 1);
		$this->assert($ret === 0);
		$ret = $ssdb->countbit($key, 0, 2);
		$this->assert($ret === 1);
		$ret = $ssdb->countbit($key, 0);
		$this->assert($ret === 1);
		$ret = $ssdb->strlen($key);
		$this->assert($ret === 2);
		$val = '0123456789';
		$ssdb->set($key, $val);
		$this->assert($ssdb->substr($key, 0, 1) === substr($val, 0, 1));
		$this->assert($ssdb->substr($key, -1, -1) === substr($val, -1, -1));
		$this->assert($ssdb->substr($key, 0, -1) === substr($val, 0, -1));
		$this->assert($ssdb->substr($key, -1, -2) === substr($val, -1, -2));
		$this->assert($ssdb->substr($key, -2, -1) === substr($val, -2, -1));
		$this->assert($ssdb->substr($key, -2, 2) === substr($val, -2, 2));
	}
	
	function test_queue(){
		$ssdb = $this->ssdb;
		$name = "TEST_" . str_repeat(mt_rand(), mt_rand(1, 3));
		$key = "TEST_" . str_repeat(mt_rand(), mt_rand(1, 3));
		$val = str_repeat(mt_rand(), mt_rand(1, 30));
				
		for($i=0; $i<7; $i++){
			$size = $ssdb->qpush($name, $i);
			$this->assert($size === $i + 1);
		}
		$size = $ssdb->qpush($name, array(7,8,9));
		$this->assert($size == 10);
		
		$ret = $ssdb->qget($name, 3);
		$this->assert($ret == 3);
		$ret = $ssdb->qslice($name, 0, -1);
		for($i=0; $i<10; $i++){
			$this->assert($ret[$i] == $i);
		}
		$ret = $ssdb->qsize($name);
		$this->assert($ret === 10);
		$ret = $ssdb->qfront($name);
		$this->assert($ret == 0);
		$ret = $ssdb->qback($name);
		$this->assert($ret == 9);
		for($i=0; $i<10; $i++){
			$ret = $ssdb->qpop($name);
			if($ret != $i){
				$this->assert(false);
				break;
			}
		}

		$ret = $ssdb->qfront($name);
		$this->assert($ret === null);
		$ret = $ssdb->qback($name);
		$this->assert($ret === null);
		
		$ssdb->qpush_back($name, 0);
		$ssdb->qpush_front($name, 9);
		$ret = $ssdb->qfront($name);
		$this->assert($ret == 9);
		$ret = $ssdb->qback($name);
		$this->assert($ret == 0);

		$ssdb->qclear($name);
		for($i=0; $i<7; $i++){
			$size = $ssdb->qpush_back($name, $i);
		}
		$ret = $ssdb->qpop_front($name, 2);
		$this->assert(is_array($ret));
		$this->assert(count($ret) == 2);
		$this->assert($ret[0] == 0);
		$this->assert($ret[1] == 1);
		
		$ret = $ssdb->qpop_back($name, 2);
		$this->assert(is_array($ret));
		$ret = $ssdb->qpop($name, 2);
		$this->assert(is_array($ret));

		$ssdb->qclear($name);
		for($i=0; $i<3; $i++){
			$ssdb->qpush_back($name, $i);
		}

		$ret = $ssdb->qset($name, 0, 'www');
		$this->assert($ret !== false);
		$ret = $ssdb->qset($name, 9990, 'www');
		$this->assert($ret === false);
		$ret = $ssdb->qget($name, 0);
		$this->assert($ret === 'www');

		$ret = $ssdb->qtrim_front($name, 2);
		$this->assert($ret === 2);
		$ret = $ssdb->qtrim_back($name, 2);
		$this->assert($ret === 1);
	}

	function test_hash(){
		$ssdb = $this->ssdb;
		$name = "TEST_" . mt_rand();
		$key = "TEST_" . mt_rand();
		$val = str_repeat(mt_rand(), mt_rand(1, 30));

		$ret = $ssdb->hsize($name);
		$this->assert($ret === 0);

		$ret = $ssdb->multi_hset($name, array('a' => 1, 'a' => 2));
		$this->assert($ret == 1);
		$ret = $ssdb->multi_hdel($name, array('a', 'a'));
		$this->assert($ret == 1);

		$ret = $ssdb->hset($name, $key, $val);
		$ret = $ssdb->hexists($name, $key);
		$this->assert($ret);
		$ret = $ssdb->hget($name, $key);
		$this->assert($ret === $val);

		$ret = $ssdb->hsize($name);
		$this->assert($ret === 1);
		$ret = $ssdb->hscan($name, '', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hrscan($name, '', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hkeys($name, '', '', 10);
		$this->assert(count($ret) == 1);

		$ret = $ssdb->hdel($name, $key);
		$ret = $ssdb->hsize($name);
		$this->assert($ret === 0);
		$ret = $ssdb->hscan($name, '', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->hrscan($name, '', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->hkeys($name, '', '', 10);
		$this->assert(count($ret) == 0);

		$ret = $ssdb->hset($name, 'a', $val);
		$ret = $ssdb->hset($name, 'b', $val);
		$ret = $ssdb->hscan($name, '', '', 10);
		$this->assert(count($ret) == 2);
		foreach($ret as $k=>$v){
			$this->assert($v === $val);
		}
		$ret = $ssdb->hscan($name, '', 'a', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hscan($name, '', 'b', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->hrscan($name, '', 'b', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hrscan($name, '', 'a', 10);
		$this->assert(count($ret) == 2);

		$ret = $ssdb->hscan($name, 'a', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hscan($name, 'b', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->hrscan($name, '', '', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->hrscan($name, 'b', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hrscan($name, 'a', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->hkeys($name, '', '', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->hkeys($name, 'a', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->hkeys($name, 'b', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->hdel($name, 'a');
		$ret = $ssdb->hdel($name, 'b');

		$ssdb->hset("TEST_a", 'a', 1);
		$ssdb->hset("TEST_b", 'a', 1);
		$ssdb->hset("TEST_c", 'a', 1);
		$ret = $ssdb->hlist("TEST_a", "TEST_b", 100);
		$this->assert(count($ret) == 1);
		$this->assert($ret[0] == "TEST_b");

		$ret = $ssdb->hexists('TEST_a', 'a');
		$this->assert($ret === true);
		$ssdb->hdel('TEST_a', 'a');
		$ret = $ssdb->hexists('TEST_a', 'a');
		$this->assert($ret === false);
		$ret = $ssdb->hget('TEST_a', 'a');
		$this->assert($ret === null);
	}

	function test_zset(){
		$ssdb = $this->ssdb;
		$name = "TEST_" . mt_rand();
		$key = "TEST_" . mt_rand();
		$val = mt_rand();

		$ret = $ssdb->zsize($name);
		$this->assert($ret === 0);

		$ret = $ssdb->multi_zset($name, array('a' => 1, 'a' => 2));
		$this->assert($ret == 1);
		$ret = $ssdb->multi_zdel($name, array('a', 'a'));
		$this->assert($ret == 1);

		$ret = $ssdb->zset($name, $key, $val);
		$ret = $ssdb->zexists($name, $key);
		$this->assert($ret);
		$ret = $ssdb->zget($name, $key);
		$this->assert($ret === $val);

		$ret = $ssdb->zsize($name);
		$this->assert($ret === 1);
		$ret = $ssdb->zscan($name, '', '', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->zrscan($name, '', '', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->zkeys($name, '', '', '', 10);
		$this->assert(count($ret) == 1);

		$ret = $ssdb->zdel($name, $key);
		$ret = $ssdb->zsize($name);
		$this->assert($ret === 0);
		$ret = $ssdb->zscan($name, '', '', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->zrscan($name, '', '', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->zkeys($name, '', '', '', 10);
		$this->assert(count($ret) == 0);

		$ret = $ssdb->zset($name, 'a', $val);
		$ret = $ssdb->zset($name, 'b', $val);

		$ret = $ssdb->zrank($name, 'aaaaaaaa');
		$this->assert($ret === null);
		$ret = $ssdb->zrank($name, 'a');
		$this->assert($ret != -1);
		$ret = $ssdb->zrrank($name, 'a');
		$this->assert($ret != -1);

		$ret = $ssdb->zrange($name, 0, 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->zrrange($name, 0, 10);
		$this->assert(count($ret) == 2);

		$ret = $ssdb->zscan($name, '', '', '', 10);
		$this->assert(count($ret) == 2);
		foreach($ret as $k=>$v){
			$this->assert($v == $val);
		}
		$ret = $ssdb->zscan($name, 'a', '', '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->zscan($name, 'b', '', '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->zrscan($name, '', '', '', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->zrscan($name, 'b', $val, '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->zrscan($name, 'a', $val, '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->zkeys($name, '', '', '', 10);
		$this->assert(count($ret) == 2);
		$ret = $ssdb->zkeys($name, 'a', $val, '', 10);
		$this->assert(count($ret) == 1);
		$ret = $ssdb->zkeys($name, 'b', $val, '', 10);
		$this->assert(count($ret) == 0);
		$ret = $ssdb->zdel($name, 'a');
		$ret = $ssdb->zdel($name, 'b');

		$ssdb->zset("TEST_a", 'a', 1);
		$ssdb->zset("TEST_b", 'a', 1);
		$ssdb->zset("TEST_c", 'a', 1);
		$ret = $ssdb->zlist("TEST_a", "TEST_b", 100);
		$this->assert(count($ret) == 1);
		$this->assert($ret[0] == "TEST_b");

		$ret = $ssdb->zexists('TEST_a', 'a');
		$this->assert($ret === true);
		$ssdb->zdel('TEST_a', 'a');
		$ret = $ssdb->zexists('TEST_a', 'a');
		$this->assert($ret === false);
		$ret = $ssdb->zget('TEST_a', 'a');
		$this->assert($ret === null);
		
		$ssdb->zclear($name);
		$ssdb->request('multi_zset', $name, 'a', '1', 'b', '2', 'c', '3', 'd', '4', 'e', '5');
		$ret = $ssdb->zcount($name, 2, 4);
		$this->assert($ret === 3);
		$ret = $ssdb->zsum($name, 2, 4);
		$this->assert($ret === 9);
		$ret = $ssdb->zavg($name, 2, 3);
		$this->assert($ret === 2.5);
		$ret = $ssdb->zRemRangeByScore($name, 4, 5);
		$this->assert($ret === 2);
		$ret = $ssdb->zRemRangeByRank($name, 1, 2);
		$this->assert($ret === 2);

		$ssdb->zclear($name);
		for($i=0; $i<10; $i++){
			$ssdb->zset($name, $i, $i);
		}
		$ret = $ssdb->zscan($name, '', 3, 10, 1);
		$vals = array_values($ret);
		$this->assert($vals[0] === 3);
		$ret = $ssdb->zscan($name, '3', 3, 10, 1);
		$vals = array_values($ret);
		$this->assert($vals[0] === 4);

		$ret = $ssdb->zrscan($name, '', 3, 1, 1);
		$vals = array_values($ret);
		$this->assert($vals[0] === 3);
		$ret = $ssdb->zrscan($name, '3', 3, 1, 1);
		$vals = array_values($ret);
		$this->assert($vals[0] === 2);

		$ssdb->zclear($name);
		for($i=0; $i<10; $i++){
			$ssdb->zset($name, $i, $i);
		}
		$ret = $ssdb->zpop_front($name, 2);
		$keys = array_keys($ret);
		$vals = array_values($ret);
		$this->assert($keys[0] === 0 && $vals[0] === 0);
		$this->assert($keys[1] === 1 && $vals[1] === 1);
		$ret = $ssdb->zpop_back($name, 2);
		$keys = array_keys($ret);
		$vals = array_values($ret);
		$this->assert($keys[0] === 9 && $vals[0] === 9);
		$this->assert($keys[1] === 8 && $vals[1] === 8);
	}
}

class UnitTest{
	private $result = array(
			'passed' => 0,
			'failed' => 0,
			'tests' => array(
				),
			);

	function run(){
		$class_name = get_class($this);
		$methods = get_class_methods($class_name);
		foreach($methods as $method){
			if(strpos($method, 'test_') === 0){
				$this->$method();
			}
		}
		$this->report();
		$this->clear();
	}

	function report(){
		$res = $this->result;
		printf("passed: %3d, failed: %3d\n", $res['passed'], $res['failed']);
		foreach($res['tests'] as $test){
			if($test[0] === false){
				printf("    Failed: %s:%d %s() %s\n", $test[2], $test[3], $test[1], $test[4]);
			}
		}
		if($res['failed']){
			printf("passed: %3d, failed: %3d\n", $res['passed'], $res['failed']);
		}
	}

	function assert($val, $desc=''){
		if($val === true){
			$this->result['passed'] ++;
		}else{
			$val = false;
			$this->result['failed'] ++;
		}
		$bt = debug_backtrace(false);
		$func = $bt[1]['function'];
		$file = basename($bt[1]['file']);
		$line = $bt[0]['line'];
		$this->result['tests'][] = array(
				$val, $func, $file, $line, $desc
				);
	}

}


$test = new SSDBTest();
$test->run();

