#include "125liveseed.h"
#include <QDebug>


void writeseed (std::vector<std::vector<float>> data, std::string start,
                QString folder, QString ID,QString fn, QString network) {


    int size;

    float *c_ch1, *c_ch2,*c_ch3;
    std::string sh1,sh2,sh3;

    sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_Z";
    sh2="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_E";
    sh3="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_N";

    size=data[0].size();

    c_ch1=&data[0][0];
    c_ch2=&data[1][0];
    c_ch3=&data[2][0];

    QString path=folder+'/'+ID;
    QString filename=path+'/'+fn;
    QDir dir;

    if(!dir.exists(path))
    {
        dir.mkpath(path);

    }

    seedOut3CHf(start.c_str(),filename.toStdString().c_str(),
               sh1.c_str(),sh2.c_str(),sh3.c_str(),
               c_ch1,c_ch2,c_ch3,size);
    return;



}

void seedOut3CH(const char* start,const char* filename,
                const char* sh1, const char *sh2, const char* sh3,
                int * c_ch1,int * c_ch2,int* c_ch3, int size)
{
    int packedrecords;
    MS3Record *c1,*c2,*c3=NULL;
    MS3TraceList *master;

    master = mstl3_init(NULL);
    c1= msr3_init (NULL);
    c2= msr3_init (NULL);
    c3= msr3_init (NULL);

    fillmsrINT(c1,start,sh1,(double)size);
    fillmsrINT(c2,start,sh2,(double)size);
    fillmsrINT(c3,start,sh3,(double)size);

    filldataINT(c1,c_ch1,size);
    filldataINT(c2,c_ch2,size);
    filldataINT(c3,c_ch3,size);

    mstl3_addmsr(master,c1,0,1,MSF_PACKVER2,NULL);
    mstl3_addmsr(master,c2,0,1,MSF_PACKVER2,NULL);
    mstl3_addmsr(master,c3,0,1,MSF_PACKVER2,NULL);

    packedrecords=mstl3_writemseed(master,filename,0,512,DE_STEIM2,MSF_PACKVER2,0);

    c1->datasamples = NULL;
    c2->datasamples = NULL;
    c3->datasamples = NULL;

    msr3_free(&c1);
    msr3_free(&c2);
    msr3_free(&c3);
    mstl3_free(&master,0);
    return;
}

void seedOut3CHf(const char* start,const char* filename,
                const char* sh1, const char *sh2, const char* sh3,
                float * c_ch1,float * c_ch2,float* c_ch3, int size)
{
    int packedrecords;
    MS3Record *c1,*c2,*c3=NULL;
    MS3TraceList *master;

    master = mstl3_init(NULL);
    c1= msr3_init (NULL);
    c2= msr3_init (NULL);
    c3= msr3_init (NULL);

    fillmsr(c1,start,sh1,(double)size,'f');
    fillmsr(c2,start,sh2,(double)size,'f');
    fillmsr(c3,start,sh3,(double)size,'f');

    filldata(c1,c_ch1,size);
    filldata(c2,c_ch2,size);
    filldata(c3,c_ch3,size);

    mstl3_addmsr(master,c1,0,1,MSF_PACKVER2,NULL);
    mstl3_addmsr(master,c2,0,1,MSF_PACKVER2,NULL);
    mstl3_addmsr(master,c3,0,1,MSF_PACKVER2,NULL);

    packedrecords=mstl3_writemseed(master,filename,0,512,'f',MSF_PACKVER2,0);

    c1->datasamples = NULL;
    c2->datasamples = NULL;
    c3->datasamples = NULL;

    msr3_free(&c1);
    msr3_free(&c2);
    msr3_free(&c3);
    mstl3_free(&master,0);
    return;
}


void writeseed (std::vector<std::vector<int>> data, std::string start,
                QString folder, QString ID,QString fn, QString network) {

    int size;
    int *c_ch1, *c_ch2,*c_ch3;
    std::string sh1,sh2,sh3;

    sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_Z";
    sh2="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_E";
    sh3="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_H_N";


//    qDebug()<<QString::fromStdString(sh1);
    size=data[0].size();


//    if (size >200) size=1000; else size=125;


    c_ch1=&data[0][0];
    c_ch2=&data[1][0];
    c_ch3=&data[2][0];

    QString path=folder+'/'+ID;
    QString filename=path+'/'+fn;
    QDir dir;

    if(!dir.exists(path))
    {
        dir.mkpath(path);
//        qDebug()<<dir.absolutePath();
    }

    seedOut3CH(start.c_str(),filename.toStdString().c_str(),
               sh1.c_str(),sh2.c_str(),sh3.c_str(),
               c_ch1,c_ch2,c_ch3,size);

    return;



}

