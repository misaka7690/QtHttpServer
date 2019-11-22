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

    
        memset(resp,0,1024);
        char recv_buf[1024] = {0};
        //阻塞等待 接受信息
        int ret = recv(ClientSock , recv_buf, 1024,0);
        QString recv_data = QString(QLatin1String(recv_buf));

        QTime time = QTime::currentTime();
        QString str = time.toString("hh:mm:ss");
        if(ret == 0){//连接断开
            emit logmsg(QString("%1 [%2:%3] is closed!!!").arg(str).arg(clientIp).arg(clientPort));
            return;
        }


        QString filepath = GetUrlRequestFilePath(recv_data);
        QString msg = QString("来自") + QString(clientIp) + QString("(端口号为") + QString::number(clientPort) + QString(")的客户端请求") + filepath;
        emit logmsg(msg);
        int filetype = judgefiletype(filepath);


        

        // 请求网站根目录  
        if(filetype==0)
        {
            response_html(ClientSock,"/index.html",webroot);
        }
        
        // 请求文本文件
        if(filetype==1)
        {
            response_html(ClientSock,filepath,webroot);
        }
        
        if(filetype==2)
        {
            response_img(ClientSock,filepath,webroot);
        }

        // emit logmsg(QString("$ 给客户端发送: %1").arg(filename));
}
