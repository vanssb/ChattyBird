#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QBitMap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->errorLine->setVisible(false);

    connect(ui->signInButton, SIGNAL(clicked(bool)), this, SLOT(signIn()) );
    connect( ui->quitButton, SIGNAL(clicked(bool)), this, SLOT(close()) );

    connect( &client, SIGNAL(clientConnected()), this, SLOT(connected()) );
    connect( &client, SIGNAL(clientAuthProblem(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );
    connect( &client, SIGNAL(clientError(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected(){

}

void MainWindow::disconnected(){
    ui->errorLine->setVisible(true);
    ui->errorLine->setText(client.errorString());
}

void MainWindow::error(QAbstractSocket::SocketError){
    ui->errorLine->setVisible(true);
    ui->errorLine->setText(client.errorString());
}

void MainWindow::signIn(){
    client.tryConnect("localhost",ui->loginEdit->text(), ui->passwordEdit->text());
}

void MainWindow::mouseMoveEvent( QMouseEvent* e ) {
    if( e->buttons() | Qt::LeftButton ) {
        setGeometry(
            pos().x() + ( e->x() - dx ),
            pos().y() + ( e->y() - dy ),
            width(),
            height()
        );
    }
}

void MainWindow::mousePressEvent( QMouseEvent* e ) {
    if( e->button() == Qt::LeftButton ) {
        dx = e->x();
        dy = e->y();
        setCursor( Qt::OpenHandCursor );
    }
}

void MainWindow::mouseReleaseEvent( QMouseEvent* e ) {
    if( e->button() == Qt::LeftButton ) {
        setCursor( Qt::ArrowCursor );
    }
}
