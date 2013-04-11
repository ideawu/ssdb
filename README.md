# SSDB - A LevelDB server with zset data type

SSDB, also known as zdb, is a fast key-value(key-string, key-zset, key-hashmap) NoSQL persistent storage server, using Google LevelDB as storage engine. 

## Features

* LevelDB client-server support, written in C/C++
* Persistent key-value, key-zset, key-hashmap storage
* Client API supports including [PHP](https://github.com/ideawu/ssdb/wiki/Documentation_PHP_API), Python, Cpy, [Java](https://github.com/ideawu/ssdb/wiki/Documentation_Java_API)
* Online backup, fast recover
* **Replication(master-slave), load balance** [\[see wiki Replication\]](https://github.com/ideawu/ssdb/wiki/Replication)
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

### Typical performance

Total 1000 requests.

```
writeseq  :    0.546 ms/op      178.7 MB/s
writerand :    0.519 ms/op      188.1 MB/s
readseq   :    0.304 ms/op      321.6 MB/s
readrand  :    0.310 ms/op      315.0 MB/s
```

### SSDB vs Redis

![Benchmark vs Redis](http://www.ideawu.com/ssdb/ssdb-vs-redis.png)

[View full SSDB vs Redis benchmark charts...](http://www.ideawu.com/ssdb/)

### Concurrency benchmark

```
======= set =======
qps: 31852, time: 0.031 s

======= get =======
qps: 29103, time: 0.034 s

======= scan =======
qps: 25871, time: 0.039 s

======= rscan =======
qps: 31181, time: 0.032 s

======= del =======
qps: 33042, time: 0.030 s

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

See [Users](https://github.com/ideawu/ssdb/wiki/Users)

## Links

* [Author's homepage](http://www.ideawu.com/blog/)
* [SSDB 中文文档](http://www.ideawu.net/blog/category/ssdb)
* [Cpy Scripting Language](https://code.google.com/p/cpy-scripting-language/)
* [Google LevelDB](https://code.google.com/p/leveldb/)
* [Lua ssdb client driver for the ngx_lua](https://github.com/LazyZhu/lua-resty-ssdb)
* [Yet another ssdb client for Python](https://github.com/ifduyue/pyssdb)
