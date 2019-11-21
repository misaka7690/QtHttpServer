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

#endif // UTILS_H