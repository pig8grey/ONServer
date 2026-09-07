#ifndef MYSERVER_H
#define MYSERVER_H
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QThreadPool>
#include <QHostAddress>
#include <QString>
#include <QList>
#include "myclient.h"
class myserver : public QTcpServer
{
    Q_OBJECT

public:
    explicit myserver(QObject *parent = 0);
    ~myserver();
    void start(uint myPort,QString indir,bool Seed,QString nw);
    void disconnect();

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

signals:
    void started(uint myPort);
    void sendaddr(QList<QString> address);
    void disconnect(QList<QString> address);
    void incomming (QList<QString> address);
    void err(QString msg);
    void sendCommand (QList<QString> command);
    void sendBox(QList<QString> b);
    void setDesire(unsigned short i);
    void graphBytes(QByteArray b);
    void sendEventNum(int number);

public slots:
    void cliaddr(QList<QString> address);
    void clidis(QList<QString> address);
    void clierr(QString msg);
//    void delcon(QTcpSocket * sock);
    void getcmd(QList<QString> command);
    void getBoxes(QList<QString> b);
    void desire(unsigned short i);
    void recvGraphBytes(QByteArray b);
    void eventReceive(int number);



private:
    QString directory,network;
    QList<myclient *>connections;
    bool isSeed;
    int isEvent;
};

#endif // MYTCPSERVER_H


