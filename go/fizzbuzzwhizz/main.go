package main

import "fmt"

func main(){
	specRule := spec(3, 5, 7)
	for i := 1; i <= 100; i++ {
		fmt.Printf("%d--->%s\n", i, specRule(i))
	}
}
