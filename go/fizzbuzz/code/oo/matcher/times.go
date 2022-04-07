package matcher

type Times struct {
	N int
}

func (this *Times) Match(m int) bool {
	return m%this.N == 0
}
