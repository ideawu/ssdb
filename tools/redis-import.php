<?php
/**
 * Copyright (c) 2014, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * PHP script for importing Redis data into SSDB.
 */

include(dirname(__FILE__) . '/../api/php/SSDB.php');

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

echo "This script will only copy entries with types in (STRING, HASH, ZSET)\n";
echo "Do you want to continue? [y/n] ";
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

echo "\nCopying data from Redis($r_host:{$r_port}[$r_db]) to SSDB($s_host, $s_port)...\n\n";


$redis = new Redis();
$redis->connect($r_host, $r_port);
$redis->select($r_db);

$ssdb = new Redis();
$ssdb->connect($s_host, $s_port);

$keys = $redis->keys('*');
$count = 0;
$total = count($keys);
foreach($keys as $key){
	if(++$count % 100 == 1){
		echo "$count/$total\n";
	}
	$type = $redis->type($key);
	switch($type){
		case Redis::REDIS_STRING:
			$val = $redis->get($key);
			$ssdb->set($key, $val);
			break;
		/*
		case Redis::REDIS_LIST:
			$list = $redis->lRange($key, 0, -1);
			foreach($list as $val){
				$$ssdb->rPush($key, $val);
			}
			break;
		*/
		case Redis::REDIS_HASH:
			$hash = $redis->hGetAll($key);
			$ssdb->hMSet($key, $hash);
			break;
		case Redis::REDIS_ZSET:
			$zset = $redis->zRange($key, 0, -1, true);
			foreach($zset as $val=>$score){
				$ssdb->zAdd($key, $score, $val);
			}
			break;
	}
}

echo "\n$total total copied.\n";
echo "Done.\n\n";

