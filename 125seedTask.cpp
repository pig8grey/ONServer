
#include "125seedTask.h"
#include <QDebug>


seedTask::seedTask(std::vector<float> datain,QString folder,
                   QString ID,QDateTime starttime, QString filein, QString nw)
{
    data=datain;
    IDfolder=ID;
    fileName=filein;
    baseFolder=folder;
    timestart=starttime;
    network=nw;
}



void seedTask::run()
{

    std::vector<std::vector<int>> mydata;

//    qDebug()<<data.size();

    std::vector< int > ch1;
    std::vector< int > ch2;
    std::vector< int > ch3;
    float maxVal;
    int dataSize=data.size()/3;

    mydata.resize(3,std::vector<int>(dataSize));

    if (IDfolder=="BWE" || IDfolder=="TWE"){

        for (auto i=0;i<data.size();i++)
        {
            switch (i%3){
                case 0:
                ch1.push_back((int)data[i]*-1.0);
                break;
                case 1:
                ch2.push_back((int)data[i]*-1.0);
                break;
                case 2:
                ch3.push_back((int)data[i]*-1.0);
                break;
            }
        }
    }
    else if (IDfolder=="BEA"){
        for (auto i=0;i<data.size();i++)
        {
    //        mydata[i%3][i]=data[i];
            switch (i%3){
                case 0:
                ch1.push_back((int)data[i]*-1.0*0.8);
                break;
                case 1:
                ch2.push_back((int)data[i]*0.8);
                break;
                case 2:
                ch3.push_back((int)data[i]*0.8);
                break;
            }
        }
    }
    else{
        for (auto i=0;i<data.size();i++)
        {
    //        mydata[i%3][i]=data[i];
            switch (i%3){
                case 0:
                ch1.push_back((int)data[i]*-1.0);
                break;
                case 1:
                ch2.push_back((int)data[i]);
                break;
                case 2:
                ch3.push_back((int)data[i]);
                break;
            }
        }
    }
    mydata[0].assign(ch1.begin(), ch1.end());
    mydata[1].assign(ch2.begin(), ch2.end());
    mydata[2].assign(ch3.begin(), ch3.end());

    std::string start=timestart.toString("yyyy-MM-ddThh:mm:ss").toStdString();
//    qDebug()<<"Time After: "<<QString::fromStdString(start);
//    qDebug()<<"Base Folder is"<<baseFolder;
//    qDebug()<<"Data Size: "<<data.size();

    writeseed(mydata,start,baseFolder,IDfolder,fileName,network);


    emit Result(1);

    return;


}



float seedTask::getMaxAcc(float din[]){
    float result=0;
    for(int i=0; i<sizeof(din)/sizeof(din[0]);i++){
        if (std::abs(din[i])>result) {result=din[i];}
    }
    return result;
}





