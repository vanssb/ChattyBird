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
    connect( this->socket, SIGNAL(connected()), this, SLOT(connected()) );
    connect( this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
    connect( this->socket, SIGNAL(readyRead()), this, SLOT(readyRead()) );
    connect( this->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );
}

Client::~Client(){
    delete socket;
}

void Client::authorize(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // надо придумать коды ко всем операциям
    command = Codes::authRequest;
    out << (quint64)0 << command  << name << password;
    out.device()->seek(0);
//вписываем размер блока на зарезервированное место
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
}

void Client::registrate(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // надо придумать коды ко всем операциям
    command = Codes::signUpRequest;
    out << (quint64)0 << command  << name << password << nickname;
    out.device()->seek(0);
//вписываем размер блока на зарезервированное место
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
}

void Client::trySignIn(QString name, QString password){
    socket->close();
    socket->connectToHost(serverIp, serverPort);
    this->name = name;
    this->password = password;
    command = Codes::authRequest;
}

void Client::trySignUp(QString name, QString password, QString nickname){
    socket->close();
    socket->connectToHost(serverIp, serverPort);
    this->name = name;
    this->password = password;
    this->nickname = nickname;
    command = Codes::signUpRequest;
}

QString Client::errorString(){
    return errorValue;
}

bool Client::isOnline(){
    return online;
}

void Client::connected(){
    switch (command) {
    case Codes::authRequest:
        authorize();
    break;
    case Codes::signUpRequest:
        registrate();
    break;
    }
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
    parseData(command, in);
}

void Client::error(QAbstractSocket::SocketError error){
    errorValue = socket->errorString();
    emit clientError();
}

void Client::disconnect(){
    socket->close();
    online = false;
    emit clientDisconnected();
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

void Client::parseData(quint8 command, QDataStream &in){
    switch (command){
    case Codes::authSuccess:
        online = true;
        emit clientConnected();
    break;
    case Codes::authProblem:
        errorValue = "Authorization problem, try again";
        emit clientAuthProblem();
        socket->close();
    break;
    case Codes::authIncorrectPair:
        errorValue = "Incorrect pair, try again";
        emit clientAuthProblem();
        socket->close();
    break;
    case Codes::authAlreadyOnline:
        errorValue = "User is already online";
        emit clientAuthProblem();
        socket->close();
    break;
    case Codes::messagePublic:{
        QString sender;
        in >> sender;
        QString text;
        in >> text;
        text = sender + ": "+text;
        emit clientMessage( text );
    break;
    }
    case Codes::signUpSuccess:
        emit clientSignUpSuccess();
    break;
    case Codes::signUpProblem:
        errorValue = "Some problem via registration";
        emit clientSignUpError();
    break;
    case Codes::signUpNameNotVacant:
        errorValue = "Name or nickname are not vacant";
        emit clientSignUpError();
    break;
    }
}

void Client::setHost(QString hostIp, int hostPort){
    serverIp = hostIp;
    serverPort = hostPort;
}
