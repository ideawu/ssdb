#!/bin/bash
# this script is used to start/stop one ssdb-server instance

ssdb_root=/usr/local/ssdb

usage(){
	echo "Usage:"
	echo "    $0 {start|stop|restart} config"
}

if [ -z "$2" ]; then
	echo "[error] no config file specified!"
	usage;
	exit;
fi
config=$2
if [ ! -f "$config" ]; then
	echo "[error] bad config file: $config"
	exit;
fi

dir=`dirname $config`
pidfile=$dir/`cat $config | sed -n 's/pidfile[[:blank:]]*=[[:blank:]]//p' | sed -n 's/^\.\///p'`

start(){
	${ssdb_root}/ssdb-server -d ${config}
	if [ $? = "0" ]; then
		echo "ssdb started."
	else
		echo ""
		echo "[error] failed to start ssdb-server"
	fi
}

stop(){
	if [ ! -f "$pidfile" ]; then
		return
	fi  

	pid=`cat $pidfile`
	ps_pid=`ps aux | grep "$pid" | grep "$ssdb_root" | grep -v grep | awk '{print $2}'`

	if [ "$pid" = "$ps_pid" ]; then
		kill $pid
	fi  

	for((i=0; i<10; i++)) do
		sleep 0.5 
		if [ ! -f "$pidfile" ]; then
			break
		fi  
		echo -n "."
	done

	if [ -f "$pidfile" ]; then
		echo "remove obsolete pidfile $pidfile"
		rm -f "$pidfile"
	fi
	echo "ssdb stopped."
}


case "$1" in
	'start')
		stop
		start
		;;
	'stop')
		stop
		;;
	'restart')
		stop
		start
		;;
	*)
		echo "Usage: $0 {start|stop|restart}"
		exit 1
	;;
esac
