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
#include <QGraphicsView>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>

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
    QString file_url = QFileDialog::getOpenFileName(this,tr("Open Image File"),QDir::currentPath(),tr("*.tiff"));
    QFileInfo fileInfo(file_url);
    file_path = fileInfo.path();
    file_name = fileInfo.fileName();
    QMessageBox::information(this,file_name,file_name);
    QMessageBox::information(this,file_path,file_path);
    QMessageBox::information(this,file_url,file_url);
    imageObject = new QImage;
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *imageObject = imageReader.read();
    image = QPixmap::fromImage(*imageObject);


    Ix = image.width();
    Iy = image.height();

    std::cout<<Ix;
    std::cout<<Iy;
    //ui->image_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));


    //std::cout<<imageObject->isGrayscale();
    for (int x=1;x<Ix-1;x++)
        for (int y=1;y<Iy-1;y++)
        {
            int current_pixel_color = imageObject->pixelIndex(x,y);
            std::cout<<current_pixel_color;
            if (current_pixel_color>1)
            {
                std::cout<<"Greater than 1";
            }
        }
    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    //ui->graphicsView->repaint();

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
    deviation = 1;
    mean = 2;
    std::cout<<"Adding Noise";
    //Add Gaussian Noise to the Image
    float pi = 3.14159265359;
    float e = 2.718;
    float prob = 0.0, min=1000,max=0;
    float** temp=new float*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new float[Iy];
    // Form7->ShowModal();

    // ImagXpress7_1->SaveToBuffer = true;
    // ImagXpress7_1->SaveFileType =  FT_TIFF;
    // ImagXpress7_1->SaveFile ();

    // unsigned char *IMAGE;
    // HANDLE hIM = (HANDLE)ImagXpress7_1->SaveBufferHandle;
    // IMAGE = (unsigned char *)GlobalLock(hIM);
    // long ln = GlobalSize(hIM);
    // long offs=ln-(long)w*Iy;
    // GlobalUnlock(hIM);

    // Screen->Cursor =  crHourGlass;
    // Form5->ProgressBar1->Max = w-1;
    // Form5->Show();

    int bl=0,wh=0;
    for (int x=0;x<Ix;x++)
    {
        // Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            prob = (1/deviation*sqrt(2*pi))*pow(e,(0-1)*(pow(((long)rand()*(255-0)/32767+1)-mean,2)/(2*pow(deviation,2))));
            temp[x][y]=prob;
            if (prob > max)max=prob;
            if (prob < min)min=prob;
        }
    }
    //ShowMessage("Minimum Probability : "+FloatToStr(min)+" %\nMaximum Probability : "+FloatToStr(max)+" %\nAverage Probability : "+FloatToStr((max+min)/2)+" %");
    for (int x=1;x<Ix-1;x++)
    {
        // Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-1;y++)
        {
            if (temp[x][y] >= (min+max)/2)
            {
                imageObject->setPixel(x,y,1);
                bl++;
            }
            else wh++;
            //imageObject->setPixel(x,y,1);
            //            int current_pixel_color = imageObject->pixelIndex(x,y);
            //            std::cout<<current_pixel_color;
        }
    }
    //        ShowMessage("Noise changed : "+IntToStr(bl)+" pixels\nUnchanged : "+IntToStr(wh)+" pixels");

    //        ImagXpress7_1->LoadBuffer((long)IMAGE);
    //        GlobalFree(IMAGE);
    //        Form5->Close();
    //        if (savestages==1)
    //        {
    //           Form1->stagesSave();
    //        }
    //        Screen->Cursor =  crDefault;

    //        image = QPixmap::fromImage(*imageObject);
    //        ui->image_label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
    //        ui->image_label->repaint();
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionRandom_Noise_triggered()
{
    //QMessageBox::information(this,"Random","Add Random Noise");
    int x,y;
    for (int i=0;i<100000;i++)
    {
        x = (long(rand())*(Ix-2))/32767+1;
        y = (long(rand())*(Iy-2))/32767+1;
        imageObject->setPixel(x,y,1);
    }

    // Form4->ImagXpress7_1->DIBUpdate();

    // unsigned char *I;

    // if ((I = (unsigned char *)malloc(Ix*Iy))==NULL)
    // {
    //   QMessageBox::alert(this,"Alert","memory problem");return;
    // }

    // // Form5->ProgressBar1->Max = Ix-1;
    // // Form5->Show();

    // for (int x=0;x<Ix;x++)
    // {
    //   // Form5->ProgressBar1->Position = x;
    //   for (int y=0;y<Iy;y++)
    //   *(I+y*Ix+x) = imageObject->getPixel(x,y);
    // }
    // Form4->ImagXpress7_2->hDIB = Form4->ImagXpress7_1->CopyDIB();

    // // Form5->ProgressBar1->Max = Ix-1;

    // for (int x=0;x<Ix;x++)
    // {
    //   // Form5->ProgressBar1->Position = x;
    //   for (int y=0;y<Iy;y++)
    //   {
    //      if (*(I+y*Ix+x)==0)
    //      {
    //        int c=0;
    //        for (int ix=x-3;ix<=x+3;ix++)
    //        for (int iy=y-3;iy<=y+3;iy++)
    //        {
    //          if ((ix>=0) && (ix<Ix) && (iy>=0) && (iy<Iy))
    //          if (*(I+iy*Ix+ix)==255)
    //            c++;
    //        }
    //        if (c>34)
    //        {
    //          Form4->ImagXpress7_2->DIBSetPixel(x,y,255);
    //        }
    //      }
    //   }
    // }
    // // Form5->Close();
    // Form4->ImagXpress7_2->DIBUpdate();
    // free(I);
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionAll_White_triggered()
{
    //Mask 4x4 All White
    for (int x=1;x<Ix-2;x++)
    {
        //          Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x-1,y)==0 && imageObject->pixelIndex(x+1,y)==0 && imageObject->pixelIndex(x,y)==1 && imageObject->pixelIndex(x,y-1)==0 && imageObject->pixelIndex(x,y+1)==0)
            {
                imageObject->setPixel(x,y,0);
            }
            //imageObject->setPixel(x,y,1);
            //            int current_pixel_color = imageObject->pixelIndex(x,y);
            //            std::cout<<current_pixel_color;
        }
    }
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionAll_Black_triggered()
{
    //Mask 4x4 All Black
    for (int x=1;x<Ix-2;x++)
    {
        //          Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x-1,y)==1 && imageObject->pixelIndex(x+1,y)==1 && imageObject->pixelIndex(x,y)==0 && imageObject->pixelIndex(x,y-1)==1 && imageObject->pixelIndex(x,y+1)==1)
            {
                imageObject->setPixel(x,y,1);
            }
            //imageObject->setPixel(x,y,1);
            //            int current_pixel_color = imageObject->pixelIndex(x,y);
            //            std::cout<<current_pixel_color;
        }
    }
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionAll_White_2_triggered()
{
    //Mask 8x8 All White
    for (int x=1;x<Ix-2;x++)
    {
        //          Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x-1,y+1)==0 && imageObject->pixelIndex(x,y+1)==0 && imageObject->pixelIndex(x+1,y+1)==0)
                if (imageObject->pixelIndex(x-1,y)==0 && imageObject->pixelIndex(x,y)==1 && imageObject->pixelIndex(x+1,y)==0)
                    if (imageObject->pixelIndex(x-1,y-1)==0 && imageObject->pixelIndex(x,y-1)==0 && imageObject->pixelIndex(x+1,y-1)==0)
                    {
                        imageObject->setPixel(x,y,0);
                    }
            //imageObject->setPixel(x,y,1);
            //            int current_pixel_color = imageObject->pixelIndex(x,y);
            //            std::cout<<current_pixel_color;
        }
    }
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionAll_Black_2_triggered()
{
    //Mask 8x8 All Black
    for (int x=1;x<Ix-2;x++)
    {
        //          Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x-1,y+1)==1 && imageObject->pixelIndex(x,y+1)==1 && imageObject->pixelIndex(x+1,y+1)==1)
                if (imageObject->pixelIndex(x-1,y)==1 && imageObject->pixelIndex(x,y)==0 && imageObject->pixelIndex(x+1,y)==1)
                    if (imageObject->pixelIndex(x-1,y-1)==1 && imageObject->pixelIndex(x,y-1)==1 && imageObject->pixelIndex(x+1,y-1)==1)
                    {
                        imageObject->setPixel(x,y,1);
                    }
            //imageObject->setPixel(x,y,1);
            //            int current_pixel_color = imageObject->pixelIndex(x,y);
            //            std::cout<<current_pixel_color;
        }
    }
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::on_actionComparison_triggered()
{

}

void MainWindow::on_actionEvaluation_triggered()
{

}

void MainWindow::on_actionAll_3x3_triggered()
{
    //Cube 3x3 dilation
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=1;x<Ix-2;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x,y)==1)
            {
                //x-1
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==0)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==0)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==0)temp[x-1][y+1]=2;
                //x
                if (y>0 && imageObject->pixelIndex(x,y-1)==0)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==0)temp[x][y+1]=2;
                //x+1
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==0)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==0)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==0)temp[x+1][y+1]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,1);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionCross_3x3_triggered()
{
    //Cross 3x3 dilation
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=1;x<Ix-2;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x,y)==1)
            {
                if (x>0 && imageObject->pixelIndex(x-1,y)==0)temp[x-1][y]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==0)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==0)temp[x][y+1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==0)temp[x+1][y]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,1);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionAll_5x5_triggered()
{
    //Cube 5x5 dilation
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=2;x<Ix-3;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=2;y<Iy-3;y++)
        {
            if (imageObject->pixelIndex(x,y)==1)
            {
                //x-2
                if (x-1>0 && y-1>0 && imageObject->pixelIndex(x-2,y-2)==0)temp[x-2][y-2]=2;
                if (x-1>0 && y>0 && imageObject->pixelIndex(x-2,y-1)==0)temp[x-2][y-1]=2;
                if (x-1>0 && imageObject->pixelIndex(x-2,y)==0)temp[x-2][y]=2;
                if (x-1>0 && y+1<Iy && imageObject->pixelIndex(x-2,y+1)==0)temp[x-2][y+1]=2;
                if (x-1>0 && y+2<Iy && imageObject->pixelIndex(x-2,y+2)==0)temp[x-2][y+2]=2;
                //x-1
                if (x>0 && y-1>0 && imageObject->pixelIndex(x-1,y-2)==0)temp[x-1][y-2]=2;
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==0)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==0)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==0)temp[x-1][y+1]=2;
                if (x>0 && y+2<Iy && imageObject->pixelIndex(x-1,y+2)==0)temp[x-1][y+2]=2;
                //x
                if (y-1>0 && imageObject->pixelIndex(x,y-2)==0)temp[x][y-2]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==0)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==0)temp[x][y+1]=2;
                if (y+2<Iy && imageObject->pixelIndex(x,y+2)==0)temp[x][y+2]=2;
                //x+1
                if (x+1<Ix && y-1>0 && imageObject->pixelIndex(x+1,y-2)==0)temp[x+1][y-2]=2;
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==0)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==0)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==0)temp[x+1][y+1]=2;
                if (x+1<Ix && y+2<Iy && imageObject->pixelIndex(x+1,y+2)==0)temp[x+1][y+2]=2;
                //x+2
                if (x+2<Ix && y-1>0 && imageObject->pixelIndex(x+2,y-2)==0)temp[x+2][y-2]=2;
                if (x+2<Ix && y>0 && imageObject->pixelIndex(x+2,y-1)==0)temp[x+2][y-1]=2;
                if (x+2<Ix && imageObject->pixelIndex(x+2,y)==0)temp[x+2][y]=2;
                if (x+2<Ix && y+1<Iy && imageObject->pixelIndex(x+2,y+1)==0)temp[x+2][y+1]=2;
                if (x+2<Ix && y+2<Iy && imageObject->pixelIndex(x+2,y+2)==0)temp[x+2][y+2]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,1);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionRhombus_5x5_triggered()
{
    //Rhombus 5x5 dilation
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=2;x<Ix-3;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=2;y<Iy-3;y++)
        {
            if (imageObject->pixelIndex(x,y)==1)
            {
                //x-2
                if (x-1>0 && imageObject->pixelIndex(x-2,y)==0)temp[x-2][y]=2;
                //x-1
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==0)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==0)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==0)temp[x-1][y+1]=2;
                //x
                if (y-1>0 && imageObject->pixelIndex(x,y-2)==0)temp[x][y-2]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==0)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==0)temp[x][y+1]=2;
                if (y+2<Iy && imageObject->pixelIndex(x,y+2)==0)temp[x][y+2]=2;
                //x+1
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==0)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==0)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==0)temp[x+1][y+1]=2;
                //x+2
                if (x+2<Ix && imageObject->pixelIndex(x+2,y)==0)temp[x+2][y]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,1);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)
        free(temp[x]);
    free(temp);
}

