#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
