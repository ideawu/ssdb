# SSDB - A fast NoSQL database for storing big list of data

[![Author](https://img.shields.io/badge/author-@ideawu-blue.svg?style=flat)](http://www.ideawu.net/) [![Platform](https://img.shields.io/badge/platform-Linux,%20BSD,%20OS%20X,%20Windows-green.svg?style=flat)](https://github.com/ideawu/ssdb) [![NoSQL](https://img.shields.io/badge/db-NoSQL-pink.svg?tyle=flat)](https://github.com/ideawu/ssdb) [![License](https://img.shields.io/badge/license-New%20BSD-yellow.svg?style=flat)](LICENSE)


SSDB is a high performace key-value(key-string, key-zset, key-hashmap) NoSQL database, __an alternative to Redis__.

SSDB is stable, production-ready and is widely used by many Internet companies including QIHU 360.

## Features

* LevelDB client-server support, written in C/C++
* Designed to store collection data
* Persistent key-value, key-zset, key-map('hashmap') storage
* Redis clients are supported
* Client API supports including C++, PHP, Python, Cpy, Java, nodejs, Ruby, Go([see all](http://ssdb.io/docs/clients.html))
* Persistent queue service
* **Replication(master-slave), load balance**
* GUI administration tool([phpssdbadmin](https://github.com/ssdb/phpssdbadmin))

## PHP client API example

```php
<?php
require_once('SSDB.php');
$ssdb = new SimpleSSDB('127.0.0.1', 8888);
$resp = $ssdb->set('key', '123');
$resp = $ssdb->get('key');
echo $resp; // output: 123
```

[More...](http://ssdb.io/docs/php/)


## Who's using SSDB?

![all-ssdb-users](http://ssdb.io/img/ssdb-users/all-ssdb-users.jpg)

[And more...](http://ssdb.io/docs/users.html)


## Documentation

* [View online](http://ssdb.io/docs/)
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
$ ./ssdb-server ssdb.conf -s stop
 # for older version
$ kill `cat ./var/ssdb.pid`
```

See [Compile and Install wiki](http://ssdb.io/docs/install.html)

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

![Benchmark vs Redis](http://ssdb.io/ssdb-vs-redis.png?github)

[View full SSDB vs Redis benchmark charts...](http://ssdb.io/)

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

![ssdb architecture](http://ssdb.io/ssdb.png)

## Windows executable

Download ssdb-server.exe from here: https://github.com/ideawu/ssdb-bin


## SSDB library for iOS

	make ios
	# ls ios/
	include/ libleveldb-ios.a libsnappy-ios.a libssdb-ios.a libutil-ios.a

Drag the static libraies files into your iOS project. Then add `ios/include` to your iOS project's __Header Search Paths__, which is set in __Build Settings__.

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

## Authors

@ideawu

## Thanks

* 刘建辉, liujianhui@gongchang.com
* wendal(陈镇铖), wendal1985@gmail.com, http://wendal.net 
