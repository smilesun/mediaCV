#ifndef CIMGCONTROLLER_H
#define CIMGCONTROLLER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cimgproc.h"
#include "cepsilonfilter.h"
class CImgController
{
public:
    CImgController();
private:
    CImgProc * pstrategy;
    cv::Mat image;
    cv::Mat rst;
    void salt(cv::Mat &,int);
    void convert_video(char*,char* );
    void sharpen(const cv::Mat &image, cv::Mat &result);//laplasian
    void getColorRange();
};

#endif // CIMGCONTROLLER_H
