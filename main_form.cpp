#include "main_form.h"
#include "ui_main_form.h"

main_form::main_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_form)
{
    ui->setupUi(this);

    QPalette* palette = new QPalette();
    palette->setBrush(QPalette::Background,*(new QBrush(*(new QPixmap("777.jpg")))));
    setPalette(*palette);

    QObject::connect(ui->exit_Button, SIGNAL(clicked()), this, SLOT(close()));
    board_2048 *f = new board_2048("localhost", 2323);
    connect(ui->classic_Button, SIGNAL(clicked()), f, SLOT(on_classic_Button_clicked()));


    ui->classic_Button->setStyleSheet("QPushButton { background-color: #ac33a8; color: #ffe2fe; font:15pt; font-weight:350; border-radius: 15px;}");
    ui->online_Button->setStyleSheet("QPushButton { background-color: #ac33a8; color: #ffe2fe; font:15pt; font-weight:350; border-radius: 15px;}");
    ui->time_Button->setStyleSheet("QPushButton { background-color: #ac33a8; color: #ffe2fe; font:15pt; font-weight:350; border-radius: 15px;}");
    ui->exit_Button->setStyleSheet("QPushButton { background-color: #ac33a8; color: #ffe2fe; font:15pt; font-weight:350; border-radius: 15px;}");

    setFixedSize(440, 700);
    setWindowTitle("2048");
}



void main_form::on_classic_Button_clicked()
{
    board_2048 *f1 = new board_2048("localhost", 2323);
    f1->show();
    f1->activateWindow();

    this ->hide();
}
