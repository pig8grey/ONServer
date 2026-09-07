#ifndef MYCLIENT_H
#define MYCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QList>
#include <QDebug>
#include <QMutex>
#include <QTimeZone>
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
#include "dispTask.h"
#include "writeEvent.h"


class myclient : public QObject
{
    Q_OBJECT
public:
    explicit myclient(qintptr socketDescriptor,QObject * parent = 0);
    ~myclient();
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
    float getTemp();
    bool compare(float &value1, float value2);
    int DetEvt();
    QTimer *timer = new QTimer(this);
    void updateEvent(bool isEvent);



    QString desiredBox,network;
    int headlen=24;
    int eventCount=0;
    int reStation=0;
    int battSeconds=0;
    int begin,end;
    int eventIndicator=0;
    QString baseFolder,fileName,ID,target,station,disFolder;


    bool isSeed,notWarned;
    bool isInserted=false;
    QByteArray graphHeader=QByteArray::fromHex("fffefdfc");
    QByteArray header=QByteArray::fromHex("88ff88ff");

    float latitude,longitude,myVolt,maxVel;
    float mytemp=-247.0;
    QDateTime mytime= QDateTime::currentDateTimeUtc();
    QList<QString> myBox;
    QMutex mutex;
    QTimeZone newZone=QTimeZone("America/Vancouver");

    std::vector <std::vector<double>> prev{3,std::vector<double>(17,0.0)};
    QDateTime et,prevtime;
    bool writeDis=false; int DisCount=0;
    bool newEvent=true;
    bool isEvent=false;
    std::vector<float> maxDis{std::vector<float>(6, 0.0)};
    std::vector<float> eventMax{std::vector<float>(6, 0.0)};
    float currentMaxDis,currentMinDis;

signals:
    void myaddr (QList<QString> address);
    void disconnect (QList<QString> address);
    void incomming (QList<QString> address);
    void errormessage (QString message);
    void delsig(QTcpSocket* s);
    void theBox (unsigned short i);
    void toGraph(QByteArray b);
    void eventInfo(int number);


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
       void updatePrev(std::vector <std::vector<double>> myVal);
       void updateMaxDis(std::vector<float> currentDis);
       void eventUpdate(int numbner);
};

#endif // MYCLIENT_H
