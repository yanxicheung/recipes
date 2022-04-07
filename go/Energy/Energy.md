# 目的：

通过一道题目体验下Go语言下的OO范式编程。



# 题目：

模拟人和机器人制造产品。

这里人和机器人在工作时都是一名worker（扮演的角色），工作的流程是一样的：如果能量没有消耗完，那么可以继续制造产品，否则停工。

区别在于**依赖的能量消耗和获取**方式不同：

1. 人制造产品会消耗吃饭得到的能量，缺乏能量后需要再吃饭补充。人吃完饭后能量值为10，每生产一件产品，消耗能量值为1，吃完饭后能量恢复到10。
2. 机器人制造产品会消耗电能，缺乏能量后需要再充电，充完电可以继续工作。充完电后能量值为100，每生产一件产品，消耗能量值为1，充电后能量值恢复到100。

基于[组合式设计](https://www.jianshu.com/p/bb9c35606d29)思想，得到一个领域模型：

![human-robot](.\pic\human-robot.png)

# Energy：

接口在Golang中是一个interface。它包含两个方法：一个是消耗能量`Consume`，另一个是能量是否耗尽`IsExhausted`。

```go
package energy

type Energy interface {
	Consume()
	IsExhausted() bool
}
```

关于接口可以参考：[Go 专栏｜接口 interface - 简书 (jianshu.com)](https://www.jianshu.com/p/f721d361baab)。



## HumanEnergy：

基于C++的知识可以将`HumanEnergy`理解为是类`Energy`的子类。

```go
package energy

const MAX_CONSUME_TIMES int = 10

type HumanEnergy struct {
	isHungry     bool
	consumeTimes int
}

func (this *HumanEnergy) Eat() {
	this.consumeTimes = 0
	this.isHungry = false
}

func (this *HumanEnergy) Consume() {
	this.consumeTimes++
	if this.consumeTimes >= MAX_CONSUME_TIMES {
		this.isHungry = true
	}
}

func (this *HumanEnergy) IsExhausted() bool {
	return this.isHungry
}
```

基础语法参考：[Go 语言函数方法 | 菜鸟教程 (runoob.com)](https://www.runoob.com/go/go-method.html)



## RobotEnergy：

同样地，`RobotEnergy`也是`Energy`的子类

```go
package energy

const (
	FULL_PERCENT    int = 100
	CONSUME_PERCENT int = 1
)

type RobotEnergy struct {
	percent int
}

func (this *RobotEnergy) Charge() {
	this.percent = FULL_PERCENT
}

func (this *RobotEnergy) Consume() {
	if this.percent > 0 {
		this.percent -= CONSUME_PERCENT
	}
}

func (this *RobotEnergy) IsExhausted() bool {
	return this.percent == 0
}

```

# Worker：

`Worker`依赖于`Energy`这个接口，具体这个接口实际上是指向`HumanEnergy`还是`RobotEnergy`，根据需要注入。

这里体现的是依赖倒置原则：

`Worker`依赖于`Energy`这个稳定接口，在`Produce`产品的时候不关心用的能量是`HumanEnergy`还是`RobotEnergy`。

```go
package worker

import "github.com/yanxicheung/energy/energy"

type Worker struct {
	producerNum int
	Energy      energy.Energy
}

func (this *Worker) Produce() {
	if this.Energy.IsExhausted() {
		return
	}
	this.producerNum++
	this.Energy.Consume()
}

func (this *Worker) GetProduceNum() int {
	return this.producerNum
}

```



# Object:

领域对象是对`具体Energy`和`Worker`的组合。

这里会介绍两种组合方式：

第一种是`Human`采用的委托方式，相当于C++中的包含。

第二种是`Robot`采用的[匿名组合](https://blog.csdn.net/tennysonsky/article/details/78904027)，相当于C++中的继承。

## Human：

`Human`领域对象组合了`HumanEnergy`和`Worker`。代码如下：

```go
package object

import (
	"github.com/yanxicheung/energy/energy"
	"github.com/yanxicheung/energy/worker"
)

type Human struct {
    // 委托方式
	Energy energy.HumanEnergy
	Worker worker.Worker
}

// 函数名字小写,包外不可见;
func (this *Human) inject() {
	this.Worker.Energy = &this.Energy
}

func NewHuman() *Human {
	h := &Human{}
	h.inject()
	return h
}

```

​	`this.Worker.Energy = &this.Energy`的作用是依赖注入，将`Worker`依赖的接口`Energy`实际指向`HumanEnergy`。

由于 `Human` 使用了委托方式组合了`HumanEnergy`和`Worker`。 所以`Human`并没有实现`Energy`的抽象接口，所以`this`不可以直接赋值给`this.Worker.Energy`，否则编译报错:`InvalidIfaceAssign`。



## Robot：

`Robot`领域对象组合了`RobotEnergy`和`Worker`。代码如下：

```go
package object

import (
	"github.com/yanxicheung/energy/energy"
	"github.com/yanxicheung/energy/worker"
)

type Robot struct {
	// 匿名组合struct 相当与C++中的继承
	energy.RobotEnergy
	worker.Worker
}

func (this *Robot) inject() {
	this.Worker.Energy = this
}

func NewRobot() *Robot {
	r := &Robot{}
	r.inject()
	return r
}

```

​	`this.Worker.Energy = this`的作用是依赖注入，将`Worker`依赖的接口`Energy`实际指向`robotEnergy`。

由于 `Robot` 使用了匿名组合方式组合了`RobotEnergy`和`Worker`。 所以`Robot`实现`Energy`的抽象接口，所以`this`可以直接赋值给`this.Worker.Energy`。



# 使用：

`Human`领域对象的具体使用如下：

```go
func HumanWork() {
	human := object.NewHuman()
	human.Energy.Eat()
	for {
		if human.Energy.IsExhausted() {
			break
		}
		human.Worker.Produce()
	}
	fmt.Printf("human produce %v products in one cycle\n", human.Worker.GetProduceNum())
}
```



由于`Robot`使用匿名组合方式组合了`RobotEnergy`和`Worker`，当我们访问该`struct`的方法时，可以通过默认的变量名访问： 

```go
func RobotWork() {
	robot := object.NewRobot()
	robot.RobotEnergy.Charge()
	for {
		if robot.Energy.IsExhausted() {
			break
		}
		robot.Worker.Produce()
	}
	fmt.Printf("robot produce %v products in one cycle\n", robot.Worker.GetProduceNum())
}
```


也可以直接访问(略去默认的变量名) ：

```go
func RobotWork() {
	robot := object.NewRobot()
	robot.Charge()
	for {
		if robot.IsExhausted() {
			break
		}
		robot.Produce()
	}
	fmt.Printf("robot produce %v products in one cycle\n", robot.GetProduceNum())
}

```



# 参考文献：

1. [Implement Domain Object in Golang - 简书 (jianshu.com)](https://www.jianshu.com/p/9fc3654b8165)
2. [Golang中小类大对象的一种实现 - 简书 (jianshu.com)](https://www.jianshu.com/p/7a9bb47115e2)