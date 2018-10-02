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

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void repaintImage();
    
    void deleteTemp();

private slots:
    void on_actionOpen_Image_triggered();

    void on_actionSave_Image_triggered();

    void on_actionEnable_Auto_Save_triggered();

    void on_actionDisable_Auto_Save_triggered();

    void on_actionExit_triggered();

    void on_actionMethod_Input_triggered();

    void on_actionAbout_triggered();

    void on_actionGaussian_Noise_triggered();

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
    QPixmap image, original_image, comparison_image;
    QImage  *imageObject, *original_imageObject, *comparison_imageObject;
    QString file_path, file_name;
    int Ix,Iy,stage_num, total_stages, ksw = 0, ksh = 0, windowsize = 3;
    bool auto_save_enable = false, from_input_method = false, undo_function=false, redo_function=false, resize_trigger=false;
    double mean, deviation;
    QGraphicsScene *scene;

};

#endif // MAINWINDOW_H
