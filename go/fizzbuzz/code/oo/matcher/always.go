package matcher

type Always struct {
}

func (this *Always) Match(m int) bool {
	return true
}
