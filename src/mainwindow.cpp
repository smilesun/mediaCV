#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <cwebcam.h>



using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{

    delete ui;

}



void MainWindow::on_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("sun open image"),".",tr("Images files(*.png *.jpg *.jpeg *.bmp)"));
    CImgController::getInstance()->setInputImage(fileName.toStdString());
    displayInImg(CImgController::getInstance()->getInput());
}

void MainWindow::displayInImg(cv::Mat image)
{
    //QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);//without the step parameter,the image couldn't be displayed right
    QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_RGB888);
    QImage dstImg = img.rgbSwapped();
    // display on label
    ui->label_input->setPixmap(QPixmap::fromImage(dstImg));
    // resize the label to fit the image
    ui->label_input->resize(ui->label_input->pixmap()->size());

    if(image.data)
    {
        ui->pushButton_2->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    CImgController::getInstance()->process();
    displayOutImg(CImgController::getInstance()->getLastOutput());
}

void MainWindow::displayOutImg(cv::Mat rst)
{

    //QImage
    QImage img= QImage((const unsigned char*)(rst.data),rst.cols,rst.rows,rst.step,QImage::Format_RGB888);
    QImage dstImg = img.rgbSwapped();
    // display on label
    ui->label_output->setPixmap(QPixmap::fromImage(dstImg));
    // resize the label to fit the image
    ui->label_output->resize(ui->label_output->pixmap()->size());
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    qDebug("comboBox");
    vd.open();
    timer=new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start();
}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    CImgController::getInstance()->bilateral_kernel_len=ui->horizontalSlider->value()/4;


}

void MainWindow::nextFrame()
{
           cv::Mat temp;
           if(!vd.getFrame(temp)) return;

           //process
           CImgController::getInstance()->setInputImage(temp);
           CImgController::getInstance()->process();
           //display
           displayInImg(CImgController::getInstance()->getInput());
           displayOutImg(CImgController::getInstance()->getLastOutput());

}

/*void MainWindow::event(QEvent *event)
{
 if(event->type()==QEvent::Close)  ~MainWindow();
}
*/
/*
class MyThread:public QThread
{
   public:

}*/
