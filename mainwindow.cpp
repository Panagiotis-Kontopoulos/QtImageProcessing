#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
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
#include <QDebug>
#include <QTextStream>
#include <QCloseEvent>
#include "oddspinbox.h"
#include "comparison.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Delete temp folder
    deleteTemp();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    resize_trigger=true;
    if (ui->actionSave_Image->isEnabled()) repaintImage();
    resize_trigger=false;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString file_url = QFileDialog::getOpenFileName(this,tr("Open Image File"),QDir::currentPath(),tr("*.tiff"));
    QFileInfo fileInfo(file_url);
    file_path = fileInfo.path();
    file_name = fileInfo.fileName();
//    QMessageBox::information(this,file_name,file_name);
//    QMessageBox::information(this,file_path,file_path);
//    QMessageBox::information(this,file_url,file_url);
    imageObject = new QImage;
    original_imageObject = new QImage;
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *imageObject = imageReader.read();
    qDebug()<<"Image Read";
    *original_imageObject = imageObject->copy();
    qDebug()<<"Image Copy";
    image = QPixmap::fromImage(*imageObject);
    qDebug()<<"Image QPixmap";
    original_image = QPixmap::fromImage(*original_imageObject);
    qDebug()<<"Original Image QPixmap";


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
            //std::cout<<current_pixel_color;
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

    //Enable Menu Options
    ui->actionSave_Image->setEnabled(true);
    ui->actionEnable_Auto_Save->setEnabled(true);
    ui->menuMasks->setEnabled(true);
    ui->menuMathematical_Morphology->setEnabled(true);
    ui->menuFilters->setEnabled(true);
    ui->actionMethod_Input->setEnabled(true);
    ui->menuF_Measure->setEnabled(true);
    ui->menuAdd_Noise->setEnabled(true);

}

void MainWindow::on_actionSave_Image_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save Image File"),file_path,tr("*.tiff"));
    imageObject->save(filename);
}

void MainWindow::on_actionEnable_Auto_Save_triggered()
{
    QMessageBox::information(this,"Auto Save","Auto Save Enabled");
    ui->actionEnable_Auto_Save->setEnabled(false);
    ui->actionDisable_Auto_Save->setEnabled(true);
    stage_num = 0;
    total_stages = 0;
    if (!QDir(file_path+"/temp").exists()){
        QDir().mkdir(file_path+"/temp");
    }
    auto_save_Function();
    auto_save_enable = true;
}

void MainWindow::auto_save_Function()
{
    if (auto_save_enable){
        if (undo_function) {
            qInfo()<<"Undo function";
        }
        else if (redo_function) {
            qInfo()<<"Redo function";
        }
        else {
            stage_num++;
            total_stages=stage_num;
            qInfo()<<"Other functions";
        }

    }
    imageObject->save(file_path+"/temp"+"/stage"+QString::number(stage_num)+"_"+file_name);
    QMessageBox::information(this,"Auto Save",file_path+"/temp"+"/stage"+QString(stage_num)+"_"+file_name);
    if (stage_num >= 1 ) ui->actionUndo->setEnabled(true);
    else ui->actionUndo->setEnabled(false);
    if (stage_num < total_stages) ui->actionRedo->setEnabled(true);
    else ui->actionRedo->setEnabled(false);
}

void MainWindow::on_actionDisable_Auto_Save_triggered()
{
    QMessageBox::information(this,"Auto Save","Auto Save Disabled");
    ui->actionEnable_Auto_Save->setEnabled(true);
    ui->actionDisable_Auto_Save->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    auto_save_enable = false;

    // Delete temp folder
    deleteTemp();
}

void MainWindow::on_actionExit_triggered()
{

    QApplication::quit();

    // Delete temp folder
    deleteTemp();
}

