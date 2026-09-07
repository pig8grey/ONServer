#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidgetItemIterator>
#include "myserver.h"
#include <QList>
#include <QFileDialog>
#include <QDir>
#include <QString>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QListWidget>
#include <QColor>
#include <QBrush>
#include <QDateTime>
#include <QScrollBar>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



signals:
    void sendcommand(QList<QString> command);
    void boxChanged(QList<QString> boxes);
private slots:
    void on_start_clicked();
    void startup(uint port);
    void cliaddr(QList<QString> address);
    void clidis(QList<QString> address);

    void cliinit(QList<QString> address);
    void clierr(QString msg);

    void on_pushButton_3_clicked();

    void on_stop_clicked();

    void on_direc_clicked();

private:
    void enable();
    void disable();
    Ui::MainWindow *ui;
    bool isSeed;
    myserver *server;

    void dotext(QString line,int level);
    uint port;
    QList<QTreeWidgetItem *> items;
    QList<QString> boxes;
    QString directory;
};
#endif // MAINWINDOW_H
