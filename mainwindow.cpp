#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->explbl->setText(QDir("/home/nrcan/test").absolutePath());
    directory=ui->explbl->text();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_start_clicked()
{
        if (ui->netName->toPlainText().length()==2){
        server=new myserver();
        disable();
        QString text=ui->portText->toPlainText();
        uint portnum=text.toInt();
        isSeed=ui->mseed->isChecked();
        connect(server,SIGNAL(started(uint)),this,SLOT(startup(uint)));
        connect(server,SIGNAL(sendaddr(QList<QString>)),this,SLOT(cliaddr(QList<QString>)));
        connect(server,SIGNAL(disconnect(QList<QString>)),this,SLOT(clidis(QList<QString>)));
        connect(server,SIGNAL(incomming(QList<QString>)),this,SLOT(cliinit(QList<QString>)));
        connect(server, SIGNAL(err(QString)), this,SLOT(clierr(QString)));
        connect(this,SIGNAL(sendcommand(QList<QString>)),server,SLOT(getcmd(QList<QString>)));
        connect(this,SIGNAL(boxChanged(QList<QString>)),server,SLOT(getBoxes(QList<QString>)));
        server->start(portnum,directory,isSeed,ui->netName->toPlainText());

        }
        else {
        dotext("NetWork Name Must be 2 characters!",0);
        }

}

void MainWindow::enable(){
    ui->direc->setEnabled(true);
    ui->start->setEnabled(true);
    ui->mseed->setEnabled(true);
    ui->stop->setEnabled(false);

}

void MainWindow::disable(){
    ui->direc->setEnabled(false);
    ui->start->setEnabled(false);
    ui->mseed->setEnabled(false);
    ui->stop->setEnabled(true);
}
void MainWindow::startup(uint port){
        if (port <=0){
            dotext("Error! Server failed to start",9);
        }
        else{
          dotext("Server Started on Port: "+QString::number(port),1);
          if (isSeed){
          dotext("Writing Miniseed Files",1);}
          else{
          dotext("Writing dat Files",1);}
          dotext("Set export directory: "+directory,0);
        }
}
void MainWindow::cliinit(QList<QString> address){
    dotext("Client from "+address[0]+" connected",3);
}

void MainWindow::clierr(QString msg){
    dotext("Error! "+msg,9);
}

void MainWindow::cliaddr(QList<QString> address){

    QTreeWidgetItem *clidata = new QTreeWidgetItem(ui->cliInfo);

    for (int i=0;i<address.length();i++){
        clidata->setText(i,address[i]);
    }

    ui->cliInfo->insertTopLevelItem(0,clidata);

    ui->totalcli->setText(QString::number(ui->cliInfo->topLevelItemCount()));
    dotext("Hello I am station# "+address[0]+" Lat/long "+address[1]+" , "+address[2],4);

    if (address[0]!="DVG" && address[0]!="LST")
    {boxes<<address[0];}
    emit boxChanged(boxes);


}

void MainWindow::clidis(QList<QString> address){

    if (address.length()>2){
    boxes.clear();
    QList<QTreeWidgetItem *> targets;
    QString txt=address[0];
    targets=ui->cliInfo->findItems(txt,Qt::MatchExactly);
    QList<QTreeWidgetItem *>::iterator i;
    for (i = targets.begin(); i != targets.end(); ++i) delete *i;
//    delete targets;


    emit boxChanged(boxes);
//    delete target;

    ui->totalcli->setText(QString::number(ui->cliInfo->topLevelItemCount() ));
        dotext("Box "+address[0]+" disconnected",2);

    }
    else if (address.length()>0){
        dotext("Client @ "+address[0]+" disconnected",2);
    }


}

void MainWindow::dotext(QString line,int level){

    QListWidgetItem *item = new QListWidgetItem();
    QDateTime now = QDateTime::currentDateTimeUtc();
    line=now.toString(Qt::ISODateWithMs)+" -> " + line;
    item->setText(line);

    switch(level)
    {
        case 1: item->setBackground(QBrush(QColor("#4bb543"))); break;
        case 2: item->setBackground(QBrush(QColor("#ffc4ff"))); break;
        case 3: item->setBackground(QBrush(QColor("#000000"))); item->setForeground(QBrush(QColor("#00aecd")));break;
        case 4: item->setBackground(QBrush(QColor(100,210,255))); break;
        case 9: item->setBackground(QBrush(QColor("#ff6262"))); break;
        default: item->setBackground(QBrush(QColor("#dddddd"))); break;
    }


    ui->timeLog->addItem(item);
    if (ui->timeLog->count()>2048){
        for (int i=0; i<(ui->timeLog->count()-2048);i++){
        delete ui->timeLog->item(0);}
    }
    ui->timeLog->verticalScrollBar()->setValue(ui->timeLog->verticalScrollBar()->maximum());

    QFile myfile("./1000log.txt");
    if(myfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
        QTextStream logstream(&myfile);
                  logstream << line <<'\n';
                  myfile.close();
    }
}




void MainWindow::on_pushButton_3_clicked()
{
    QList<QString> command;
    QString ID=ui->tgtID->toPlainText();
    QString theCommand=ui->tgtCommand->toPlainText();

    bool goodCommand=false;


    if (theCommand=="ltereset" || theCommand=="bdreset")
    {
        goodCommand=true;
    }

    if (goodCommand)
    {
    command.append(ID);
    command.append(theCommand);
    emit sendcommand(command);
    dotext("Command: "+command[1]+" is sent to Box: "+command[0],1);
    }
    else {
        QMessageBox::critical(
            this,
            tr("Server"),
            tr("Invalid Command") );
    }


    return;
}



void MainWindow::on_stop_clicked()
{

    delete server;
    ui->cliInfo->clear();
    dotext("Stop Button Clicked!",9);
    enable();

//    qApp->exit( -9876 );


}

void MainWindow::on_direc_clicked()
{
    QString expName = QFileDialog::getExistingDirectory(this,"Select Directory");
    ui->explbl->setText(expName);
    directory=expName;
}
