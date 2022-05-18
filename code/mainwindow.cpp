#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lab_imgpath=new QLabel();
    lab_pospix=new QLabel();
    ui->statusbar->addWidget(lab_imgpath);
    ui->statusbar->addWidget(lab_pospix);


    QGraphicsOpacityEffect *opacityEffect=new QGraphicsOpacityEffect;

    opacityEffect->setOpacity(0.7);

    // bind signal and communication widget and processor
    connect(ui->gView,SIGNAL(sig_CurrentPos(float,float)),this,SLOT(slot_sig_CurrentPos(float,float)));

    //lab_skele->setWindowOpacity(0.5);
    setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_sig_CurrentPos(float x, float y)
{
    QString str;
    QString formatinfo;

    str=QString::asprintf("x=%.1f,y=%.1f",x,y);

    lab_pospix->setText(str);
}


/* =↓=↓=↓=↓=↓=↓=  ZOOMING  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_actFit_Width_triggered()
{
    ui->gView->resetTransform();
    pixratio=ui->gView->width()/(ui->gView->scene->width()+30);
    ui->gView->scale(pixratio,pixratio);
}
void MainWindow::on_actFit_Height_triggered()
{
    ui->gView->resetTransform();
    pixratio=ui->gView->height()/(ui->gView->scene->height()+30);
    ui->gView->scale(pixratio,pixratio);
}

void MainWindow::on_actZoomRealSize_triggered()
{
    pixratio=1;  //恢复显示比例为1
    ui->gView->resetTransform();
}

void MainWindow::on_actZoomIn_triggered()
{

    ui->gView->scale(1.1,1.1);
}

void MainWindow::on_actZoomOut_triggered()
{
    ui->gView->scale(.9,.9);

}

/* =↓=↓=↓=↓=↓=↓=  DOCKWIDGET  =↓=↓=↓=↓=↓=↓= */

void MainWindow::on_dock_pipline_visibilityChanged(bool visible)
{
    ui->actDockerPip->setChecked(visible);
}

void MainWindow::on_actDockerPip_triggered(bool checked)
{
    ui->dock_pipline->setVisible(checked);
}


/* =↓=↓=↓=↓=↓=↓=  DELAUNAY  =↓=↓=↓=↓=↓=↓= */

void MainWindow::on_btn_delaunay_all_clicked()
{
//    ui->gView->ConstrainBowyer();
//    ui->gView->showData();
    ui->gView->Bowyer();
}

void MainWindow::on_btn_delaunay_each_clicked()
{
    //ui->gView->ConBowEachContour();
    ui->gView->EachBowyer();
//    ui->gView->updateTriangle();
}

void MainWindow::on_btn_delaunay_constrain_clicked()
{
    ui->gView->ConstrainBowyer();
}

void MainWindow::on_btn_execute_clicked()
{
    int mode=ui->combo_genseed->currentIndex();
    if(mode==0)
    {
        int num=ui->edit_seed_num->text().toInt();
        //generate from seed num
        ui->gView->GeneratePoints(num);
    }
    else
    {
        //draw mode
        ui->gView->Is_draw=ui->chk_draw->isChecked();
        if (!ui->chk_draw->isChecked())
            ui->gView->clearCurrentitem();
    }
}


/* =↓=↓=↓=↓=↓=↓=  CLEAR  =↓=↓=↓=↓=↓=↓= */
void MainWindow::on_btn_clear_clicked()
{
    ui->gView->CleanAll();
}

void MainWindow::on_btn_clear_2_clicked()
{
    ui->gView->CleanResult();
}
