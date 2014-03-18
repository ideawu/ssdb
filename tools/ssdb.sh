#!/bin/bash
# this script is used to start/stop ssdb-server instances on
# system start/stop
# put this script into /etc/init.d directory

ssdb_root=/usr/local/ssdb
# each config file for one instance
# configs="/data/ssdb_data/test/ssdb.conf /data/ssdb_data/test2/ssdb.conf"
configs="/data/ssdb_data/test/ssdb.conf"

case "$1" in
	'start')
		for conf in $configs; do
			$ssdb_root/ssdb-ins.sh start $conf
		done
		;;
	'stop')
		for conf in $configs; do
			$ssdb_root/ssdb-ins.sh stop $conf
		done
		;;
	'restart')
		for conf in $configs; do
			$ssdb_root/ssdb-ins.sh restart $conf
		done
		;;
	*)
		echo "Usage: $0 {start|stop|restart}"
		exit 1
	;;
esac
