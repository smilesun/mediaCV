#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include "cimgcontroller.h"
#include "cimgproc.h"
#include "cepsilonfilter.h"
#include "cwebcam.h"
#include <vector>
#include <string>
#include <iostream>

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

friend class CWebCam;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_horizontalSlider_sliderReleased();

    void nextFrame();

private:
    Ui::MainWindow *ui;
    ///*******user defined
    CImgController* pImgController;
    CImgProc * pstrategy;
    cv::Mat image;
    cv::Mat rst;
    cv::Mat* psharedImg;
    class CWebCam *pwebcam;
    void displayOutImg(cv::Mat* prst);
    void displayInImg(cv::Mat& prst);
    int bilateral_kernel_len;
    bool bsync;
    cv::VideoCapture* pcap;
    cv::CascadeClassifier cascade;
    cv::CascadeClassifier nestedCascade;

};

#endif // MAINWINDOW_H
