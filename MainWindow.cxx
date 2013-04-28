#include "MainWindow.h"
/*
QByteArray MainWindow::CreateDatagramm(quint8 comm,QString message) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    return block;
}
QByteArray MainWindow::CreateDatagramm(quint8 comm) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    return block;
}

void MainWindow::doSendCommand(quint8 comm, QTcpSocket *client, QString message) const
{
    QByteArray block=CreateDatagramm(comm,message);
    client->write(block);
}
void MainWindow::doSendCommand(quint8 comm, QTcpSocket *client) const
{

}
*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QString ipAddress;

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    setupUi(this);
    blocksize=0;
    //ui->setupUi(this);
    //stackedWidget->setCurrentWidget(loginPage);
    //ui->PortLineEdit->setValidator(new QIntValidator(1, 65535, this));
    //ui->PortLineEdit->setText("32023");

    // Make sure that we are showing the login page when we startup:
    stackedWidget->setCurrentWidget(loginPage);
    serverLineEdit->setText(ipAddress);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}


void MainWindow::on_loginButton_clicked()
{
    socket->connectToHost(serverLineEdit->text(), 32023);
}

void MainWindow::on_sayButton_clicked()
{
    QByteArray block;
    QString message = sayLineEdit->text();
    if(!message.isEmpty())
    {
        if (privateMsgBox->isChecked())
        {
            QString s;
            foreach (QListWidgetItem *i, userListWidget->selectedItems())
                s += i->text()+"|";
            s.remove(s.length()-1, 1);
            qDebug() << "List of users: " << s;
            block=CreateDatagramm(comMessageToUsers,s+":"+message);
        }
        else
        {
            block=CreateDatagramm(comMessageToAll,message);
        }
        socket->write(block);
    }
    sayLineEdit->clear();
    sayLineEdit->setFocus();
    //out << ui->pteMessage->document()->toPlainText();


}

// This function gets called whenever the chat server has sent us some text:
void MainWindow::readyRead()
{
    qDebug() <<"Ready Read start";
    while (socket->bytesAvailable() >0)
    {
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
                qDebug() << "Received command comAuthorizationSuccess" ;
                stackedWidget->setCurrentWidget(chatPage);
                roomTextEdit->append("<b>" + userLineEdit->text() + "</b>: Connected succsessful");
                //ui->pbSend->setEnabled(true);
                //AddToLog("Enter as "+_name,Qt::green);d

            }
            break;

            case comUserJoin:
            {
                qDebug() << "comUserJoin" ;
                QString name;
                in >> name;
                roomTextEdit->append("<b>" + name + "</b>: Connected to our chat");
                QStringList tmp;
                tmp.push_back(name);
                userListWidget->addItems(tmp);
            }
            break;

            case comUserLeft:
            {
                qDebug() << "comUserLeft" ;
                QString name;
                in >> name;
                roomTextEdit->append("<b>" + name + "</b> was left from our chat");
                QList<QListWidgetItem*> templist=userListWidget->findItems(name,Qt::MatchFixedString);
                if (templist.count()==1)
                    userListWidget->takeItem(userListWidget->row(templist.last()));
                else qDebug() << "NOT = 1";
            }
            break;

            case comUsersOnline:
            {
                qDebug() << "comUsersOnline" ;
                QString names;
                in >> names;
                userListWidget->clear();
                QStringList temp=names.split("|");
                userListWidget->addItems(temp);
            }
            break;

            case comMessageToAll:
            {
                QString message;
                in >> message;
                roomTextEdit->append(message);
            }
            break;

            case comMessageToUsers:
            {
                QString message;
                in >> message;
                roomTextEdit->append(message);
            }
            break;
        }
    qDebug() << "Command ended, bytes avaliable" <<socket->bytesAvailable();
    }

}

// This function gets called when our socket has successfully connected to the chat
// server. (see the connect() call in the MainWindow constructor).
void MainWindow::connected()
{
    QByteArray block;

    block=CreateDatagramm(comAuthorization,userLineEdit->text());
    socket->write(block);

    qDebug() <<"Block sended!"<< block;

}