void MainWindow::on_actionMethod_Input_triggered()
{
    QMessageBox::information(this,"Input","Input Methods");
    QString file_url = QFileDialog::getOpenFileName(this,tr("Open txt File"),QDir::currentPath(),tr("*.txt"));
    QFileInfo fileInfo(file_url);
    QFile file(file_url);
    if (!file.open(QFile::ReadWrite | QFile::Text))
    {
        QMessageBox::warning(this,"Error on Load","Error loading file "+file_url);
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString text = in.readLine();
        if (!text.compare("Mask 4x4 white"))
        {
            ui->actionAll_White->trigger();
        }
        else if (!text.compare("Mask 4x4 black"))
        {
            ui->actionAll_Black->trigger();
        }
        else if (!text.compare("Mask 8x8 white"))
        {
            ui->actionAll_White_2->trigger();
        }
        else if (!text.compare("Mask 8x8 black"))
        {
            ui->actionAll_Black_2->trigger();
        }
        else if (!text.compare("Dilation All 3x3"))
        {
            ui->actionAll_3x3->trigger();
        }
        else if (!text.compare("Dilation Cross 3x3"))
        {
            ui->actionCross_3x3->trigger();
        }
        else if (!text.compare("Dilation All 5x5"))
        {
            ui->actionAll_5x5->trigger();
        }
        else if (!text.compare("Dilation Rhombus 5x5"))
        {
            ui->actionRhombus_5x5->trigger();
        }
        else if (!text.compare("Erosion All 3x3"))
        {
            ui->actionAll_3x3_2->trigger();
        }
        else if (!text.compare("Erosion Cross 3x3"))
        {
            ui->actionCross_3x3_2->trigger();
        }
        else if (!text.compare("Erosion All 5x5"))
        {
            ui->actionAll_5x5_2->trigger();
        }
        else if (!text.compare("Erosion Rhombus 5x5"))
        {
            ui->actionRhombus_5x5_2->trigger();
        }
        else if (!text.compare("Swell-Filter"))
        {
            ui->actionSwell_Filter->trigger();
        }
        else if (!text.compare("Shrink-Filter"))
        {
            ui->actionShrink_Filter->trigger();
        }
        else
        {
            QStringList parts = text.split(" ");
            if (!parts[0].compare("Swell-Filter"))
            {
                windowsize = parts[1].toInt();
                ksw = parts[2].toInt();
                from_input_method = true;
                ui->actionSwell_Filter->trigger();
                from_input_method = false;
            }
            else if (!parts[0].compare("Shrink-Filter"))
            {
                windowsize = parts[1].toInt();
                ksh = parts[2].toInt();
                from_input_method = true;
                ui->actionShrink_Filter->trigger();
                from_input_method = false;
            }
            else
                QMessageBox::warning(this,"Warning","Unknown Command");
        }
    }
    file.close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"Qt Image Processing Program","Image processing program that covers a variaty of methods applied on an image. The program was developed under the scope of Special Communications & Signal Processing Cases with focus on Text Image Improvement.");
}

void MainWindow::on_actionGasussian_Noise_triggered()
{
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add the lineEdits with their respective labels
    QList<QDoubleSpinBox *> fields;
    QDoubleSpinBox *ws = new QDoubleSpinBox(&dialog);
    ws->setValue(1.0);
    QDoubleSpinBox *wt = new QDoubleSpinBox(&dialog);
    wt->setValue(2.0);

    form.addRow("Deviation", ws);
    form.addRow("Mean", wt);

    fields << ws;
    fields << wt;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    if (dialog.exec() == QDialog::Accepted) {
        deviation = fields[0]->value();
        mean = fields[1]->value();
    }

    std::cout<<"Adding Noise";
    //Add Gaussian Noise to the Image
    double pi = 3.14159265359;
    double e = 2.718;
    double prob = 0.0, min=1000,max=0;
    double** temp=new double*[Ix];
    for (int x=0;x<Ix;x++)
        temp[x]=new double[Iy];
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
            prob = (1.0/deviation*sqrt(2.0*pi))*pow(e,(-1.0)*(pow((long(rand())*(255-0)/32767+1)-mean,2)/(2*pow(deviation,2))));
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
    repaintImage();
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
    repaintImage();
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
    repaintImage();
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
    repaintImage();
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
    repaintImage();
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
    repaintImage();
}

