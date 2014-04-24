<?php
/**
 * Copyright (c) 2014, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * PHP script for importing Redis data into SSDB.
 */

$r_host = '127.0.0.1';
$r_port = 6666;
$r_port = 6379;
$r_db = 0;

$redis = new Redis();
$redis->connect($r_host, $r_port);
$redis->select($r_db);

var_dump($redis->dbsize());

$scan_command_available = false;
if(method_exists($redis, 'scan')){
	$it = null;
	$tmp = $redis->scan($it);
	if($tmp){
		$scan_command_available = true;
	}
}
var_dump($scan_command_available);

$it = NULL;
$redis->setOption(Redis::OPT_SCAN, Redis::SCAN_RETRY);
while($arr_keys = $redis->scan($it)) {
	foreach($arr_keys as $str_key) {
		echo "Here is a key: $str_key\n";
	}
	echo "No more keys to scan!\n";
}
var_dump($arr_keys);

