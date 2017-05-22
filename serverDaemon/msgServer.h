/*

-*  Name: msgServer.h

-*  Function: C++ delegate service model (Server)

*/


#ifndef MSG_SERVER_H

#define MSG_SERVER_H

#include <iostream>
#include <string>

#include <sstream>  // for stringstream
#include <map>
//#include <unordered_map>
#include <algorithm>
using namespace std;

#include "serviceMessage.h"

class MsgServer
{
public:
    MsgServer() { }
    virtual ~MsgServer()  // 定义为虚函数，使得派生类多态析构
    {
    }

public:
    // message service processor
    virtual int HandleMessage(ServiceKey const * const svc_key,
                           MessageData &msg_data)
    {
        std::cout << svc_key->msgType << "_" << svc_key->msgName << std::endl;
        return 0;
    }

    // message service processor
    virtual int ProcessMessage(ServiceIdent const * const svc_ident,
                           MessageData &msg_data, void * const msg_param)
    {
        std::cout << svc_ident->svcKey.msgType << "_" << svc_ident->svcKey.msgName << std::endl;
        return 0;
    }

private:
};

#endif // MSG_SERVER_H
