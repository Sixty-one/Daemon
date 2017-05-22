/*
-*  Name: delegateManager.cpp
-*  Function: C++ delegate service model (Manager implementation)
-*  Usage Example:
-*    int key = USER_DATAQUERY;
-*    char szCode[32]= {0};
-*    sprintf(szCode, "%d", key);
-*    string strKey = szCode;
-*    TestMessage *pTestMessage = new TestMessage();
-*    DelegateHandler<TestMessage> *pTestMessageHandler
-*            = new DelegateHandler<TestMessage>
-*                    (pTestMessage, &TestMessage::InvokeTest);
-*    DelegateManager::Instance()->RegServiceHndlr(strKey, pTestMessageHandler);
-*
-*  Note:
-*    delete the TestMessage object and DelegateHandler<TestMessage> object.
-*
*/

#include <ctime>
#include <string>
#include <iostream>
#include <map>
//#include <unordered_map>
#include <algorithm>
using namespace std;

#include "delegateHandler.h"
#include "delegateManager.h"

#include "msgServer.h"

//unique_ptr<DelegateManager> DelegateManager::ptr_Instance;
DelegateManager *DelegateManager::ptr_Instance = NULL;

DelegateManager *DelegateManager::Instance()
{
    //if(ptr_Instance.get() == nullptr)
    //    ptr_Instance.reset(new DelegateManager());
    //return ptr_Instance.get();
    if(ptr_Instance == NULL)
        ptr_Instance = new DelegateManager();
    return ptr_Instance;
}
// ctor
DelegateManager::DelegateManager()
{
    //ptr_Mutex = new OSMutex();
}
// dtor
DelegateManager::~DelegateManager()
{
    Clear();
}
// clearance
void DelegateManager::Clear()
{
	DlgtHndlrMap::iterator hndlrIter;
    for (hndlrIter = dlgt_Hndlrs.begin(); hndlrIter != dlgt_Hndlrs.end(); ++hndlrIter)
    {
        if (NULL/*nullptr*/ != hndlrIter->second)
        {
            delete (hndlrIter->second);
        }
    }
    dlgt_Hndlrs.clear();

    DlgtServerMap::iterator servIter;
    for (servIter = msg_Servers.begin(); servIter != msg_Servers.end(); ++servIter)
    {
        if (servIter->second != NULL/*nullptr*/)
        {
            delete (servIter->second);
        }
    }
    msg_Servers.clear();

    //if(ptr_Mutex != nullptr)
    //{
    //    delete ptr_Mutex;
    //    ptr_Mutex = nullptr;
    //}
}

int DefaultHndlr(ServiceKey const * const svc_key, MessageData & msg_data)
{
    std::cout << "something wrong\n" << std::endl;
    return 0;
}

// register default services and handlers
int DelegateManager::RegDefaultServiceHndlr(void)
{
    std::string serverTypeName;
    ServiceKey svcKey;
    svcKey.msgType = 0;
    svcKey.msgName = "debug";
    DelegateHandler *pHandler = NULL/*nullptr*/;
    pHandler = new TDelegateHandler<void>(&DefaultHndlr);
    serverTypeName = "void";
    msg_Servers[serverTypeName] = NULL/*nullptr*/;
    RegServiceHndlr(svcKey, pHandler);
    pHandler = NULL/*nullptr*/;

    return 0;
}
// register delegate service and handler
int DelegateManager::RegServiceHndlr(ServiceKey const &svc_key,
                                     DelegateHandler * const handler)
{
    //OSMutexLock lock(ptr_Mutex);

    if (svc_key.msgType <= 0 || handler == NULL/*nullptr*/)
        return -1;  // 参数错误

    std::map<ServiceKey, DelegateHandler *>::const_iterator iter;
    iter = dlgt_Hndlrs.find(svc_key);
    if (iter != dlgt_Hndlrs.end())
        return 1;  // 服务已注册

    dlgt_Hndlrs[svc_key] = handler;
    return 0;
}

static int entry_count = 0;  // for test

// invoke message processor
int DelegateManager::InvokeMessage(int msg_type, std::string msg_name, MessageData &msg_data)
{
    //OSMutexLock lock(ptr_Mutex);
    ++entry_count;

    DelegateHandler *handler = NULL/*nullptr*/;
    ServiceKey svc_key;
    svc_key.msgType = msg_type;
    svc_key.msgName = msg_name;
    DlgtHndlrMap::const_iterator iter = dlgt_Hndlrs.find(svc_key);
    if (iter != dlgt_Hndlrs.end())
    {
        handler = iter->second;
    }

    if(handler == NULL/*nullptr*/)
        return -1;

    time_t tick;
    struct tm * timeinfo;
    char buffer[32];

    time(&tick);
    timeinfo = localtime (&tick);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    //MessageParam msgParam(entry_count, buffer);
    handler->Invoke(&svc_key, msg_data);

    return 0;
}

// invoke message processor
int DelegateManager::InvokeMessage(ServiceIdent const &svc_ident,
                                    MessageData &msg_data,
                                    void * const msg_param)
{
    //OSMutexLock lock(ptr_Mutex);
    entry_count++;

    DelegateHandler *pHandler = NULL/*nullptr*/;
    DlgtHndlrMap::const_iterator iter = dlgt_Hndlrs.find(svc_ident.svcKey);
    if (iter != dlgt_Hndlrs.end())
    {
        pHandler = iter->second;
    }

    if(pHandler == NULL/*nullptr*/)
        return -1;

    time_t tick;
    struct tm * timeinfo;
    char buffer[32];

    time(&tick);
    timeinfo = localtime (&tick);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    pHandler->Invoke(&svc_ident, msg_data, NULL/*nullptr*/);

    return 0;
}
