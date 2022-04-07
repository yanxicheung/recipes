package rule

type Allof struct {
	Rules []Rule
}

func (this *Allof) Apply(m int) (s string) {
	for _, elem := range this.Rules {
		s += elem.Apply(m)
	}
	return s
}