void MainWindow::on_actionAll_3x3_2_triggered()
{
    //Cube 3x3 dilation
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=1;x<Ix-2;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x,y)==0)
            {
                //x-1
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==1)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==1)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==1)temp[x-1][y+1]=2;
                //x
                if (y>0 && imageObject->pixelIndex(x,y-1)==1)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==1)temp[x][y+1]=2;
                //x+1
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==1)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==1)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==1)temp[x+1][y+1]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,0);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionCross_3x3_2_triggered()
{
    //Cross 3x3 erosion
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=1;x<Ix-2;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=1;y<Iy-2;y++)
        {
            if (imageObject->pixelIndex(x,y)==0)
            {
                if (x>0 && imageObject->pixelIndex(x-1,y)==1)temp[x-1][y]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==1)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==1)temp[x][y+1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==1)temp[x+1][y]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,0);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionAll_5x5_2_triggered()
{
    //Cube 5x5 erosion
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=2;x<Ix-3;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=2;y<Iy-3;y++)
        {
            if (imageObject->pixelIndex(x,y)==0)
            {
                //x-2
                if (x-1>0 && y-1>0 && imageObject->pixelIndex(x-2,y-2)==1)temp[x-2][y-2]=2;
                if (x-1>0 && y>0 && imageObject->pixelIndex(x-2,y-1)==1)temp[x-2][y-1]=2;
                if (x-1>0 && imageObject->pixelIndex(x-2,y)==1)temp[x-2][y]=2;
                if (x-1>0 && y+1<Iy && imageObject->pixelIndex(x-2,y+1)==1)temp[x-2][y+1]=2;
                if (x-1>0 && y+2<Iy && imageObject->pixelIndex(x-2,y+2)==1)temp[x-2][y+2]=2;
                //x-1
                if (x>0 && y-1>0 && imageObject->pixelIndex(x-1,y-2)==1)temp[x-1][y-2]=2;
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==1)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==1)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==1)temp[x-1][y+1]=2;
                if (x>0 && y+2<Iy && imageObject->pixelIndex(x-1,y+2)==1)temp[x-1][y+2]=2;
                //x
                if (y-1>0 && imageObject->pixelIndex(x,y-2)==1)temp[x][y-2]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==1)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==1)temp[x][y+1]=2;
                if (y+2<Iy && imageObject->pixelIndex(x,y+2)==1)temp[x][y+2]=2;
                //x+1
                if (x+1<Ix && y-1>0 && imageObject->pixelIndex(x+1,y-2)==1)temp[x+1][y-2]=2;
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==1)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==1)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==1)temp[x+1][y+1]=2;
                if (x+1<Ix && y+2<Iy && imageObject->pixelIndex(x+1,y+2)==1)temp[x+1][y+2]=2;
                //x+2
                if (x+2<Ix && y-1>0 && imageObject->pixelIndex(x+2,y-2)==1)temp[x+2][y-2]=2;
                if (x+2<Ix && y>0 && imageObject->pixelIndex(x+2,y-1)==1)temp[x+2][y-1]=2;
                if (x+2<Ix && imageObject->pixelIndex(x+2,y)==1)temp[x+2][y]=2;
                if (x+2<Ix && y+1<Iy && imageObject->pixelIndex(x+2,y+1)==1)temp[x+2][y+1]=2;
                if (x+2<Ix && y+2<Iy && imageObject->pixelIndex(x+2,y+2)==1)temp[x+2][y+2]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,0);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionRhombus_5x5_2_triggered()
{
    //Rhombus 5x5 dilation
    long offs=0;
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];
    for (int x=2;x<Ix-3;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=2;y<Iy-3;y++)
        {
            if (imageObject->pixelIndex(x,y)==0)
            {
                //x-2
                if (x-1>0 && imageObject->pixelIndex(x-2+offs,y)==1)temp[x-2][y]=2;
                //x-1
                if (x>0 && y>0 && imageObject->pixelIndex(x-1,y-1)==1)temp[x-1][y-1]=2;
                if (x>0 && imageObject->pixelIndex(x-1,y)==1)temp[x-1][y]=2;
                if (x>0 && y+1<Iy && imageObject->pixelIndex(x-1,y+1)==1)temp[x-1][y+1]=2;
                //x
                if (y-1>0 && imageObject->pixelIndex(x,y-2)==1)temp[x][y-2]=2;
                if (y>0 && imageObject->pixelIndex(x,y-1)==1)temp[x][y-1]=2;
                if (y+1<Iy && imageObject->pixelIndex(x,y+1)==1)temp[x][y+1]=2;
                if (y+2<Iy && imageObject->pixelIndex(x,y+2)==1)temp[x][y+2]=2;
                //x+1
                if (x+1<Ix && y>0 && imageObject->pixelIndex(x+1,y-1)==1)temp[x+1][y-1]=2;
                if (x+1<Ix && imageObject->pixelIndex(x+1,y)==1)temp[x+1][y]=2;
                if (x+1<Ix && y+1<Iy && imageObject->pixelIndex(x+1,y+1)==1)temp[x+1][y+1]=2;
                //x+2
                if (x+2<Ix && imageObject->pixelIndex(x+2,y)==1)temp[x+2][y]=2;
            }
        }
    }
    for (int x=0;x<Ix;x++)
    {
        //         Form5->ProgressBar1->Position = x;
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,0);
        }
    }

    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)
        free(temp[x]);
    free(temp);
}

