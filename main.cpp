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
    
    QtHttpServer::instance().start();
    
    return a.exec();
}