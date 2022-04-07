package object

import (
	"github.com/yanxicheung/energy/energy"
	"github.com/yanxicheung/energy/worker"
)

type Human struct {
	// 委托方式 相当于C++中的包含
	Energy energy.HumanEnergy
	Worker worker.Worker
}

// 函数名字小写,包外不可见;
// 依赖注入   将HumanEnergy注入到Worker中的Energy接口
func (this *Human) inject() {
	/* human 使用了委托方式组合了HumanEnergy和Worker
	   所以human并没有实现Energy的抽象接口
	   所以this不可以直接赋值给this.Worker.Energy
	   否则编译报错:InvalidIfaceAssign
	*/
	this.Worker.Energy = &this.Energy
}

func NewHuman() *Human {
	// go的编译器会分析,将Human分配到堆上;也可以使用new运算符
	h := &Human{}
	h.inject()
	return h
}

func NewHuman_() *Human{
	h:=new(Human)
	h.inject()
	return h
}
