package action

type To struct {
	Word string
}

func (this *To) Exec(m int) string {
	return this.Word
}
