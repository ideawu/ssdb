<?php
/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB PHP API demo.
 */

include('SSDB.php');
$host = '127.0.0.1';
$port = 8888;


try{
	$ssdb = new SSDB($host, $port);
}catch(Exception $e){
	die(__LINE__ . ' ' . $e->getMessage());
}

echo $ssdb->set('test', date('Y-m-d H:i:s')) . "\n";
echo $ssdb->get('test') . "\n";
echo $ssdb->incr('test', 1) . "\n";
echo $ssdb->decr('test', 1) . "\n";
echo $ssdb->scan('a', 'z', 10) . "\n";
echo $ssdb->rscan('z', 'a', 10) . "\n";
echo $ssdb->keys('a', 'z', 10) . "\n";
echo $ssdb->del('test') . "\n";
echo $ssdb->get('test') . "\n";
echo "\n";
echo $ssdb->zset('test', 'a', time()) . "\n";
echo $ssdb->zget('test', 'a') . "\n";
echo $ssdb->zincr('test', 'a', 1) . "\n";
echo $ssdb->zdecr('test', 'a', 1) . "\n";
echo $ssdb->zscan('test', 'a', time() - 100, time() + 10, 10) . "\n";
echo $ssdb->zrscan('test', 'a', '', time() - 100, time() + 10) . "\n";
echo $ssdb->zkeys('test', 'a', time() - 100, time() + 10, 10) . "\n";
echo $ssdb->zdel('test', 'a') . "\n";
echo $ssdb->zget('test', 'a') . "\n";
echo "\n";
echo $ssdb->hset('test', 'b', time()) . "\n";
echo $ssdb->hget('test', 'b') . "\n";
echo $ssdb->hincr('test', 'b', 1) . "\n";
echo $ssdb->hdecr('test', 'b', 1) . "\n";
echo $ssdb->hscan('test', 'a', 'z', 10) . "\n";
echo $ssdb->hrscan('test', 'z', 'a', 10) . "\n";
echo $ssdb->hkeys('test', 'a', 'z', 10) . "\n";
echo $ssdb->hdel('test', 'b') . "\n";
echo $ssdb->hget('test', 'b') . "\n";
echo "\n";
echo $ssdb->multi_set(array('a' => '1', 'b' => 2)) . "\n";
echo $ssdb->multi_get(array('a', 'b')) . "\n";
echo $ssdb->multi_del(array('a', 'b')) . "\n";
echo $ssdb->multi_get(array('a', 'b')) . "\n";

$ssdb->close();

die();

/* a simple bench mark */

$data = array();
for($i=0; $i<1000; $i++){
	$k = '' . mt_rand(0, 100000);
	$v = mt_rand(100000, 100000 * 10 - 1) . '';
	$data[$k] = $v;
}

speed();
try{
	$ssdb = new SSDB($host, $port);
}catch(Exception $e){
	die(__LINE__ . ' ' . $e->getMessage());
}
foreach($data as $k=>$v){
	$ret = $ssdb->set($k, $v);
	if($ret === false){
		echo "error\n";
		break;
	}
}
$ssdb->close();
speed('set speed: ', count($data));


speed();
try{
	$ssdb = new SSDB($host, $port);
}catch(Exception $e){
	die(__LINE__ . ' ' . $e->getMessage());
}
foreach($data as $k=>$v){
	$ret = $ssdb->get($k);
	if($ret === false){
		echo "error\n";
		break;
	}
}
$ssdb->close();
speed('get speed: ', count($data));



function speed($msg=null, $count=0){
	static $stime;
	if(!$msg && !$count){
		$stime = microtime(1);
	}else{
		$etime = microtime(1);
		$ts = ($etime - $stime == 0)? 1 : $etime - $stime;
		$speed = $count / floatval($ts);
		$speed = sprintf('%.2f', $speed);
		echo "$msg: " . $speed . "\n";

		$stime = $etime;
	}
}
