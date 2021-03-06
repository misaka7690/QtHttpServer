#include "QtHttpServer.h"
#include "utils.h"
QtHttpServer &QtHttpServer::instance()
{
    static QtHttpServer obj;
    return obj;
}

// 服务器启动
void QtHttpServer::start()
{
    setui();
    setslot();
}

// UI界面设计
void QtHttpServer::setui()
{
    // 设置主界面
    window = new QWidget;
    window->setWindowTitle("QtHttpServer");
    window->setWindowIcon(QIcon("./icons/favicon.ico"));
    window->resize(1000,1000);
    

    // 设置主布局
    vlayout = new QVBoxLayout;

    // 添加信息展示框
    msg_browser = new QTextBrowser;
    msg_browser->setStyleSheet("border-image:url(./images/3.jpg);font: bold 40px;border: 10px solid;margin-right: 20px;");
    vlayout->addWidget(msg_browser);

    // 添加虚拟地址选择框
    hlayout1 = new QHBoxLayout;
    addrline = new QLineEdit;
    addrbutton = new QPushButton;
    addrline->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
    addrbutton->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;background-color:blue;color: yellow;");
    addrline->setText(ROOT);

    addrbutton->setText("设置虚拟地址");
    hlayout1->addWidget(addrline);
    hlayout1->addWidget(addrbutton);
    vlayout->addLayout(hlayout1);
    // 添加监听地址框和监听端口号
    hlayout2 = new QHBoxLayout;
    ipline = new QLineEdit;
    ipline->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
    ipline->setText("127.0.0.1");
    portline = new QLineEdit;
    portline->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
    portline->setText("80");
    iplabel = new QLabel("IP地址:");
    iplabel->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;background-color: pink;color: black");
    portlabel = new QLabel("端口:");
    portlabel->setStyleSheet("border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;background-color: pink;color: black");

    hlayout2->addWidget(iplabel);
    hlayout2->addWidget(ipline);
    hlayout2->addSpacing(10);
    hlayout2->addWidget(portlabel);
    hlayout2->addWidget(portline);
    vlayout->addLayout(hlayout2);

    // 添加启动按钮
    hlayout3 = new QHBoxLayout;
    button = new QPushButton;
    button->setText("启动服务器");
    button->setStyleSheet("background-color: green;border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
    hlayout3->addWidget(button);
    hlayout3->setAlignment(Qt::AlignJustify);
    vlayout->addLayout(hlayout3);
    window->setLayout(vlayout);
    window->show();

    // button->setMaximumSize(button->size());
    // ipline->setMaximumSize(ipline->size());
    // iplabel->setMaximumSize(iplabel->size());
    // portline->setMaximumSize(portline->size());
    // portlabel->setMaximumSize(portlabel->size());
    // addrline->setMaximumSize(addrline->size());
    // addrbutton->setMaximumSize(addrbutton->size());

    msg_browser->setText(msg);
}

// 消息接口设置
void QtHttpServer::setslot()
{
    connect(button, SIGNAL(clicked()), this, SLOT(button_clicked()));
    connect(addrbutton, SIGNAL(clicked()), this, SLOT(addrbutton_clicked()));
}

// 当点击服务器启动按钮
void QtHttpServer::button_clicked()
{
    if (server_running == false)
    {
        button->setStyleSheet("background-color: red;border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
        sendmsg("服务器启动中...");
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        // 获取IP地址

        if (!IP_valid(ipline->text()))
            return;

        QByteArray ba = ipline->text().toLatin1();
        char *IP = ba.data();
        sendmsg(QString("你设置的IP地址为：") + ipline->text());
        sendmsg("IP地址有效！");
        // 获取端口号
        if (!port_valid(portline->text()))
        {
            return;
        }
        int p = portline->text().toInt();
        if (p > 65535 || p <= 0)
        {
            QMessageBox::information(nullptr, "Title", "端口号错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            return;
        }
        unsigned short port = (unsigned short)p;
        sendmsg(QString("你设置的监听端口为：") + portline->text());
        sendmsg("监听端口有效！");
        QByteArray pa = addrline->text().toUtf8();
        const char *filefolder = pa.data();
        sendmsg("你设置的虚拟地址为：" + addrline->text());
        sendmsg("虚拟地址有效！");
        // 创建socket
        serversocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serversocket == INVALID_SOCKET)
        {
            sendmsg("服务器套接字创建失败！");
        }

        // socket绑定ip和端口

        sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.S_un.S_addr = inet_addr(IP);
        if (bind(serversocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
        {
            sendmsg("Socket绑定ip和端口失败！");
            return;
        }

        if (listen(serversocket, 10) == SOCKET_ERROR)
        {
            sendmsg("Socket绑定ip和端口失败！");
            return;
        }

        sendmsg("Socket创建成功");
        server_running = true;
        button->setText("关闭服务器");
        SocketThread = new class SocketThread(serversocket,filefolder);
        // 信号槽连接
        connect(SocketThread,&SocketThread::logmsg,this,sendmsg);
        SocketThread->start();
        sendmsg("服务器启动成功！！！");
    }
    else
    {
        button->setStyleSheet("background-color: green;border-style: outset;border-radius: 10px;border-color: beige;font: bold;padding: 6px;");
        sendmsg("服务器关闭中...");
        // 关闭子线程
        if(SocketThread != nullptr){
            SocketThread->requestInterruption();
            SocketThread->terminate();
            sendmsg("释放 SocketThread");
            SocketThread->quit();
            SocketThread->wait();
            delete SocketThread;
        }
        
        // 停止监听
        closesocket(serversocket);

        server_running = false;
        sendmsg("服务器关闭成功!");
        button->setText("启动服务器");
    }
}

// 当点击配置虚拟地址按钮
void QtHttpServer::addrbutton_clicked()
{
    QString dir = SelectServerRoot();
    ROOT = dir;
    addrline->setText(ROOT);
}

// 服务器启动
void QtHttpServer::run(const QHostAddress &address, const quint16 &port)
{
    if (server_running == false)
    {
        sendmsg("服务器开始启动...");
        // m_httpServer->listen(address, port);
        sendmsg("服务器启动成功!");
        server_running = true;
        button->setText("关闭服务器");
    }
    else
    {
        sendmsg("服务器关闭中...");
        server_running = false;
        sendmsg("服务器关闭成功!");
        button->setText("启动服务器");
    }
}

// 更新配置信息
void QtHttpServer::sendmsg(QString m)
{
    msg = msg + "\n" + m;
    msg_browser->setText(msg);
}


// void QtHttpServer::sendtext(QTcpSocket *&socket, QString &filepath)
// {
//     QString tmpdata = ReadTextData(ROOT, filepath);
//     QByteArray response = tmpdata.toUtf8();

//     QString http = "HTTP/1.1 200 OK\r\n";
//     http += "Server: nginx\r\n";
//     http += "Content-Type: text/html;charset=utf-8\r\n";
//     http += "Connection: keep-alive\r\n";
//     http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(response.size()));

//     socket->write(http.toUtf8());
//     socket->write(response);
//     socket->flush();
//     socket->waitForBytesWritten(http.size() + response.size());
//     socket->close();
// }

// void QtHttpServer::sendimage(QTcpSocket *&socket, QString &filepath)
// {
//     QByteArray tmpdata = ReadImageData(ROOT, filepath);

//     QString http = "HTTP/1.1 200 OK\r\n";
//     http += "Server: nginx\r\n";
//     http += "Content-Type: image/jpg\r\n";
//     http += "Connection: keep-alive\r\n";
//     http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(tmpdata.size()));

//     socket->write(http.toUtf8());
//     socket->write(tmpdata);
//     socket->flush();
//     socket->waitForBytesWritten(http.size() + tmpdata.size());
//     socket->close();
// }

QtHttpServer::QtHttpServer(QObject *parent) : QObject(parent)
{
    // m_httpServer = new QTcpServer(this);
    // m_httpServer->setMaxPendingConnections(1024); //设置最大允许连接数
    // QObject::connect(m_httpServer, &QTcpServer::newConnection, this, &QtHttpServer::newConnection);
}

QtHttpServer::~QtHttpServer()
{
    WSACleanup();
}
