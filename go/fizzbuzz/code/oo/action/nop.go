package action

import "strconv"

type Nop struct {
}

func (this *Nop) Exec(m int) string {
	return strconv.Itoa(m)
}
