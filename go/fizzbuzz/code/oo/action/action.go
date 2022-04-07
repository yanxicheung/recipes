package action

type Action interface {
	Exec(m int) string
}
