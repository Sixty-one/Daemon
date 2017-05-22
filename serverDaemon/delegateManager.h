/*
-*  Name: delegateManager.h
-*  Function: C++ delegate service model (Manager)
*/

#ifndef DELEGATE_MANAGER_H
#define DELEGATE_MANAGER_H

#include <cassert>
#include <string>
#include <typeinfo>
#include <memory>  // for unique_ptr
#include <map>
//#include <unordered_map>
#include <algorithm>
using namespace std;

//#include "osmutex.h"

#include "msgServer.h"

#include "delegateHandler.h"

class DelegateManager
{
public:
    static DelegateManager *Instance();
    ~DelegateManager(void);

protected:  // not useable outside
    //~DelegateManager(void);
    DelegateManager(void);
    DelegateManager(DelegateManager const &);
    DelegateManager & operator= (DelegateManager const &);
    // c++11标准新增：
    //DelegateManager (DelegateManager &&);               // 移动构造函数 (move constructor)
    //DelegateManager & operator= (DelegateManager &&);   // 移动赋值操作符函数 (move assignment)

public:
    // register default services and handlers
    int RegDefaultServiceHndlr(void);

    // invoke message processor
    int InvokeMessage(int msg_type, std::string msg_name, MessageData &msg_data);
    // invoke message processor
    int InvokeMessage(ServiceIdent const &svc_ident, MessageData &msg_data, void * const msg_param);

public:
    template <typename T>
    int RegisterService(ServiceKey svc_key, int (T::*hndlr)(ServiceKey const * const, MessageData &))
    {
        if (svc_key.msgType <= 0 || hndlr == NULL/*nullptr*/)
            return -1;  // 参数错误

        std::map<ServiceKey, DelegateHandler *>::const_iterator iterH;
        iterH = dlgt_Hndlrs.find(svc_key);
        if (iterH != dlgt_Hndlrs.end())
            return 1;  // 服务已注册

        T *server = NULL/*nullptr*/;
        std::string type_name = typeid(T).name();
        std::map<std::string, MsgServer *>::iterator iterS = msg_Servers.end();
        
		// 判断该类服务器是否已存在
        iterS = msg_Servers.find(type_name);
        if (iterS != msg_Servers.end())
        {
            T *old_server = dynamic_cast<T *>(iterS->second);
            if (NULL/*nullptr*/ != old_server)
            {
                server = old_server;
            }
            else
            {
                server = new T();
                if(NULL/*nullptr*/ == dynamic_cast<MsgServer *>(server))
                {
                    delete server;
                    return -1;
                }
            }
        }
        else
        {
            server = new T();
            std::map<std::string, MsgServer *>::iterator iterS = msg_Servers.end();
            if(NULL/*nullptr*/ == dynamic_cast<MsgServer *>(server))
            {
                delete server;
                return -1;
            }
            // 将服务器插入到容器
            msg_Servers[type_name] = server;
        }
        // assert(server != NULL/*nullptr*/);
        TDelegateHandler<T> *dlgt_hndlr = NULL/*nullptr*/;
        dlgt_hndlr = new TDelegateHandler<T>(server, hndlr);
        if (NULL/*nullptr*/ == dynamic_cast<DelegateHandler *>(dlgt_hndlr))
        {
            delete dlgt_hndlr;
            if (iterS == msg_Servers.end())  // 已新增服务器并插入到容器
            {
                iterS = msg_Servers.find(type_name);
            }
            if (iterS == msg_Servers.end())  // 在容器中已找到该服务器
            {
                assert(server == iterS->second);
                delete server;
                msg_Servers.erase(iterS);
            }
            return -1;
        }

        dlgt_Hndlrs[svc_key] = dlgt_hndlr;
        return 0;
    }

public:
    template <typename T>
    int RegisterService(ServiceKey svc_key, int (T::*handler)(ServiceIdent const * const, MessageData &, void * const))
    {
        if (svc_key.msgType <= 0 || handler == NULL/*nullptr*/)
            return -1;  // 参数错误

        std::map<ServiceKey, DelegateHandler *>::const_iterator iterH;
        iterH = dlgt_Hndlrs.find(svc_key);
        if (iterH != dlgt_Hndlrs.end())
            return 1;  // 服务已注册

        T *server = NULL/*nullptr*/;
        std::string type_name = typeid(T).name();
        std::map<std::string, MsgServer *>::iterator iterS = msg_Servers.end();
        
		// 判断该类服务器是否已存在
        iterS = msg_Servers.find(type_name);
        if (iterS != msg_Servers.end())
        {
            T *old_server = dynamic_cast<T *>(iterS->second);
            if (NULL/*nullptr*/ != old_server)
            {
                server = old_server;
            }
            else
            {
                server = new T();
                if(NULL/*nullptr*/ == dynamic_cast<MsgServer *>(server))
                {
                    delete server;
                    return -1;
                }
            }
        }
        else
        {
            server = new T();
            std::map<std::string, MsgServer *>::iterator iterS = msg_Servers.end();
            if(NULL/*nullptr*/ == dynamic_cast<MsgServer *>(server))
            {
                delete server;
                return -1;
            }
            // 将服务器插入到容器
            msg_Servers[type_name] = server;
        }
        // assert(server != NULL/*nullptr*/);
        TDelegateHandler<T> *dlgt_hndlr = NULL/*nullptr*/;
        dlgt_hndlr = new TDelegateHandler<T>(server, handler);
        if (NULL/*nullptr*/ == dynamic_cast<DelegateHandler *>(dlgt_hndlr))
        {
            delete dlgt_hndlr;
            if (iterS == msg_Servers.end())  // 已新增服务器并插入到容器
            {
                iterS = msg_Servers.find(type_name);
            }
            if (iterS == msg_Servers.end())  // 在容器中已找到该服务器
            {
                assert(server == iterS->second);
                delete server;
                msg_Servers.erase(iterS);
            }
            return -1;
        }

        dlgt_Hndlrs[svc_key] = dlgt_hndlr;
        return 0;
    }

protected:
    // clearance
    void Clear();

protected:
    // register delegate service and handler
    int RegServiceHndlr(ServiceKey const &svc_key, DelegateHandler * const dlgt_hndlr);

protected:
    static DelegateManager *ptr_Instance;
    //static std::unique_ptr<DelegateManager> ptr_Instance;

private:
    // container of message servers
    typedef std::map<std::string, MsgServer *> DlgtServerMap;  // 消息服务器容器
    DlgtServerMap msg_Servers;
    // container of message handlers
    typedef std::map<ServiceKey, DelegateHandler *> DlgtHndlrMap;  // 消息服务处理对象容器
    DlgtHndlrMap dlgt_Hndlrs;

private:
    //OSMutex *ptr_Mutex;
    friend class auto_ptr<DelegateManager>;
};

#endif  // DELEGATE_MANAGER_H
