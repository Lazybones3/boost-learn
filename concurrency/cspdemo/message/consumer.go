package message

import (
	"context"
	"fmt"
	"sync"
	"time"
)

var consumer *Consumer = nil
var consumer_once sync.Once

func init() {
	// Consumer1 = new(Consumer)
	//类似于C++ std::call_once
	consumer_once.Do(func() {
		consumer = new(Consumer)
		consumer._exited = make(chan struct{})
		consumer._ctx, consumer._cancel = context.WithCancel(context.Background())
		consumer.StartWork()
	})
}

func ConsumerInst() *Consumer {
	return consumer
}

type Consumer struct {
	_exited chan struct{}
	_ctx    context.Context
	_cancel context.CancelFunc
}

func (consumer *Consumer) Join() {
	<-consumer._exited
	fmt.Println("consumer exited")
}

func (consumer *Consumer) Exit() {
	consumer._cancel()
}

func (consumer *Consumer) StartWork() {
	go func() {
		i := 0
		for {
			select {
			case <-consumer._ctx.Done():
				{
					close(consumer._exited)
					return
				}
			case i = <-msgChan:
				fmt.Println("consumer consum number is ", i)
			}

			time.Sleep(100 * time.Millisecond)
		}
	}()
}
