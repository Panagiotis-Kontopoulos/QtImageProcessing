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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString file_url = QFileDialog::getOpenFileName(this,"Open Image File",QDir::currentPath(),"*.tiff");
    QFileInfo fileInfo(file_url);
    file_path = fileInfo.path();
    file_name = fileInfo.fileName();
    QMessageBox::information(this,file_name,file_name);
    QMessageBox::information(this,file_path,file_path);
    imageObject = new QImage;
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *imageObject = imageReader.read();
    if (!imageObject->isNull())
    {
        image = QPixmap::fromImage(*imageObject);

        int w = ui->image_label->width();
        int h = ui->image_label->height();

        std::cout<<w;
        std::cout<<h;
        ui->image_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
    }

}

void MainWindow::on_actionSave_Image_triggered()
{
    QMessageBox::information(this,"Save","Save File to "+file_path);
    imageObject->save(file_path+"/saved_"+file_name);

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
