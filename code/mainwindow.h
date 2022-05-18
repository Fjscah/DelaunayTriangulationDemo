#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stdxfm.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

#include <QList>
#include <QScrollArea>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QResizeEvent>

#include <QRgb>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPixmap curPixmap; //当前的图片
    QImage * curimg;
    QPixmap curskele;
    float pixRatio;

private slots:

    void on_dock_pipline_visibilityChanged(bool visible);

    void on_actDockerPip_triggered(bool checked);

    void on_actFit_Width_triggered();

    void on_actFit_Height_triggered();
    void slot_sig_CurrentPos(float x,float y);

    void on_btn_delaunay_all_clicked();

    void on_btn_delaunay_each_clicked();

    void on_btn_clear_clicked();

    void on_btn_execute_clicked();

    void on_actZoomOut_triggered();

    void on_actZoomRealSize_triggered();

    void on_actZoomIn_triggered();

    void on_btn_delaunay_constrain_clicked();

    void on_btn_clear_2_clicked();

protected:


private:
    void imgZoomFitW();
    void on_ImgOpened_triggered();

    //void printGeometry();
    Ui::MainWindow *ui;

    void JumpToFrame(int idx);

    double pixratio=1.0;

    //-----Info widget------//
    QLabel *lab_imgpath;
    QLabel *lab_pospix;


};
#endif // MAINWINDOW_H
