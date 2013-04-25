// We need to include a couple Qt classes that we'll use:
#include <QMainWindow>
#include <QTcpSocket>
#include<QWidget>
#include <QtNetwork>

// This is the include file that Qt generates for us from the
// GUI we built in Designer  
#include "ui_MainWindow.h"

/*
 * This is the MainWindow class that we have told to inherit from
 * our Designer MainWindow (ui::MainWindow)
 */
class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

    public:

        // Every QWidget needs a constructor, and they should allow
        // the user to pass a parent QWidget (or not).
        MainWindow(QWidget *parent=0);
        static const quint8 comAuthorization = 1;
        static const quint8 comAuthorizationSuccess = 2;

        static const quint8 comUserJoin = 3;
        static const quint8 comUserLeft = 4;
        static const quint8 comUsersOnline = 5;

        static const quint8 comMessageToAll = 6;
        static const quint8 comMessageToUsers = 7;

        static const quint8 comErrNameInvalid = 201;
        static const quint8 comErrNameUsed = 202;

    private slots:

        void on_loginButton_clicked();
        void on_sayButton_clicked();

        // This is a function we'll connect to a socket's readyRead()
        // signal, which tells us there's text to be read from the chat
        // server.
        void readyRead();
        void connected();

    private:
        // This is the socket that will let us communitate with the server.
        QTcpSocket *socket;
        quint16 blocksize;
    protected:

        QByteArray CreateDatagramm(quint8 comm,QString message) const;
        QByteArray CreateDatagramm(quint8 comm) const;

        void doSendCommand(quint8 comm, QTcpSocket *client,QString message) const;
        void doSendCommand(quint8 comm, QTcpSocket *client) const;


};
