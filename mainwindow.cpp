#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDir>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString url = "C:/Users/John/Documents/qtimageprocessing/Images/H01.tiff";

    QPixmap img;
    QImageReader r(url);
    r.setDecideFormatFromContent(true);
    QImage i = r.read();
    if (!i.isNull())
    {
        img = QPixmap::fromImage(i);

        int w = ui->image_label->width();
        int h = ui->image_label->height();

        std::cout<<w;
        std::cout<<h;
        ui->image_label->setPixmap(img.scaled(w,h,Qt::KeepAspectRatio));


    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
