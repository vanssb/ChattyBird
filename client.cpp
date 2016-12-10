#include "client.h"
#include <QDataStream>
#include "codes.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    initialize();
}

Client::Client(QTcpSocket* socket,QObject *parent) : QObject(parent){
    this->socket = socket;
    initialize();
}

void Client::initialize(){
    blockSize = 0;
    serverPort = 1234;
    connect( this->socket, SIGNAL(connected()), this, SLOT(connected()) );
    connect( this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
    connect( this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()) );
    connect( this->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );
}

Client::~Client(){
    delete socket;
}

void Client::tryAuth(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // надо придумать коды ко всем операциям
    command = Codes::auth;
    out << (quint64)0 << command  << name << password;
    out.device()->seek(0);
//вписываем размер блока на зарезервированное место
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
}

void Client::tryConnect(QString serverIp, QString name, QString password){
    socket->connectToHost(serverIp, serverPort);
    this->name = name;
    this->password = password;
}

QString Client::errorString(){
    return errorValue;
}

bool Client::isOnline(){
    return online;
}

void Client::connected(){
    tryAuth();
}

void Client::disconnected(){
    online = false;
    emit clientDisconnected();
}

void Client::readyRead(){
    QDataStream in(socket);
    if (blockSize == 0) {
        //Ожидаем пока будет получен хотя бы размер блока
        if (socket->bytesAvailable() < (int)sizeof(quint64))
            return;
        //считываем размер блока Int
            in >> blockSize;
    }
    //ждем пока блок прийдет полностью
    if (socket->bytesAvailable() < blockSize)
        return;
    else
        //можно принимать новый блок
        blockSize = 0;
    //3 байт - комманда
    quint8 command;
    in >> command;
    switch (command){
    case Codes::auth:
        int code;
        in >> code;
        switch (code) {
        case Codes::authSuccess:
            online = true;
            emit clientConnected();
            break;
        case Codes::authProblem:
            errorValue = "Authorization problem, try again";
            emit clientAuthProblem(QAbstractSocket::SocketError());
            //disconnect();
        break;
        case Codes::authIncorrectPair:
            errorValue = "Incorrect pair, try again";
            emit clientAuthProblem(QAbstractSocket::SocketError());
            //disconnect();
        break;
        }
    break;
    case Codes::messagePublic:
        QString text;
        in >> text;
        emit clientMessage(text);
    break;
    }
}

void Client::error(QAbstractSocket::SocketError error){
    errorValue = socket->errorString();
    emit clientError(error);
}

void Client::disconnect(){
    socket->close();
    online = false;
}

void Client::sendPublicMessage(QString text){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // надо придумать коды ко всем операциям
    quint8 command;
    command = Codes::messagePublic;
    out << (quint64)0 << command  << text;
    out.device()->seek(0);
//вписываем размер блока на зарезервированное место
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
}
