

# IDE：

使用vscode

go mod init github/daniel/fizzbuzz

# 基础结构：

标识符（包括常量、变量、类型、函数名、结构字段等等）以一个大写字母开头，如：Group1，那么使用这种形式的标识符的对象就可以被外部包的代码所使用（客户端程序需要先导入这个包），这被称为导出（像面向对象语言中的 public）；标识符如果以小写字母开头，则对包外是不可见的，但是他们在整个包的内部是可见并且可用的（像面向对象语言中的 protected ）。



- 文件名与包名没有直接关系，不一定要将文件名与包名定成同一个。
-  文件夹名与包名没有直接关系，并非需要一致。
-  同一个文件夹下的文件只能有一个包名，否则编译报错。



**派生类型:**
包括：

- (a) 指针类型（Pointer）
- (b) 数组类型
- (c) 结构化类型(struct)
- (d) Channel 类型
- (e) 函数类型
- (f) 切片类型
- (g) 接口类型（interface）
- (h) Map 类型
- 

```go
package main

import (
	"fmt"

	mathClass "github.com/yanxicheung/test/math"
)

// hello
func main() {
	var add = mathClass.Add(3, 5)
	var sub = mathClass.Sub(4, 5)
	fmt.Println(add)
	fmt.Println(sub)
	var url = "Code=%d&endDate=%s"
	var stockcode = 123
	var endDate = "2020-12-31"
	var target_url = fmt.Sprintf(url, stockcode, endDate)
	fmt.Println("My weight on mars is", 149*0.3783, "lbs , and i would be", 41*365.2425/687, "years old.")
	fmt.Println(target_url)
	fmt.Println("helloworld")
	fmt.Print("helloworld\n")

	fmt.Printf("I am %v years old\n", 30)
	// 宽度为正数填充在文本左边;负数则在右边
	fmt.Printf("%-15v $%4v\n", "SpaceX", 94)
	fmt.Printf("%-15v $%4v\n", "Virgin Galactic", 100)
}

```



# 变量

```go
package main

import "fmt"
//全局变量是允许声明但不使用的,局部变量不使用会编译报错;
var x, y int
var (
	global_a int
	global_b bool
)

// hello
func main() {
	// 指定变量类型;
	var str string = "hello world"
	fmt.Println(str)

	var a int
	fmt.Println(a)

	var i int
	var f float64
	var b bool
	var s string
	fmt.Printf("%v %v %v %q\n", i, f, b, s)

	// 自动推断类型;
	var d = true
	fmt.Println(d)

	// 初始化声明
    //这是使用变量的首选形式，但是它只能被用在函数体内，而不可以用于全局变量的声明与赋值！！！
	intVal := 1 // 等价于var intVal int = 1
	ss := "helloworld"
	fmt.Printf("%v %v\n", intVal, ss)

	// 多变量声明;
	var a1, a2, a3 = 1, 2, 3
	fmt.Printf("%v %v %v\n", a1, a2, a3)
	a4, a5, a6 := 7, 8, 9
	fmt.Printf("%v %v %v\n", a4, a5, a6)
	aaaa, bbb := 2, "hello"
	fmt.Printf("%v %v\n", aaaa, bbb)
}
```



空白标识符 _ 也被用于抛弃值，如值 5 在：_, b = 5, 7 中被抛弃。

_ 实际上是一个只写变量，你不能得到它的值。这样做是因为 Go 语言中你必须使用所有被声明的变量，但有时你并不需要使用从一个函数得到的所有返回值。

# Big包：



# 常量：

```go
//常量枚举;
const (
	Unknow = 0
	Female = 1
	Male   = 2
)

func const_demo() {
	const LENGTH int = 10
	const WIDTH int = 5
	var area int
	const a, b, c = 1, false, "string"
	area = LENGTH * WIDTH
	fmt.Printf("area = %v", area)
	println()
	println(a, b, c)
	const ddd = 1000
	fmt.Println(ddd)
}
```



# 格式化打印：

```go

func format_demo() {
	year := 2018
	fmt.Printf("%v\n", year) // %v是默认格式输出
	fmt.Printf("%x\n", year)
	var red, green, blue = 0x00, 0x8d, 0xd5
	fmt.Printf("%02x %02x %02x\n", red, green, blue)
	fmt.Printf("%08b\n", green)
	target := fmt.Sprintf("%08b\n", green)
	fmt.Println(target)
}
```



