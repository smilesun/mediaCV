#ifndef CIMGPROC_H
#define CIMGPROC_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include "cdetecttrack.h"

using namespace cv;



class CImgProc
{
    //Q_OBJECT
public:
    CImgProc();
    virtual void process(const cv::Mat &image, cv::Mat &result);
    void salt(cv::Mat &,int);
    void convert_video(char*,char* );
    void sharpen(const cv::Mat &image, cv::Mat &result);//laplasian
    void getColorRange();

//signals:

//public slots:
private:
    CDetectTrack cd;
    cv::Mat inMat;
    cv::Mat rstMat;


};

#endif // CIMGPROC_H
