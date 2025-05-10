package message

const MAX_COUNT = 200

var msgChan = make(chan int, MAX_COUNT)
