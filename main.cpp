#include <QString>
#include <iostream>
// #include <utils.h>
// #include <httpserver.h>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include "QtHttpServer.h"
using namespace std;

int main(int argc, char *argv[])
{

    std::cout << "hello world" << '\n';
    QApplication a(argc, argv);
    
    // QString data=readdata("C:/vscode_workspace/space1/finaltest-qttcp/web/","index.html");
    // qDebug() <<data;
    // printf("%s\n",data);
    // cout<<"hello world"<<std::endl;
    // QHostAddress addr=InputHostAddress();
    // quint16 port=InputPort();
    // QString Root=SelectServerRoot();
    // std::cout << addr.toString() << std::endl;
    // bool isOK;
    // QString text = QInputDialog::getText(NULL, "Input Dialog",
    //                                      "Please input your comment",
    //                                      QLineEdit::Normal,
    //                                      "your comment",
    //                                      &isOK);
    

    // if (isOK)
    // {
    //     QMessageBox::information(NULL, "Information",
    //                              "Your comment is: <b>" + text + "</b>",
    //                              QMessageBox::Yes | QMessageBox::No,
    //                              QMessageBox::Yes);
    // }

    // QString title = "消息框";
    // QString info="啦啦啦";
    // QMessageBox::information(NULL, title, info,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    // HttpServer::instance().run();
    QtHttpServer::instance().start();
    // while(true)
    // {
    //     obj.newConnection();
    // }
    // QTcpServer ts;
    // ts.listen(QHostAddress::Any, 80);
    // while (true)
    // {
    //     // bool ok = ts.isListening();
    //     // if (ok)
    //     // {
    //     //     printf("is listening\n");
    //     // }
    //     bool ok = ts.hasPendingConnections();
    //     if (ok)
    //     {
    //         QTcpSocket* socket=ts.nextPendingConnection();
    //         Qobject::connect(socket,&QTcpSocket::connected,this,&QTcpServer::readready)
    //     }
    // }

    // return 0;
    return a.exec();
}