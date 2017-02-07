#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <QMessageBox>
#include <opencv\cv.h>
#include <resultwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = 0;
    dst = 0;
}

MainWindow::~MainWindow()
{
   // delete ui;
}

void MainWindow::on_Open_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбор файла", "D:/coding/Qt/homography/shots/shot1.png", "Изображения (*.png);;Изображения (*.bmp);;Изображения (*.jpg)");
        if(QFile(fileName).exists())
    {
        //функция перевода
        img = cvLoadImage(fileName.toLocal8Bit());

        QImage* qImg = new QImage((uchar *)img->imageData,img->width, img->height, QImage::Format_RGB888);
        ui->photo_before_rotate->setPixmap(QPixmap::fromImage(qImg->rgbSwapped()));
    }
}

void MainWindow::on_Result_pushButton_clicked()
{
    if (!img)
    {
        QMessageBox::warning(this, "Ошибка", "Выберете картинку");
        return;
    }

    double Kren = ui->Kren_doubleSpinBox->value();
    double Tangazh = -ui->Tangazh_doubleSpinBox->value();
    double Kurs = ui->Kurs_doubleSpinBox->value();
    double H = ui->H_doubleSpinBox->value();
    double Fokus = ui->Fokus_doubleSpinBox->value();;
    double Pixel = ui->Pixel_doubleSpinBox->value();;
    double X0 = ui->X0_doubleSpinBox->value();;
    double Y0 = ui->Y0_doubleSpinBox->value();;
    double Fokus_in_Pixel = Fokus / (Pixel*0.001);

    //Перевод в радианы
    Kren = qDegreesToRadians(Kren);
    Tangazh = qDegreesToRadians(Tangazh);
    Kurs = qDegreesToRadians(Kurs);

    double a1 = qCos(Kren) * qCos(Kurs) - qSin(Kren) * qSin(Tangazh) * qSin(Kurs);
    double a2 = -qCos(Kren) * qSin(Kurs) - qSin(Kren) * qSin(Tangazh) * qCos(Kurs);
    double a3 = -qSin(Kren) * qCos(Tangazh);

    double b1 = qCos(Tangazh) * qSin(Kurs);
    double b2 = qCos(Tangazh) * qCos(Kurs);
    double b3 = -qSin(Tangazh);

    double c1 = qSin(Kren) * qCos(Kurs) + qCos(Kren) * qSin(Tangazh) * qSin(Kurs);
    double c2 = -qSin(Kren) * qSin(Kurs) + qCos(Kren) * qSin(Tangazh) * qCos(Kurs);
    double c3 = qCos(Kren) * qCos(Tangazh);

    // точки
        CvPoint2D32f srcQuad[4], dstQuad[4];
    // матрица преобразования
        CvMat* warp_matrix = cvCreateMat(3,3,CV_32FC1);

    // создаем картинку
    dst = cvCreateImage(cvSize(512, 512), 8, 3);


    // задаём точки исходного снимка в осях openCv
    srcQuad[0].x = 0;           //src Top left
    srcQuad[0].y = 0;
    srcQuad[1].x = img->width - 1;  //src Top right
    srcQuad[1].y = 0;
    srcQuad[2].x = 0;           //src Bottom left
    srcQuad[2].y = img->height - 1;
    srcQuad[3].x = img->width - 1;  //src Bot right
    srcQuad[3].y = img->height - 1;
    //- - - - - - - - - - - - - -//


    //главная точка исходного снимка в осях проходящих через центр снимка
    double X0new = -Fokus_in_Pixel*(a1 * 0 + a2 * 0 - a3 * Fokus_in_Pixel)/(c1 * 0 + c2 * 0 - c3 * Fokus_in_Pixel);
    double Y0new = -Fokus_in_Pixel*(b1 * 0 + b2 * 0 - b3 * Fokus_in_Pixel)/(c1 * 0 + c2 * 0 - c3 * Fokus_in_Pixel);

    double popravkaX = (dst->width/2) - X0new;
    double popravkaY = -(dst->height/2) - Y0new;

    dstQuad[0].x = (-Fokus_in_Pixel*(a1 * -img->width/2 + a2 * img->height/2 - a3 * Fokus_in_Pixel)/(c1 * -img->width/2 + c2 *img->height/2  - c3 * Fokus_in_Pixel)) + popravkaX;  //dst Top left
    dstQuad[0].y = -(-Fokus_in_Pixel*(b1 * -img->width/2 + b2 * img->height/2 - b3 * Fokus_in_Pixel)/(c1 * -img->width/2 + c2 * img->height/2  - c3 * Fokus_in_Pixel)) - popravkaY;
    dstQuad[1].x = (-Fokus_in_Pixel*(a1 * img->width/2 + a2 * img->height/2 - a3 * Fokus_in_Pixel)/(c1 * img->width/2 + c2 *img->height/2  - c3 * Fokus_in_Pixel)) + popravkaX;  //dst Top right
    dstQuad[1].y = -(-Fokus_in_Pixel*(b1 * img->width/2 + b2 * img->height/2 - b3 * Fokus_in_Pixel)/(c1 * img->width/2 + c2 * img->height/2  - c3 * Fokus_in_Pixel)) - popravkaY;
    dstQuad[2].x = (-Fokus_in_Pixel*(a1 * -img->width/2 + a2 * -img->height/2 - a3 * Fokus_in_Pixel)/(c1 * -img->width/2 + c2 * -img->height/2  - c3 * Fokus_in_Pixel)) + popravkaX;  //dst Bottom left
    dstQuad[2].y = -(-Fokus_in_Pixel*(b1 * -img->width/2 + b2 * -img->height/2 - b3 * Fokus_in_Pixel)/(c1 * -img->width/2 + c2 * -img->height/2  - c3 * Fokus_in_Pixel)) - popravkaY;
    dstQuad[3].x = (-Fokus_in_Pixel*(a1 * img->width/2 + a2 * -img->height/2 - a3 * Fokus_in_Pixel)/(c1 * img->width/2 + c2 *-img->height/2  - c3 * Fokus_in_Pixel)) + popravkaX;  //dst Bot right
    dstQuad[3].y = -(-Fokus_in_Pixel*(b1 * img->width/2 + b2 * -img->height/2 - b3 * Fokus_in_Pixel)/(c1 * img->width/2 + c2 *-img->height/2  - c3 * Fokus_in_Pixel)) - popravkaY;


    //вычисляем средний оттенок снимка
    CvScalar fillval = cvAvg(img);


    // получаем матрицу преобразования
    cvGetPerspectiveTransform(srcQuad,dstQuad,warp_matrix);
    // преобразование перспективы
    cvWarpPerspective(img,dst,warp_matrix, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, fillval);


    // освобождаем ресурсы
    cvReleaseMat(&warp_matrix);

    QImage* qImg2 = new QImage((uchar *)dst->imageData,dst->width, dst->height, QImage::Format_RGB888);
    ResultWindow *reswnd = new ResultWindow(qImg2, this);
    reswnd->show();
}


void MainWindow::on_close_action_triggered()
{
    close();
}

void MainWindow::on_open_action_triggered()
{
    on_Open_pushButton_clicked();
}

void MainWindow::on_run_action_triggered()
{
    on_Result_pushButton_clicked();
}


