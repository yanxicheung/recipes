package rule

import (
	"github.com/yanxicheung/fizzbuzzwhizz/oo/action"
	"github.com/yanxicheung/fizzbuzzwhizz/oo/matcher"
)

type Atom struct {
	Matcher matcher.Matcher
	Action  action.Action
}

func (this *Atom) Apply(m int) string {
	if this.Matcher.Match(m) {
		return this.Action.Exec(m)
	}
	return ""
}
