#ifndef UTILS_H
#define UTILS_H

#include <QInputDialog>
#include <QtNetwork>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include<iostream>
#include<QDataStream>
#include<QRegExp>
#include<winsock2.h>
using namespace std;

// 判断IP地址是否合法


bool IP_valid(const QString& IP);

// 判断端口号是否合法

bool port_valid(const QString& src);

QHostAddress InputHostAddress();

quint16 InputPort();

QString SelectServerRoot();

QString ReadTextData(QString root,QString filename);


QByteArray ReadImageData(QString root,QString filename);

QString GetUrlRequestFilePath(QString request);

int judgefiletype(QString filepath);

// 响应 msg
int response_msg(SOCKET ClientSock,QString msg);

// 响应文本文件
int response_html(SOCKET ClientSock,QString filepath,QString webroot);

// 响应图片文件
int response_img(SOCKET ClientSock,QString filepath,QString webroot);
#endif // UTILS_H