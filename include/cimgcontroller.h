#ifndef CIMGCONTROLLER_H
#define CIMGCONTROLLER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cimgproc.h"
#include "cepsilonfilter.h"
#include "cdetecttrack.h"
#include "mainwindow.h"
#include <QDebug>
#define BEFORE_CPU_COUNT  double duration;duration = static_cast<double>(cv::getTickCount());
#define AFTER_CPU_COUNT    duration = static_cast<double>(cv::getTickCount())-duration;duration /= cv::getTickFrequency();qDebug()<<"exe time is"<<duration;


class CImgContext
{
public:
    int _constrast;
    int _brightness;
};


class CImgController //:public QObject
{
//Q_OBJECT
private:
    static CImgController* pSingleton;
    cv::Mat inMat;
    cv::Mat rstMat;
    cv::Mat imgCapture;

    CImgController()//singletion pattern,constructor must be private!!!
    {
         pstrategy=new CImgProc();


        //pstrategy=static_cast<CImgProc*>(new CepsilonFilter());
    }

public:
    CImgContext context;
    CImgProc * pstrategy;

    void setCapture()
    {
        imgCapture=inMat.clone();
        cv::imwrite("capture.jpg",imgCapture);

    }
    void streamProcess()// handle video stream
    {

    }

    void processOnePicture()
    {

        //BEFORE_CPU_COUNT
        if(inMat.empty()||rstMat.empty()) return;
        pstrategy->processOnePicture(inMat,rstMat);
        //AFTER_CPU_COUNT

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
    void setInputImage(cv::Mat in)
    {
        inMat=in;
        rstMat=inMat.clone();
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
