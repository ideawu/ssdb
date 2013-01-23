# SSDB - A LevelDB server with zset data type

SSDB, also known as zdb, is a fast key-value(key-string, key-zset, key-hashmap) NoSQL persistent storage server, using Google LevelDB as storage engine. 

## Features

* LevelDB client-server support, written in C/C++
* Persistent key-value, key-zset, key-hashmap storage([Commands](Commands))
* Client API supports including [PHP](https://github.com/ideawu/ssdb/wiki/Documentation_PHP_API), Python, Cpy
* Online backup, fast recover
* **Replication(master-slave), load balance**
* <i>Future Features</i>
  * <i>Distributed</i>

## PHP client API example

```php
<?php
require_once('SSDB.php');
$ssdb = new SSDB('127.0.0.1', 8888);
$resp = $ssdb->set('key', '123');
$resp = $ssdb->get('key');
echo $resp->data; // output: 123
```

[More...](https://github.com/ideawu/ssdb/wiki/Documentation_PHP_API)

## Performance

### SSDB vs Redis

![Benchmark vs Redis](http://www.ideawu.com/ssdb/ssdb-vs-redis.png)

[View full SSDB vs Redis benchmark charts...](http://www.ideawu.com/ssdb/)

### ssdb-bench

```
======= set =======
qps: 13639, time: 0.733 s

======= get =======
qps: 19944, time: 0.501 s

======= scan =======
qps: 18515, time: 0.540 s

======= del =======
qps: 21256, time: 0.470 s

======= zset =======
qps: 14894, time: 0.671 s

======= zget =======
qps: 19049, time: 0.525 s

======= zscan =======
qps: 8271, time: 1.209 s

======= zdel =======
qps: 15641, time: 0.639 s
```

See Benchmark 

## Compile and Install

```sh
$ make

# start master
$ ./ssdb-server ssdb.conf

# start slave
$ ./ssdb-server ssdb_slave.conf

# ssdb command line
$ ./tools/ssdb-cli
```

## Who's using SSDB?

See Users

## About

* [Author's homepage](http://www.ideawu.com/blog/)
* [SSDB 中文文档](http://www.ideawu.net/blog/category/ssdb)
* [Cpy Scripting Language](https://code.google.com/p/cpy-scripting-language/)
* [Google LevelDB](https://code.google.com/p/leveldb/)
