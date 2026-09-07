#include "writeEvent.h"
#include <QDebug>



writeEvent::writeEvent(std::vector<float> datain,QString folder,QString ID,
                   QDateTime starttime, QString filein, QString nw)
{
    data=datain;
    fileName=filein;
    baseFolder=folder;
    timestart=starttime;
    network=nw;
    myID=ID;
}



void writeEvent::run()
{
    QString path=baseFolder+'/'+network+"_event";
    QDir dir;
    QByteArray line;
    mutex.lock();
    QTimeZone newZone("America/Winnipeg");

    if(!dir.exists(path))  dir.mkpath(path);
    timestart=timestart.toTimeZone(newZone);

    QString myfilePath=path+'/'+network+"_"+
            timestart.toString("yyyy-MM-dd")+"_logs.csv";

    QTime lt=timestart.time();
    QDateTime endT=QDateTime::currentDateTimeUtc().toTimeZone(newZone);
    QFile myfile(myfilePath);
    bool isFirst=!myfile.exists();

//    float Max=*std::max_element ( std::begin(data),std::end(data) );
//    float Min=*std::min_element ( std::begin(data),std::end(data) );

    if(myfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)){
        QStringList wordList;
        while (!myfile.atEnd()) {
            line = myfile.readLine();
        }

        if (!line.isEmpty()){
            wordList.append(line.split(',').first());
            if (lt.secsTo(QTime::fromString(wordList[1],"HH:mm"))<300) lt=QTime::fromString(wordList[1],"HH:mm");

        }


            QTextStream logstream(&myfile);


                  if (isFirst) logstream<<"Start Date (CST)"<<','<<"Start Time (CST)"
                                          <<','<<"Box ID"<<','<<"Z(+ve)"<<','<<
                                          "Longitudinal(+ve)"<<','<<"Lateral(+ve)"<<','
                                         <<"Z(-ve)"<<','
                                        <<"Longitudinal(-ve)"<<','<<"Lateral(-ve)"<<','
                                       <<"End Time (CST)"<<'\n';
                  logstream << timestart.toString("yyyy/MM/dd")<<','
                            <<lt.toString("hh:mm") << ',' << myID <<','<<
                  QString::number(data[0],'f',4)<<','<<
                  QString::number(data[2],'f',4)<<','<<
                  QString::number(data[1],'f',4)<<','<<
                  QString::number(data[3],'f',4)<<','<<
                  QString::number(data[5],'f',4)<<','<<
                  QString::number(data[4],'f',4)<<','<<
                  endT.toString("yyyy/MM/ddThh:mm")
                                               <<'\n';
                  myfile.close();
        }
    mutex.unlock();
    emit Result(1);
    return;

//    if (Max>0.3 || Min<-0.3 ||myID=="P1A"){
//        if (Max>0.1 || Min <-0.1){
//        QString myalertpath=path+'/'+network+"_"+
//                "alerts.txt";
//        QFile myalert(myalertpath);
//        if(myalert.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
//            QTextStream logstream(&myalert);

//                      if (isFirst) logstream<<"Start Date (UTC)"<<','<<"Start Time (UTC)"
//                                              <<','<<"Box ID"<<','<<"Z(+ve)"<<','<<
//                                              "Longitudinal(+ve)"<<','<<"Lateral(+ve)"<<','
//                                             <<"Z(-ve)"<<','
//                                            <<"Longitudinal(-ve)"<<','<<"Lateral(-ve)"<< '\n';
//                      logstream << timestart.toString("yyyy/MM/dd")<<','
//                                <<timestart.toString("hh:mm") << ',' << myID <<','<<
//                      QString::number(data[0],'f',4)<<','<<
//                      QString::number(data[2],'f',4)<<','<<
//                      QString::number(data[1],'f',4)<<','<<
//                      QString::number(data[3],'f',4)<<','<<
//                      QString::number(data[5],'f',4)<<','<<
//                      QString::number(data[4],'f',4)<<'\n';
//                      myalert.close();
//        }
//    }

//}





}











