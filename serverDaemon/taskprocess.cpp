#include "taskprocess.h"
#include "serviceMessage.h"
#include "msgtype.h"
#include"messageserver.h"
#include "libtool.h"
TaskProcess::TaskProcess(QObject *parent) : QObject(parent)
{
  connect(this,SIGNAL(MessageReadySignal()),this,SLOT(MessageReadySlot()));
}

TaskProcess::~TaskProcess()
{

}

TaskProcess *TaskProcess::Instance()
{
    static TaskProcess taskProcess;

    return &taskProcess;
}
void TaskProcess::MessageNotify(int data_type,void *msg_data)
{
    if(m_queue.count()==0)
    {
        return;
    }

    char *message=NULL;
    int msglen = sizeof(data_type)+strlen((char*)msg_data);
    message = (char*)malloc(msglen+1);
    memcpy(message,&data_type,sizeof(data_type));
    memcpy(message+sizeof(data_type),msg_data,strlen((char*)msg_data));
    message[msglen+1]='\0';

    {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(message);
    }

    emit MessageReadySignal();
}
void TaskProcess::MessageReadySlot()
{
  int cmd_type=0;
  char databuf[1024]={0};
  char *msg = m_queue.dequeue();
  int datalen = strlen(msg);
  datalen = datalen<1024?datalen:1024;
  if(datalen<sizeof(int))
  {
      std::cout<<"unknown cmd type"<<std::endl;
      return;
  }
  else
  {
      memcpy(databuf,msg,strlen(msg));
      cmd_type = NTOH_Int(databuf);
      ProcQueueMessage(cmd_type,databuf+sizeof(cmd_type),datalen-sizeof(cmd_type));
      return;
  }

}

void TaskProcess::ProcQueueMessage(int cmd_type,void *cmd_data,int data_len)
{
    int ret=0;
    std::string msg_name;
    int msg_type=0;
    MessageData msg_data;
    msg_data.pRcvData=msg_data.pSndData=NULL;
    msg_data.rcvLen=msg_data.sndLen=0;

    switch(cmd_type)
    {
    case MESSAGE_1_REQ:
        msg_type=MESSAGE_1_REQ;
        msg_name="MESSAGE_1_REQ";
        msg_data.pRcvData=cmd_data;
        msg_data.rcvLen=data_len;
        msg_data.pSndData=NULL;
        msg_data.sndLen=0;
        ret = DelegateManager::Instance()->InvokeMessage(msg_type,msg_name,msg_data);
        break;
    case MESSAGE_1_RET:
       break;
    case MESSAGE_2_REQ:
        break;
    case MESSAGE_2_RET:
       break;
    case MESSAGE_3_REQ:
        break;
    case MESSAGE_3_RET:
       break;
    case MESSAGE_4_REQ:
        break;
    case MESSAGE_4_RET:
       break;
    }
}

int TaskProcess::RegMessageServiceHndlrs(DelegateManager *pDlgtMgr)
{
   RegReqMessage1(pDlgtMgr);
   RegReqMessage2(pDlgtMgr);
   RegReqMessage3(pDlgtMgr);
   RegReqMessage4(pDlgtMgr);

   return 0;
}
void TaskProcess::RegReqMessage1(DelegateManager *pDlgtMgr)
{
   ServiceKey svc_key;
   DelegateHandler *msg_dlgt_hndlr=NULL;

   //注册消息请求处理器
   svc_key.msgType=MESSAGE_1_REQ;
   svc_key.msgName="MESSAGE_1_REQ";
   pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::HandleReqMessage1);

   //注册返回消息处理器
   svc_key.msgType=MESSAGE_1_RET;
   svc_key.msgName="MESSAGE_1_RET";
   pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::ProcessMessage);

}

void TaskProcess::RegReqMessage2(DelegateManager *pDlgtMgr)
{
    ServiceKey svc_key;
    //注册消息请求处理器
    svc_key.msgType=MESSAGE_2_REQ;
    svc_key.msgName="MESSAGE_2REQ";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::HandleReqMessage1);

    //注册返回消息处理器
    svc_key.msgType=MESSAGE_2_RET;
    svc_key.msgName="MESSAGE_2_RET";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::ProcessMessage);
}

void TaskProcess::RegReqMessage3(DelegateManager *pDlgtMgr)
{
    ServiceKey svc_key;
    //注册消息请求处理器
    svc_key.msgType=MESSAGE_3_REQ;
    svc_key.msgName="MESSAGE_3_REQ";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::HandleReqMessage1);

    //注册返回消息处理器
    svc_key.msgType=MESSAGE_3_RET;
    svc_key.msgName="MESSAGE_3_RET";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::ProcessMessage);
}

void TaskProcess::RegReqMessage4(DelegateManager *pDlgtMgr)
{
    ServiceKey svc_key;
    //注册消息请求处理器
    svc_key.msgType=MESSAGE_4_REQ;
    svc_key.msgName="MESSAGE_4_REQ";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::HandleReqMessage1);

    //注册返回消息处理器
    svc_key.msgType=MESSAGE_4_RET;
    svc_key.msgName="MESSAGE_4_RET";
    pDlgtMgr->RegisterService<MessageServer>(svc_key,&MessageServer::ProcessMessage);
}
