#include "board_2048.h"
#include "ui_board_2048.h"


board_2048::board_2048(const QString &strHost, int nPort, QWidget *parent) : QMainWindow(parent), ui(new Ui::board_2048)
{
    ui->setupUi(this);

    file = new QFile("best.txt");
    file->open(QIODevice::ReadOnly);
    QString strBest1 = file->readAll();

    Score = 0;
    Best = strBest1.toInt();

    TcpSocket = new QTcpSocket(this);

    TcpSocket->connectToHost(strHost, nPort);
    connect(TcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));


    socket = new QUdpSocket(this);
    QTimer *timer = new QTimer(this);
    timer->setInterval(500);
    timer->start();
    connect(timer, SIGNAL(timeout()), SLOT(slotSendToServer()));


    QObject::connect(ui->newGame, SIGNAL(clicked()), this, SLOT(NewGameSlot()));
    QObject::connect(ui->right_But, SIGNAL(clicked()), this, SLOT(But_clicked(str)));
    QObject::connect(ui->left_But, SIGNAL(clicked()), this, SLOT(But_clicked()));
    QObject::connect(ui->up_But, SIGNAL(clicked()), this, SLOT(But_clicked()));
    QObject::connect(ui->down_But, SIGNAL(clicked()), this, SLOT(But_clicked()));

    Initialization();
    random_Tiles();
    this->setFocus();
    setFixedSize(440, 700);
    setWindowTitle("2048");

}

void board_2048::Initialization()
{
    //Установка фона
    QPalette* palette = new QPalette();
    palette->setBrush(QPalette::Background,*(new QBrush(*(new QPixmap("21_2.jpg")))));
    setPalette(*palette);

    //Ui -> Labels
    labels[0][0] = ui->label;
    labels[0][1] = ui->label_2;
    labels[0][2] = ui->label_3;
    labels[0][3] = ui->label_4;
    labels[1][0] = ui->label_5;
    labels[1][1] = ui->label_6;
    labels[1][2] = ui->label_7;
    labels[1][3] = ui->label_8;
    labels[2][0] = ui->label_9;
    labels[2][1] = ui->label_10;
    labels[2][2] = ui->label_11;
    labels[2][3] = ui->label_12;
    labels[3][0] = ui->label_13;
    labels[3][1] = ui->label_14;
    labels[3][2] = ui->label_15;
    labels[3][3] = ui->label_16;

    for (int i = 0; i < 4; ++i)
       for (int j = 0; j < 4; ++j)
          {
              labels[i][j]->setText("");
              labels[i][j]->setAlignment(Qt::AlignCenter);
              change_Image(i, j);
          }

    //Загрузка изображения кнопок управления
    QPixmap image(".//45.jpg");
    QPixmap inr(".//456.jpg");
    QPixmap inup(".//535.jpg");
    QPixmap indow(".//99.jpg");

    //Установки кнопок, лэйблов
     ui->left_But->setIcon(image);
     ui->left_But->setIconSize(image.size());
     ui->right_But->setIcon(inr);
     ui->right_But->setIconSize(inr.size());
     ui->down_But->setIcon(indow);
     ui->down_But->setIconSize(indow.size());
     ui->up_But->setIcon(inup);
     ui->up_But->setIconSize(inup.size());

     ui->bestScore->setText("Best: \n" + QString::number(Best));
     ui->bestScore->setStyleSheet("QLabel { background-color: #cda1b5; color: black; font:20pt; font-weight:400; border-radius: 10px; border-color: red;}");
     ui->bestScore->setAlignment(Qt::AlignCenter);

     ui->onlinelab->setStyleSheet("QLabel { background-color: #cda1b5; color: black; font:20pt; font-weight:400; border-radius: 10px; border-color: red;}");
     ui->onlinelab->hide();

     ui->curScore->setText("Score: \n" + QString::number(Score));
     ui->curScore->setStyleSheet("QLabel { background-color: #cda1b5; color: black; font:20pt; font-weight:400; border-radius: 10px; border-color: red;}");
     ui->curScore->setAlignment(Qt::AlignCenter);
}

void board_2048::slotSendDatagram()
{
    /*for UDP
     * QByteArray dataGram;
    QDataStream out(&dataGram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);
    QString str;
    QString way;
    bool q = gameEnded();
    if(q) way = "1";
    else  way = "0";

    bool win = winGame();
    if(win) str = "1";
    else    str = "0";

    QTime dt = QTime::currentTime();


    out << str << way << dt;
    socket->writeDatagram(dataGram, QHostAddress::LocalHost, 2424);
    */

}

