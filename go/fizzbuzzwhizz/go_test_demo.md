# 约束：

使用go自身的单元测试框架testing包来写单元测试有如下约束：

- 单元测试，要导入 `testing` 包；
- 承载测试用例的测试文件，固定以 `_test.go`结尾；
- 测试用例函数名称一定要以 `Test` 开头，同时`Test`后的第一字母一定要大写，如`TestSpec`，写成这样`Testspec`不会被框架识别；
- 测试用例函数的参数有且只有一个，一般是 `t *testing.T`。



# 命令：

测试用例文件使用`go test`指令来执行，没有也不需要 `main()` 作为函数入口；

所有在以_test结尾的源码内以Test开头的函数会自动被执行；

测试用例文件不会参与正常源码编译，不会被包含到可执行文件中。

`go test`常见选项：

| 选项 | 用途                                  |
| ---- | ------------------------------------- |
| -v   | 显示测试的详细命令                    |
| -run | -run regexp，只运行 regexp 匹配的函数 |
| -c   | 生成 test 的二进制文件                |

使用示例：

1. 运行整个项目的测试文件：

```shell
$ go test
PASS
ok      _/home/wangbm/golang/math   0.003s
```

2. 只运行某个测试文件（ math_test.go， math.go 是一对，缺一不可，前后顺序可对调）

```shell
$ go test math_test.go math.go
ok      command-line-arguments  0.002s
```

3. 加`-v`查看详细结果

```shell
$ go test math_test.go math.go
=== RUN   TestAdd
    TestAdd: main_test.go:22: the result is ok
    TestAdd: main_test.go:22: the result is ok
    TestAdd: main_test.go:22: the result is ok
    TestAdd: main_test.go:22: the result is ok
    TestAdd: main_test.go:22: the result is ok
--- PASS: TestAdd (0.00s)
PASS
ok      command-line-arguments  0.003s
```

4. 只测试某个函数，-run 支持正则，如下例子中 TestAdd，如果还有一个测试函数为 TestAdd02，那么它也会被运行。

```shell
$ go test -v -run="TestAdd"
=== RUN   TestAdd
    TestAdd: math_test.go:22: the result is ok
    TestAdd: math_test.go:22: the result is ok
    TestAdd: math_test.go:22: the result is ok
    TestAdd: math_test.go:22: the result is ok
    TestAdd: math_test.go:22: the result is ok
--- PASS: TestAdd (0.00s)
PASS
ok      _/home/wangbm/golang/math   0.003s
```

5. 生成 test 的二进制文件：加 `-c` 参数

```shell
   $ go test -v -run="TestAdd" -c
   $
   $ ls -l
   total 3208
   -rw-r--r-- 1 root root      95 May 25 20:56 math.go
   -rwxr-xr-x 1 root root 3272760 May 25 21:00 math.test
   -rw-r--r-- 1 root root     525 May 25 20:56 math_test.go
```



# IDE配置：

使用`goland`来写单元测试时，需要添加一个`Go Test`的配置，否则无法在`goland`里面运行测试。

![go_test](.\go_test.png)

# 示例：

代码可以从github下载：[recipes/go/fizzbuzzwhizz at main · yanxicheung/recipes (github.com)](https://github.com/yanxicheung/recipes/tree/main/go/fizzbuzzwhizz)

```go
package main

import (
	"testing"
)

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
```



# 参考文献：

1. [6.1 Go 命令：go test 工具详解 ](https://golang.iswbm.com/c06/c06_01.html)
2. [6.2 单元测试：如何进行单元测试](https://golang.iswbm.com/c06/c06_02.html)
3. [8.1 测试技巧：单元测试](https://golang.iswbm.com/c08/c08_01.html)