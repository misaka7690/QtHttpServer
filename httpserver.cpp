#include "httpserver.h"


HttpServer &HttpServer::instance()
{
    static HttpServer obj;
    return obj;
}

void HttpServer::run(const QHostAddress &address, const quint16 &port)
{
    printf("server starting");
    m_httpServer->listen(address,port);
}

void HttpServer::newConnection()
{
    qDebug() << "newConnection";
    QTcpSocket *m_socket = m_httpServer->nextPendingConnection();
    QObject::connect(m_socket,&QTcpSocket::readyRead,this,&HttpServer::readyRead);
}

void HttpServer::readyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if(socket){
        QByteArray request = socket->readAll();

//        qDebug() << "Request Data:" << request;
//        QString temp;
        // qDebug()<<socket->peerPort();
        // qDebug()<<socket->peerAddress();

        static int count = 0;
        count++;
        QByteArray response = QString("<h1><center>Hello World %1</center></h1>\r\n").arg(count).toUtf8();


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
}

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    m_httpServer = new QTcpServer(this);
    m_httpServer->setMaxPendingConnections(1024);//设置最大允许连接数
    QObject::connect(m_httpServer,&QTcpServer::newConnection,this,&HttpServer::newConnection);
}

HttpServer::~HttpServer()
{

}
