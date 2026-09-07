#include "myclient.h"
#include <cmath>
myclient::myclient(qintptr socketDescriptor,QObject * parent) : QObject(parent)
{


    qRegisterMetaType<std::vector<std::vector<double> >>("std::vector<std::vector<double> >");
    qRegisterMetaType<std::vector<float> >("std::vector<float> ");
    connect(timer, SIGNAL(timeout()),
          this, SLOT(timeOut()));
    timer->start(30000);

    setupSocket(socketDescriptor);

}
myclient::~myclient(){
    return;
}

QHostAddress myclient::getHost(){
    return socket->peerAddress();
}

quint16 myclient::getPort(){
    return socket->peerPort();
}


void myclient::setupSocket(qintptr socketDescriptor)
{
       socket = new QTcpSocket(this);
       socket->setSocketDescriptor(socketDescriptor);
       addr={socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())};
       connect(socket,SIGNAL(connected()),this,SLOT(connected()));
       connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
       connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));


}

void myclient::setupFolder(QString Folder,bool Seed, QString nw){

    baseFolder=Folder;
    disFolder="/home/nrcan/displacement";
    isSeed=Seed;
    network=nw;
}

void myclient::connected(){
    qDebug()<<"Connected Event";
}

void myclient::disconnected()
{
    if (!dashInfo.isEmpty())emit disconnect(dashInfo);
    else emit errormessage("Client "+ socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())+" disconnected");
    socket->close();
    socket->disconnectFromHost();
    socket->deleteLater();
    this->deleteLater();

}

void myclient::readyRead()
{


    data_recv=socket->readAll();
    mybytes+=data_recv;


    begin=mybytes.indexOf(header);
    end=mybytes.lastIndexOf(header);
//    qDebug()<<mybytes.length()<<begin<<end;

    if (begin>=0){
    ID=mybytes.mid(begin+4,3);

    }




    if (ID=="DVG" || ID=="LST")
    {
        timer->start();
        setGraph();


    }
    else if (!ID.isNull()){
        procBytes();
    }
    data_recv.squeeze();
    return;

}

void myclient::timeOut(){
    updateEvent(false);
    errorResponse("Client Timed Out!");
}

void myclient::setGraph(){

    int finish=mybytes.lastIndexOf(header);
    setBoxBytes();
//    qDebug()<<bytesToSend.mid(0,10)<<graphHeader;

    socket->write(bytesToSend+graphHeader+values);

    bytesToSend.clear();
    socket->waitForReadyRead(1000);
    if (!isInserted)
    {

    dashInfo={ID,
         QString::number(0),
         QString::number(0),
         socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())};
    emit myaddr(dashInfo);

    isInserted=true;
    }

//    qDebug()<<mybytes.mid(begin+7,2);
    if (mybytes.mid(begin+7,2)!=QByteArray::fromHex("9999")){
        QByteArray ba = mybytes.mid(begin+7,2);
        QDataStream ds(&ba, QIODevice::ReadWrite);
        unsigned short i;
        ds >> i;
        emit theBox(i);
    }
    if (begin!=finish)mybytes.remove(0,finish);

}

void myclient::setBoxBytes(){
    values.clear();

    for (int i=0;i<myBox.length();i++)
    {

        unsigned short b=(unsigned short) myBox[i].toInt();

        QByteArray ba((const char*)&b, sizeof(ushort));
        std::reverse(ba.begin(),ba.end());
        values+=ba;
    }
}