void MainWindow::on_actionSwell_Filter_triggered()
{
    //Swell Filter
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add the lineEdits with their respective labels
    QList<QSpinBox *> fields;
    QSpinBox *ws = new QSpinBox(&dialog);
    ws->setValue(10);
    QSpinBox *ht = new QSpinBox(&dialog);
    ht->setValue(1);
    QSpinBox *wt = new QSpinBox(&dialog);
    wt->setValue(1);

    form.addRow("Window Size", ws);
    form.addRow("W Threshhold", ht);
    form.addRow("H Threshhold",wt);

    fields << ws;
    fields << ht;
    fields << wt;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    int windowsize,ksw,ksh;

    if (dialog.exec() == QDialog::Accepted) {
        windowsize = fields[0]->value();
        ksw = fields[1]->value();
        ksh = fields[2]->value();

        int maxnumofpixels=windowsize*windowsize;
        int** temp=new int*[Ix];
        for (int x=0;x<Ix;x++)
            temp[x]=new int[Iy];

        int black=0;
        for (int y=windowsize;y<Iy-windowsize;y++)
        {
            for (int x=windowsize;x<Ix-windowsize;x++)
            {
                int y1=y-((windowsize-1)/2);
                if (y1<0)y1=0;
                int maxy=y+((windowsize-1)/2);
                if (maxy>Iy)maxy=Iy;
                black=0;
                while (y1<maxy)
                {
                    int x1=x-((windowsize-1)/2);
                    if (x1<0)x1=0;
                    int maxx=x+((windowsize-1)/2);
                    if (maxx>Ix)maxx=Ix;
                    while (x1<maxx)
                    {
                        if (imageObject->pixelIndex(x1,y1)==1 && x1!=x && y1!=y)
                        {
                            black++;
                        }
                        x1++;
                    }
                    y1++;
                }
                if (black>ksw)temp[x][y]=2;
            }
        }

        for (int x=0;x<Ix;x++)
        {
            for (int y=0;y<Iy;y++)
            {
                if (temp[x][y]==2)imageObject->setPixel(x,y,1);
            }
        }
        update();
        image = QPixmap::fromImage(*imageObject);
        if(scene==NULL) scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
        ui->graphicsView->repaint();
        for (int x=0;x<Ix;x++)free(temp[x]);
        free(temp);
    }
}

