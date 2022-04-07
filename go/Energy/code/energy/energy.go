package energy

type Energy interface {
	Consume()
	IsExhausted() bool
}
