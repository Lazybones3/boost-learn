#ifndef BOOSTLEARN_MSGNODE_H
#define BOOSTLEARN_MSGNODE_H

#include <iostream>

#define MAX_LENGTH (1024*2)
#define HEAD_TOTAL_LEN 4
#define HEAD_ID_LEN 2
#define HEAD_DATA_LEN 2

enum MSG_IDS {
    LOGIN_REQ = 1001,
    LOGIN_RSP = 1002
};

class MsgNode {
public:
    MsgNode(short max_len) : total_len(max_len), cur_len(0) {
        data = new char[total_len + 1]();
        data[total_len] = '\0';
    }

    ~MsgNode() {
        std::cout << "destruct MsgNode" << std::endl;
        delete[] data;
    }

    void clear() {
        ::memset(data, 0, total_len);
        cur_len = 0;
    }

    short cur_len;
    short total_len;
    char *data;
};

class RecvNode : public MsgNode {
public:
    RecvNode(short max_len, short msg_id);

public:
    short _msg_id;
};

class SendNode : public MsgNode {
public:
    SendNode(const char *msg, short max_len, short msg_id);

private:
    short _msg_id;
};

#endif //BOOSTLEARN_MSGNODE_H
