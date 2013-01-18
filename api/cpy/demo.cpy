/**
 * Copyright (c) 2012, ideawu
 * All rights reserved.
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *
 * SSDB cpy API demo.
 */

import SSDB.SSDB;

try{
	ssdb = new SSDB('127.0.0.1', 8888);
}catch(Exception e){
	print e;
	sys.exit(0);
}

print(ssdb.request('set', ['test', '123']));
print(ssdb.request('get', ['test']));
print(ssdb.request('incr', ['test', '1']));
print(ssdb.request('decr', ['test', '1']));
print(ssdb.request('scan', ['a', 'z', 10]));
print(ssdb.request('rscan', ['z', 'a', 10]));
print(ssdb.request('keys', ['a', 'z', 10]));
print(ssdb.request('del', ['test']));
print(ssdb.request('get', ['test']));
print "\n";
print(ssdb.request('zset', ['test', 'a', 20]));
print(ssdb.request('zget', ['test', 'a']));
print(ssdb.request('zincr', ['test', 'a', 20]));
print(ssdb.request('zdecr', ['test', 'a', 20]));
print(ssdb.request('zscan', ['test', 'a', 0, 100, 10]));
print(ssdb.request('zrscan', ['test', 'a', 100, 0, 10]));
print(ssdb.request('zkeys', ['test', 'a', 0, 100, 10]));
print(ssdb.request('zdel', ['test', 'a']));
print(ssdb.request('zget', ['test', 'a']));
print "\n";
print(ssdb.request('hset', ['test', 'a', 20]));
print(ssdb.request('hget', ['test', 'a']));
print(ssdb.request('hincr', ['test', 'a', 20]));
print(ssdb.request('hdecr', ['test', 'a', 20]));
print(ssdb.request('hscan', ['test', '0', 'z', 10]));
print(ssdb.request('hrscan', ['test', 'z', '0', 10]));
print(ssdb.request('hkeys', ['test', '0', 'z', 10]));
print(ssdb.request('hdel', ['test', 'a']));
print(ssdb.request('hget', ['test', 'a']));
print "\n";
