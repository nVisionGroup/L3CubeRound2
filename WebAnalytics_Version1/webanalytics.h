#ifndef WEBANALYTICS_H
#define WEBANALYTICS_H

#include <QMainWindow>
#include <QFileInfo>
#include "mydialog.h"

namespace Ui {
class WebAnalytics;
}

class WebAnalytics : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebAnalytics(QWidget *parent = 0);
    ~WebAnalytics();

    void start_analysis_process();
    void domain_analysis();

private slots:
    void on_actionAnalysis_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionList_of_Domains_triggered();

private:
    Ui::WebAnalytics *ui;
    QString filepath;
    QFileInfo checkfile;
    QStringList uniqueDomains;
    mydialog *md1;

};

#endif // WEBANALYTICS_H