void MainWindow::on_actionComparison_triggered()
{
    QString file_url = QFileDialog::getOpenFileName(this,tr("Open Image File"),QDir::currentPath(),tr("*.tiff"));
    QFileInfo fileInfo(file_url);
    file_path = fileInfo.path();
    file_name = fileInfo.fileName();
//    QMessageBox::information(this,file_name,file_name);
//    QMessageBox::information(this,file_path,file_path);
//    QMessageBox::information(this,file_url,file_url);
    comparison_imageObject = new QImage;
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *comparison_imageObject = imageReader.read();
    comparison_image = QPixmap::fromImage(*comparison_imageObject);


    int IxC = comparison_image.width();
    int IyC = comparison_image.height();

    float CTP=0,CFP=0,CFN=0; //Counter True Positive, Counter False Positive, Counter False Negative
    int error=0;
    if (Ix!=IxC || Iy!=IyC)
    {
       QMessageBox::information(this,"F-Measure Error","Images have different size");
       error=1;
       return;
    }
    float RC=0,PR=0,FM=0; //Anakthsh, Akribeia, F-Measure
    for (int x=1;x<Ix-1;x++)
    {
        for (int y=1;y<Iy-1;y++)
         {
            if (imageObject->pixelIndex(x,y)==1 && comparison_imageObject->pixelIndex(x,y)==1)CTP=CTP+1;
            if (imageObject->pixelIndex(x,y)==1 && comparison_imageObject->pixelIndex(x,y)==0)CFP=CFP+1;
            if (imageObject->pixelIndex(x,y)==0 && comparison_imageObject->pixelIndex(x,y)==1)CFN=CFN+1;
         }
         if (error)break;
      }
      if (!error)
      {

        RC=CTP/(CFN+CTP);
        PR=CTP/(CFP+CTP);
        FM=((2*RC*PR/(RC+PR))*100); // FM=(((2*RC*PR)/(RC+PR))*100)/100;
        QString str = QString::number(double(FM));
        QMessageBox::information(this,"F-Measure Comparison","Similarity : "+str+" %");
        Comparison *comparison_form;
        comparison_form = new Comparison;
        comparison_form->set_image(image);
        comparison_form->set_comparison_image(comparison_image);
        comparison_form->showMaximized();
      }
}

void MainWindow::on_actionEvaluation_triggered()
{


    float CTP=0,CFP=0,CFN=0; //Counter True Positive, Counter False Positive, Counter False Negative
    int error=0;
    float RC=0,PR=0,FM=0; //Anakthsh, Akribeia, F-Measure
    for (int x=1;x<Ix-1;x++)
    {
        for (int y=1;y<Iy-1;y++)
         {
            if (x>Ix || y>Iy)
            {
               QMessageBox::information(this,"F-Measure Error","Images have different size");
               error=1;
               break;
            }
            if (imageObject->pixelIndex(x,y)==1 && original_imageObject->pixelIndex(x,y)==1)CTP=CTP+1;
            if (imageObject->pixelIndex(x,y)==1 && original_imageObject->pixelIndex(x,y)==0)CFP=CFP+1;
            if (imageObject->pixelIndex(x,y)==0 && original_imageObject->pixelIndex(x,y)==1)CFN=CFN+1;
         }
         if (error)break;
      }
      if (!error)
      {

        RC=CTP/(CFN+CTP);
        PR=CTP/(CFP+CTP);
        FM=((2*RC*PR/(RC+PR))*100); // FM=(((2*RC*PR)/(RC+PR))*100)/100;
        QString str = QString::number(double(FM));
        QMessageBox::information(this,"F-Measure Evaluation","Similarity : "+str+" %");
      }

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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
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

    repaintImage();
    for (int x=0;x<Ix;x++)
        free(temp[x]);
    free(temp);
}

