#ifndef MYDIALOG2_H
#define MYDIALOG2_H

#include <QDialog>

namespace Ui {
class mydialog2;
}

class mydialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit mydialog2(QWidget *parent = 0);
    ~mydialog2();
    void display_data(QStringList);

private:
    Ui::mydialog2 *ui;
};

#endif // MYDIALOG2_H
