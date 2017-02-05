#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:

    void on_Open_pushButton_clicked();

    void on_Result_pushButton_clicked();

    void on_close_action_triggered();

    void on_open_action_triggered();

    void on_run_action_triggered();


private:

    Ui::MainWindow *ui;
    IplImage* img;
    IplImage* dst;
};

#endif // MAINWINDOW_H
