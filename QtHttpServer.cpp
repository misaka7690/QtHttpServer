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
    window=new QWidget;
    window->setWindowTitle("QtHttpServer");
    window->setWindowIcon(QIcon("./icons/favicon.ico"));

// 设置主布局
    vlayout = new QVBoxLayout;

// 添加信息展示框
    msg_browser= new QTextBrowser;
    vlayout->addWidget(msg_browser);

// 添加虚拟地址选择框
    hlayout1 = new QHBoxLayout;
    addrline = new QLineEdit;
    addrbutton = new QPushButton;

    addrline->setPlaceholderText(ROOT);
    addrbutton->setText("设置虚拟地址");
    hlayout1->addWidget(addrline);
    hlayout1->addWidget(addrbutton);
    vlayout->addLayout(hlayout1);
// 添加监听地址框和监听端口号
    hlayout2 = new QHBoxLayout;
    ipline = new QLineEdit;
    ipline->setPlaceholderText("127.0.0.1");
    portline = new QLineEdit;
    portline->setPlaceholderText("80");
    iplabel =new QLabel("IP地址:");
    portlabel =new QLabel("端口:");

    hlayout2->addWidget(iplabel);
    hlayout2->addWidget(ipline);
    hlayout2->addSpacing(10);
    hlayout2->addWidget(portlabel);
    hlayout2->addWidget(portline);
    vlayout->addLayout(hlayout2);

// 添加启动按钮
    button = new QPushButton;
    button->setText("启动服务器");
    vlayout->addWidget(button);

    window->setLayout(vlayout);
    window->resize(1000,1000);
    window->show();

    msg_browser->setText(msg);
}


// 消息接口设置
void QtHttpServer::setslot()
{
    connect(button,SIGNAL(clicked()),this,SLOT(button_clicked()));
    connect(addrbutton,SIGNAL(clicked()),this,SLOT(addrbutton_clicked()));
}

// 当点击服务器启动按钮
void QtHttpServer::button_clicked()
{
    run();
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
        m_httpServer->listen(address,port);
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
    msg = msg + "\n" + m ;
    msg_browser->setText(msg);
}

void QtHttpServer::newConnection()
{
    
    QTcpSocket *m_socket = m_httpServer->nextPendingConnection();
    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&QtHttpServer::readyRead);
}

void QtHttpServer::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(socket){
        QByteArray request = socket->readAll();
        // qDebug() << socket->peerAddress();
        qDebug() << socket->peerPort();
        // qDebug() << request;

        QString filepath=GetUrlRequestFilePath(request);

        // qDebug() <<"对方服务器请求"<<filepath;

        QString logmsg=QString("来自")+QString(socket->peerAddress().toString())+QString("(端口号为")+QString::number(socket->peerPort())+QString(")的客户端请求")+filepath;

        sendmsg(logmsg);
        int filetype=judgefiletype(filepath);
        
        if(filetype==0)
        {
            filepath="index.html";
            QtHttpServer::sendtext(socket,filepath);
        }
        if(filetype==1)
        {
            QtHttpServer::sendtext(socket,filepath);
        }
        if(filetype==2)
        {
            QtHttpServer::sendimage(socket,filepath);
        }
    }
}


void QtHttpServer::sendtext(QTcpSocket* &socket,QString &filepath)
{
    QString tmpdata= ReadTextData(ROOT,filepath);
    QByteArray response = tmpdata.toUtf8();

    QString http = "HTTP/1.1 200 OK\r\n";
    http += "Server: nginx\r\n";
    http += "Content-Type: text/html;charset=utf-8\r\n";
    http += "Connection: keep-alive\r\n";
    http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(response.size()));

    socket->write(http.toUtf8());
    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten(http.size() + response.size());
    socket->close();
}


void QtHttpServer::sendimage(QTcpSocket* &socket,QString &filepath)
{
    QByteArray tmpdata= ReadImageData(ROOT,filepath);
    

    QString http = "HTTP/1.1 200 OK\r\n";
    http += "Server: nginx\r\n";
    http += "Content-Type: image/jpg\r\n";
    http += "Connection: keep-alive\r\n";
    http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(tmpdata.size()));

    socket->write(http.toUtf8());
    socket->write(tmpdata);
    socket->flush();
    socket->waitForBytesWritten(http.size() + tmpdata.size());
    socket->close();
}



QtHttpServer::QtHttpServer(QObject *parent) : QObject(parent)
{
    m_httpServer = new QTcpServer(this);
    m_httpServer->setMaxPendingConnections(1024);//设置最大允许连接数
    QObject::connect(m_httpServer,&QTcpServer::newConnection,this,&QtHttpServer::newConnection);
}

QtHttpServer::~QtHttpServer()
{

}
