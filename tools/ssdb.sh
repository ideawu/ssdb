#!/bin/bash
# 
# ssdb - this script starts and stops the ssdb-server instances
# 
# chkconfig: - 85 15
# description: SSDB is an NoSQL server 
# processname: ssdb-server
# config: /usr/local/ssdb/conf/ssdb.conf
# pidfile: /usr/local/ssdb/var/ssdb.pid

ulimit -SHn 102400

# Source function library.
. /etc/rc.d/init.d/functions

# Source networking configuration.
. /etc/sysconfig/network

# Check that networking is up.
[ "$NETWORKING" = "no" ] && exit 0

ssdb_daemon="/usr/local/ssdb/sbin/ssdb-server"
ssdb_root="/usr/local/ssdb"
prog=$(basename $ssdb_daemon)

# each config file for one instance
# configs="/data/ssdb_data/test/ssdb.conf /data/ssdb_data/test2/ssdb.conf"
configs="${ssdb_root}/conf/ssdb.conf"

pidfile="${ssdb_root}/var/ssdb.pid"

start() {
    [ -x ${ssdb_daemon} ] || exit 5
    [ -f ${configs} ] || exit 6
    echo -n $"Starting $prog: "
    daemon ${ssdb_daemon} -d ${configs}
    retval=$?
    echo
    [ $retval -eq 0 ] && touch $pidfile
    return $retval
}

stop() {
    echo -n $"Stopping $prog: "
    killproc $prog -QUIT
    retval=$?
    echo
    [ $retval -eq 0 ] && rm -f $pidfile
    return $retval
}

restart() {
    stop
    start
}

rh_status() {
    status $prog
}

rh_status_q() {
    rh_status >/dev/null 2>&1
}


case "$1" in
    start)
        rh_status_q && exit 0
        $1
    ;;
    stop)
        rh_status_q || exit 0
        $1
    ;;
    restart)
        $1
    ;;
    status)
        rh_status
    ;;
    *)
        echo $"Usage: $0 {start|stop|status|restart}"
        exit 2
esac