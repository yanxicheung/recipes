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
