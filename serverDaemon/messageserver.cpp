#include "messageserver.h"
#include <QDebug>
#include "logging.h"
#include "log_severity.h"
MessageServer::MessageServer()
{
   n=0;
}

MessageServer::~MessageServer()
{

}


int MessageServer::HandleReqMessage1(ServiceKey const * const svc_key,
                                MessageData &msg_data)
{
    GLOG(INFO)<< "HandleReqMessage1：svcType = " << svc_key->msgType << ", " << svc_key->msgName << std::endl;
    //assert(msg_data.rcvLen == sizeof(antiapi::SImitateTicketOperCmd));
    //    antiapi::SImitateTicketOperCmd *anti_cmd = (antiapi::SImitateTicketOperCmd *)msg_data.pRcvData;
    return 0;
}


int MessageServer::HandleReqMessage2(ServiceKey const * const svc_key,
                                MessageData &msg_data)
{
     GLOG(INFO)<< "HandleReqMessage1：svcType = " << svc_key->msgType << ", " << svc_key->msgName << std::endl;
     return 0;
}

int MessageServer::HandleReqMessage3(ServiceKey const * const svc_key,
                                MessageData &msg_data)
{
     GLOG(INFO)<< "HandleReqMessage1：svcType = " << svc_key->msgType << ", " << svc_key->msgName << std::endl;
     return 0;
}


int MessageServer::HandleReqMessage4(ServiceKey const * const svc_key,
                                MessageData &msg_data)
{
     GLOG(INFO)<< "HandleReqMessage1：svcType = " << svc_key->msgType << ", " << svc_key->msgName << std::endl;
     return 0;
}