void myclient::procBytes(){


    if (end > begin && end>=0 && begin>=0)
    {

        data_buffer=mybytes.mid(begin,end-begin);
        setData();


//        maxVel=*std::max_element(std::begin(nums),std::end(nums));
        mutex.lock();
        std::vector<float> rawData;
        rawData=processBuffer(data_buffer);

        if (isSeed && mytime.isValid() && !station.isNull())
        {

            QDateTime fileTime=mytime;
            if (prevtime.secsTo(fileTime)>2) {
//                for (auto &i : prev)
//                    std::fill(i.begin(), i.end(), 0);
                errorResponse("Not Continous Time");}
            fileName=station+"_"+network+"_"+fileTime.toString("yyyy_MM_dd")+".mseed";

//            for (auto j=0;j<nums.size();j++){
//                if (!isnormal(nums[j])) ()<<j<<nums[j];
//            }
            seedTask *seedtask = new seedTask(rawData,baseFolder,
                               station,fileTime,fileName,network);
            seedtask->setAutoDelete(true);
            connect(seedtask, SIGNAL(Result(int)), this,
                    SLOT(TaskResult(int)), Qt::QueuedConnection);


            dispTask *disptask = new dispTask(rawData,prev,disFolder,
                               station,fileTime,fileName,network,writeDis);
            disptask->setAutoDelete(true);
            if (DisCount>40) writeDis=true;
            else DisCount++;

            connect(disptask, SIGNAL(Result(int)), this,
                    SLOT(TaskResult(int)), Qt::QueuedConnection);
            connect(disptask, SIGNAL(previousNumber(std::vector<std::vector<double>>)), this,
                    SLOT(updatePrev(std::vector<std::vector<double>>)), Qt::QueuedConnection);
            connect(disptask, SIGNAL(myMaxDis(std::vector<float>)), this,
                    SLOT(updateMaxDis(std::vector<float>)), Qt::QueuedConnection);


            QThreadPool::globalInstance()->start(disptask);
            QThreadPool::globalInstance()->start(seedtask);
            prevtime=fileTime;


            eventIndicator=DetEvt();
            switch (eventIndicator){
            case 1:
                updateEvent(false);
                // qDebug()<<station<<"Event OVER";
                break;
            case 2:
                updateEvent(true);
//                qDebug()<<station<<fileTime<<"Event Identified";
                break;
            default:

                ;

            }




        }
        else if (mytime.isValid()){
//        fileName=mytime.toString("yyyyMMddhhmmss")+".dat";
//        MyTask *mytask = new MyTask(data_buffer,baseFolder,ID,fileName);
//        mytask->setAutoDelete(true);
//        connect(mytask, SIGNAL(Result(int)), this,
//                SLOT(TaskResult(int)), Qt::QueuedConnection);
//        QThreadPool::globalInstance()->start(mytask);
            }
        else{


            errorResponse("Invalid Time Parameters.");
        }


        mybytes.remove(0,end);
        mybytes.squeeze();
        mutex.unlock();

    }

    return;
}

void myclient::eventUpdate(int number){
    eventIndicator=number;
    return;
}


void myclient::errorResponse(QString msg){
    if (!ID.isEmpty())emit errormessage("Box "+ID+" "+msg+" Battery is at "+QString::number(myVolt,'f',2)+"V");
    else emit errormessage(socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())+" "+msg);
//    emit disconnect(dashInfo);
    socket->close();
    socket->disconnectFromHost();
    socket->deleteLater();
    this->deleteLater();
    return;

}

