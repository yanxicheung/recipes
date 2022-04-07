package matcher

import (
	"strconv"
	"strings"
)

type Contains struct {
	N int
}

func toStr(num int) string {
	return strconv.Itoa(num)
}

func (this *Contains) Match(m int) bool {
	return strings.Contains(toStr(m), toStr(this.N))
}
