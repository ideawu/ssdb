#!/bin/sh
#
# chkconfig: 2345 64 36
# description: SSDB startup scripts
#
ssdb_root=/usr/local/ssdb
ssdb_bin=$ssdb_root/ssdb-server
# each config file for one instance
# configs="/data/ssdb_data/test/ssdb.conf /data/ssdb_data/test2/ssdb.conf"
configs="/data/ssdb_data/test/ssdb.conf"

 
if [ -f /etc/rc.d/init.d/functions ]; then
	. /etc/rc.d/init.d/functions
fi
 
start() {
	for conf in $configs; do
		$ssdb_bin $conf -s restart -d
	done
}
 
stop() {
	for conf in $configs; do
		$ssdb_bin $conf -s stop -d
	done
}
 
# See how we were called.
case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        start
        ;;
    *)
        echo $"Usage: $0 {start|stop|restart}"
        ;;
esac
exit $RETVAL
