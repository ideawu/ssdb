package main

import (
	"fmt"
	"os"

	"./ssdb"
)

func main() {
	ip := "127.0.0.1"
	port := 8888
	db, err := ssdb.Connect(ip, port)
	if err != nil {
		os.Exit(1)
	}

	var val interface{}
	db.Set("a", "xxx")
	val, err = db.Get("a")
	fmt.Printf("%s\n", val)
	db.Del("a")
	val, err = db.Get("a")
	fmt.Printf("%s\n", val)

	db.Do("zset", "z", "a", 3)
	db.Do("multi_zset", "z", "b", -2, "c", 5, "d", 3)
	resp, err := db.Do("zrange", "z", 0, 10)
	if err != nil {
		os.Exit(1)
	}
	if len(resp)%2 != 1 {
		fmt.Printf("bad response")
		os.Exit(1)
	}

	fmt.Printf("Status: %s\n", resp[0])
	for i := 1; i < len(resp); i += 2 {
		fmt.Printf("  %s : %3s\n", resp[i], resp[i+1])
	}
	return
}
