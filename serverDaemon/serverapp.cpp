#include "serverapp.h"
#include "libtool.h"
ServerApp::ServerApp(int argc, char *argv[]):QCoreApplication(argc,argv)
{

}

ServerApp::~ServerApp()
{

}

void ServerApp::InitInstance()
{
#if defined (WIN32)
    WSAStartup(MAKEWORD(2,2), &wsa_data);
#endif

}

void ServerApp::exitInstance()
{
    // 必须在进程退出前卸载 Protobuf 以避免内存泄露
    google::protobuf::ShutdownProtobufLibrary();

#if defined(WIN32)
    WSACleanup();
#endif

}
