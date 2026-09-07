
#include "mytask.h"
#include <QDebug>


MyTask::MyTask(QByteArray datain,QString folder,QString ID,QString filein)
{
    baseFolder=folder;
    bytes=datain;
    IDfolder=ID;
    fileName=filein;
}



void MyTask::run()
{
    // time consumer

//    qDebug() << "Task start";
//    qDebug()<<IDfolder;
    QString path=baseFolder+'/'+IDfolder +"/";
    QFile file(path+fileName);

//    qDebug()<<path+fileName;
//    qDebug()<<bytes;
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }

    if (!file.open(QFile::WriteOnly | QIODevice::Append)){
        qDebug()<<"Write Failed";
        emit Result(0);
    }
    else{

    file.write(bytes);

//    qDebug() << "Task done";
    emit Result(1);
    }
}
