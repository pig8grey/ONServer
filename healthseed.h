#ifndef HEALTHSEED_H
#define HEALTHSEED_H

#include <QRunnable>
#include <QObject>
#include <QRunnable>
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



class healthSeed: public QObject, public QRunnable
{
    Q_OBJECT

public:
    healthSeed(float datain,QString folder,QString ID,
               QDateTime starttime, QString filein, QString nw, char Type);
//    healthSeed(int datain,QString folder,QString ID,
//               QDateTime starttime, QString filein, QString nw, char Type);
    float data;
    int data_int=-247;
    QString mytype;

signals:
    // notify to the main thread when we're done
    void Result(int number);

protected:
    void run();
    QString IDfolder;
    QString baseFolder;
    QString fileName, network;
    QDateTime timestart;
};

#endif // HEALTHSEED_H
