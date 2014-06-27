<?php
/**
 * Copyright (c) 2014, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * PHP script for importing Redis data into SSDB.
 */

function usage(){
	global $argv;
	echo "Usage:\n";
	echo "    php {$argv[0]} redis_host redis_port redis_db ssdb_host ssdb_port\n";
	echo "\n";
}

if(count($argv) != 6){
	usage();
	die();
}

echo "This script will only copy entries with types in (STRING, HASH, ZSET, LIST)\n";
echo "ZSET scores are converted to intergers from floating numbers.\n";
echo "Do you want to continue? [y/n](y) ";
$line = fgets(STDIN);
$line = trim($line);
if($line == 'n' || $line == 'N'){
	echo "Operation cancelled\n";
	die();
}

$r_host = $argv[1];
$r_port = $argv[2];
$r_db   = $argv[3];
$s_host = $argv[4];
$s_port = $argv[5];

$redis = new Redis();
$ret = $redis->connect($r_host, $r_port);
if($ret === false){
	echo "ERROR: could not connect to Redis server!\n";
	die();
}
$redis->select($r_db);

$ssdb = new Redis();
$ret = $ssdb->connect($s_host, $s_port);
if($ret === false){
	echo "ERROR: could not connect to SSDB server!\n";
	die();
}


echo "\nCopying data from Redis($r_host:{$r_port}[$r_db]) to SSDB($s_host, $s_port)...\n";
if(scan_command_available()){
	echo "Using SCAN.\n";
}else{
	echo "Using KEYS.\n";
}


$count = 0;
$total = 0;
$entries = 0;

echo "==============\n";
// check if phpredis and redis-server supports SCAN
if(scan_command_available()){
	$total = $redis->dbsize();
	$it = NULL;
	$redis->setOption(Redis::OPT_SCAN, Redis::SCAN_RETRY);
	while($keys = $redis->scan($it)){
		copy_keys($keys);
	}
}else{
	$keys = $redis->keys('*');
	$total = count($keys);
	copy_keys($keys);
}
echo date('Y-m-d H:i:s') . " $total keys, $entries entries copied.\n";
echo "==============\n";
echo "Done.\n";
echo "\n";


function copy_keys($keys){
	global $redis, $ssdb, $count, $total, $entries;

	foreach($keys as $key){
		copy_key($key);
		if(++$count % 100 == 1){
			echo date('Y-m-d H:i:s') . " $count/$total entries: $entries\n";
		}
	}
}

function copy_key($key){
	global $redis, $ssdb, $count, $total, $entries;

	$type = $redis->type($key);
	switch($type){
		case Redis::REDIS_STRING:
			$val = $redis->get($key);
			$ssdb->set($key, $val);
			$entries ++;
			break;
		case Redis::REDIS_LIST:
			$list = $redis->lRange($key, 0, -1);
			foreach($list as $val){
				$ssdb->rPush($key, $val);
				$entries ++;
			}
			break;
		case Redis::REDIS_HASH:
			$hash = $redis->hGetAll($key);
			foreach($hash as $k=>$v){
				$ssdb->hset($key, $k, $v);
				$entries ++;
			}
			break;
		case Redis::REDIS_ZSET:
			$zset = $redis->zRange($key, 0, -1, true);
			foreach($zset as $val=>$score){
				$ssdb->zAdd($key, $score, $val);
				$entries ++;
			}
			break;
	}
}

function scan_command_available(){
	global $redis;

	if(method_exists($redis, 'scan')){
		$info = $redis->info();
		$redis_version = $info['redis_version'];
		$ps = explode('.', $redis_version);
		if(count($ps) > 2){
			$n = $ps[0] * 10 + $ps[1];
			if($n >= 28){
				return true;
			}
		}
	}
	return false;
}

