#include "dispTask.h"


dispTask::dispTask(std::vector<float> datain,std::vector<std::vector<double>> previous, QString folder,
                   QString ID,QDateTime starttime, QString filein, QString nw, bool isDis
                   )
{

    data=datain;
    prev=previous;
    IDfolder=ID;
    fileName=filein;
    baseFolder=folder;
    timestart=starttime;
    network=nw;
    writeDis=isDis;



}



void dispTask::run()
{

    std::vector<std::vector<float>> mydata;
    std::vector< float > ch1;
    std::vector< float > ch2;
    std::vector< float > ch3;
    int dataSize=data.size()/3;

    mydata.resize(3,std::vector<float>(dataSize));
    if (IDfolder=="BWE"){

        for (auto i=0;i<data.size();i++)
        {
    //        mydata[i%3][i]=data[i];
            switch (i%3){
                case 0:
                ch1.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
                break;
                case 1:
                ch2.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
                break;
                case 2:
                ch3.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
                break;
            }
        }
    }
    else if (IDfolder=="TWE"){
        for (auto i=0;i<data.size();i++)
        {

            switch (i%3){
                case 0:
                ch1.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
                break;
                case 1:
                //ch2.push_back(0.0);
                ch2.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
                break;
                case 2:
                //ch3.push_back(0.0);
                ch3.push_back(std::isnormal(data[i]) ? data[i]*-1.0 : 0.0);
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
                ch1.push_back(data[i]*-1.0*0.8);
                break;
                case 1:
                ch2.push_back(data[i]*0.8);
                break;
                case 2:
                ch3.push_back(data[i]*0.8);
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
                ch1.push_back(data[i]*-1.0);
                break;
                case 1:
                ch2.push_back(data[i]);
                break;
                case 2:
                ch3.push_back(data[i]);
                break;
            }
        }
    }

    mydata[0].assign(ch1.begin(), ch1.end());
    mydata[1].assign(ch2.begin(), ch2.end());
    mydata[2].assign(ch3.begin(), ch3.end());


    std::string start=timestart.toString("yyyy-MM-ddThh:mm:ss").toStdString();


//    applyDisplacement(mydata,true,prev);
    applyMaxDisplacement(mydata,true,prev,maxDis,minDis);
//    qDebug()<<QString::number(mydata[0][0]);
    std::vector<float> maxMin=maxDis;
    maxMin=maxDis;
    maxMin.insert( maxMin.end(), minDis.begin(), minDis.end() );
    emit previousNumber(prev);





    if (writeDis) {
        fileName="DIS_"+fileName;
        emit myMaxDis (maxMin);
        writedis(mydata,start,baseFolder,IDfolder,fileName,network);

    }
    emit Result(1);

    return;


}
