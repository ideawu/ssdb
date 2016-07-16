<?php
$host = '127.0.0.1';
$port = 8888;
$sock = @stream_socket_client("$host:$port", $errno, $errstr);
$s = "3\r\nget\n1\r\nk\r\n\r\n";
$s .= str_replace("\r\n", "\n", $s);

for($i=0; $i<strlen($s); $i++){
	fwrite($sock, $s[$i]);
	fflush($sock);
	usleep(100 * 1000);
	printf("write %d byte(s)\n", $i+1);
}


