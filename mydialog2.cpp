#include "mydialog2.h"
#include "ui_mydialog2.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

mydialog2::mydialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mydialog2)
{
    ui->setupUi(this);
    setWindowTitle("Visitor Analysis");
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);

}

mydialog2::~mydialog2()
{
    delete ui;
}

void mydialog2::display_data(QStringList uniqueDomains)
{
    ui->tableWidget->setRowCount(0);
    for(int i=0;i<uniqueDomains.size();i++){
        QFile file("/home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt");

        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("Error opening file 4 !");
            msgBox.exec();
            return;
        }

        QTextStream in(&file);

        int linecount=0;
        while(!in.atEnd()){
            in.readLine();
            linecount++;
        }

        file.close();

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(uniqueDomains.at(i)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(linecount)));
    }
}