void board_2048::readFile()
{

    if(file->open(QIODevice::ReadOnly))
    {
       QString str = file->readAll();
       ui->bestScore->setText(str);
    }
}

void board_2048::saveFile()
{

    QTextStream out(file);

   if(file){
    if(Score > Best)
   {
       ui->bestScore->setText("Best: \n" + QString::number(Score));

       file->open(QIODevice::WriteOnly | QIODevice::Truncate);
       file->open(QIODevice::Append);
       out << QString::number(Score);
       file->close();
   }}
}

void board_2048::change_Image(int i, int j) {

    switch (labels[i][j]->text().toInt()) {
        case 0:
            labels[i][j]->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 10px;}");
            break;
        case 2:
            labels[i][j]->setStyleSheet("QLabel { background-color: #cda1b5; color: black; font:20pt; font-weight:400; border-radius: 10px; border-color: red;}");
            break;
        case 4:
            labels[i][j]->setStyleSheet("QLabel { background-color: #eddfc4; color: black; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 8:
            labels[i][j]->setStyleSheet("QLabel { background-color: #f4b17a; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 16:
            labels[i][j]->setStyleSheet("QLabel { background-color: #f79663; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 32:
            labels[i][j]->setStyleSheet("QLabel { background-color: #f67d62; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 64:
            labels[i][j]->setStyleSheet("QLabel { background-color: #f65e39; color: white; font:20pt; font-weight:400; border-radius: 10px;}");
            break;
        case 128:
            labels[i][j]->setStyleSheet("QLabel { background-color: #edce73; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 256:
            labels[i][j]->setStyleSheet("QLabel { background-color: #e9cf58; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 512:
            labels[i][j]->setStyleSheet("QLabel { background-color: #edc651; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 1024:
            labels[i][j]->setStyleSheet("QLabel { background-color: #eec744; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
        case 2048:
            labels[i][j]->setStyleSheet("QLabel { background-color: #edca64; color: white; font:20pt; font-weight:400;border-radius: 10px;}");
            break;
    }
}

//Поиск соседних одинаковых плиток
bool board_2048::findPairDown()
{
    bool success = false;
    int x, y;
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 3; y++) {
            if (labels[x][y]->text() == labels[x][y+1]->text()) {
                return true;
            }
        }
    }
    return success;
}

//Поиск свободных ходов
int board_2048::countEmpty() const
{
    int x, y;
    int count = 0;
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            if (labels[x][y]->text() == "") {
                count++;
            }
        }
    }
    return count;
}

bool board_2048::winGame()
{
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            if (labels[x][y]->text() == "2048")
            return true;
    return false;
}

bool board_2048::gameEnded() {
    bool ended = true;
    if(winGame())
    {
         ui->curScore->setText("WIN");
         return ended;
    }
    if (countEmpty() > 0) {
        return false;
    }
    if (findPairDown()) {
        return false;
    }

    transpose();
    if (findPairDown()) {
        ended = false;
    }
    transpose();
    transpose();
    transpose();
    if(ended==true)
    ui->curScore->setText("END");
    return ended;
}


void board_2048::NewGameSlot()
{
    for (int i = 0; i < 4; ++i)
          for (int j = 0; j < 4; ++j)
          {
              labels[i][j]->setText("");
              change_Image(i, j);
          }

Score = 0;
ui->curScore->setText(QString::number(Score));
random_Tiles();

      ui->newGame->clearFocus();
      ui->left_But->clearFocus();
      ui->right_But->clearFocus();
      ui->up_But->clearFocus();
      ui->down_But->clearFocus();
}



void board_2048::keyPressEvent(QKeyEvent * event) {

    QString* ArrayNumber[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            ArrayNumber[i][j] = new QString(labels[i][j]->text());

    if(gameEnded())
        return;


    switch (event->key()) {

            case Qt::Key_Up:
               scroll_Up();
               break;
           case Qt::Key_Down:
               scroll_Down();
               break;
           case Qt::Key_Left:
              scroll_Left();
               break;
           case Qt::Key_Right:
               scroll_Right();
               break;

        default:
          QWidget::keyPressEvent(event);
    }
    bool flag = false;
           for (int i = 0; i < 4; ++i)
               for (int j = 0; j < 4; ++j)
                   if (ArrayNumber[i][j] != labels[i][j]->text())
                       flag = true;
           if (flag)
               random_Tiles();
           saveFile();

}

void board_2048::random_Tiles() {


    int row = 0, column = 0;
    do {
        row = rand() % 4;
        column = rand() % 4;
    } while (labels[row][column]->text() != "");
    labels[row][column]->setText(QString::number(labels[row][column]->text().toInt() + 2));
    change_Image(row,column);
}

void board_2048::scroll_Down()
{
    for (int j = 0; j < 4; ++j)
        for (int i = 3; i >= 0; --i)
            if (labels[i][j]->text() == "")
                for (int k = i-1; k >= 0; --k)
                    if (labels[k][j]->text() != "") {
                        labels[i][j]->setText(labels[k][j]->text());
                        change_Image(i,j);
                        labels[k][j]->setText("");
                        change_Image(k,j);
                        break;
                    }
    int k;
    for (int j = 0; j < 4; ++j) {
        for (int i = 2; i >= 0; i--) {
            if (labels[i][j]->text() == labels[i+1][j]->text() && labels[i][j]->text() != "") {
                    labels[i+1][j]->setText(QString::number(labels[i+1][j]->text().toInt() * 2));
                    Score += labels[i+1][j]->text().toInt();
                    ui->curScore->setText("Score: \n" + QString::number(Score));
                    change_Image(i+1, j);
                    k = i;
                    while (k > 0) {
                        k--;
                        labels[k+1][j]->setText(labels[k][j]->text());
                        change_Image(k+1, j);
                    }
                    if (k == 0) {
                        labels[0][j]->setText("");
                        change_Image(0, j);
                    }
            }
        }
    }
 }


void board_2048::scroll_Left() {
    transpose();
    scroll_Down();
    transpose();
    transpose();
    transpose();
}

void board_2048::scroll_Right() {
    transpose();
    transpose();
    transpose();
    scroll_Down();
    transpose();
}

void board_2048::scroll_Up() {
    transpose();
    transpose();
    scroll_Down();
    transpose();
    transpose();
}

void board_2048::transpose() {
    int a[4][4];
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            a[4-j-1][i] = labels[i][j]->text().toInt();


    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            if (a[i][j] == 0) {
                labels[i][j]->setText("");
            } else {
                labels[i][j]->setText(QString::number(a[i][j]));
            }
            change_Image(i, j);
        }
}

void board_2048::on_back_Burron_clicked()
{
    main_form *f1 = new main_form;
    f1->show();
    this ->setVisible(false);
}

void board_2048::save_boar_on_string_array()
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            Arr[i][j] = new QString(labels[i][j]->text());
}

void board_2048::update_boar_after_moved()
{
    bool flag = false;
           for (int i = 0; i < 4; ++i)
               for (int j = 0; j < 4; ++j)
                   if (Arr[i][j] != labels[i][j]->text())
                       flag = true;
           if (flag)
               random_Tiles();
           saveFile();
}

void board_2048::on_right_But_clicked()
{
   save_boar_on_string_array();
   scroll_Right();
   update_boar_after_moved();
   ui->right_But->clearFocus();
}

void board_2048::on_up_But_clicked()
{

   save_boar_on_string_array();
   scroll_Up();
   update_boar_after_moved();
   ui->up_But->clearFocus();
}

void board_2048::on_down_But_clicked()
{
    save_boar_on_string_array();
    scroll_Down();
    update_boar_after_moved();
    ui->down_But->clearFocus();
}

void board_2048::on_left_But_clicked()
{
    save_boar_on_string_array();
    scroll_Left();
    update_boar_after_moved();
    ui->left_But->clearFocus();
}

//TCP
void board_2048::slotReadyRead()
{
    QDataStream in(TcpSocket);
    in.setVersion(QDataStream::Qt_5_4);

    for(;;)
    {
        if(!NextBlockSize){
            if(TcpSocket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> NextBlockSize;
        }

        if(TcpSocket->bytesAvailable() < NextBlockSize)
        {
            break;
        }

    QString cur, best;
    in >> cur >> best;
    NextBlockSize = 0;
    }
}


void board_2048::slotError(QAbstractSocket::SocketError err)
{
    QString strError = "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                                    "The host was not found" :
                                    err == QAbstractSocket::RemoteHostClosedError ?
                                     "The remote host is closed" :
                                    err == QAbstractSocket::ConnectionRefusedError ?
                                     "The connection was refused" :
                                    QString(TcpSocket->errorString())
                                     );
}


void board_2048::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    out << quint16(0) << ui->curScore->text() << ui->bestScore->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    TcpSocket->write(arrBlock);
}