# 运算符：

go没有前置自增、自减

```go
func main() {
	var a int = 4
	var b int32
	var c float32
	var ptr *int // 指针类型;
	fmt.Printf("a 类型%T\n", a)
	fmt.Printf("b 类型%T\n", b)
	fmt.Printf("c 类型%T\n", c)
	fmt.Printf("ptr 类型%T\n", ptr)

	ptr = &a
	fmt.Printf("ptr = %v, *ptr = %v", ptr, *ptr)
}
```



# 条件语句：

switch 语句用于基于不同条件执行不同动作，每一个 case 分支都是唯一的，从上至下逐一测试，直到匹配为止。

switch 语句执行的过程从上至下，直到找到匹配项，匹配项后面也不需要再加 break。

switch 默认情况下 case 最后自带 break 语句，匹配成功后就不会执行其他 case，如果我们需要执行后面的 case，可以使用 **fallthrough** 。fallthrough 不会判断下一条 case 的表达式结果是否为 true。



```go
// 基本用法
switch var1 {
    case val1:
        ...
    case val2:
        ...
    default:
        ...
}
```



变量 var1 可以是任何类型，而 val1 和 val2 则可以是同类型的任意值。类型不被局限于常量或整数，但必须是相同的类型；或者最终结果为相同类型的表达式。

您可以同时测试多个可能符合条件的值，使用逗号分割它们，例如：case val1, val2, val3。

```go
switch{
    case 1,2,3,4:
    default:
}
```



switch 语句还可以被用于 type-switch 来判断某个 interface 变量中实际存储的变量类型。

Type Switch 语法格式如下：

```go
switch x.(type){
    case type:
       statement(s);      
    case type:
       statement(s); 
    /* 你可以定义任意个数的case */
    default: /* 可选 */
       statement(s);
}
```

**<u>Select语句先不看了</u>**





# 循环：

go中只有for循环;

Go 语言的 For 循环有 3 种形式，只有其中的一种使用分号。

和 C 语言的 for 一样：

```go
for init; condition; post { }
```

和 C 的 while 一样：

```go
for condition { }
```

和 C 的 for(;;) 一样：

```go
for { }
```



```go
package main

import "fmt"

func main() {
	//////////
	var cnt int
	for cnt = 0; cnt < 10; cnt++ {
		fmt.Println("cnt = ", cnt)
	}
	////////////
	var num int
	for num < 5 {
		fmt.Printf("num = %v\n", num)
		num++
	}
	/////////
	var loop int
	for {
		loop++
		if loop > 10 {
			fmt.Println("loop = ", loop)
			break
		}
	}
	fmt.Println("Hello world")
	//////////
	sum := 1
	for sum <= 10 {
		sum += sum
	}
	fmt.Println(sum)

	for sum < 10 {
		sum += sum
	}
	fmt.Println(sum)
	///////////
	// range -for
	strings := []string{"google", "runoob"}  // 切片
	for i, s := range strings {
		fmt.Println(i, s)
	}

	numbers := [6]int{1, 2, 3, 5}    // 数组, 数组定义时[]内要指明长度，或者用...
	for i, x := range numbers {
		fmt.Printf("第 %d 位 x 的值 = %d\n", i, x)
	}
}

```

for—each range 循环;

```go
package main
import "fmt"

func main() {
        strings := []string{"google", "runoob"}
        for i, s := range strings {
                fmt.Println(i, s)
        }


        numbers := [6]int{1, 2, 3, 5}
        for i,x:= range numbers {
                fmt.Printf("第 %d 位 x 的值 = %d\n", i,x)
        }  
}
```



# 函数：

函数如果使用参数，该变量可称为函数的形参。

形参就像定义在函数体内的局部变量。

调用函数，可以通过两种方式来传递参数：