void writedis (std::vector<std::vector<float>> data, std::string start,
               QString folder, QString ID,QString fn, QString network){


    int size;


    float *c_ch1, *c_ch2,*c_ch3;
    std::string sh1,sh2,sh3;


    sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_S_Z";
    sh2="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_S_E";
    sh3="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__B_S_N";


//    qDebug()<<QString::fromStdString(sh1);
    size=data[0].size();


//    if (size >200) size=1000; else size=125;


    c_ch1=&data[0][0];
    c_ch2=&data[1][0];
    c_ch3=&data[2][0];


    QString path=folder+'/'+ID;

    QString filename=path+'/'+fn;
    QDir dir;

    if(!dir.exists(path))
    {
        dir.mkpath(path);
//        qDebug()<<dir.absolutePath();
    }

    seedOut3CHf(start.c_str(),filename.toStdString().c_str(),
               sh1.c_str(),sh2.c_str(),sh3.c_str(),
               c_ch1,c_ch2,c_ch3,size);
    return;
//    qDebug()<<master->numtraces;
//    qDebug()<<packedrecords;


}
void seedOutf(const char* start,const char* filename, const char* sh1,float data,int size){

    int packedrecords;
    MS3Record *h1=NULL;
    MS3TraceList *hm;

    h1=msr3_init(NULL);
    strcpy (h1-> sid, sh1);
    h1 -> formatversion = 2;
    h1 -> reclen =512;
    h1 -> samprate = 1.0;
    h1 -> encoding = DE_FLOAT32;
    h1 -> starttime = ms_timestr2nstime(start);

    h1 -> numsamples = 1;

    h1 -> datasamples = &data;
    h1 -> sampletype = 'f';
    hm=mstl3_init(NULL);
    mstl3_addmsr(hm,h1,0,1,MSF_PACKVER2,NULL);
    packedrecords=mstl3_writemseed(hm,filename,0,512,'f',MSF_PACKVER2,0);
    h1->datasamples=NULL;
    msr3_free(&h1);
    mstl3_free(&hm,0);
}

void seedOut(const char* start,const char* filename, const char* sh1,int data,int size){

    int packedrecords;
    MS3Record *h1=NULL;
    MS3TraceList *hm;

    h1=msr3_init(NULL);
    strcpy (h1-> sid, sh1);
    h1 -> formatversion = 2;
    h1 -> reclen =512;
    h1 -> samprate = 1.0;
    h1 -> encoding = DE_FLOAT32;
    h1 -> starttime = ms_timestr2nstime(start);

    h1 -> numsamples = 1;

    h1 -> datasamples = &data;
    h1 -> sampletype = 'i';
    hm=mstl3_init(NULL);
    mstl3_addmsr(hm,h1,0,1,MSF_PACKVER2,NULL);
    packedrecords=mstl3_writemseed(hm,filename,0,512,'f',MSF_PACKVER2,0);
    h1->datasamples=NULL;
    msr3_free(&h1);
    mstl3_free(&hm,0);
}

void writeseed (float data, std::string start,
                QString folder, QString ID,QString fn, QString network,QString folderType) {


    std::string sh1;


    if (folderType=="Battery")sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__V_Q_1";
    else sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__V_K_1";

    QString path=folder+'/'+folderType;
    QString filename=path+'/'+fn;
    QDir dir;

    if(!dir.exists(path))
    {
        dir.mkpath(path);

    }

//    packedrecords=msr3_writemseed(c1,filename.toStdString().c_str(),0,MSF_PACKVER2,1);
    seedOutf(start.c_str(),filename.toStdString().c_str(),sh1.c_str(),data,1);
    return;
}


void writeseed (int data, std::string start,
                QString folder, QString ID,QString fn, QString network) {

    int size0=1;
    int packedrecords;

    int *c_ch1;
    std::string sh1;

    c_ch1=&data;


    sh1="FDSN:"+network.toStdString()+"_"+ID.toStdString()+"__K_O_C";


//    qDebug()<<QString::fromStdString(sh1);


    QString path=folder+'/'+"temp";

    QString filename=path+'/'+fn;
    QDir dir;

    if(!dir.exists(path))
    {
        dir.mkpath(path);

    }

    seedOut(start.c_str(),filename.toStdString().c_str(),sh1.c_str(),data,1);
    return;
//    qDebug()<<master->numtraces;
//    qDebug()<<packedrecords;


}


void fillmsr(MS3Record *&msr,const char* starttime,const char* sid, double sr,char sp){

    strcpy (msr-> sid, sid);
    msr -> formatversion = 2;
    msr -> reclen =512;
    msr -> samprate = sr;
    msr -> encoding = DE_FLOAT32;
    msr -> starttime = ms_timestr2nstime(starttime);
    msr -> sampletype = sp;
    msr -> numsamples = sr;
    return;
}

void fillmsrINT(MS3Record *&msr,const char* starttime,const char* sid, double sr){

    strcpy (msr-> sid, sid);
    msr -> formatversion = 2;
    msr -> reclen =512;
    msr -> samprate = sr;
    msr -> encoding = DE_STEIM2;
    msr -> starttime = ms_timestr2nstime(starttime);
    msr -> sampletype = 'i';
    return;
}

void filldata(MS3Record *&msr,float* data,int size){
    msr -> datasamples = data;
    msr -> samplecnt = size;
    return;


}

void filldataINT(MS3Record *&msr,int* data,int size){
    msr -> datasamples =data;
    msr -> numsamples  = size;
    return;

}







