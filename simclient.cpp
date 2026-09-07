#include "simclient.h"

simclient::simclient(qintptr socketDescriptor,QObject * parent) : QObject(parent)
{


    qRegisterMetaType<std::vector<std::vector<double> >>("std::vector<std::vector<double> >");
    qRegisterMetaType<std::vector<float> >("std::vector<float> ");
    connect(timer, SIGNAL(timeout()),
          this, SLOT(timeOut()));
    timer->start(30000);

    setupSocket(socketDescriptor);

}
simclient::~simclient(){
    return;
}

QHostAddress simclient::getHost(){
    return socket->peerAddress();
}

quint16 simclient::getPort(){
    return socket->peerPort();
}


void simclient::setupSocket(qintptr socketDescriptor)
{
       socket = new QTcpSocket(this);
       socket->setSocketDescriptor(socketDescriptor);
       addr={socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())};
       connect(socket,SIGNAL(connected()),this,SLOT(connected()));
       connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
       connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
       return;


}

void simclient::setupFolder(QString Folder,bool Seed, QString nw){

    baseFolder=Folder;
    isSeed=Seed;
    network=nw;
    return;
}

void simclient::connected(){
    qDebug()<<"Connected Event";
    return;
}

void simclient::disconnected()
{
    emit disconnect(dashInfo);
    socket->close();
    socket->disconnectFromHost();
    this->deleteLater();
    return;

}

void simclient::readyRead()
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

    return;

}

void simclient::timeOut(){
    errorResponse("Client Timed Out!");
    return;
}

void simclient::setGraph(){

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
    return;

}

void simclient::setBoxBytes(){
    values.clear();
//    qDebug()<<myBox;
    for (int i=0;i<myBox.length();i++)
    {

        unsigned short b=(unsigned short) myBox[i].toInt();
//        qDebug()<<b;
        QByteArray ba((const char*)&b, sizeof(ushort));
        std::reverse(ba.begin(),ba.end());
        values+=ba;
    }
    return;
}

void simclient::procBytes(){


    if (end > begin && end>=0 && begin>=0)
    {

        data_buffer=mybytes.mid(begin,end-begin);
        if (ID.toInt()==tgt && data_buffer.length()>10000){
            emit toGraph(data_buffer);
        }
        setData();

        nums=processBuffer(data_buffer);
        maxVel=*std::max_element(std::begin(nums),std::end(nums));

//            qDebug()<<mytime.isValid();
        if (isSeed && mytime.isValid() && !station.isNull())
        {


        fileName=station+"_"+network+"_"+mytime.toString("yyyy_MM_dd")+".mseed";

            if (!std::any_of(nums.cbegin(),nums.cend(),[](float i){return std::isfinite(i);})){
                errorResponse("Corrupt Data.");
                return;
            }
            else{
                seedTask *seedtask = new seedTask(nums,baseFolder,
                                   station,mytime,fileName,network);
                seedtask->setAutoDelete(true);

        //        int timediff= QDateTime::currentDateTimeUtc().secsTo(mytime);
        //        if(timediff>0) {
        //            qDebug()<<QString::number(timediff)<<mytime.toString();
        //            QFile myfile("/home/nrcan/timetest/timeDifference.txt");
        //            if(myfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        //                QTextStream logstream(&myfile);
        //                          logstream << "Difference is: "<<timediff<<"Board Time is: "<< mytime.toString() <<'\n';
        //                          myfile.close();
        //            }
        //        }
                connect(seedtask, SIGNAL(Result(int)), this,
                        SLOT(TaskResult(int)), Qt::QueuedConnection);

                QThreadPool::globalInstance()->start(seedtask);




                }




        }
        else if (mytime.isValid()){
        fileName=mytime.toString("yyyyMMddhhmmss")+".dat";
        MyTask *mytask = new MyTask(data_buffer,baseFolder,ID,fileName);
        mytask->setAutoDelete(true);
        connect(mytask, SIGNAL(Result(int)), this,
                SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(mytask);
            }
        else{
            errorResponse("Invalid Time Parameters.");
        }



//        qDebug()<<"Before Delete"<<end<<mybytes.length();
        mybytes.remove(0,end);
        return;
//        qDebug()<<"After Delete"<<mybytes.length();
    }

}
void simclient::errorResponse(QString msg){
    if (!ID.isEmpty())emit errormessage("Box "+ID+" "+msg+" Battery is at "+QString::number(myVolt,'f',2)+"V");
    else emit errormessage(socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())+" "+msg);
//    emit disconnect(dashInfo);
    socket->close();
    socket->disconnectFromHost();
    this->deleteLater();
    return;


}

