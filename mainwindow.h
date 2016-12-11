#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <client.h>
#include "codes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Client client;
    int dx,dy;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void connected();
    void disconnected();
    void errorSignIn();
    void errorSignUp();
    void errorChat();
    void signIn();
    void signUp();
    void signedUp();
    void sendMessage();
    void newMessage(QString text);
    void transferToSignIn();
    void transferToSignUp();
    void mouseMoveEvent( QMouseEvent* e );
    void mousePressEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );
};

#endif // MAINWINDOW_H
