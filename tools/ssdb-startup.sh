#!/bin/bash

ssdb_dir=/usr/local/ssdb
config=$ssdb_dir/ssdb.conf
pidfile=$ssdb_dir/var/ssdb.pid

start(){
	${ssdb_dir}/ssdb-server -d ${config}
	echo "ssdb started."
}

stop(){
	if [ ! -f "$pidfile" ]; then
		return
	fi  

	pid=`cat $pidfile`
	ps_pid=`ps aux | grep "$pid" | grep "$ssdb_dir" | grep -v grep | awk '{print $2}'`

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
