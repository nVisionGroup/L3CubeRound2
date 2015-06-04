#include "webanalytics.h"
#include "ui_webanalytics.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "mydialog.h"

WebAnalytics::WebAnalytics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebAnalytics)
{
    ui->setupUi(this);
    setWindowTitle("Web Log Analysis");

    ui->frame->setEnabled(false);

    md1=new mydialog();
}

WebAnalytics::~WebAnalytics()
{
    delete ui;
}

void WebAnalytics::on_actionAnalysis_triggered()
{
    /*check for previously created ouput folder.If it exists,delete it and create again.*/

    QDir outputfolder("/home/Shubham/OutputFolder");
    if(outputfolder.exists()){
        outputfolder.remove("/home/Shubham/OutputFolder/uniqueDomains.txt");
        QDir().rmdir("/home/Shubham/OutputFolder");
    }

    outputfolder.mkpath("/home/Shubham/OutputFolder");

    ui->frame->setEnabled(true);
}

void WebAnalytics::on_pushButton_clicked()
{
    /*file browser*/
    filepath=QFileDialog::getOpenFileName(this,tr("Open Weblog"),"/home",tr("Text Files(*.txt)"));
    ui->lineEdit->setText(filepath);
}

void WebAnalytics::on_pushButton_2_clicked()
{
    /*check if weblog.txt exists*/

    filepath=ui->lineEdit->text();

    checkfile.setFile(filepath);

    if(checkfile.exists()&&checkfile.isFile()){
        start_analysis_process();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("File not found ! Please check path.");
        msgBox.exec();
        ui->frame->setEnabled(false);
    }
}

void WebAnalytics::start_analysis_process()
{
    ui->menuBar->setEnabled(false);
    ui->label_4->setText("Analyzing...");

    domain_analysis();

    ui->label_4->setText("Complete.");
    ui->frame->setEnabled(false);
    ui->menuBar->setEnabled(true);
}

/*read weblog and find unique domains using regular expressions*/

void WebAnalytics::domain_analysis()
{
    uniqueDomains.clear();
    QRegExp rx("(?:GET|POST) www.([A-Za-z0-9]+).");

    QFile weblog(filepath);
    if(!weblog.open(QIODevice::ReadOnly|QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("Error opening file !");
        msgBox.exec();
        ui->frame->setEnabled(false);
        return;
    }

    QTextStream in(&weblog);

    QFile file("/home/Shubham/OutputFolder/uniqueDomains.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox msgBox;
        msgBox.setText("Oops ! Error occured !");
        msgBox.exec();
        ui->frame->setEnabled(false);
        return;
    }

    QTextStream out(&file);

    while(!in.atEnd()){
        if(rx.indexIn(in.readLine())>=0){
        QString captured=rx.cap(1);

        if(!uniqueDomains.contains(captured)){
            uniqueDomains << captured;  //insert into list
            out << captured << "\n";    //insert into file to be accessed from any other window
        }
        }
    }

    file.close();
    weblog.close();

}

void WebAnalytics::on_actionList_of_Domains_triggered()
{
    md1->display_data();
    md1->show();
}