void MainWindow::on_actionSwell_Filter_triggered()
{
    //Swell Filter
    if (!from_input_method)
    {


    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add the lineEdits with their respective labels
    QList<QSpinBox *> fields;
    OddSpinBox *ws = new OddSpinBox(&dialog);
    ws->setValue(9);
    QSpinBox *wt = new QSpinBox(&dialog);
    wt->setValue(1);

    form.addRow("Window Size", ws);
    form.addRow("W Threshhold", wt);

    fields << ws;
    fields << wt;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    if (dialog.exec() == QDialog::Accepted) {
        windowsize = fields[0]->value();
        ksw = fields[1]->value();
    }
    else
        return;
    }

//        int maxnumofpixels=windowsize*windowsize;
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
        repaintImage();
        for (int x=0;x<Ix;x++)free(temp[x]);
        free(temp);
}

void MainWindow::on_actionShrink_Filter_triggered()
{
    //Shrink Filter
    if (!from_input_method)
    {
    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add the lineEdits with their respective labels
    QList<QSpinBox *> fields;
    OddSpinBox *ws = new OddSpinBox(&dialog);
    ws->setValue(9);
    QSpinBox *ht = new QSpinBox(&dialog);
    ht->setValue(1.0);

    form.addRow("Window Size", ws);
    form.addRow("H Threshhold",ht);

    fields << ws;
    fields << ht;

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    if (dialog.exec() == QDialog::Accepted) {
        windowsize = fields[0]->value();
        ksh = fields[1]->value();
    }
    else
        return;
}

        int** temp=new int*[Ix];
        for (int x=0;x<Ix;x++)
            temp[x]=new int[Iy];

//        int maxnumofpixels=windowsize*windowsize;
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
        repaintImage();
        for (int x=0;x<Ix;x++)free(temp[x]);
        free(temp);
}

void MainWindow::on_actionUndo_triggered()
{
    undo_function=true;
    if (stage_num > 0) stage_num--;
    QString file_url(file_path+"/temp"+"/stage"+QString::number(stage_num)+"_"+file_name);
    QFileInfo fileInfo(file_url);
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *imageObject = imageReader.read();
    if (stage_num == 0) {
        ui->actionUndo->setEnabled(false);
    }
    qDebug()<<"Undo Stage: "+QString::number(stage_num);
    qDebug()<<"Undo Total: "+QString::number(total_stages);
    qDebug()<<"Image Read";
    repaintImage();
    undo_function=false;
}

void MainWindow::on_actionRedo_triggered()
{
    redo_function=true;
    if (total_stages > stage_num) stage_num++;
    if (total_stages == stage_num) {
        ui->actionRedo->setEnabled(false);
    }
    QString file_url(file_path+"/temp"+"/stage"+QString::number(stage_num)+"_"+file_name);
    QFileInfo fileInfo(file_url);
    QImageReader imageReader(file_url);
    imageReader.setDecideFormatFromContent(true);
    *imageObject = imageReader.read();
    qDebug()<<"Redo Stage: "+QString::number(stage_num);
    qDebug()<<"Redo Total: "+QString::number(total_stages);
    qDebug()<<"Image Read";
    repaintImage();
    redo_function=false;
}

void MainWindow::repaintImage()
{
    if (auto_save_enable && !resize_trigger){
        auto_save_Function();
    }
    image = QPixmap::fromImage(*imageObject);
    update();
    if(scene==nullptr) scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView->repaint();
}

void MainWindow::deleteTemp()
{
    QDir dir(file_path+"/temp");
    if (dir.exists())
        dir.removeRecursively();
}
