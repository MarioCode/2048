#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QLabel>


class MyClient : public QWidget
{
    Q_OBJECT
private:


public:
    MyClient(const QString &strHost, int nPort, QWidget *parent = 0);

private slots:


};

#endif // MYCLIENT_H
