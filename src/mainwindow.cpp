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
    CImgController::getInstance()->processOnePicture();
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
   // if(ui->comboBox->currentIndex()==0) vd.close();
   // else
    vd.open();
    timer=new QTimer(this);
    timer->setInterval(FRAME_RATE);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start();
}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    //CImgController::getInstance()->bilateral_kernel_len=ui->horizontalSlider->value()/4;

    this->timer->setInterval(11*ui->horizontalSlider->value()+1);

}

void MainWindow::nextFrame()
{
           cv::Mat temp;
           if(!vd.getFrame(temp)) return; //robust code to prevent crash
           //process
           CImgController::getInstance()->setInputImage(temp);
           CImgController::getInstance()->streamProcess();
           //display
           displayInImg(CImgController::getInstance()->getInput());
           displayOutImg(CImgController::getInstance()->getLastOutput());

}


void MainWindow::on_pushButton_3_clicked()
{

        CImgController::getInstance()->setCapture();
                  ui->pushButton_3->setText("saved");

}

void MainWindow::on_horizontalSlider_3_sliderReleased()
{
    CImgController::getInstance()->context._constrast=(ui->horizontalSlider_3->value())*2;
    qDebug("slider is"+ui->horizontalSlider_3->value());
    //ui->lcdNumber->set;
    CImgController::getInstance()->processOnePicture();
    qDebug("contrast is "+CImgController::getInstance()->context._constrast);
    displayOutImg(CImgController::getInstance()->getLastOutput());

}

void MainWindow::on_horizontalSlider_2_sliderReleased()
{

    CImgController::getInstance()->context._brightness=(ui->horizontalSlider_2->value())*2;
    qDebug("slider is"+ui->horizontalSlider_2->value());

    CImgController::getInstance()->processOnePicture();

    qDebug("brightness is "+CImgController::getInstance()->context._brightness);

    displayOutImg(CImgController::getInstance()->getLastOutput());

}
