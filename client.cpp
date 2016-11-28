#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    serverPort = 1234;
}

void Client::tryAuth(QString name, QString password){

}

void Client::tryConnect(QString serverIp){
    socket.connectToHost(serverIp, serverPort);
}

QString Client::errorString(){
    return errorValue;
}

void Client::connected(){

}

void Client::disconnected(){

}

void Client::readyRead(){

}

void Client::error(){

}
