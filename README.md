# SSDB - A fast NoSQL database for storing big list of data

SSDB is a high performace key-value(key-string, key-zset, key-hashmap) NoSQL database, an alternative to Redis. 

SSDB is stable, production-ready and is widely used by many Internet companies including QIHU 360.

## Features

* LevelDB client-server support, written in C/C++
* Designed to store collection data
* Persistent key-value, key-zset, key-map('hashmap') storage
* Redis clients are supported
* Client API supports including [C++](http://www.ideawu.com/ssdb/docs/cpp/), [PHP](http://www.ideawu.com/ssdb/docs/php/), Python, Cpy, [Java](http://www.ideawu.com/ssdb/docs/java/), [nodejs](https://github.com/ideawu/ssdb/wiki/Documentation_nodejs_API), [Ruby](https://github.com/bsm/ssdb-rb), [Go](http://www.ideawu.com/ssdb/docs/go/)
* Persistent queue service
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

## Documentation

* [View online](http://www.ideawu.com/ssdb/)
* [Contribute to SSDB documentation project](https://github.com/ideawu/ssdb-docs)

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

# ssdb command line
$ ./tools/ssdb-cli -p 8888

# stop ssdb-server
$ kill `cat ./var/ssdb.pid`
```

See [Compile and Install wiki](http://www.ideawu.com/ssdb/docs/install.html)

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

![Benchmark vs Redis](http://www.ideawu.com/ssdb/ssdb-vs-redis.png?github)

[View full SSDB vs Redis benchmark charts...](http://www.ideawu.com/ssdb/)

### Concurrency benchmark

```
========== set ==========
qps: 44251, time: 0.226 s
========== get ==========
qps: 55541, time: 0.180 s
========== del ==========
qps: 46080, time: 0.217 s
========== hset ==========
qps: 42338, time: 0.236 s
========== hget ==========
qps: 55601, time: 0.180 s
========== hdel ==========
qps: 46529, time: 0.215 s
========== zset ==========
qps: 37381, time: 0.268 s
========== zget ==========
qps: 41455, time: 0.241 s
========== zdel ==========
qps: 38792, time: 0.258 s
```

Run on a 2013 MacBook Pro 13 inch with Retina display.

## Architecture

![ssdb architecture](http://www.ideawu.com/ssdb/ssdb.png)

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

## LICENSE

SSDB is licensed under [New BSD License](http://opensource.org/licenses/BSD-3-Clause), a very flexible license to use.

