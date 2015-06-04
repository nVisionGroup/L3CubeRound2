#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>

namespace Ui {
class mydialog;
}

class mydialog : public QDialog
{
    Q_OBJECT

public:
    explicit mydialog(QWidget *parent = 0);
    ~mydialog();

    void display_data();

private:
    Ui::mydialog *ui;
};

#endif // MYDIALOG_H
