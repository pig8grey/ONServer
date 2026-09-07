#ifndef DISPTASK_H
#define DISPTASK_H


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
#include <QDebug>
#include <QThread>
#include <QMutex>
class dispTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    dispTask(std::vector<float> datain,std::vector<std::vector<double>> previous,QString folder,QString ID,
             QDateTime starttime, QString filein, QString nw, bool isDis
             );
    std::vector<float> data;

signals:
    // notify to the main thread when we're done
    void Result(int number);
    void previousNumber(std::vector<std::vector<double>> previous);
    void myMaxDis(std::vector<float> myDis);

protected:
    void run();
    QString IDfolder;
    QString baseFolder;
    QString fileName, network;
    QDateTime timestart;
    QMutex mutex;
    std::vector<std::vector<double>> prev;
    std::vector<float> datain;
    std::vector<float> maxDis{std::vector<float>(3, 0.0)};
    std::vector<float> minDis{std::vector<float>(3, 0.0)};

    bool writeDis;
};

#endif // DISPTASK_H
