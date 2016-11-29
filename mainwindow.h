#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <client.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Client client;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void connected();
    void disconnected();
};

#endif // MAINWINDOW_H
