import socket
import struct
import threading
import time

HEAD_ID_LEN = 2
HEAD_DATA_LEN = 2

def send_work(sock):
    while True:
        time.sleep(0.002)  # 2 milliseconds
        request_id = 1001
        request = "hello world!"
        request_length = len(request)

        # 构造消息头和消息
        send_data = struct.pack('!H', request_id) + struct.pack('!H', request_length) + request.encode('utf-8')

        # 发送数据
        sock.sendall(send_data)

def recv_work(sock):
    print("begin to receive...")
    while True:
        time.sleep(0.002)  # 2 milliseconds

        # 读取消息ID
        reply_id = sock.recv(HEAD_ID_LEN)
        if not reply_id:
            break
        msgid = struct.unpack('!H', reply_id)[0]

        # 读取消息头
        reply_head = sock.recv(HEAD_DATA_LEN)
        if not reply_head:
            break
        msglen = struct.unpack('!H', reply_head)[0]

        # 读取消息体
        msg = sock.recv(msglen)
        print("Reply id: ", msgid, " len: ", msglen, " msg: ", msg.decode('utf-8'))

def main():
    try:
        # 创建socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # 连接到服务器
        sock.connect(("127.0.0.1", 10086))

        # 启动发送和接收线程
        t1 = threading.Thread(target=send_work, args=(sock,), daemon=True)
        t1.start()
        t2 = threading.Thread(target=recv_work, args=(sock,), daemon=True)
        t2.start()

        t1.join()
        t2.join()
    except Exception as e:
        print("Exception:", str(e))
    finally:
        sock.close()

if __name__ == "__main__":
    main()
