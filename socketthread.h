#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QWidget>
#include <msgthread.h>
#include <winsock2.h>
#include <stdlib.h>
#include <QTime>
#include <QDebug>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketThread(SOCKET ServSock, QString path ,QWidget *parent = nullptr);
    void run();
    ~SocketThread();
signals:
    void isMsg(QString msg);//新客户端连接了 客户端socket关闭了 客户端发消息来了 都使用此信号
    void isClose();//服务端 关闭信号
public slots:
private:
    SOCKET ServSock;
    QString folderpath;
};

#endif // SOCKETTHREAD_H
