
#ifndef QTHTTPSERVER_H
#define QTHTTPSERVER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QtWidgets/QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QIcon> 
#include <QColor> 
#include <winsock2.h>
#include "socketthread.h"
#include <QFrame>
class QtHttpServer : public QObject
{
    Q_OBJECT
public:
    static QtHttpServer &instance();
    void setui();
    void setslot();
    void start();
    void run(const QHostAddress &address= QHostAddress::Any, const quint16 &port= 80);

    void sendmsg(QString m);
signals:

public slots:
private slots:
    void button_clicked();
    void addrbutton_clicked();
private:
    explicit QtHttpServer(QObject *parent = nullptr);
    ~QtHttpServer();
    // void sendimage(QTcpSocket* &socket,QString &filepath);
    // void sendtext(QTcpSocket* &socket,QString &filepath);
    Q_DISABLE_COPY(QtHttpServer)
private:
// GUI 部件
    QWidget *window;
    QVBoxLayout *vlayout;
    QPushButton *button;
    QLabel *server_state;
    // 信息展示框
    QTextBrowser *msg_browser;
    // 虚拟地址栏
    QHBoxLayout *hlayout1;
    QLineEdit *addrline;
    QPushButton *addrbutton;
    // 监听地址和监听端口
    QHBoxLayout *hlayout2;

    QLabel *iplabel;
    QLabel *portlabel;

    QLineEdit *ipline;
    QLineEdit *portline;
    
    QHBoxLayout *hlayout3;
    
// 服务器部件
    QTcpServer *m_httpServer;
    QString ROOT="C:/vscode_workspace/space1/QtHttpServer/web";
    SocketThread *SocketThread;


// 服务器信息
    QString msg = "这是一个利用Qt制作的简单的HTTP服务器";
    bool server_running = false;

// windows socket 信息
    WSADATA wsadata;
    SOCKET serversocket = INVALID_SOCKET;
};

#endif // QTHTTPSERVER_H
