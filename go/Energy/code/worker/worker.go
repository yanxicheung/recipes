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