void simclient::warningResponse(QString msg){
    if (!ID.isEmpty())emit errormessage("Box "+ID+" "+msg+" Battery is at "+QString::number(myVolt,'f',2)+"V");
    else emit errormessage(socket->peerAddress().toString()+":"+ QString::number(socket->peerPort())+" "+msg);
    return;


}
void simclient::setData(){


    ID=data_buffer.mid(4,3);
    getdt(data_buffer.mid(0,24));
    latitude=getgps(data_buffer.mid(15,4));
    longitude=getgps(data_buffer.mid(19,4));
    myVolt=getVoltage();

    if (myVolt<9.0 && notWarned){ warningResponse(" Battery Low! "); notWarned=false;};
    fileName=station+"_"+network+"_"+mytime.toString("yyyy_MM_dd")+"_battery.mseed";




    if (!isInserted)
    {

    if(compare(latitude,56.00004578f) && compare(longitude,-120.6096954f)) station="031";
    if(compare(latitude,55.98433685f) && compare(longitude,-120.6081238f)) station="029";
    if(compare(latitude,56.0f) && compare(longitude,-120.5263214f)) station="009";
    if(compare(latitude,56.0225f) && compare(longitude,-120.596f)) station="016";
    if(compare(latitude,55.96354294) && compare(longitude,-120.5647049)) station="020";
    if(compare(latitude,56.575809478759766) && compare(longitude,-121.31744384765625)) station="CNRL";
    if (ID=="042") station="P1A";
    if (ID=="039") station="P1B";

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

        healthSeed *healthseed = new healthSeed(myVolt,baseFolder,
                           station,mytime,fileName,network,'V');
        healthseed->setAutoDelete(true);

        connect(healthseed, SIGNAL(Result(int)), this,
                SLOT(TaskResult(int)), Qt::QueuedConnection);
        QThreadPool::globalInstance()->start(healthseed);
        battSeconds=0;
    }

    battSeconds++;
    return;
//    fileName=mytime.toString("yyyyMMddhhmmss")+".mseed";




}
float simclient::getgps(QByteArray arrin){

    float result;
    memcpy(&result, arrin.data(),static_cast<size_t>(sizeof(result)));
    return result;
}


void simclient::getdt(QByteArray data){
    QByteArray y = data.mid(7,2);
    QByteArray mdhms=data.mid(9,5);

    unsigned short year;
    unsigned char* dates=new unsigned char[5];

    std::reverse(y.begin(),y.end());
    memcpy(&year, y.data(),static_cast<size_t>(sizeof(year)));
    memcpy(&dates,mdhms.data(),static_cast<size_t>(sizeof(dates)));
    mytime=QDateTime(QDate(year,mdhms[0],mdhms[1]),QTime(mdhms[2],mdhms[3],mdhms[4]), Qt::UTC);
    if (mytime.isValid())timer->start();
    return;

}





std::vector<float> simclient::processBuffer(QByteArray &bytes)
{
    std::vector<float> myNums;
    if (socket->isValid()){
    socket->write(bytes.mid(0,7));

    QVector<float> nums;
    bytes=bytes.remove(0,24);

    QDataStream in(&bytes, QIODevice::ReadOnly);
    in.setByteOrder(QDataStream::LittleEndian);
    QVector<float> results;
    int check=bytes.length()%4;
    results.resize(bytes.length()/4);
    in.readRawData((char*)results.data(), bytes.length()-check);

    myNums=results.toStdVector();
//    qDebug()<<nums;


    }

    return myNums;

}


void simclient::procCmd(QList<QString> cmd){
    target=cmd[0];
    if (cmd[1]=="ltereset"){
        resetCommand=QByteArray::fromHex("55aa55");
    }else if(cmd[1]=="bdreset"){
        resetCommand=QByteArray::fromHex("aa55aa");
    }
    return;
}

void simclient::TaskResult(int number)
{
    if (target==ID){
        for (int i=0;i<3;i++){
        socket->write(resetCommand);}
        target="";
    }
    return;

}

void simclient::setTgt(unsigned short i){
    tgt=i;
    return;

}

void simclient::setBox(QList<QString> b){

    if (ID=="LST" || ID=="DVG") myBox=b;
    return;

}

void simclient::setGraphByte(QByteArray b){
//    qDebug()<<b;
//    qDebug()<<b.length();
    if (ID=="LST" || ID=="DVG") {bytesToSend=b;}
    return;
}


float simclient::getVoltage(){
    QByteArray startByte,endByte,result;
    float voltage;
    quint16 baseNum;
    startByte=data_buffer.mid(23,1);
    endByte=data_buffer.mid(14,1);

    result=startByte+endByte;


    QDataStream myStream(result);
    myStream >> baseNum;

    voltage=baseNum/65536.0*3.3*5.0*1.1;

    return voltage;
}



bool simclient::compare(float &value1, float value2)
{
    return std::abs(value1 - value2) < std::pow(10, -3);
}

