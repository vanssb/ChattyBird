#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
private:
    QString name, nickname, password;
    QTcpSocket* socket;
    QString serverIp;
    int serverPort;
    quint64 blockSize;
    quint8 command;
    QString errorValue;
    bool serverStatus;
    bool online;
    void authorize();
    void registrate();
    void initialize();
    void parseData(quint8 command, QDataStream &in);
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
    void trySignIn(QString name = "", QString password = "");
    void trySignUp(QString name = "", QString password="", QString nickname = "");
    void setHost(QString hostIp = "localhost", int hostPort = 1234);
    void disconnect();
    void sendPublicMessage(QString text);
signals:
    void clientConnected();
    void clientError();
    void clientDisconnected();
    void clientMessage(QString message);
    void clientAuthProblem();
    void clientSignUpSuccess();
    void clientSignUpError();
};

#endif // CLIENT_H
