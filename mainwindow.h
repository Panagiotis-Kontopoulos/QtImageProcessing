#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void repaintImage();
    
private slots:
    void on_actionOpen_Image_triggered();

    void on_actionSave_Image_triggered();

    void on_actionEnable_Auto_Save_triggered();

    void on_actionDisable_Auto_Save_triggered();

    void on_actionExit_triggered();

    void on_actionMethod_Input_triggered();

    void on_actionMore_triggered();

    void on_actionGasussian_Noise_triggered();

    void on_actionRandom_Noise_triggered();

    void on_actionAll_White_triggered();

    void on_actionAll_Black_triggered();

    void on_actionAll_White_2_triggered();

    void on_actionAll_Black_2_triggered();

    void on_actionComparison_triggered();

    void on_actionEvaluation_triggered();

    void on_actionRhombus_5x5_triggered();

    void on_actionAll_3x3_triggered();

    void on_actionCross_3x3_triggered();

    void on_actionAll_5x5_triggered();

    void on_actionAll_3x3_2_triggered();

    void on_actionCross_3x3_2_triggered();

    void on_actionAll_5x5_2_triggered();

    void on_actionRhombus_5x5_2_triggered();

    void on_actionSwell_Filter_triggered();

    void on_actionShrink_Filter_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void auto_save_Function();

private:
    Ui::MainWindow *ui;
    QPixmap image, original_image;
    QImage  *imageObject, *original_imageObject;
    QString file_path, file_name;
    int Ix,Iy,stage_num;
    bool auto_save_enable;
    float mean, deviation;
    QGraphicsScene *scene;

};

#endif // MAINWINDOW_H
