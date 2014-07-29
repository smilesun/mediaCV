#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QCloseEvent>
/************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
/***************************************************************/
#include "cimgcontroller.h"
#include "cwebcam.h"
#include "cvideodevice.h"
/*******************************************************************/

using namespace cv;
namespace Ui {
class MainWindow;
}

class CImgController;
#define BEFORE_CPU_COUNT  double duration;duration = static_cast<double>(cv::getTickCount());
#define AFTER_CPU_COUNT    duration = static_cast<double>(cv::getTickCount())-duration;duration /= cv::getTickFrequency();qDebug()<<"exe time is"<<duration;

#define FRAME_RATE 200// if the frame rate is too fast, the process is not enough to complete

class MainWindow : public QMainWindow
{
    Q_OBJECT

friend class CWebCam;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_horizontalSlider_sliderReleased();
/***************************************************************/
    void nextFrame();
/***************************************************************/


    void on_pushButton_3_clicked();

    void on_horizontalSlider_3_sliderReleased();

    void on_horizontalSlider_2_sliderReleased();

private:
    Ui::MainWindow *ui;

    ///*******user defined
    void displayOutImg(cv::Mat);
    void displayInImg(cv::Mat);
/****************************************/
    CVideoDevice vd;
    QTimer *timer;
    CImgController* pImgController;
/****************************************/

};

#endif // MAINWINDOW_H
