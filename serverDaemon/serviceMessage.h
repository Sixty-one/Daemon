/*
-*  Name: serviceMessage.h
-*  Function: C++ delegate service model (Handler)
*/

#ifndef SERVICE_MESSAGE_H
#define SERVICE_MESSAGE_H

#include <string>
#include <sstream>  // for stringstream
#include <map>
// #include <unordered_map>
#include <algorithm>
using namespace std;

typedef struct ServiceKey_T
{
    int msgType;
    std::string msgName;

    ServiceKey_T()
    {
        msgType = 0;
        msgName = "";
    }
    bool operator< (const struct ServiceKey_T &v) const
    {
        std::stringstream strm;
        std::stringstream strmV;
        strm << msgType << '_' << msgName;
        strmV << v.msgType << '_' << v.msgName;
        return (0 > strm.str().compare(strmV.str()));
    }
} ServiceKey;

typedef struct ServiceIdent_T
{
    ServiceKey  svcKey;
    std::string clntIdent;
    
    ServiceIdent_T()
    {
        svcKey.msgType = 0;
        svcKey.msgName = "";
        clntIdent = "";
    }
} ServiceIdent;

typedef struct MessageData_T
{
    void *pRcvData;
    size_t  rcvLen;
    void *pSndData;
    size_t  sndLen;

    MessageData_T()
    {
        pRcvData = NULL/*nullptr*/;
        rcvLen = 0;
        pSndData = NULL/*nullptr*/;
        sndLen = 0;
    }
} MessageData;

#endif  // SERVICE_MESSAGE_H
