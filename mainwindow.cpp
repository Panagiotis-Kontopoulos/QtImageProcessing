#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString url = QFileDialog::getOpenFileName(this,"Open Image File","C:/Users/");

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

void MainWindow::on_actionOpen_Image_triggered()
{
    QMessageBox::information(this,"Open","Choose File to open");
    QString url2 = QFileDialog::getOpenFileName(this,"Open Image File","C:/Users/",".tiff");

}

void MainWindow::on_actionSave_Image_triggered()
{
    QMessageBox::information(this,"Save","Save File");
}

void MainWindow::on_actionEnable_Auto_Save_triggered()
{
    QMessageBox::information(this,"Auto Save","Auto Save Enabled");
}

void MainWindow::on_actionDisable_Auto_Save_triggered()
{
    QMessageBox::information(this,"Auto Save","Auto Save Disabled");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionMethod_Input_triggered()
{
    QMessageBox::information(this,"Input","Input Methods");
}

void MainWindow::on_actionMore_triggered()
{
    QMessageBox::information(this,"More","More Options");
}

void MainWindow::on_actionGasussian_Noise_triggered()
{
    QMessageBox::information(this,"Gaussian","Add Gaussian Noise");
}

void MainWindow::on_actionRandom_Noise_triggered()
{
    QMessageBox::information(this,"Random","Add Random Noise");
}

void MainWindow::on_actionAll_White_triggered()
{

}

void MainWindow::on_actionAll_Black_triggered()
{

}

void MainWindow::on_actionAll_White_2_triggered()
{

}

void MainWindow::on_actionAll_Black_2_triggered()
{

}

void MainWindow::on_actionComparison_triggered()
{

}

void MainWindow::on_actionEvaluation_triggered()
{

}

void MainWindow::on_actionAll_3x3_triggered()
{

}

void MainWindow::on_actionCross_3x3_triggered()
{

}

void MainWindow::on_actionAll_5x5_triggered()
{

}

void MainWindow::on_actionRhombus_5x5_triggered()
{

}

void MainWindow::on_actionAll_3x3_2_triggered()
{

}

void MainWindow::on_actionCross_3x3_2_triggered()
{

}

void MainWindow::on_actionAll_5x5_2_triggered()
{

}

void MainWindow::on_actionRhombus_5x5_2_triggered()
{

}

void MainWindow::on_actionSwell_Filter_triggered()
{

}

void MainWindow::on_actionShrink_Filter_triggered()
{

}

void MainWindow::on_actionUndo_triggered()
{

}

void MainWindow::on_actionRedo_triggered()
{

}
