#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>
#include "board_2048.h"

namespace Ui {
class main_form;
}

class main_form : public QWidget
{
    Q_OBJECT

public:
    explicit main_form(QWidget *parent = 0);

signals:
    void sendData(QString str);

private slots:

    void on_classic_Button_clicked();

private:
    Ui::main_form *ui;
};

#endif // MAIN_FORM_H
