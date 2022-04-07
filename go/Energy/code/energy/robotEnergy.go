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
