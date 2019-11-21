#include "socketthread.h"

// SocketThread::SocketThread(SOCKET ServSock,QString path) : QThread(parent)
// {
//     this->ServSock = ServSock;
//     this->folderpath = path;
// }

SocketThread::SocketThread()
{

}
SocketThread::~SocketThread()
{
    // closesocket(ServSock);
    // emit isClose();
}

void SocketThread::run()
{
       qDebug() << "there";
}
