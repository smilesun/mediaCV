#ifndef CIMGPROC_H
#define CIMGPROC_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include "cdetecttrack.h"
#include <QThread>
#include <QDebug>
#include <QMutex>


using namespace cv;

extern QMutex mutex;
extern cv::Mat sharedImg;
extern bool flag_begin;

class ThreadDetectTrack:public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        QString result="this a a separate thread";
        // ... here is the expensive or blocking operation ...
        while(1)
        {
            qDebug()<<"thread detect beging of while";
            mutex.lock();
            if(flag_begin)
                {
                    this->setImage(sharedImg);
                    qDebug()<<"in mutex: in thread detect run ";
                    //cv::imshow("detec",this->image);
                    cd.detectAndDraw(this->image,1, true);
                    flag_begin=false;

                }
            mutex.unlock();
            qDebug()<<"out of mutex in thread detect run";

        }
        emit resultReady(result);
    }
public:
    ThreadDetectTrack()
    {

    }

    void fetchImage(cv::Mat& img)
    {

    }
    void setImage(cv::Mat& img)
    {

            this->image=img.clone();//deep copy!!!

    }
    void setImage(const cv::Mat& img)
    {

            this->image=img.clone();

    }
private:
    cv::Mat image;
    CDetectTrack cd;

signals:
    void resultReady(const QString &s);
};


class CImgProc:public QObject
{
    Q_OBJECT

public:
    CImgProc();
    virtual void processOnePicture(const cv::Mat &image, cv::Mat &result);
    void YcbcrEqualize(const cv::Mat &image, cv::Mat &result);
    void RgbEqualize(const cv::Mat &image, cv::Mat &result);
    void HsvEqualize(const cv::Mat &image, cv::Mat &result);
    void doEnhanceOnHeadROI(const cv::Mat &image, cv::Mat &result);
    void updateBrightnessContrast(cv::Mat&image,int _brightness,int _contrast,cv::Mat& result);

    virtual void processStream(const cv::Mat &image, cv::Mat &result);

    void salt(cv::Mat &,int);
    void convert_video(char*,char* );
    void sharpen(const cv::Mat &image, cv::Mat &result);//laplasian
    void getColorRange();

signals:
    void operate(const QString &);// tell the thread to operate

public slots:
    void handleResults(const QString &str){qDebug()<<str;} //after the thread run is ok, handle the output
private:
    ThreadDetectTrack t_dt;
    CDetectTrack c_d_t;
    cv::Mat inMat;
    cv::Mat rstMat;


};



#endif // CIMGPROC_H
