## 网络编程

- 同步服务：SyncServer、SyncClient
- 应答模式的异步服务：AsyncServer01
- 全双工通信模式以及缓冲队列：AsyncServer02
- 处理粘包方式一：AsyncServer03、SyncClient03
- 处理粘包方式二：ReadAtLeastServer、PacketStickClient
- 粘包测试：PacketStickServer、PacketStickClient
- 处理大小端字节序：AsyncServer04、SyncClient04
- Protobuf序列化：ProtobufServer、ProtobufClient
- Json序列化：JsonServer、JsonClient
- tlv消息格式：MsgIdServer、MsgIdClient
- 逻辑处理服务（单例类、条件变量、消息回调函数）：LogicServer01、MsgIdClient
- Asio方式优雅退出：LogicServer02、MsgIdClient
- IOServicePool多线程模型：IOContextPoolServer、IOContextPoolClient
- IOThreadPool多线程模型：ThreadPoolServer、IOContextPoolClient
- 改进的IOThreadPool多线程模型：StrandPoolServer、IOContextPoolClient
- 协程：CoroutineServer、IOContextPoolClient