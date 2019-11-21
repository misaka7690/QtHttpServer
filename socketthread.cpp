#include "socketthread.h"

SocketThread::SocketThread(SOCKET ServSock,QString path) : QThread(parent)
{
    this->ServSock = ServSock;
    this->folderpath = path;
}

SocketThread::~SocketThread()
{
    closesocket(ServSock);
    emit isClose();
}

void SocketThread::run()
{
        sockaddr_in clientAddr;
        int Addrlen = sizeof(clientAddr);
        while(!isInterruptionRequested()){
            //每次接受新客户端将之前的地址信息清0
            memset(&clientAddr,0,sizeof(clientAddr));

            //等待新客户端连接,阻塞函数，结束线程
            SOCKET client = ::accept(ServSock, (sockaddr*)&clientAddr, &Addrlen);

            //获取客户端IP和Port
            char* clientIp = inet_ntoa(clientAddr.sin_addr);
            int clientPort = ntohs(clientAddr.sin_port);
            QTime time = QTime::currentTime();
            QString str = time.toString("hh:mm:ss");
            QString msg = QString("%1 [%2:%3] connect success").arg(str).arg(clientIp).arg(clientPort);

            //新客户端连接，通知 UI 更新界面
            emit isMsg(msg);
            //开启新线程和客户端进行通信
            MsgThread* msgThread = new MsgThread(client,clientAddr,folderpath,parent);
            msgThread->start();

            connect(msgThread,&MsgThread::isMsg,this,[=](QString msg){
                //转发消息给 UI进程，UI进行界面更新
                emit isMsg(msg);
            });

            connect(this,&SocketThread::isClose,this,[=](){
                msgThread->terminate();
                msgThread->quit();
                delete msgThread;
            });
        }
}
