package main

import (
	"fmt"

	"github.com/yanxicheung/energy/object"
)

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

/*Robot使用匿名组合方式组合了RobotEnergy和Worker
当我们访问该struct的方法时，可以通过默认的变量名访问。 --->如RobotWork()_函数中的方式
也可以直接访问(略去默认的变量名) --->如RobotWork()函数中的方式
*/
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

func RobotWork_() {
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

func main() {
	HumanWork()
	RobotWork()
	RobotWork_()
}
