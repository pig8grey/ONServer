#include "myserver.h"

myserver::myserver(QObject *parent) : QTcpServer(parent)
{


}
myserver::~myserver(){
    while(!connections.isEmpty()){
    connections.removeAt(0);
    }
}


void myserver::start(uint myPort,QString indir,bool Seed,QString nw)
{

       if (!listen(QHostAddress::AnyIPv4, myPort))
       {
           emit started(0);
       }
       else
       {
           emit started(myPort);
           directory=indir;
           isSeed=Seed;
           network=nw;

       }



}

void myserver::incomingConnection(qintptr socketDescriptor){

    myclient *client = new myclient(socketDescriptor);
    client->setupFolder(directory,isSeed,network);

    connect(client, SIGNAL(myaddr(QList<QString>)), this,SLOT(cliaddr(QList<QString>)));
    connect(client, SIGNAL(disconnect(QList<QString>)), this,SLOT(clidis(QList<QString>)));

    connect(client, SIGNAL(errormessage(QString)), this,SLOT(clierr(QString)));

    connect(this, SIGNAL(sendCommand (QList<QString>)),client,SLOT(procCmd(QList<QString>)));

    connect(this,SIGNAL(sendBox(QList<QString>)),client,SLOT(setBox(QList<QString>)));

    connect(client, SIGNAL(theBox (unsigned short)), this,SLOT(desire(unsigned short)));
    connect(this, SIGNAL(setDesire (unsigned short)),client,SLOT(setTgt(unsigned short)));

    connect(client, SIGNAL(toGraph (QByteArray)), this,SLOT(recvGraphBytes(QByteArray)));
    connect(this, SIGNAL(graphBytes (QByteArray)),client,SLOT(setGraphByte(QByteArray)));

    connect(client, SIGNAL(eventInfo (int)), this,SLOT(eventReceive(int)));
    connect(this, SIGNAL(sendEventNum (int)),client,SLOT(eventUpdate(int)));


    emit incomming(client->addr);
    connections.append(client);
}

void myserver::cliaddr(QList<QString> address){
    if (address.length()>3){
    emit sendaddr(address);
    }
    else{
    emit incomming(address);
    }
}

//void myserver::delcon(QTcpSocket * sock){
//    int target=-1;
//    for (int i=0; i<connections.length();i++){
//        if (connections[i]->getHost()==sock->peerAddress() &&
//           connections[i]->getPort()==sock->peerPort() ){
//            target=i;
//            break;
//        }
//    }
//    if (target>0){

//    connections[target]->deleteLater();
//    connections.removeAt(target);
//    }else{
//        qDebug()<<"Nothing Found";
//    }
//}
void myserver::clidis(QList<QString> address){

    emit disconnect(address);

}
void myserver::clierr(QString msg){
    emit err(msg);

}

void myserver::getcmd(QList<QString> command){
    emit sendCommand (command);

}
void myserver::desire(unsigned short i){
    emit setDesire(i);
}
void myserver::getBoxes(QList<QString> b){
    emit sendBox(b);
}
void myserver::eventReceive(int number){
    emit sendEventNum(number);
}
void myserver::recvGraphBytes(QByteArray b){
//    qDebug()<<b;
    emit graphBytes(b);
}

