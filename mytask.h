#ifndef MYTASK_H
#define MYTASK_H

#include <QRunnable>
#include <QObject>
#include <QRunnable>
#include <QFile>
#include <QByteArray>
#include <QString>
#include <QIODevice>
#include <QDir>

class MyTask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MyTask(QByteArray datain,QString folder,QString ID,QString filein);
    QByteArray bytes;

signals:
    // notify to the main thread when we're done
    void Result(int number);

protected:
    void run();
    QDir dir;
    QString baseFolder;
    QString IDfolder;
    QString fileName;
};

#endif // MYTASK_H
