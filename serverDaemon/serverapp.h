#ifndef SERVERAPP_H
#define SERVERAPP_H

#include <QCoreApplication>
#include "logging.h"
#include "log_severity.h"
class ServerApp : public QCoreApplication
{
public:
    ServerApp(int argc, char *argv[]);
    ~ServerApp();
public:
    void InitInstance();
    void exitInstance();
#if defined(WIN32)
    //##ModelId=3F136BF9031A
    WSADATA	wsa_data;		// 版本信息
#endif
};

#endif // SERVERAPP_H
