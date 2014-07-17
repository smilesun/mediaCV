#ifndef CIMGCONTROLLER_H
#define CIMGCONTROLLER_H



#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cimgproc.h"
#include "cepsilonfilter.h"
#include "mainwindow.h"
#include <QDebug>
#define BEFORE_CPU_COUNT  double duration;duration = static_cast<double>(cv::getTickCount());
#define AFTER_CPU_COUNT    duration = static_cast<double>(cv::getTickCount())-duration;duration /= cv::getTickFrequency();qDebug()<<"exe time is"<<duration;



class CImgController
{

private:

    static CImgController* pSingleton;
    CImgProc * pstrategy;
    cv::Mat inMat;
    cv::Mat rstMat;
    CImgController()
    {
        pstrategy=static_cast<CImgProc*>(new CepsilonFilter());
        this->bilateral_kernel_len=7;
    }
public:
    int bilateral_kernel_len;
    void process()
    {


        pstrategy->process(inMat,rstMat);
        BEFORE_CPU_COUNT
        //bilateralFilter(inMat,rstMat,this->bilateral_kernel_len,bilateral_kernel_len*2,bilateral_kernel_len/2);
        AFTER_CPU_COUNT
    }

    static CImgController* getInstance()
    {
       if(pSingleton==0)
       {
           pSingleton=new CImgController;
           return pSingleton;
       }
       else
           return pSingleton;
    }
    bool setInputImage(std::string filename) {

        inMat= cv::imread(filename);

        if (!inMat.data)
            return false;
        else
        {
            rstMat=inMat.clone();
            return true;
        }
    }

    // Releases the singleton instance of this controller.
    static void destroy() {

        if (pSingleton != 0) {
            delete pSingleton;
            pSingleton= 0;
        }
    }
    //void setInputImage(std::string);
    cv::Mat getInput()
    {

            return inMat;
    }
    cv::Mat getLastOutput()
    {
        return rstMat;
    }

};

#endif // CIMGCONTROLLER_H
