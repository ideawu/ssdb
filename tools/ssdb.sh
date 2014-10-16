#!/bin/sh
#
# chkconfig: 2345 64 36
# description: SSDB startup scripts
#
ssdb_root=/usr/local/ssdb
# each config file for one instance
# configs="/data/ssdb_data/test/ssdb.conf /data/ssdb_data/test2/ssdb.conf"
configs="/data/ssdb_data/test/ssdb.conf"

 
if [ -f /etc/rc.d/init.d/functions ]; then
	. /etc/rc.d/init.d/functions
fi
 
start() {
	for conf in $configs; do
		$ssdb_root/ssdb-ins.sh start $conf
	done
}
 
stop() {
	for conf in $configs; do
		$ssdb_root/ssdb-ins.sh stop $conf
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
    status)
        ;;
    restart)
        stop
        start
        ;;
    condrestart)
        ;;
    reload)
        ;;
    *)
        echo $"Usage: $0 {start|stop|status|restart|condrestart|reload}"
        ;;
esac
exit $RETVAL
