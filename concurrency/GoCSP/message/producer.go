package message

import (
	"context"
	"fmt"
	"sync"
	"time"
)

var producer *Producer = nil
var producer_once sync.Once

func init() {
	// Consumer1 = new(Consumer)
	//类似于C++ std::call_once
	producer_once.Do(func() {
		producer = new(Producer)
		producer._exited = make(chan struct{})
		producer._ctx, producer._cancel = context.WithCancel(context.Background())
		producer.StartWork()
	})
}

func ProducerInst() *Producer {
	return producer
}

type Producer struct {
	_exited chan struct{}
	_ctx    context.Context
	_cancel context.CancelFunc
}

func (producer *Producer) Join() {
	<-producer._exited
	fmt.Println("producer exited")
}

func (producer *Producer) Exit() {
	producer._cancel()
}

func (producer *Producer) StartWork() {
	go func() {
		i := 0
		for {
			i++
			select {
			case <-producer._ctx.Done():
				{
					close(producer._exited)
					return
				}
			case msgChan <- i:
				fmt.Println("producer produce number is ", i)
			}

			time.Sleep(50 * time.Millisecond)
		}
	}()
}
