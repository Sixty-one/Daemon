/*
-*  Name: delegateHandler.h
-*  Function: C++ delegate service model (Handler)
*/

#ifndef DELEGATE_HANDLER_H
#define DELEGATE_HANDLER_H

#include <string>
#include <sstream>  // for stringstream
#include <map>
// #include <unordered_map>
#include <algorithm>
using namespace std;

#include "serviceMessage.h"

class DelegateHandler
{
public:
    DelegateHandler() { }
    virtual ~DelegateHandler()  // ����Ϊ�麯����ʹ���������̬����
    {
    }

public:
    // message data processor (����Ϊ���麯������������ʵ��)
    virtual int Invoke( ServiceKey const * const svc_key,
                        MessageData &msg_data) = 0;

    // message data processor (����Ϊ�麯������������̳�)
    virtual int Invoke( ServiceIdent const * const svc_ident,
                        MessageData &msg_data, void * const msg_param = NULL/*nullptr*/)
    {
        return Invoke(&svc_ident->svcKey, msg_data);
    }

protected:
};

template<typename T>
class TDelegateHandler : public DelegateHandler
{
public:
    TDelegateHandler(T *server,
                    int (T::*hndlr)(ServiceKey const * const, MessageData &))
        : ptr_T(server), ptr_Hndlr(hndlr)
    {
        ptr_Handler = NULL/*nullptr*/;
    }

    TDelegateHandler(T *server,
                    int (T::*handler)(ServiceIdent const * const, MessageData &, void * const))
        : ptr_T(server), ptr_Handler(handler)
    {
        ptr_Hndlr = NULL/*nullptr*/;
    }
    
    TDelegateHandler(T *server,
                    int (T::*hndlr)(ServiceKey const * const, MessageData &),
                    int (T::*handler)(ServiceIdent const * const, MessageData &, void * const))
        : ptr_T(server), ptr_Hndlr(hndlr), ptr_Handler(handler)
    {
    }

public:
    // invoke service processor (����Ϊ�麯����ʹ����������Ϊ��̬��)
    virtual int Invoke(ServiceKey const * const svc_key,
                       MessageData &msg_data)
    {
        if (ptr_Hndlr == NULL/*nullptr*/)
            return -1;
        else
            return (ptr_T->*ptr_Hndlr)(svc_key, msg_data);
    }

    // invoke service processor (����Ϊ�麯����ʹ����������Ϊ��̬��)
    virtual int Invoke(ServiceIdent const * const svc_ident,
                       MessageData &msg_data, void * msg_param = NULL/*nullptr*/)
    {
        if (ptr_Handler == NULL/*nullptr*/)
        {
            return (ptr_T->*ptr_Hndlr)(&svc_ident->svcKey, msg_data);
        }
        else
        {
            return (ptr_T->*ptr_Handler)(svc_ident, msg_data, msg_param);
        }
    }

protected:
    int (T::*ptr_Hndlr)(ServiceKey const * const, MessageData &);
    int (T::*ptr_Handler)(ServiceIdent const * const, MessageData &, void * const);

private:
    T *ptr_T;

private:
    //std::tr1::shared_ptr<T> ptr_T;
};

template<>
class TDelegateHandler<void> : public DelegateHandler
{
public:
    TDelegateHandler(int (*hndlr)(ServiceKey const * const, MessageData &))
        : ptr_Hndlr(hndlr)
    {
        ptr_Hndlr = hndlr;
        ptr_Handler = NULL/*nullptr*/;
    }

    // set message handler
    void SetHandler(int (*handler)(ServiceIdent const * const, MessageData &, void * const))
    {
        ptr_Handler = handler;
    }

    // message data processor (����Ϊ�麯����ʹ����������Ϊ��̬��)
    virtual int Invoke(ServiceKey const * const svc_key,
                       MessageData &msg_data)
    {
        return (*ptr_Hndlr)(svc_key, msg_data);
    }

    // message data processor (����Ϊ�麯����ʹ����������Ϊ��̬��)
    virtual int Invoke(ServiceIdent const * const svc_ident,
                       MessageData &msg_data, void * const msg_param = NULL/*nullptr*/)
    {
        if (ptr_Handler == NULL/*nullptr*/)
            return (*ptr_Hndlr)(&svc_ident->svcKey, msg_data);
        else
            return (*ptr_Handler)(svc_ident, msg_data, msg_param);
    }

protected:
    int (*ptr_Hndlr)(ServiceKey const * const, MessageData &);
    int (*ptr_Handler)(ServiceIdent const * const, MessageData &, void * const);

private:
};

#endif  // DELEGATE_HANDLER_H
