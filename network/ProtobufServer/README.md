- Ubuntu安装Protobuf

```shell
# 安装C++库
sudo apt install libprotobuf-dev
# 安装protoc编译工具
sudo apt install protobuf-compiler
```

- 编译proto文件

```shell
protoc --cpp_out=. ./msg.proto
```
