SSDB Go API Documentation {#mainpage}
============

@author: [ideawu](http://www.ideawu.com/)


## About

All SSDB operations go with ```ssdb.Client.Do()```, it accepts variable arguments. The first argument of Do() is the SSDB command, for example "get", "set", etc. The rest arguments(maybe none) are the arguments of that command.

The Do() method will return an array of string if no error. The first element in that array is the response code, ```"ok"``` means the following elements in that array(maybe none) are valid results. The response code may be ```"not_found"``` if you are calling "get" on an non-exist key.

Refer to the [PHP documentation](http://www.ideawu.com/ssdb/docs/php/) to checkout a complete list of all avilable commands and corresponding responses.


## Install

    go get -u github.com/ideawu/ssdb/api/go/ssdb


## Example

	package main
	
	import (
			"fmt"
			"os"

            "github.com/ideawu/ssdb/api/go/ssdb"
		   )
		   
	func main(){
		ip := "127.0.0.1";
		port := 8888;
		db, err := ssdb.Connect(ip, port);
		if(err != nil){
			os.Exit(1);
		}

        defer db.Close()
		
		var val interface{};
		db.Set("a", "xxx");
		val, err = db.Get("a");
		fmt.Printf("%s\n", val);
		db.Del("a");
		val, err = db.Get("a");
		fmt.Printf("%s\n", val);
		
		db.Do("zset", "z", "a", 3);
		db.Do("multi_zset", "z", "b", -2, "c", 5, "d", 3);
		resp, err := db.Do("zrange", "z", 0, 10);
		if err != nil{
			os.Exit(1);
		}
		if len(resp) % 2 != 1{
			fmt.Printf("bad response");
			os.Exit(1);
		}
		
		fmt.Printf("Status: %s\n", resp[0]);
		for i:=1; i<len(resp); i+=2{
			fmt.Printf("  %s : %3s\n", resp[i], resp[i+1]);
		}
		return;
	}

