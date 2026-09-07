#include "healthseed.h"

healthSeed::healthSeed(float datain,QString folder,QString ID,
                       QDateTime starttime, QString filein, QString nw, char Type)
{

    data=datain;
    IDfolder=ID;
    fileName=filein;
    baseFolder=folder;
    timestart=starttime;
    network=nw;

    if (Type=='V') mytype="Battery";
    if (Type=='T') mytype="Temperature";
}

//healthSeed::healthSeed(int datain,QString folder,QString ID,
//                       QDateTime starttime, QString filein, QString nw, char Type)
//{

//    data=data_int;
//    IDfolder=ID;
//    fileName=filein;
//    baseFolder=folder;
//    timestart=starttime;
//    network=nw;
//    if (Type=='V') mytype="Battery";
//    if (Type=='T') mytype="Temperature";
//}



void healthSeed::run()
{

    std::string start=timestart.toString("yyyy-MM-ddThh:mm:ss").toStdString();
    writeseed(data,start,baseFolder,IDfolder,fileName,network,mytype);
//    if (data_int<-273) writeseed(data,start,baseFolder,IDfolder,fileName,network,mytype);
//    else writeseed(data_int,start,baseFolder,IDfolder,fileName,network);
    emit Result(1);
    return;


}











