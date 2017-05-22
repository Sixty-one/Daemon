#ifndef TASKPROCESS_H
#define TASKPROCESS_H

#include <QObject>
#include <QQueue>
#include <QMutexLocker>
#include <QMutex>
#include "msgServer.h"
#include "delegateManager.h"
class TaskProcess : public QObject
{
    Q_OBJECT
public:
    explicit TaskProcess(QObject *parent = 0);
    ~TaskProcess();
public:
    static TaskProcess *Instance();
    void ProcQueueMessage(int cmd_type, void *cmd_data, int data_len);
    int RegMessageServiceHndlrs(DelegateManager *pDlgtMgr);
    void MessageNotify(int data_type,void *msg_data);
public:
    void RegReqMessage1(DelegateManager *pDlgtMgr);
    void RegReqMessage2(DelegateManager *pDlgtMgr);
    void RegReqMessage3(DelegateManager *pDlgtMgr);
    void RegReqMessage4(DelegateManager *pDlgtMgr);
signals:
    void MessageReadySignal();
public slots:
    void MessageReadySlot();
private:
   QMutex m_mutex;
   QQueue<char *> m_queue;
};

#endif // TASKPROCESS_H
