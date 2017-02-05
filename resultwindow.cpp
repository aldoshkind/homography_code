#include "resultwindow.h"
#include "ui_resultwindow.h"
#include <QFileDialog>

ResultWindow::ResultWindow(QImage *img, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    ui->result_label->setPixmap(QPixmap::fromImage(img->rgbSwapped()));

}

ResultWindow::~ResultWindow()
{
    delete ui;
}



void ResultWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить как", "D:/coding/Qt/homography/shots/shot1.png", "Изображения (*.png);;Изображения (*.bmp);;Изображения (*.jpg)");
    if(!filename.isEmpty())
        ui->result_label->pixmap()->save(filename);
}
