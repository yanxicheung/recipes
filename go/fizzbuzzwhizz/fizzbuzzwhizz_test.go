package main

import (
	"testing"
)

// 文件名要以_test结尾
// Test开头
// Test后的第一字母一定要大写
/*

go test命令
*/
func TestSpec_Example_Succ1(t *testing.T) {
	t.Log("example go test1")
	cases := []struct {
		input    int
		expected string
	}{{1, "1"},
		{5, "Buzz"},
		{30, "Fizz"},
		{105, "FizzBuzzWhizz"},
	}

	r := spec(3, 5, 7)
	for _, elem := range cases {
		actual := r(elem.input)
		if actual != elem.expected {
			t.Errorf("failed! input %d, expected %v, actual %v", elem.input, elem.expected, actual)
		}
	}
}

//一个测试用例还可以分为多个子测试用例
func TestSpec_Example_Succ2(t *testing.T) {
	r := spec(3, 5, 7)
	t.Run("should_times_succ", func(t *testing.T) {
		cases := []struct {
			input    int
			expected string
		}{
			{3, "Fizz"},
			{5, "Buzz"},
			{7, "Whizz"}}

		for _,elem:=range cases{
			actual:=r(elem.input)
			if actual != elem.expected {
				t.Errorf("failed! input %d, expected %v, actual %v", elem.input, elem.expected, actual)
			}
		}
	})

	t.Run("should_contains_succ", func(t *testing.T) {
		cases := []struct {
			input    int
			expected string
		}{
			{30, "Fizz"},
			{31, "Fizz"},
		}

		for _,elem:=range cases{
			actual:=r(elem.input)
			if actual != elem.expected {
				t.Errorf("failed! input %d, expected %v, actual %v", elem.input, elem.expected, actual)
			}
		}
	})
}


