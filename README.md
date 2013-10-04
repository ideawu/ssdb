# SSDB - A LevelDB server with zset data type

SSDB is a high performace key-value(key-string, key-zset, key-hashmap) NoSQL persistent storage server, using Google LevelDB as storage engine. 

SSDB is stable, production-ready and is widely used by many Internet companies such as QIHU 360.

## Features

* LevelDB client-server support, written in C/C++
* Designed to store collection data
* Persistent key-value, key-zset, key-map('hashmap') storage
* Client API supports including [C/C++](https://github.com/ideawu/ssdb/wiki/Documentation_C_API), [PHP](http://www.ideawu.com/ssdb/docs/php/), Python, Cpy, [Java](http://www.ideawu.com/ssdb/java-doc/), [nodejs](https://github.com/ideawu/ssdb/wiki/Documentation_nodejs_API), [Ruby](https://github.com/bsm/ssdb-rb)
* Online backup, fast recover
* **Replication(master-slave), load balance** [\[see wiki Replication\]](https://github.com/ideawu/ssdb/wiki/Replication)
* <i>Future Features</i>
  * <i>Distributed</i>

## PHP client API example

```php
<?php
require_once('SSDB.php');
$ssdb = new SimpleSSDB('127.0.0.1', 8888);
$resp = $ssdb->set('key', '123');
$resp = $ssdb->get('key');
echo $resp; // output: 123
```

[More...](http://www.ideawu.com/ssdb/docs/php/)

## Compile and Install

```sh
$ wget --no-check-certificate https://github.com/ideawu/ssdb/archive/master.zip
$ unzip master
$ cd ssdb-master
$ make
$ #optional, install ssdb in /usr/local/ssdb
$ sudo make install

# start master
$ ./ssdb-server ssdb.conf

# or start as daemon
$ ./ssdb-server -d ssdb.conf

# start slave
$ ./ssdb-server ssdb_slave.conf

# ssdb command line
$ ./tools/ssdb-cli -p 8888

# stop ssdb-server
$ kill `cat ./var/ssdb.pid`
```

See [Compile and Install wiki](https://github.com/ideawu/ssdb/wiki/Compile_and_Install)

## Architecture

![ssdb architecture](https://a248.e.akamai.net/camo.github.com/305d969fb81bb1f1c73eb205e4afa096c9b2b8c0/687474703a2f2f7777772e6964656177752e636f6d2f737364622f737364622e706e67)

## Performance

### Typical performance

Total 1000 requests.

```
writeseq  :    0.546 ms/op      178.7 MB/s
writerand :    0.519 ms/op      188.1 MB/s
readseq   :    0.304 ms/op      321.6 MB/s
readrand  :    0.310 ms/op      315.0 MB/s
```

### SSDB vs Redis

![Benchmark vs Redis](https://a248.e.akamai.net/camo.github.com/e4f078b24ac603f4af874c3fbac6f9908d521e20/687474703a2f2f7777772e6964656177752e636f6d2f737364622f737364622d76732d72656469732e706e67)

[View full SSDB vs Redis benchmark charts...](http://www.ideawu.com/ssdb/)

### Concurrency benchmark

```
======= set =======
qps: 31852, time: 0.031 s

======= get =======
qps: 29103, time: 0.034 s

======= del =======
qps: 33042, time: 0.030 s

======= scan =======
qps: 25871, time: 0.039 s

======= rscan =======
qps: 31181, time: 0.032 s

======= zset =======
qps: 24352, time: 0.041 s

======= zget =======
qps: 27836, time: 0.036 s

======= zscan =======
qps: 10442, time: 0.096 s

======= zrscan =======
qps: 28224, time: 0.035 s

======= zdel =======
qps: 27137, time: 0.037 s

======= hset =======
qps: 31507, time: 0.032 s

======= hget =======
qps: 28388, time: 0.035 s

======= hscan =======
qps: 25319, time: 0.039 s

======= hrscan =======
qps: 30399, time: 0.033 s

======= hdel =======
qps: 29741, time: 0.034 s
```

See Benchmark 

## Who's using SSDB?

See [Users wiki](https://github.com/ideawu/ssdb/wiki/Users)

## Windows executable

Download ssdb-server.exe from here: https://github.com/ideawu/ssdb-bin


## Links

* [Author's homepage](http://www.ideawu.com/blog/)
* [Cpy Scripting Language](https://github.com/ideawu/cpy)
* [Google LevelDB](https://code.google.com/p/leveldb/)
* [Lua ssdb client driver for the ngx_lua](https://github.com/LazyZhu/lua-resty-ssdb)
* [Yet another ssdb client for Python](https://github.com/ifduyue/pyssdb)
* [SSDB 中文文档](http://www.ideawu.net/blog/category/ssdb)

## Changes made to LevelDB

See [Changes-Made-to-LevelDB wiki](https://github.com/ideawu/ssdb/wiki/Changes-Made-to-LevelDB)



