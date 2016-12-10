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
    QTcpSocket* socket;
    QString serverIp;
    int serverPort;
    quint64 blockSize;
    quint8 command;
    QString errorValue;
    bool serverStatus;
    bool online;
    void tryAuth();
    void initialize();
private slots:
    void connected();
    void disconnected();
    void readyRead();
    void error(QAbstractSocket::SocketError error);
public:
    explicit Client(QObject *parent = 0);
    explicit Client(QTcpSocket* socket, QObject *parent = 0);
    ~Client();
    QString errorString();
    bool isOnline();
    void tryConnect(QString serverIp = "localhost", QString name="", QString password="");
    void disconnect();
    void sendPublicMessage(QString text);
signals:
    void clientConnected();
    void clientError(QAbstractSocket::SocketError error);
    void clientDisconnected();
    void clientMessage(QString message);
    void clientAuthProblem(QAbstractSocket::SocketError);
};

#endif // CLIENT_H
