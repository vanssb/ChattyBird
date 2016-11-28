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
    QString errorValue;
    bool serverStatus;
    void tryAuth(QString name, QString password);
    void tryConnect(QString serverIp = "localhost");
private slots:
    void connected();
    void disconnected();
    void readyRead();
    void error();
public:
    explicit Client(QObject *parent = 0);
    QString errorString();
signals:
    void clientConnected();
    void clientError();
    void clientDisconnected();
    void clientReadyRead();
public slots:

};

#endif // CLIENT_H
