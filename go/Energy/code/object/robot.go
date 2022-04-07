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

// 依赖注入   将RobotEnergy注入到Worker中的Energy接口
func (this *Robot) inject() {
	/* Robot使用匿名组合方式组合了RobotEnergy和Worker
	   所以Robot实现了Energy的抽象接口
	   所以this可以直接赋值给this.Worker.Energy
	*/
	this.Worker.Energy = this
}

func NewRobot() *Robot {
	// go的编译器会分析,将Robot分配到堆上;也可以使用new运算符
	r := &Robot{}
	r.inject()
	return r
}
