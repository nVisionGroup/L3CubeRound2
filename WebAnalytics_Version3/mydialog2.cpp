#include "mydialog2.h"
#include "ui_mydialog2.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QRect>

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
            msgBox.setText("Error opening file!");
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

    drawHistogram();
}

void mydialog2::drawHistogram()
{
    int pos=0;
    int max=0;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(max < ui->tableWidget->item(i,1)->text().toInt())
            max=ui->tableWidget->item(i,1)->text().toInt();
    }

    int scale= 400/max;
    int firstc=128;
    int secondc=128;
    int thirdc=128;


    ui->label->setParent(ui->scrollArea);
    QPixmap pix((ui->tableWidget->rowCount())*70,420);
    pix.fill(Qt::white);
    ui->label->setFixedSize(pix.size());

    QPainter painter(&pix);
    painter.setPen(Qt::black);
    painter.drawLine(0,400,1000,400);

    for(int i=0;i<ui->tableWidget->rowCount();i++){
        pos+=50;
        firstc+=10;
        secondc-=10;
        thirdc+=20;
        QRgb color=qRgb(firstc,secondc,thirdc);

        int histoheight=ui->tableWidget->item(i,1)->text().toInt();

        QPoint topleft(pos,400-(histoheight*scale));
        QPoint bottomright(pos+20,400);
        QRect rect(topleft,bottomright);


        painter.drawRect(rect);
        painter.fillRect(rect,color);
        ui->tableWidget->item(i,0)->setBackgroundColor(color);

    }

    ui->textBrowser->append("Instructions:\n1.Check table cell color to compare with histogram.\n");
    ui->textBrowser->append("2.Scale : 1 visitor = "+QString::number(scale)+"Pixels" );
    ui->textBrowser->setEnabled(false);

    ui->scrollArea->setWidget(ui->label);
    ui->label->setPixmap(pix);
}
