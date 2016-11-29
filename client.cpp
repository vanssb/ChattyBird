#include "client.h"
#include <QDataStream>

Client::Client(QObject *parent) : QObject(parent)
{
    serverPort = 1234;

    connect( &socket, SIGNAL(connected()), this, SLOT(connected()) );
    connect( &socket, SIGNAL(disconnected()), this, SLOT(disconnected()) );
    connect( &socket, SIGNAL(readyRead()), this, SLOT(readyRead()) );
    connect( &socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );
}

void Client::tryAuth(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // надо придумать коды ко всем операциям
    command = 100;
    out << (quint16)0 << (quint8)0  << name << password;
    out.device()->seek(0);
//вписываем размер блока на зарезервированное место
    out << (quint16)(block.size() - sizeof(quint16)) << 100;
    socket.write(block);
}

void Client::tryConnect(QString serverIp, QString name, QString password){
    socket.connectToHost(serverIp, serverPort);
    this->name = name;
    this->password = password;
}

QString Client::errorString(){
    return errorValue;
}

void Client::connected(){
    tryAuth();
}

void Client::disconnected(){

}

void Client::readyRead(){
    QDataStream in(&socket);

    if (blockSize == 0) {
        //Ожидаем пока будет получен хотя бы размер блока
        if (socket.bytesAvailable() < (int)sizeof(quint16))
            return;
        //считываем размер блока (2 байта)
            in >> blockSize;
    }
    //ждем пока блок прийдет полностью
    if (socket.bytesAvailable() < blockSize)
        return;
    else
        //можно принимать новый блок
        blockSize = 0;
    //3 байт - команда серверу
    quint8 command;
    in >> command;
    switch (command){
    case 100:
        int code;
        in >> code;
        if (code == 200){
            //авторизованы
            online = true;
            emit clientConnected();
        }
        else{
            QString error;
            in >> error;
            errorValue = QString::number(code) + " : " + error;
            emit clientAuthProblem();
        }
    break;
    }
}

void Client::error(QAbstractSocket::SocketError){
    errorValue = socket.errorString();
    emit clientError();
}

void Client::clientAuthProblem(){

}
