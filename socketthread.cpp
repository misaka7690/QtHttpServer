#include "socketthread.h"

SocketThread::SocketThread(SOCKET ServSock,QString path)
{
    this->ServSock = ServSock;
    this->folderpath = path;
}


SocketThread::~SocketThread()
{
    // closesocket(ServSock);
    // emit isClose();
}

void SocketThread::run()
{
    // emit logmsg("hello");
    sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    while(!isInterruptionRequested())
    {
        // 地址信息清零
        memset(&clientaddr,0,sizeof(clientaddr));

        // 等待新客户端连接，阻塞函数
        SOCKET client = ::accept(ServSock,(sockaddr*)&clientaddr,&addrlen);
        char* clientIp = inet_ntoa(clientaddr.sin_addr);
        int clientPort = ntohs(clientaddr.sin_port);
        QTime time = QTime::currentTime();
        QString str = time.toString("hh:mm:ss");
        // QString msg = QString("%1 [%2:%3]客户端发起请求！").arg(str).arg(clientIp).arg(clientPort);

        //新客户端连接，通知 GUI 更新界面
        // emit logmsg(msg);

        MsgThread *p_msgthread = new MsgThread(client,clientaddr,folderpath);

        // 信号槽连接
        connect(p_msgthread,&MsgThread::logmsg,this,logmsg);

        p_msgthread->start();

        // connect(this,)


    }
}
