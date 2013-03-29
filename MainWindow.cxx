#include "MainWindow.h"

// We'll need some regular expression magic in this code:
#include <QRegExp>

// This is our MainWindow constructor (you C++ n00b)
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QString ipAddress;

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    setupUi(this);
    blocksize=0;
    //ui->setupUi(this);
    //stackedWidget->setCurrentWidget(loginPage);
    //Wi


    //ui->PortLineEdit->setValidator(new QIntValidator(1, 65535, this));
    //ui->PortLineEdit->setText("32023");

    // Make sure that we are showing the login page when we startup:
    stackedWidget->setCurrentWidget(loginPage);
    serverLineEdit->setText(ipAddress);

    // Instantiate our socket (but don't actually connect to anything
    // yet until the user clicks the loginButton:
    socket = new QTcpSocket(this);

    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}


void MainWindow::on_loginButton_clicked()
{
    socket->connectToHost(serverLineEdit->text(), 32023);
}
/*
void MainWindow::on_sayButton_clicked()
{
    QString message = sayLineEdit->text().trimmed();
    if(!message.isEmpty())
    {
        socket->write(QString(message + "\n").toUtf8());
    }
    sayLineEdit->clear();
    sayLineEdit->setFocus();
}
*/
// This function gets called whenever the chat server has sent us some text:
void MainWindow::readyRead()
{
    qDebug() <<"Ready Read start";
    qDebug() << "blocksize now " << blocksize<<", "<<socket->bytesAvailable();
    QDataStream in(socket);
    if (blocksize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blocksize;
        qDebug() << "blocksize now " << blocksize;

    }
    if (socket->bytesAvailable() < blocksize)
        return;
    else
        blocksize = 0;

    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case comAuthorizationSuccess:
        {
            stackedWidget->setCurrentWidget(chatPage);
            //ui->pbSend->setEnabled(true);
            //AddToLog("Enter as "+_name,Qt::green);d
        }
        break;

    }// We'll loop over every (complete) line of text that the server has sent us:

}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    //try autch
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)MainWindow::comAuthorization;
    out << userLineEdit->text();

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    qDebug() <<"Block sended!"<< block;
    // Flip over to the chat page:


    // And send our username to the chat server.
    //socket->write(QString("/me:" + userLineEdit->text() + "\n").toUtf8());
}
