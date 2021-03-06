#include "utils.h"

// 判断IP地址是否合法
bool IP_valid(const QString& IP)
{
    QRegExp regExpIP("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");

    if(!regExpIP.exactMatch(IP))
    {
        QMessageBox::information(nullptr, "Title", "IP错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return 0;
    }
    return 1;
}

// 判断端口号是否合法

bool port_valid(const QString& src)
{
    QRegExp reg("\\d*");
    if(!reg.exactMatch(src))
    {
        QMessageBox::information(nullptr, "Title", "端口号错误", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return 0;
    }
    return 1;
}

QHostAddress InputHostAddress()
{
    bool valid;
    QString title = "监听地址";
    QString description = "请设置你所要监听的地址,其中：\n";
    description += "0.0.0.0 表示监听所有地址 \n";
    description += "127.0.0.1 表示监听本地地址\n";
    description += "其他具体地址即监听目标地址\n";
    description += "默认监听所有地址\n";
    QString text = QInputDialog::getText(NULL, title, description, QLineEdit::Normal, "0.0.0.0", &valid);
    if (valid)
    {
        return QHostAddress(text);
    }
}

quint16 InputPort()
{
    bool valid;
    QString title = "监听端口";
    QString description = "请设置你所要监听的端口）,其中：\n";
    description += "默认端口为80端口\n";
    QString text = QInputDialog::getText(NULL, title, description, QLineEdit::Normal, "请输入1-65535之间的整数", &valid);
    if (valid)
    {
        int port=text.toInt();
        if(port>=1 && port<=65535)
        {
            return port;
        }
        else
        {
            QMessageBox::warning(NULL,"警告","你输入的端口无效，请重新输入");
            return InputPort();
        }
    }
}

QString SelectServerRoot()
{
    QString path = QFileDialog::getExistingDirectory(NULL, "选择服务器虚拟路径",".");
    if (path.length() == 0)
    {
        QMessageBox::information(NULL, "提醒", "你没有选择任何路径");
        return SelectServerRoot();
    }
    else
    {
        return path;
    }
    
}

QString ReadTextData(QString root,QString filename)
{
    QString filepath = root + filename;
    QFile file(filepath);
   

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return NULL;
    }
    
    return file.readAll();

}


QByteArray ReadImageData(QString root,QString filename)
{
    QFile file(root+filename);
    

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << root+filename;
        return "ERROR:请求的文件不存在或无法打开";
    }
    
    return file.readAll();
}

QString GetUrlRequestFilePath(QString request)
{
    std::string data = request.toStdString();
    // cout<<data<<endl;
    int pos=0;
    while(data[pos]!='/')
    {
        pos++;
    }
    QString filepath;
    while(data[pos]!=' ')
    {
        filepath += data[pos++];
    }
    qDebug() << filepath;
    return filepath;
}

int judgefiletype(QString filepath)
{
    int l=filepath.length();
    // 请求默认网页,index.html
    if(l<=1)
    {
        return 0;
    }
    // 请求文本文件
    if(filepath[l-1]=='l')
    {
        return 1;
    }
    if(filepath[l-1]=='t')
    {
        return 1;
    }
    if(filepath[l-1]=='d')
    {
        return 1;
    }
    // 其他均为请求图片
    return 2;
}

int response_msg(SOCKET ClientSock,QString msg)
{
    char header[1024] = "HTTP/1.1 200 OK\r\n";
    strcat(header, "Content-Type: text/plain;charset=utf-8\r\n");
    strcat(header, "Connection: keep-alive\r\n\r\n");
    // qDebug() << strlen(header);
    send(ClientSock, header, strlen(header), 0);

    string tmp = msg.toStdString();
    const char* ch = tmp.c_str();
    // qDebug() << ch ;
    // qDebug() << strlen(ch);

    send(ClientSock, ch,strlen(ch),0);

    shutdown(ClientSock, SD_SEND);
    
    return 1;
}


int response_html(SOCKET ClientSock,QString filepath,QString webroot)
{
    char header[1024] = "HTTP/1.1 200 OK\r\n";
    strcat(header, "Content-Type: text/html;charset=utf-8\r\n");
    strcat(header, "Connection: keep-alive\r\n\r\n");
    // qDebug() << strlen(header);
    send(ClientSock, header, strlen(header), 0);

    QString data = ReadTextData(webroot,filepath);
    if(data.length()==0)
    {
        char* ch = "404 NOT Found";
        send(ClientSock, ch,strlen(ch),0);
        shutdown(ClientSock, SD_SEND);
        return 0;
    }
    string tmp = data.toStdString();
    const char* ch = tmp.c_str();

    send(ClientSock, ch,strlen(ch),0);

    shutdown(ClientSock, SD_SEND);
    
    return 1;
}


int response_img(SOCKET ClientSock,QString filepath,QString webroot)
{
    QByteArray data = ReadImageData(webroot,filepath);
    qDebug() << data.size();
     if(data.length()==45)
    {
        response_msg(ClientSock,"404 NOF Found");
        return 0;
    }

    char header[1024] = "HTTP/1.1 200 OK\r\n";
    strcat(header, "Content-Type: image/jpeg;charset=utf-8\r\n");
    strcat(header, "Connection: keep-alive\r\n\r\n");
    send(ClientSock, header, strlen(header), 0);

    // data.toUShort
    // string  tmp = data.toStdString();
    // const char* imagebyte = tmp.c_str();
    const char* imagebyte = reinterpret_cast<char*>(data.data());
    // qDebug() << strlen(imagebyte);
    send(ClientSock,imagebyte,data.size(), 0);

    shutdown(ClientSock, SD_SEND);

    return 1;
}