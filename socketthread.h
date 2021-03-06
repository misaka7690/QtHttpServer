#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QWidget>
#include <msgthread.h>
#include <winsock2.h>
#include <stdlib.h>
#include <QTime>
#include <QDebug>
#include <QThread>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketThread(SOCKET ServSock,QString path);
    void run();
    ~SocketThread();
signals:
    void logmsg(QString msg);
public slots:
private:
    SOCKET ServSock;
    QString folderpath;
};

#endif // SOCKETTHREAD_H
