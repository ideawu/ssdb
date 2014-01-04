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
$host = 'baidu.com';
$host = '127.0.0.1';
$port = 8888;


try{
	$ssdb = new SimpleSSDB($host, $port, 1000);
	//$ssdb->easy();
}catch(Exception $e){
	die(__LINE__ . ' ' . $e->getMessage() . "\n");
}

while(1){
	$val = $ssdb->qpop('h');
	if($val === null){
		$size = $ssdb->qsize('h');
		echo $size . "\n";
		usleep(100 * 1000);
	}
}
die();

/*
$kvs = array();
for($i=0; $i<12345; $i++){
	$kvs[$i] = $i;
}
$chunks = array_chunk($kvs, 100, true);
foreach($chunks as $chunk){
	$key = mt_rand(0, 1000000);
	$ssdb->multi_set($chunk);
	$ssdb->multi_hset($key, $chunk);
	$ssdb->multi_zset($key, $chunk);
}
die();
*/

$val = str_repeat('123456789 ', 100000);
//$val = 'a';
for($i=0; $i<15008; $i++){
	$key = 'k' . $i;
	$key = mt_rand(0, 10000000);
	/*
	$ssdb->hset('h'.$i, $key, $val);
	$ssdb->zset('z'.$i, $key, $val);
	*/
	$ssdb->hset('h', $key, $val);
	$ssdb->zset('z', $key, $val);
	if(mt_rand() % 10 == 0){
		$key = mt_rand(0, 10000000);
		$ssdb->hdel('h', $key);
		$ssdb->zdel('z', $key);
	}
}

die();

$ret = $ssdb->multi_zset('h', array(
			'a' => 1,
			'b' => 2,
			));
var_dump($ret);
$ret = $ssdb->multi_zset('h', array(
			'a' => 1,
			'b' => 2,
			));
var_dump($ret);
$ret = $ssdb->multi_zget('h', array(
			'a',
			'b',
			'x',
			));
var_dump($ret);
$ret = $ssdb->multi_zdel('h', array(
			'a',
			'b',
			));
var_dump($ret);
$ret = $ssdb->multi_hdel('h', array(
			'a',
			'b',
			));
var_dump($ret);
die();

$val = str_repeat(time().'', 110);
echo 'strlen: ' . strlen($val) . "\n";

$ssdb->set('a', $val);
$stime = microtime(1);
$NUM = 1000;
for($i=0; $i<$NUM; $i++){
	$ret = $ssdb->get('a');
	if($ret != $val){
		echo "error\n";
		die();
	}
}
$etime = microtime(1);
$ts = $etime - $stime;
$speed = $NUM / $ts;
printf("speed: %f\n", $speed);
die();


$keys = $ssdb->keys("", "", 100);
var_dump($keys);
die();

$str = str_pad('', 344 * 1024);

$ret = $ssdb->set('test', $str);
for($i=0; $i<1000; $i++){
	$ret = $ssdb->set('test'.mt_rand(0, 100000000), $str);
}
var_dump($ret);
$ret = $ssdb->get('test');
echo md5($ret) . ' ' . strlen($ret) . "\n";
echo md5($str) . ' ' . strlen($str) . "\n";
if(md5($ret) !== md5($str)){
	echo "error!\n";
}

