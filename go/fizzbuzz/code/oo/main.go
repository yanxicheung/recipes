package main

import (
	"fmt"

	"github.com/yanxicheung/fizzbuzzwhizz/oo/action"
	"github.com/yanxicheung/fizzbuzzwhizz/oo/matcher"
	"github.com/yanxicheung/fizzbuzzwhizz/oo/rule"
)

func calc(x int, y int) (func(int) int, func() int) {
	f1 := func(z int) int {
		return (x + y) * z / 2
	}

	f2 := func() int {
		return 2 * (x + y)
	}

	return f1, f2
}

func main__() {
	n1, n2, n3 := 3, 5, 7
	r1_1 := rule.Atom{Matcher: &matcher.Times{N: n1}, Action: &action.To{Word: "Fizz"}}
	r1_2 := rule.Atom{Matcher: &matcher.Times{N: n2}, Action: &action.To{Word: "Buzz"}}
	r1_3 := rule.Atom{Matcher: &matcher.Times{N: n3}, Action: &action.To{Word: "Whizz"}}

	r1 := rule.Allof{Rules: []rule.Rule{&r1_1, &r1_2, &r1_3}}
	r2 := rule.Atom{Matcher: &matcher.Contains{N: n1}, Action: &action.To{Word: "Fizz"}}
	rd := rule.Atom{Matcher: &matcher.Always{}, Action: &action.Nop{}}

	spec := rule.Anyof{Rules: []rule.Rule{&r2, &r1, &rd}}
	var word string
	for i := 1; i <= 100; i++ {
		word = spec.Apply(i)
		fmt.Println(word)
	}
}

func GetAverage1(arr [5]int) int {
	sum := 0
	fmt.Println("len:", len(arr))
	for _, elem := range arr {
		sum += elem
	}
	return sum / 5
}

func GetAverage2(arr []int) int {
	sum := 0
	fmt.Println("len:", len(arr))
	for _, elem := range arr {
		sum += elem
	}
	return sum / len(arr)
}

func main() {
	arr := [5]int{1, 1, 1, 1, 1}
	fmt.Println(GetAverage1(arr))

	slice := []int{1, 2, 3, 4}
	fmt.Print(GetAverage2(slice))
}
