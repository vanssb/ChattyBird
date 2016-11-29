#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
private:
    QString name;
    QString password;
    QTcpSocket socket;
    QString serverIp;
    int serverPort;
    quint16 blockSize;
    quint8 command;
    QString errorValue;
    bool serverStatus;
    bool online;
    void tryAuth();
private slots:
    void connected();
    void disconnected();
    void readyRead();
    void error(QAbstractSocket::SocketError error);
public:
    explicit Client(QObject *parent = 0);
    QString errorString();

    void tryConnect(QString serverIp = "localhost", QString name="", QString password="");
signals:
    void clientConnected();
    void clientError();
    void clientDisconnected();
    void clientMessage(QString message);
    void clientAuthProblem();
public slots:

};

#endif // CLIENT_H
