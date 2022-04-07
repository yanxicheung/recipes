package rule

type Rule interface {
	Apply(m int) string
}
