#ifndef WRITEEVENT_H
#define WRITEEVENT_H

#include <QRunnable>
#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QDateTime>
#include <QDir>
#include <vector>
#include <string>
#include "125liveseed.h"
#include <numeric>      // std::adjacent_difference
#include <math.h>       /* pow */
#include <cstdlib>
#include <climits>
#include <QTextStream>
#include <QThread>
#include <QMutex>
#include <QTimeZone>

class writeEvent : public QObject, public QRunnable
{
    Q_OBJECT
public:
    writeEvent(std::vector<float> datain,QString folder,QString ID,
             QDateTime starttime, QString filein, QString nw);
    std::vector<float> data;

signals:
    // notify to the main thread when we're done
    void Result(int number);
    void sigEvent(bool isEvent);

protected:
    void run();
    QString myID;
    QString baseFolder;
    QString fileName, network;
    QDateTime timestart;
    QMutex mutex;


};






#endif // WRITEEVENT_H