void myclient::warningResponse(QString msg){
    if (!ID.isEmpty())emit errormessage("Box "+ID+" "+msg+" Battery is at "+QString::number(myVolt,'f',2)+"V");
    else emit errormessage(socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())+" "+msg);
    return;


}
void myclient::setData(){


    ID=data_buffer.mid(4,3);
//    qDebug()<<ID<<mytime<<data_buffer.mid(0,24).toHex();
    getdt(data_buffer.mid(0,headlen));

    latitude=getgps(data_buffer.mid(15,4));
    longitude=getgps(data_buffer.mid(19,4));
    myVolt=getVoltage();
    mytemp=getTemp();
    timer->start();
    if (myVolt<9.0 && notWarned){ warningResponse(" Battery Low! "); notWarned=false;};

    if (!isInserted)
    {


      if (ID=="083") station="SBTM";
      if (ID=="082") station="SBTM";
      if (ID=="084") station="NBTM";


//          qDebug()<<ID<<latitude<<longitude;


    if (station.isNull() and reStation<3)
    {
        reStation++;
    }
    else{
    if (station.isNull())station=ID;
    dashInfo={station+" Box:"+ID,
          QString::number(latitude,'f',10),
          QString::number(longitude,'f',10),
         socket->peerAddress().toString()+":"+ QString::number(socket->peerPort()),
          QString::number(myVolt,'f',2),};
    emit myaddr(dashInfo);
    isInserted=true;

    }
    }

    if (battSeconds>4 && !station.isNull() ){


        QDateTime fileTime = mytime.toTimeZone(newZone);
        fileName=station+"_"+network+"_"+fileTime.toString("yyyy_MM_dd")+"_battery.mseed";
        healthSeed *healthseed = new healthSeed(myVolt,baseFolder,
                           station,fileTime,fileName,network,'V');
        healthseed->setAutoDelete(true);

        connect(healthseed, SIGNAL(Result(int)), this,
                SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(healthseed);


        fileName=station+"_"+network+"_"+fileTime.toString("yyyy_MM_dd")+"_temperature.mseed";
        healthSeed *tempseed = new healthSeed(mytemp,baseFolder,
                           station,fileTime,fileName,network,'T');
        tempseed->setAutoDelete(true);

        connect(tempseed, SIGNAL(Result(int)), this,
                SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(tempseed);
        battSeconds=0;
    }

    battSeconds++;
    return;





}
float myclient::getgps(QByteArray arrin){

    float result;
    memcpy(&result, arrin.data(),static_cast<size_t>(sizeof(result)));
    return result;
}


void myclient::getdt(QByteArray data){
//    QByteArray y = data.mid(7,2);
    QByteArray mdhms=data.mid(9,5);
    QDateTime temp=QDateTime::currentDateTimeUtc();
//    unsigned short year;
//    unsigned char* dates=new unsigned char[5];
//    qDebug()<<temp.date();
//    std::reverse(y.begin(),y.end());
//    memcpy(&year, y.data(),static_cast<size_t>(sizeof(year)));


//    memcpy(&dates,mdhms.data(),static_cast<size_t>(sizeof(dates)));
    mytime=QDateTime(QDate(temp.date().year(),mdhms[0],mdhms[1]),QTime(mdhms[2],mdhms[3],mdhms[4]), Qt::UTC);

    if (mytime.isValid()){timer->start();}

    return;

}

std::vector<float> myclient::processBuffer(QByteArray &bytes)
{
    std::vector<float> myNums;
    if (socket->isValid()){
    socket->write(bytes.mid(0,7));

    bytes=bytes.remove(0,headlen);

    QDataStream in(&bytes, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
  //  QVector<float> results;
    int check=bytes.length()%4;
    myNums.resize((int)bytes.length()/4);
  //  results.resize(bytes.length()/4);
 //    qDebug()<<bytes.length();
    in.readRawData((char*)myNums.data(), bytes.length()-check);
   // in.readRawData((char*)results.data(), bytes.length()-check);
    //myNums=results.toStdVector();

//    qDebug()<<nums;


    }

    return myNums;

}


void myclient::procCmd(QList<QString> cmd){
    target=cmd[0];
    if (cmd[1]=="ltereset"){
        resetCommand=QByteArray::fromHex("55aa55");
    }else if(cmd[1]=="bdreset"){
        resetCommand=QByteArray::fromHex("aa55aa");
    }
    return;
}

void myclient::TaskResult(int number)
{
    if (target==ID){
        for (int i=0;i<3;i++){
        socket->write(resetCommand);}
        target="";
    }
    return;

}

void myclient::setTgt(unsigned short i){
    tgt=i;
    return;

}

void myclient::setBox(QList<QString> b){

    if (ID=="LST" || ID=="DVG") myBox=b;
    return;

}

void myclient::setGraphByte(QByteArray b){
//    qDebug()<<b;
//    qDebug()<<b.length();
    if (ID=="LST" || ID=="DVG") {bytesToSend=b;}
    return;
}


float myclient::getVoltage(){
    QByteArray startByte,endByte,result;
    float voltage;
    quint16 baseNum;
//    startByte=data_buffer.mid(23,1);
//    endByte=data_buffer.mid(14,1);

    startByte=data_buffer.mid(23,1);
    endByte=data_buffer.mid(24,1);

    result=startByte+endByte;


    QDataStream myStream(result);
    myStream >> baseNum;

    voltage=baseNum/65536.0*3.3*5.0*1.1;

    return voltage;
}

float myclient::getTemp(){
    QByteArray startByte,endByte;
    float temperature;

    unsigned char det,startNum,endNum;
    quint8 sN,fN;


    startByte=data_buffer.mid(7,1).toHex();
    endByte=data_buffer.mid(8,1);


    QDataStream st1(&startByte,QIODevice::ReadOnly);
    st1 >> det;
    det=det & 0x1f; det=det & 0x10;

    st1 >> startNum;
//    qDebug()<<startByte<<endByte;
    startNum=startNum & 0x0f;
//    qDebug()<<startNum;
    QDataStream st2(&endByte,QIODevice::ReadOnly);
    st2 >> endNum;
//    qDebug()<<endNum;

    sN=startNum;fN=endNum;


    if (det==0x10)
    {
        temperature= 256 - (sN * 16.0 + fN / 16.0);
    }
    else {
        temperature= sN * 16.0 + fN / 16.0;
    }

//    qDebug()<<temperature;
    return temperature;
}


bool myclient::compare(float &value1, float value2)
{
    return std::abs(value1 - value2) < std::pow(10, -3);
}

void myclient::updatePrev(std::vector <std::vector<double>> myVal){
    prev=myVal;
    return;
}

void myclient::updateEvent(bool isEvent){

    int minSeconds=180;

    if (isEvent || (eventCount>0 && eventCount<minSeconds)){

        for (auto i=0; i<3; i++){
            if (maxDis[i]>eventMax[i]) {eventMax[i]=maxDis[i];}
        }

        for (auto i=3; i<6; i++){
            if (maxDis[i]<eventMax[i]) {eventMax[i]=maxDis[i];}
        }
        if (newEvent){ et=mytime;  newEvent=false;}

        eventCount++;
//        if (isEvent && eventCount >3) eventCount=eventCount/2;
//        if (*std::max_element(eventMax.begin(),eventMax.end())>0.5) eventCount=540;

    }

    if (!isEvent && !newEvent && eventCount>=minSeconds){
        if (!et.isValid()) et=mytime.addSecs(-120);
        writeEvent *myevent=new writeEvent(eventMax,baseFolder,
                                           station,et,fileName,network);
        connect(myevent, SIGNAL(Result(int)), this,
                SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(myevent);
        newEvent=true;
        std::fill(eventMax.begin(), eventMax.end(), 0.0);
//        qDebug()<<station<<"Event is writing";
        eventCount=0;

    }

    return;

}

int myclient::DetEvt(){
    // if (station=="SBTM") return 0;
        if ((currentMaxDis-currentMinDis)>0.2 && writeDis) {return 2;}
        if ((currentMaxDis-currentMinDis)<0.06 && writeDis
                && (currentMaxDis-currentMinDis)>0.005) return 1;


    return 0;
}


void myclient::updateMaxDis(std::vector<float> currentDis){

    maxDis=currentDis;

    std::vector<float> data={0.0,0.0,0.0,0.0,0.0,0.0};
//    if (station == "SBTM"){
//        maxDis[2]=0.0;
//        maxDis[5]=0.0;}

    for (int i = 0, j = 0; i < maxDis.size(); ++i) {

        if (station == "SBTM"){
            if (i != 2 && i != 5) {data[i]=maxDis[i];}
        }

        if (station != "SBTM")data[i]=maxDis[i];
    }

    currentMaxDis=*std::max_element(data.begin() , data.end());
    currentMinDis=*std::min_element(data.begin() , data.end());

    return;

}