| [值传递](https://www.runoob.com/go/go-function-call-by-value.html) | 值传递是指在调用函数时将实际参数复制一份传递到函数中，这样在函数中如果对参数进行修改，将不会影响到实际参数。 |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [引用传递](https://www.runoob.com/go/go-function-call-by-reference.html) | 引用传递是指在调用函数时将实际参数的地址传递到函数中，那么在函数中对参数所进行的修改，将影响到实际参数。 |

默认情况下，Go 语言使用的是值传递，即在调用过程中不会影响到实际参数。

函数可以返回多个值；

引用传递：

```go
func swap(x *int, y *int) {
   var temp int
   temp = *x    /* 保存 x 地址上的值 */
   *x = *y      /* 将 y 值赋给 x */
   *y = temp    /* 将 temp 值赋给 y */
}
```

**函数作为参数（回调）**

**闭包**

**方法（oop）**



## 集合示例：

```go
package main

import (
	"fmt"
	"math"
)

type cb func(int) int

func testcb(x int, f cb) {
	fmt.Printf("f(%v) = %v\n", x, f(x))
}

func twice(x int) int {
	return 2 * x
}

func main() {

	// 定义函数变量;
	f := func(x float64) float64 {
		return math.Sqrt(x)
	}
	fmt.Println(f(45))

	testcb(1, twice)
	testcb(2, func(x int) int {
		return 3 * x
	})

	// 闭包;
	nextNumber := getSequence() // nextNumber是一个函数对象;
	fmt.Println(nextNumber())

	//方法--->oop
	var c Circle
	c.radius = 10
	fmt.Println("area = ", c.getArea())
	c.updateRadius(100)
	fmt.Println("area = ", c.getArea())
}

type Circle struct {
	radius float64
}

// 修改对象的值需要传递指针;
func (c *Circle) updateRadius(radius float64) {
	c.radius = radius
}

func (c Circle) getArea() float64 {
	return 3.14 * c.radius * c.radius
}

// 闭包;
func getSequence() func() int {
	i := 0
	return func() int {
		i += 1
		return i
	}
}

```



# 作用域：

Go 语言中变量可以在三个地方声明：

- 函数内定义的变量称为局部变量

- 函数外定义的变量称为全局变量

- 函数定义中的变量称为形式参数




```go
func switch_test() {
	era := "AD"
	year := 2018
    // month 的作用域在switch之内
	switch month := rand.Intn(12) + 1; month {
	case 2: // 每个case都有一个自己的独立作用域
		day := rand.Intn(28) + 1   // 代码里面的day不是同一个变量
		fmt.Println(era, year, month, day)
	case 4, 6, 9, 11:
		day := rand.Intn(30) + 1
		fmt.Println(era, year, month, day)
	default:
		day := rand.Intn(31) + 1
		fmt.Println(era, year, month, day)
	}
}
```



# 数组：

基本操作：

```go
package main

import "fmt"

func main() {
	var arr [10]int
	arr[0] = 2
	fmt.Println(arr)

	hello := [5]int{1, 2, 3, 4, 5}
	fmt.Println(hello)

	//数组长度自动推导
	balance := [...]int{1, 2, 3, 5}
	fmt.Println(balance)

	//指定下标元素初始化;
	balance1 := [5]int{1: 3, 3: 5}
	fmt.Println(balance1)

	//遍历
	for index, elem := range balance1 {
		fmt.Println("index=", index, "elem=", elem)
	}

	for index := 0; index < len(balance1); index++ {
		fmt.Println("index=", index, "elem=", balance1[index])
	}
}

```

数组作为参数是值传递：

```go
// 数组是一种值,这里通过传递值接受参数,函数效率低
// 不会影响到实参;
func terraform(plants [3]string) {
	for i := range plants {
		plants[i] = plants[i] + "New"
	}
}
// 返回一个数组;
func terraformII(plants [3]string) [3]string {
	for i := range plants {
		plants[i] = plants[i] + "New"
	}
	return plants
}

func main() {
	plants := [3]string{"Mercury", "Venus", "Earth"}

	terraform(plants)
	fmt.Println(plants)

	plants = terraformII(plants)
	fmt.Println(plants)
}
```

**切片传递的是引用：**

```go
func transform(elems []int) {
	for i := range elems {
		elems[i]++
	}
}

func main() {
	elems := []int{1, 2, 3, 4}
	transform(elems)
	fmt.Println(elems)
}
```



```go
package main

import "fmt"

func getAverage_(arr [5]int) int {
	sum := 0
	for index := 0; index < 5; index++ {
		sum += arr[index]
	}
	return sum / 5
}
// 切片;
func getAverage(arr []int, size int) int {
	sum := 0
	for index := 0; index < size; index++ {
		sum += arr[index]
	}
	return sum / size
}

func main() {
    // 切片
	arr := []int{1, 2, 3, 4, 5}
	fmt.Println("average=", getAverage(arr, len(arr)))
	// 数组
	var arr1 [5]int
	for i := 0; i < 5; i++ {
		arr1[i] = i
	}

	fmt.Println("average=", getAverage_(arr1))
}

```



如果我们想通过一个函数改变一个数组的值，有两种方法

1. 将这个数组的切片做为参数传给函数
2. 将这个数组的指针做为参数传给函数

尽管二者都可以实现我们的目的，但是按照 Go 语言的使用习惯，建议使用第一种方法，因为第一种方法，写出来的代码会更加简洁，易读。具体你可以参数下面两种方法的代码实现：

```go
package main

import "fmt"

func modifyArrayBySlice(arr []int){
	arr[0] = 1000
}

func modifyArrayByPointer(arr *[5]int){
	(*arr)[0] = 198
}

func main() {
	arr1:=[...]int{1,2,4,5,6}
	modifyArrayBySlice(arr1[:])  //更加简洁
	fmt.Println(arr1)
	modifyArrayByPointer(&arr1)
	fmt.Println(arr1)
}
```



# 切片：

基本用法，作为函数参数

[go语言中实现切片(slice)的三种方式 - 简书 (jianshu.com)](https://www.jianshu.com/p/6aaab016f0d4)

**<u>字符串支持切片</u>**

```go
package main

import (
	"fmt"
	"strings"
)

func main() {
	var scores [5]int
	fmt.Println(scores)

	id := [...]int{1, 2, 3, 5, 56} // 数组; []内需要... 或者指定长度
	fmt.Println(id)

	slice_id := []int{5, 6, 7, 8} //创建切片
	fmt.Println(slice_id)
	fmt.Printf("id type :%T\n", id)
	fmt.Printf("slice_id type :%T\n", slice_id)
    
    // make([]Type, len, cap)
    vv:=make([]string,10)  // cap >= len >= 0的条件必须成立
	fmt.Println(vv) // 10个空字符串

	// 切片作为函数参数;
	plants := []string{" Venus ", "Earth ", " Mars"}
	fmt.Println(plants[:])     // [ Venus  Earth   Mars]
	fmt.Println(plants[:3])    // [ Venus  Earth   Mars]
	fmt.Println(plants[1:3])   // [Earth   Mars]
	fmt.Println(plants[1:])    // [Earth   Mars]
	fmt.Printf("%q\n", plants) // [" Venus " "Earth " " Mars"]
	hyperspace(plants) // 去除空格
	fmt.Printf("%q\n", plants)
	str := strings.Join(plants, "")
	fmt.Println(str)

	//
	slice := make([]int, 3, 5)
	fmt.Println(slice)
	fmt.Println(getSliceInfo(slice))
}

func getSliceInfo(x []int) (int, int) {
	return cap(x), len(x)
}

func hyperspace(worlds []string) {
	for index := range worlds {
		worlds[index] = strings.TrimSpace(worlds[index])
	}
}

```

**切片append、copy**

```go
package main

import "fmt"

func main() {
	var slice []int
	printslice(slice)

	slice = append(slice, 0)  // 一定要保存append的返回值，否则编译报错;
	slice = append(slice, 1)
	printslice(slice)

	numbers := make([]int, len(slice), cap(slice)*2)
	copy(numbers, slice)
	printslice(numbers)
}

func printslice(slice []int) {
	fmt.Printf("len = %v, cap = %v, slice = %v\n", len(slice), cap(slice), slice)
}

```



```go
func printinfo(slice []int) {
	fmt.Printf("len[%v], cap[%v]\n", len(slice), cap(slice))
}

func main() {
	d1 := []int{1, 2, 3, 4, 5}
	printinfo(d1)  // len[5]  cap[5]

	// d1 和 d2 指向不同底层数组
	d2 := append(d1, 3)
	printinfo(d2)  // len[6]  cap[10]
	// d2 和 d3 指向同一个底层数组
	d3 := append(d2, 3)
	printinfo(d3)  // len[7], cap[10]
	d3[0] = 0
	fmt.Println("d1", d1)
	fmt.Println("d2", d2)
	fmt.Println("d3", d3)
    
    dddd:=make([]int, 0)
	printinfo(dddd)  // len[0], cap[0]
}
```

**三索引切片:**

```go
func main() {
	d1 := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	printinfo(d1)
	d2 := d1[1:3] // d2和d1指向同一底层数组  1:起始索引  3:终止索引
	d2[0] = 22    // 修改后d1也会变化
	fmt.Println("d1:", d1, " d2:", d2)

	//三索引切片操作 d3指向新的底层数组;
	d3 := d1[2:4:8] // 2:起始索引  4:终止索引  8:容量不能超过原切片的最大索引值
	fmt.Println(d3)
}

```



# 指针：

指针数组：

```go
func main() {

	arr := []int32{1, 2, 3, 4, 5}
	var ptrArr [5]*int32 // 指针数组;
	for i := 0; i < 5; i++ {
		ptrArr[i] = &arr[i]
	}

	for _, address := range ptrArr {
		fmt.Printf("addr[%v] ", address)
	}
}
```



# 结构体：

```go
package main

import "fmt"

type Book struct {
	title   string
	author  string
	book_id int
}

func (book Book) print() {
	fmt.Println("title:", book.title)
	fmt.Println("author:", book.author)
	fmt.Println("book_id:", book.book_id)
}

func main() {
	fmt.Println(Book{"Go", "daniel", 4})
	fmt.Println(Book{title: "go", author: "daniel", book_id: 34})
	var book Book
	book.author = "go"
	book.title = "daniel"
	book.book_id = 100
	book.print()

	var pBook *Book = &book
	fmt.Println(pBook.author)
}

```



**结构体可以只赋一部分值；**

```go
type Student struct {
	id       int
	name     string
	email    string
	addr     string
	phoneNum string
}

func main() {

	student := Student{id: 1234, name: "zpp"} // 可以先赋一部分值
	student.email = "1206767659@qq.com"
	student.addr = "suzhou"
	student.phoneNum = "18868823969"
	fmt.Println(student)
}
```



# 范围：

Go 语言中 range 关键字用于 for 循环中迭代数组(array)、切片(slice)、通道(channel)或集合(map)的元素。在数组和切片中它返回元素的索引和索引对应的值，在集合中返回 key-value 对。

```go
package main

import "fmt"

func main() {
	//这是我们使用range去求一个slice的和。使用数组跟这个很类似
	nums := []int{2, 3, 4}
	sum := 0
	for _, num := range nums {
		sum += num
	}
	fmt.Println("sum:", sum)
	//在数组上使用range将传入index和值两个变量。上面那个例子我们不需要使用该元素的序号，所以我们使用空白符"_"省略了。有时侯我们确实需要知道它的索引。
	for i, num := range nums {
		if num == 3 {
			fmt.Println("index:", i)
		}
	}
	//range也可以用在map的键值对上。
	kvs := map[string]string{"a": "apple", "b": "banana"}
	for k, v := range kvs {
		fmt.Printf("%s -> %s\n", k, v)
	}
	//range也可以用来枚举Unicode字符串。第一个参数是字符的索引，第二个是字符（Unicode的值）本身。
	for i, c := range "go" {
		fmt.Println(i, c)
	}
}

```



# Map：

```go
/* 声明变量，默认 map 是 nil  不能存储元素*/
var map_variable map[key_data_type]value_data_type
```



```go
//以下方式可以存储元素
var map_variable map[key_data_type]value_data_type
map_variable = make(map[key_data_type]value_data_type)
```



建议还是这种比较好：

```go
/* 使用 make 函数 */
map_variable := make(map[key_data_type]value_data_type)   // 可以存储元素
```



```go
package main

import "fmt"

func main() {
	// 增加 初始化;
	fmt.Println("=================================")
	countryCapitalMap := make(map[string]string)
	countryCapitalMap["France"] = "巴黎"
	countryCapitalMap["Italy"] = "罗马"
	for k, v := range countryCapitalMap {
		fmt.Println(k, v)
	}

	// 直接初始化
	fmt.Println("=================================")
	init_directly := map[string]string{"France": "Paris", "Italy": "Rome", "Japan": "Tokyo", "India": "New delhi"}
	fmt.Println(init_directly)
	init_directly["hello"] = "world"
	for k, v := range init_directly {
		fmt.Println(k, v)
	}

	// 删除
	fmt.Println("=================================")
	delete(init_directly, "France")
	fmt.Println(init_directly)

	// 查询
	fmt.Println("=================================")
	var capital string
	var exist bool
	capital, exist = countryCapitalMap["American"]
	fmt.Println(capital, exist)
	capital, exist = countryCapitalMap["Italy"]
	fmt.Println(capital, exist)

}

```



# 接口：

1. [Go 专栏｜接口 interface - 简书 (jianshu.com)](https://www.jianshu.com/p/f721d361baab)

interface给人感觉既是指针又是变量，感觉具体类型是动态的：

```go
var c Duck = &Cat{} // 类型是 *Cat

var c Duck = Cat{} // 类型是 Cat
```



通常这么写：

```go
package main

import "fmt"

// 定义接口，包含 Eat 方法
type Duck interface {
	Eat()
}

// 定义 Cat 结构体，并实现 Eat 方法
type Cat struct{}

func (c *Cat) Eat() {
	fmt.Println("cat eat")
}

// 定义 Dog 结构体，并实现 Eat 方法
type Dog struct{}

func (d *Dog) Eat() {
	fmt.Println("dog eat")
}

func main() {
	var c Duck = &Cat{}  // var c Duck = Cat{} ---->这么写会报错 要这么写可以看下面----\|/
	c.Eat()
	fmt.Printf("%T\n", c) // *main.Cat
	var d Duck = &Dog{}
	d.Eat()
	fmt.Printf("%T\n", d) // *main.Dog
	s := []Duck{
		&Cat{},
		&Dog{},
	}
	for _, n := range s {
		n.Eat()
		fmt.Printf("%T\n", n) // *main.Cat   *main.Dog
	}
}

```



也可以这么写：

```go
package main

import "fmt"

// 定义接口，包含 Eat 方法
type Duck interface {
	Eat()
}

// 定义 Cat 结构体，并实现 Eat 方法
type Cat struct{}

func (c Cat) Eat() {
	fmt.Println("cat eat")
}

// 定义 Dog 结构体，并实现 Eat 方法
type Dog struct{}

func (d Dog) Eat() {
	fmt.Println("dog eat")
}

func main() {
	var c Duck = Cat{}
	c.Eat()
	fmt.Printf("%T\n", c) // main.Cat
	var d Duck = Dog{}
	d.Eat()
	fmt.Printf("%T\n", d) // main.Dog
	s := []Duck{
		Cat{},
		Dog{},
	}
	for _, n := range s {
		n.Eat()
		fmt.Printf("%T\n", n) // main.Cat   main.Dog
	}
}

```



# 类型转换：

不同类型不能混用，要类型转换

# type：

起别名：

```go
type byte = uint8
type rune = int32
// byte 和 uint8 是两个相同的类型，不需要进行类型转换
```

自定义类型：

```go
type celsius float64
// celsius 和 float64是两个不同的类型，混用时需要进行类型转换
```

声明函数类型：

```go
type kelvin float64
type sensor func() kelvin
```



```go
type celsius float64
type kelvin float64

func kelvinToCelsius(k kelvin) celsius {
	return celsius(k - 273.15) // 需要转换类型
}

func celsiusToKevin(c celsius) kelvin {
	return kelvin(c + 273.15)
}

func main() {
	var k kelvin = 20
	kelvinToCelsius(k)

	var k1 int = 30
	kelvinToCelsius(k1)         // 类型不匹配,编译报错;
	kelvinToCelsius(kelvin(k1)) // 进行类型转换
	kelvinToCelsius(30)         // 接受字面量或者无类型常量作为实参
}
```



# 方法：

可以在类型上绑定方法：

```go
type celsius float64
type kelvin float64
// go在类型上声明方法的能力比其他语言的类class相比更为通用
func (this *kelvin) Celsius() celsius {
	return celsius(*this - 273.15)
}

func main() {
	var k kelvin = 20
	r := k.Celsius()
	fmt.Println(r)
}
```



还有一个标准库的例子：

```go
type StringSlise []string
func (p StringSlice) Sort()
```



**打印整形切片（带有方法的切片）**

```go
type IntSlice []int

func (p IntSlice) Print() {
	for i := range p {
		fmt.Println(p[i])
	}
}

func main() {
	a := []int{1, 2, 3, 5}
	IntSlice(a).Print()
}

```



# 一等函数：

匿名函数是没有名字的函数，也叫做函数字面量；

闭包：匿名函数封闭并且包围作用域中的变量。

闭包保留的是周围变量的引用而不是副本，所以修改被闭包捕获的变量会导致调用匿名函数的结果发生变化。



```go
type celsius float64
type kelvin float64

func main() {

	var k kelvin = 294.0
	sensor := func() kelvin {
		return k
	}
	fmt.Println(sensor()) // 294
	k++
	fmt.Println(sensor()) //295
}

```



# OO范式：

1. [Implement Domain Object in Golang - 简书 (jianshu.com)](https://www.jianshu.com/p/9fc3654b8165)
2. [Golang中小类大对象的一种实现 - 简书 (jianshu.com)](https://www.jianshu.com/p/7a9bb47115e2)





# FP范式：

1. [The Coding Kata: FizzBuzzWhizz in Go - 简书 (jianshu.com)](https://www.jianshu.com/p/78505b14a14e)
2. [Closure in Golang - 简书 (jianshu.com)](https://www.jianshu.com/p/3934e62d78a1)





# 之前没有见过的写法:

```go
package main

import (
	"fmt"
	"strconv"
)
//这样c_string和c_int就直接定义了
func add(a int, b int) (c_string string, c_int int) {
	c_int = a + b
	c_string = strconv.Itoa(c_int)
	return c_string, c_int
}

func main() {
	fmt.Println(add(4, 6))
}

```



go语言中，`if`语句有两种，一种的普通的`if`语句，一种是带赋值语句的`if`语句，if语句不带括号，类似python，并且，**无论if语句下面有几条语句，都必须带上花括号，不能省略**。

```go
package main

import "fmt"

func main() {
	var a, b = 10, 5
	if a > b {
		fmt.Println("a > b")
	}
	else if a == b && a != 0 {
		fmt.Println("a == b")
	}
	else {
		fmt.Println("a < b")
	}
	
	// 带赋值语句的if语句，不支持多变量赋值，但支持平行赋值
	if a, b := 10, 20; a > b {
		fmt.Println("a > b")
	}
	else {
		fmt.Println("a < b")
	}
	
	// 注：如果if-else中包含return时，编译器无法解析else中的return
	
}
————————————————
版权声明：本文为CSDN博主「血小板自动机」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u011544909/article/details/94878000
```





# Init函数：

[理解go中的init函数 - 简书 (jianshu.com)](https://www.jianshu.com/p/5763893a6992)



协程、线程、channel



# 变参函数：



# 类型转换：



# 参考文献

1. [从零开始搭建Go语言开发环境 - aaronthon - 博客园 (cnblogs.com)](https://www.cnblogs.com/aaronthon/p/10587487.html)
2. [理解Golang包导入 - Go语言中文网 - Golang中文社区 (studygolang.com)](https://studygolang.com/articles/3189)
3. [(37条消息) 亲测GO环境搭建，理解go build、go install、go get_Leeon的博客-CSDN博客_go install](https://blog.csdn.net/zhangliangzi/article/details/77914943)
4. [Go条件判断语句之if - louis-sun - 博客园 (cnblogs.com)](https://www.cnblogs.com/louis181214/p/10209860.html)
5. [Go语言学习记录之一（返回指针与返回值的区别） - Go语言中文网 - Golang中文社区 (studygolang.com)](https://studygolang.com/articles/4932)
6. [Golang精编100题 - 简书 (jianshu.com)](https://www.jianshu.com/p/f690203ff168)