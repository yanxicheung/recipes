package main

import (
	"fmt"
	"math/rand"
	"strconv"
	"strings"
	"time"
)

type matcher func(n int) bool

func times(m int) matcher {
	return func(n int) bool {
		return n%m == 0
	}
}

func contains(m int) matcher {
	return func(n int) bool {
		return strings.Contains(strconv.Itoa(n), strconv.Itoa(m))
	}
}

func always() matcher {
	return func(n int) bool {
		return true
	}
}

type action func(n int) string

func nop() action {
	return func(n int) string {
		return strconv.Itoa(n)
	}
}

func to(word string) action {
	return func(n int) string {
		return word
	}
}

type rule func(n int) string

func atom(m matcher, a action) rule {
	return func(n int) string {
		if m(n) {
			return a(n)
		}
		return ""
	}
}

func allof(rules []rule) rule {
	return func(n int) (s string) {
		for _, rule := range rules {
			s += rule(n)
		}
		return s
	}
}

func anyof(rules []rule) rule {
	return func(n int) (s string) {
		for _, rule := range rules {
			if s = rule(n); len(s) != 0 {
				return s
			}
		}
		return s
	}
}

func spec_rule(n1, n2, n3 int) rule {
	r1_1 := atom(times(n1), to("Fizz"))
	r1_2 := atom(times(n2), to("Buzz"))
	r1_3 := atom(times(n3), to("Whizz"))

	r1 := allof([]rule{r1_1, r1_2, r1_3})
	r2 := atom(contains(n1), to("Fizz"))
	rd := atom(always(), nop())

	return anyof([]rule{r2, r1, rd})
}

func game(n int, game_rule rule) {
	for i := 1; i <= n; i++ {
		fmt.Printf("%v--->%v\n", i, game_rule(i))
	}
}

func switch_test() {
	rand.Seed(time.Now().UnixNano())
	era := "AD"
	year := 2018
	day := 31
	month := rand.Intn(12) + 1
	switch month {
	case 2:
		day = rand.Intn(28) + 1
	case 4, 6, 9, 11:
		day = rand.Intn(30) + 1
	default:
		day = rand.Intn(31) + 1
	}
	fmt.Println(era, year, month, day)
}

func isLeapYear(year int) bool {

	if year%400 == 0 {
		return true
	}

	if year%4 == 0 && year%100 != 0 {
		return true
	}

	return false
}

func dayInMonth(year, month int) int {
	switch month {
	case 2:
		if isLeapYear(year) {
			return 29
		} else {
			return 29
		}
	case 4, 6, 9, 11:
		return 30
	default:
		return 31
	}
}

func rand_date() {
	rand.Seed(time.Now().UnixNano())
	era := "AD"
	year := rand.Intn(50) + 1992
	month := rand.Intn(12) + 1
	day := rand.Intn(dayInMonth(year, month)) + 1
	fmt.Println(era, year, month, day)
}

func format_demo() {
	year := 2018
	fmt.Printf("%v\n", year) // %v是默认格式输出
	fmt.Printf("%x\n", year)
	var red, green, blue = 0x00, 0x8d, 0xd5
	fmt.Printf("%02x %02x %02x\n", red, green, blue)
	fmt.Printf("%08b\n", green)

	//
	target := fmt.Sprintf("%018b\n", green)
	fmt.Println(target)
}

func game_test() {
	n1, n2, n3 := 3, 5, 7
	spec := spec_rule(n1, n2, n3)
	game(100, spec)
}

func printinfo(slice []int) {
	fmt.Printf("len[%v], cap[%v]\n", len(slice), cap(slice))
}

func main() {
	d1 := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	printinfo(d1)
	d2 := d1[1:3] // d2和d1指向同一底层数组  1:起始索引  3:终止索引
	d2[0] = 22    // 修改后d1也会变化
	fmt.Println("d1:", d1, " d2:", d2)

	//三索引切片操作 d3指向新的底层数组;
	d3 := d1[2:4:8] // 2:起始索引  4:终止索引  8:容量不能超过原切片的最大索引值
	fmt.Println(d3)

	slice := make([]int, 3, 5)
	fmt.Println(slice)

	str := []string{"hello", "world", "daniel"}
	fmt.Println(str)
	sss := append(str, "zpp")
	fmt.Println(sss)
}
