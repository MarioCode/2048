#ifndef BOARD_2048_H
#define BOARD_2048_H

#include "main_form.h"


#include <QMainWindow>
#include <QLabel>
#include <QImage>
#include <QPalette>
#include <QDialog>
#include <QKeyEvent>
#include <QString.h>
#include <QFile>
#include <QByteArray>
#include <QPushButton>
#include <QPainter>
#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QPixmap>
#include <QByteArray>
#include <QUdpSocket>
#include <QTime>
#include <QTcpSocket>


namespace Ui {
class board_2048;
}

class board_2048 : public QMainWindow
{
    Q_OBJECT

public:

    explicit board_2048(const QString &strHost, int nPort, QWidget *parent = 0);
    void readFile();
    void saveFile();

private:

    QTcpSocket *TcpSocket;
    quint16 NextBlockSize;

    Ui::board_2048 *ui;

    QLabel *labels[4][4];  //Плитки
    QTimer *timer;
    QFile *file;           //Загрузка лучшего результата
    QString *Arr[4][4];     //Плитки -> Строки

    void random_Tiles();                        //Генерация случайной плитки
    void change_Image(int i, int j);            //Смена изображения плиток
    int Score;                                  //Текущий результат
    void keyPressEvent(QKeyEvent * event);


    int Best;                   //Лучший результат(из файла)
    bool gameEnded();           //Проверка на завершение игры
    int countEmpty() const;     //Проверка на свободные ходы
    bool winGame();
    bool findPairDown();                //Наличие одинаковых соседних плиток
    void transpose();                   //Транспонирование матрицы
    void Initialization();              //Инициализация поля
    void save_boar_on_string_array();   //Сохранение состояние поля до сдвига
    void update_boar_after_moved();     //Обновление поля после сдвига

    QUdpSocket *socket;
 private slots:

    void slotSendDatagram();

    void NewGameSlot();   //Новая игра

    //Перемещение плиток
    void scroll_Down();
    void scroll_Up();
    void scroll_Left();
    void scroll_Right(); 

    //Переход в начальное меню
    void on_back_Burron_clicked();

    //Перемещение с помощью QPushButton
    void on_right_But_clicked();
    void on_up_But_clicked();
    void on_down_But_clicked();
    void on_left_But_clicked();

    //Сеть
    void slotReadyRead ();
    void slotError (QAbstractSocket::SocketError);
    void slotSendToServer ();
};

#endif // BOARD_2048_H
