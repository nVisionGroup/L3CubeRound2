#include "mydialog.h"
#include "ui_mydialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

mydialog::mydialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mydialog)
{
    ui->setupUi(this);
    setWindowTitle("List of Domains");
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

mydialog::~mydialog()
{
    delete ui;
}

/*to read the file containing list of domains and put them into a table*/
void mydialog::display_data()
{
    ui->tableWidget->setRowCount(0);
    QFile file("/home/Shubham/OutputFolder/uniqueDomains.txt");

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("Oops ! Error occured ! Try to analyse first.");
        msgBox.exec();
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()){
        QString domain=in.readLine();
        int i=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(domain));
    }

    file.close();
}
