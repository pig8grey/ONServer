#ifndef simclient_H
#define simclient_H
#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QList>
#include <QDebug>
#include <QByteArray>
#include <QThreadPool>
#include <QVector>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QIODevice>
#include <QDataStream>
#include <QHostAddress>
#include <QTimer>
#include <vector>
#include <stdlib.h>
#include "mytask.h"
#include "125seedTask.h"
#include "healthseed.h"
#include "writeEvent.h"
#include <QObject>

class simclient: public QObject
{
    Q_OBJECT
public:
    explicit simclient(qintptr socketDescriptor,QObject * parent = 0);
    ~simclient();
    void setupSocket(qintptr socketDescriptor);
    void setupFolder(QString Folder,bool Seed,QString nw);
    QHostAddress getHost();
    quint16 getPort();
    QList<QString> addr,dashInfo;

private:
    QTcpSocket *socket;
    QByteArray data_recv, mybytes,data_buffer,resetCommand;
    QByteArray bytesToSend,values;
    int tgt=-1;
    std::vector<float> processBuffer(QByteArray &bytes);
    float getgps(QByteArray arrin);
    void getdt(QByteArray data);
    void setData();
    void errorResponse(QString msg);
    void warningResponse(QString msg);
    void procBytes();
    void setGraph();
    void setBoxBytes();
    float getVoltage();
    bool compare(float &value1, float value2);
    QTimer *timer = new QTimer(this);
    void updateEvent(bool isEvent);



    QString desiredBox,network;

    int reStation=0;
    int battSeconds=5;
    int begin,end;
    QString baseFolder,fileName,ID,target,station;
    std::vector<float> nums;
    bool isSeed,notWarned;
    bool isInserted=false;
    QByteArray graphHeader=QByteArray::fromHex("fffefdfc");
    QByteArray header=QByteArray::fromHex("88ff88ff");
    float latitude,longitude,myVolt,maxVel;
    QDateTime mytime= QDateTime::currentDateTime();
    QList<QString> myBox;

signals:
    void myaddr (QList<QString> address);
    void disconnect (QList<QString> address);
    void incomming (QList<QString> address);
    void errormessage (QString message);
    void delsig(QTcpSocket* s);
    void theBox (unsigned short i);
    void toGraph(QByteArray b);


public slots:
       void readyRead();
       void disconnected();
       void connected();
       void TaskResult(int number);
       void timeOut();
       void procCmd(QList<QString> cmd);
       void setBox(QList<QString> b);
       void setTgt(unsigned short i);
       void setGraphByte(QByteArray b);


};

#endif // simclient_H
