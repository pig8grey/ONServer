#ifndef seedTask_H
#define seedTask_H

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
#include <cstdlib>
#include <climits>
#include <QMutex>


class seedTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    seedTask(std::vector<float> datain,QString folder,QString ID,
             QDateTime starttime, QString filein, QString nw);
    std::vector<float> data;
    float getMaxAcc(float din[]);
signals:
    // notify to the main thread when we're done
    void Result(int number);


protected:
    void run();
    QString IDfolder;
    QString baseFolder;
    QString fileName, network;
    QDateTime timestart;
    QMutex mutex;

};

#endif // seedTask_H
