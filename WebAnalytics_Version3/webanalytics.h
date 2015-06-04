#ifndef WEBANALYTICS_H
#define WEBANALYTICS_H

#include <QMainWindow>
#include <QFileInfo>
#include "mydialog.h"
#include "mydialog2.h"

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
    void visitor_analysis();
    void refined_visitor_analysis();

private slots:
    void on_actionAnalysis_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionList_of_Domains_triggered();

    void on_actionVisitor_Frequency_triggered();

private:
    Ui::WebAnalytics *ui;
    QString filepath;
    QFileInfo checkfile;
    QStringList uniqueDomains;
    mydialog *md1;
    mydialog2 *md2;

};

#endif // WEBANALYTICS_H
