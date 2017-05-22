#include <QCoreApplication>
#include "serverapp.h"


static void initLog(char *arg)
{
    // 初始化日志记录接口
    fLB::FLAGS_logtostderr = false;
    fLB::FLAGS_alsologtostderr = true;
    fLB::FLAGS_colorlogtostderr = true;
    setbuf(stderr, NULL);
    fLI::FLAGS_minloglevel = google::GLOG_INFO;
    fLI::FLAGS_stderrthreshold = google::GLOG_INFO;
    fLI::FLAGS_v = 0;
    // 设置日志缓存时间（0为实时输出）
    fLI::FLAGS_logbufsecs  = 0;
    // 设置日志文件的大小（MB）
    fLI::FLAGS_max_log_size = 16;
    // 设置日志文件保存目录
    std::string log_path;
    std::string log_name_info;
    std::string log_name_warnning;
    std::string log_name_error;
    log_path = "./";
    log_name_info = log_path+"TESTSRV-INFO";
    log_name_warnning = log_path+"TESTSRV-WARNING";;
    log_name_error = log_path+"TESTSRV-ERROR";;

    fLS::FLAGS_log_dir = log_path;  // 这个目录必须是已经存在的,否则不能生成日志文件.
    //初始化 Glog，参数 argv[0] 为程序名
    google::InitGoogleLogging(arg);
    // 指定INFO、ERROR、WARNING 三个级别的日志文件存放位置。
    // 如果指向同一个位置，则三个文件会合并成一个文件
    // INFO级别的日志存放位置
    google::SetLogDestination(google::GLOG_INFO, log_name_info.c_str());
    // WARNING级别的日志存放位置
    google::SetLogDestination(google::GLOG_WARNING, log_name_warnning.c_str());
    // ERROR级别的日志存放位置
    google::SetLogDestination(google::GLOG_ERROR, log_name_error.c_str());
    // 指定日志文件后缀名
    //google::SetLogFilenameExtension(".dat");
    // 将INFO级别及以上的日志都发送到标准输出
    google::SetStderrLogging(google::GLOG_INFO);
}

int main(int argc, char *argv[])
{
    initLog(argv[0]);
    ServerApp a(argc, argv);
    a.InitInstance();
    a.exec();
    a.exitInstance();
    // 退出日志记录，释放系统资源
    google::ShutdownGoogleLogging();
    return 0;
}
