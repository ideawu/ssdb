<?php
/**
 * Copyright (c) 2017, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * Demonstrate how to iterate over the whole db.
 */

include(dirname(__FILE__) . '/../api/php/SSDB.php');

$host = '127.0.0.1';
$port = 8888;
$ssdb = new SimpleSSDB($host, $port);

$size = 1000;

// KV
$s_key = '';
while(1){
	$kvs = $ssdb->scan($s_key, '', $size);
	if(!$kvs){
		break;
	}
	foreach($kvs as $k=>$v){
		$s_key = $k;
		// do your stuff
		echo "KV: $k\n";
	}
}


// HASH
$s_name = '';
while(1){
	$names = $ssdb->hlist($s_name, '', $size);
	if(!$names){
		break;
	}
	foreach($names as $name){
		$s_name = $name;

		$s_key = '';
		while(1){
			$kvs = $ssdb->hscan($name, $s_key, '', $size);
			if(!$kvs){
				break;
			}
			foreach($kvs as $k=>$v){
				$s_key = $k;
				// do your stuff
				echo "HASH: $name - $k\n";
			}
		}
	}
}


// ZSET
$s_name = '';
while(1){
	$names = $ssdb->zlist($s_name, '', $size);
	if(!$names){
		break;
	}
	foreach($names as $name){
		$s_name = $name;

		$s_key = '';
		$s_score = '';
		while(1){
			$kvs = $ssdb->zscan($name, $s_key, $s_score, '', $size);
			if(!$kvs){
				break;
			}
			foreach($kvs as $k=>$v){
				$s_key = $k;
				$s_score = $v;
				// do your stuff
				echo "ZSET: $name - $k : $v\n";
			}
		}
	}
}


// QUEUE/LIST
$s_name = '';
while(1){
	$names = $ssdb->qlist($s_name, '', $size);
	if(!$names){
		break;
	}
	foreach($names as $name){
		$s_name = $name;

		$start = 0;
		while(1){
			$kvs = $ssdb->qrange($name, $start, $size);
			if(!$kvs){
				break;
			}
			foreach($kvs as $k=>$v){
				$start ++;
				$index = $start - 1;
				// do your stuff
				echo "LIST: $name - [$index]\n";
			}
		}
	}
}
