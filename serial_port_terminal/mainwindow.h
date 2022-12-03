#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_button_clicked();

    void on_disconnect_button_clicked();

    void receiveMessage();

    void refresh_ports();

    void on_refresh_ports_button_clicked();

    void on_clear_button_clicked();

    void on_send_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