void MainWindow::on_actionShrink_Filter_triggered()
{
    //Shrink Filter
    int** temp=new int*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new int[Iy];

    int windowsize = 10; //Must be given from user
    int ksw = 1; //Must be given by user, W Threshhold
    int ksh = 1; //Must be given by user, H Threshhold
    int maxnumofpixels=windowsize*windowsize;
    int white=0;
    for (int y=windowsize;y<Iy-windowsize;y++)
    {

        for (int x=windowsize;x<Ix-windowsize;x++)
        {
            int y1=y-((windowsize-1)/2);
            if (y1<0)y1=0;
            int maxy=y+((windowsize-1)/2);
            if (maxy>Iy)maxy=Iy;
            white=0;
            while (y1<maxy)
            {
                int x1=x-((windowsize-1)/2);
                if (x1<0)x1=0;
                int maxx=x+((windowsize-1)/2);
                if (maxx>Ix)maxx=Ix;
                while (x1<maxx)
                {
                    if (imageObject->pixelIndex(x1,y1)==0 && x1!=x && y1!=y)
                    {
                        white++;
                    }
                    x1++;
                }
                y1++;
            }
            if (white>ksh)temp[x][y]=2;
        }
    }

    for (int x=0;x<Ix;x++)
    {
        for (int y=0;y<Iy;y++)
        {
            if (temp[x][y]==2)imageObject->setPixel(x,y,0);
        }
    }
    update();
    image = QPixmap::fromImage(*imageObject);
    if(scene==NULL) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
    for (int x=0;x<Ix;x++)free(temp[x]);
    free(temp);
}

void MainWindow::on_actionUndo_triggered()
{

}

void MainWindow::on_actionRedo_triggered()
{

}
