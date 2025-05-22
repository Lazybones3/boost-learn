import sys
sys.path.append("../../cmake-build-debug/example/PythonClient")
from PyTcpModule import MyClient

client = MyClient("127.0.0.1", 10086)
client.connect()

for i in range(100):
    msg = f"bind-{i+1}"
    client.writeMessage(msg)
    rsp = client.readMessage()
    print("Client received: " + rsp)
