#include "webanalytics.h"
#include "ui_webanalytics.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QProcess>
#include "mydialog.h"
#include "mydialog2.h"

WebAnalytics::WebAnalytics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WebAnalytics)
{
    ui->setupUi(this);
    setWindowTitle("Web Log Analysis");

    ui->frame->setEnabled(false);

    md1=new mydialog();
    md2=new mydialog2();
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

        for(int i=0;i<uniqueDomains.size();i++){
            outputfolder.remove("/home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt");
        }

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
    visitor_analysis();
    refined_visitor_analysis();

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

void WebAnalytics::visitor_analysis()
{
    for(int i=0;i<uniqueDomains.size();i++){
        QRegExp rx1("(?:GET|POST) www."+uniqueDomains.at(i)+".");
        QRegExp rx2("^([0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3})\\s");

        QFile weblog(filepath);

        if(!weblog.open(QIODevice::ReadOnly|QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("Error opening file !");
            msgBox.exec();
            ui->frame->setEnabled(false);
            return;
        }

        QTextStream in(&weblog);

        QFile file("/home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt");

        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QMessageBox msgBox;
            msgBox.setText("Error opening file !");
            msgBox.exec();
            ui->frame->setEnabled(false);
            return;
        }

        QTextStream out(&file);

        QString perline;
        while(!in.atEnd()){
            if((perline=in.readLine()).contains(rx1)){
                if(rx2.indexIn(perline)>=0){
                    QString captured=rx2.cap(1);
                    out << captured <<"\n";
                }
            }
        }

        file.close();
        weblog.close();
    }
}

void WebAnalytics::refined_visitor_analysis()
{
    for(int i=0;i<uniqueDomains.size();i++){

        QString cmd1="sort /home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt";
        QString cmd2="uniq";

        QProcess proc1,proc2;

        proc1.setStandardOutputProcess(&proc2);
        proc2.setStandardOutputFile("/home/Shubham/OutputFolder/output.txt",QIODevice::WriteOnly);

        proc1.start(cmd1);
        proc2.start(cmd2);
        proc1.waitForFinished();
        proc2.waitForFinished();

        QFile::remove("/home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt");
        QFile::rename("/home/Shubham/OutputFolder/output.txt","/home/Shubham/OutputFolder/"+uniqueDomains.at(i)+".txt");
    }
}

void WebAnalytics::on_actionList_of_Domains_triggered()
{
    md1->display_data();
    md1->show();
}

void WebAnalytics::on_actionVisitor_Frequency_triggered()
{
    md2->display_data(uniqueDomains);
    md2->show();
}
