#include "msgthread.h"
#include "utils.h"

MsgThread::MsgThread(SOCKET ClientSock,sockaddr_in mAddr,QString path) 
{
    this->ClientSock = ClientSock;
    this->mAddr = mAddr;
    this->webroot=path;
}

MsgThread::~MsgThread()
{
    int ret = closesocket(ClientSock);
}

void MsgThread::run(){
        //inet_addr点分十进制转网络ip地址 ,inet_ntoa网络转点分十进制
        char* clientIp = inet_ntoa(mAddr.sin_addr);
        int clientPort = ntohs(mAddr.sin_port);

        while(true){
            memset(resp,0,1024);
            char recv_buf[1024] = {0};
            //阻塞等待 接受信息
            int ret = recv(ClientSock , recv_buf, 1024,0);
            QString recv_data = QString(QLatin1String(recv_buf));

            QTime time = QTime::currentTime();
            QString str = time.toString("hh:mm:ss");
            if(ret == 0){//连接断开
                emit logmsg(QString("%1 [%2:%3] is closed!!!").arg(str).arg(clientIp).arg(clientPort));
                break;
            }


            QString filepath = GetUrlRequestFilePath(recv_data);
            QString msg = QString("来自") + QString(clientIp) + QString("(端口号为") + QString::number(clientPort) + QString(")的客户端请求") + filepath;
            emit logmsg(msg);
            int filetype = judgefiletype(filepath);

            filepath = webroot + filepath;


            // if(filetype==0){
            //     filepath = "D:\\404.html";
            //     if(!sendText(ClientSock,filepath))
            //         emit logmsg(QString("$ the file cannot be found!"));
            // }
            // if(filetype==1)
            // {
            //     if(!sendText(ClientSock,filepath))
            //         emit logmsg(QString("$ the file cannot be found!"));
            // }
            // if(filetype==2)
            // {
            //     if(!sendImage(ClientSock,filepath))
            //         emit logmsg(QString("$ the file cannot be found!"));
            // }

            // emit logmsg(QString("$ 给客户端发送: %1").arg(filename));

        }
}
