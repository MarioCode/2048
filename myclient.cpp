#include "myclient.h"
#include "board_2048.h"

MyClient::MyClient(const QString &strHost, int nPort, QWidget *parent)
    : QWidget(parent)
{



    txtInfo = new QTextEdit;
    txtInput = new QLineEdit;
    txtInfo->setReadOnly(true);

    QPushButton *pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServer(QString *cursc, QString *bestsc)));
    connect(txtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer(QString *cursc, QString *bestsc)));


}

