#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QBitMap>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    connect( ui->signInButton, SIGNAL( clicked(bool) ), this, SLOT( signIn() ) );
    connect( ui->quitButton, SIGNAL( clicked(bool) ), this, SLOT( close( )) );
    connect( ui->send, SIGNAL( clicked(bool) ), this, SLOT( sendMessage() ) );
    connect( ui->signInTransfer, SIGNAL( clicked(bool) ), this, SLOT( transferToSignIn() ) );
    connect( ui->signUpTransfer, SIGNAL( clicked(bool) ), this, SLOT( transferToSignUp() ) );
    connect( ui->signUpButton, SIGNAL(clicked(bool)), this, SLOT(signUp()) );

    connect( &client, SIGNAL( clientMessage(QString) ), this, SLOT(newMessage(QString)) );
    connect( &client, SIGNAL( clientConnected() ), this, SLOT( connected() ) );
    connect( &client, SIGNAL( clientDisconnected() ), this, SLOT( disconnected() ) );
    connect( &client, SIGNAL( clientAuthProblem() ), this, SLOT( errorSignIn() ) );
    connect( &client, SIGNAL( clientError() ), this, SLOT( errorSignIn() ) );
    connect( &client, SIGNAL( clientSignUpError() ), this, SLOT( errorSignUp() ) );
    connect( &client, SIGNAL( clientSignUpSuccess() ), this, SLOT( signedUp() ) );



    ui->stackedWidget->setCurrentWidget(ui->signIn);
    ui->friends->setVisible(false);
    ui->errorLine->setVisible(false);
    ui->loginEdit->setPlaceholderText("Login");
    ui->passwordEdit->setPlaceholderText("Password");
    ui->nicknameEditSignUp->setPlaceholderText("Nickname");
    ui->loginEditSignUp->setPlaceholderText("Login");
    ui->passwordEditSignUp->setPlaceholderText("Password");

    client.setHost();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected(){
    ui->messages->clear();
    ui->message->clear();
    ui->stackedWidget->setCurrentWidget(ui->chat);
}

void MainWindow::disconnected(){
    ui->stackedWidget->setCurrentWidget(ui->signIn);
    ui->errorLine->setVisible(true);
    ui->errorLine->setText(client.errorString());
}

void MainWindow::errorSignIn(){
    ui->errorLine->setVisible(true);
    ui->errorLine->setText(client.errorString());
}

void MainWindow::errorSignUp(){
    ui->errorLineSignUp->setVisible(true);
    ui->errorLineSignUp->setText(client.errorString());
}

void MainWindow::errorChat(){

}

void MainWindow::signIn(){
    client.trySignIn(ui->loginEdit->text(), ui->passwordEdit->text());
}

void MainWindow::signUp(){
    client.trySignUp(ui->loginEditSignUp->text(), ui->passwordEditSignUp->text(), ui->nicknameEditSignUp->text());
}

void MainWindow::signedUp(){
    ui->stackedWidget->setCurrentWidget(ui->signIn);
}

void MainWindow::sendMessage(){
    QString text = ui->message->text();
    client.sendPublicMessage(text);
    ui->message->clear();
}

void MainWindow::newMessage(QString text){
    ui->messages->append("[" + QTime::currentTime().toString("hh:mm:ss") + "] " + text);
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

void MainWindow::transferToSignIn(){
    ui->stackedWidget->setCurrentWidget(ui->signIn);
}

void MainWindow::transferToSignUp(){
    ui->stackedWidget->setCurrentWidget(ui->signUp);
}


