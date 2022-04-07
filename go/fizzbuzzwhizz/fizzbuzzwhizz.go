package main

import (
	"strconv"
	"strings"
)

func str(n int) string {
	return strconv.Itoa(n)
}

type matcher func(int) bool
type action func(int) string
type rule func(int) string

func times(n int) matcher {
	return func(m int) bool {
		return m%n == 0
	}
}

func contains(n int) matcher {
	return func(m int) bool {
		return strings.Contains(str(m), str(n))
	}
}

func always() matcher {
	return func(m int) bool {
		return true
	}
}

func nop() action {
	return func(m int) string {
		return str(m)
	}
}

func to(str string) action {
	return func(m int) string {
		return str
	}
}

func atom(m matcher, a action) rule {
	return func(n int) string {
		if m(n) {
			return a(n)
		}
		return ""
	}
}

func all(rules []rule) rule {
	return func(n int) (s string) {
		for _, r := range rules {
			s += r(n)
		}
		return s
	}
}

func any(rules []rule) rule {
	return func(n int) string {
		for _, r := range rules {
			if s := r(n); len(s) != 0 {
				return s
			}
		}
		return ""
	}
}

func spec(n1, n2, n3 int) rule {
	rn1 := atom(times(n1), to("Fizz"))
	rn2 := atom(times(n2), to("Buzz"))
	rn3 := atom(times(n3), to("Whizz"))

	r3 := atom(contains(n1), to("Fizz"))
	r2 := all([]rule{rn1, rn2, rn3})
	rd := atom(always(), nop())
	return any([]rule{r3, r2, rd})
}

