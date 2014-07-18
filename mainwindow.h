#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
/***************************************************************/
#include "cimgcontroller.h"
#include "cwebcam.h"
/*******************************************************************/

using namespace cv;
namespace Ui {
class MainWindow;
}

class CImgController;
#define BEFORE_CPU_COUNT  double duration;duration = static_cast<double>(cv::getTickCount());
#define AFTER_CPU_COUNT    duration = static_cast<double>(cv::getTickCount())-duration;duration /= cv::getTickFrequency();qDebug()<<"exe time is"<<duration;


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


private:
    Ui::MainWindow *ui;


    ///*******user defined

    CImgController* pImgController;

    cv::Mat image;
    cv::Mat rst;

    void displayOutImg(cv::Mat);
    void displayInImg(cv::Mat);
/****************************************/
    class CWebCam *pwebcam;
    cv::VideoCapture* pcap;
    QTimer *timer;

/****************************************/

};

#endif // MAINWINDOW_H
