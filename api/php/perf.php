<?php
require_once("SSDB.php");
$ssdb = new SimpleSSDB('127.0.0.1', 8888);

$DATA_LEN = 100 * 1024;

$str = str_pad('', $DATA_LEN);
$resp = $ssdb->set('key', $str);


$keys = array(
		'seq' => array(),
		);
for($i=0; $i<1000; $i++){
	$key = sprintf('%010s', $i);
	$keys['seq'][] = $key;
}

$REQUESTS = 1000;
$stime = 0;
$etime = 0;


start();
foreach($keys['seq'] as $key){
	$resp = $ssdb->set($key, $str);
}
output('writeseq');

$ks = $keys['seq'];
shuffle($ks);
start();
foreach($ks as $key){
	$resp = $ssdb->set($key, $str);
}
output('writerand');

start();
foreach($keys['seq'] as $key){
	$resp = $ssdb->get($key);
	if(strlen($resp) != $DATA_LEN){
		echo "$key ERROR!\n";
		die();
	}
}
output('readseq');


$ks = $keys['seq'];
shuffle($ks);
start();
foreach($ks as $key){
	$resp = $ssdb->get($key);
	if(strlen($resp) != $DATA_LEN){
		echo "$key ERROR!\n";
		die();
	}
}
output('readrand');




function start(){
	global $stime, $etime, $DATA_LEN, $REQUESTS;
	$stime = microtime(1);
}

function output($op){
	global $stime, $etime, $DATA_LEN, $REQUESTS;
	$etime = microtime(1);
	$time_consumed = $etime - $stime;
	$tpr = $time_consumed/$REQUESTS * 1000;
	$sps = ($REQUESTS * $DATA_LEN)/$time_consumed/1024/1024;
	printf("%-10s: %8s ms/op %10.1f MB/s\n", $op, number_format($tpr, 3), $sps);// . "ms/op\n";
}

