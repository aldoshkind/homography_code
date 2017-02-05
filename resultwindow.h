#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(QImage* img, QWidget *parent = 0);
    ~ResultWindow();



private slots:
    void on_pushButton_clicked();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
