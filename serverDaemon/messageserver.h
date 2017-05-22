#ifndef MESSAGESERVER_H
#define MESSAGESERVER_H
#include"msgServer.h"

class MessageServer : public MsgServer
{
public:
    MessageServer();
    virtual ~MessageServer();
public:

    int HandleReqMessage1(ServiceKey const * const svc_key,
                                    MessageData &msg_data);


    int HandleReqMessage2(ServiceKey const * const svc_key,
                                    MessageData &msg_data);
    int HandleReqMessage3(ServiceKey const * const svc_key,
                                    MessageData &msg_data);


    int HandleReqMessage4(ServiceKey const * const svc_key,
                                    MessageData &msg_data);
protected:
    int n;
};

#endif // MESSAGESERVER_H
