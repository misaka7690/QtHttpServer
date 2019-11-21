#ifndef MSGTHREAD_H
#define MSGTHREAD_H

#include <QThread>
#include <QWidget>
#include <winsock2.h>
#include <QTime>
#include <QDebug>
#include <string.h>

class MsgThread : public QThread
{
    Q_OBJECT
public:
    explicit MsgThread(SOCKET ClientSock,sockaddr_in mAddr,QString path);
    ~MsgThread();
    void run();
signals:
    void logmsg(QString msg);//客户端socket关闭了 客户端发消息来了
public slots:
private:
    //和客户端通信的 socket
    SOCKET ClientSock;
    //客户端地址
    sockaddr_in mAddr;
    // 服务器虚拟地址
    QString webroot;
    char resp[1024] = {0};
};

#endif // MSGTHREAD_H
