<?php

	require_once('SSDB.php');

	$db = new SimpleSSDB('127.0.0.1', 8888);

	// load hash. Comment out on second run for faster test times
	for($i=0;$i<100000;$i++){
		if ($i > 0 && ($i % 10000) == 0)
			echo "Loaded $i entries...\n";

		$val = $i;

		// Test for null values too
		switch(rand(0,5)){
			case 0:
				$val = null;
				break;
			case 1:
				$val = "\0";
				break;
			case 2:
				$val = "";
				break;
			default:
				$val = $i;
		}
		$db->hSet('large-response-test-hash', "This is a hash key $i", $val);
	}

	$keys = count($db->hkeys('large-response-test-hash', '', '', 1000000000));
	echo "There are $keys keys in the response\n";

	// $db->hClear('large-response-test-hash');

