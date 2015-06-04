#include "webanalytics.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebAnalytics w;
    w.show();

    return a.exec();
}
