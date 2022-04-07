package rule

type Anyof struct {
	Rules []Rule
}

func (this *Anyof) Apply(m int) (s string) {
	for _, elem := range this.Rules {
		s = elem.Apply(m)
		if len(s) != 0 {
			return s
		}
	}
	return ""
}
