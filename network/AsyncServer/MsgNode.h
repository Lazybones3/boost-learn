#ifndef ASYNCSERVER_MSGNODE_H
#define ASYNCSERVER_MSGNODE_H

#include "CSession.h"

class MsgNode {
    friend class CSession;

public:
    MsgNode(char *msg, int max_len) {
        _data = new char[max_len];
        memcpy(_data, msg, max_len);
    }

    ~MsgNode() {
        delete[] _data;
    }

private:
    int _cur_len;
    int _max_len;
    char* _data;
};

#endif //ASYNCSERVER_MSGNODE_H
