#ifndef CVIDEODEVICE_H
#define CVIDEODEVICE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
class CVideoDevice
{
private:
    cv::VideoCapture* pcap;
public:
    CVideoDevice()
    {
        pcap=NULL;
    }
    ~CVideoDevice()
    {
        if(pcap)
        {
            pcap->release();
            delete pcap;
            pcap=NULL;
        }
    }
    void open()
    {
        pcap=new VideoCapture(0);
        if(!pcap->isOpened())  return;// check if we succeeded

    }

bool getFrame(cv::Mat& temp)
    {
            if(pcap==NULL) return false;
            (*pcap)>>temp;
            return true;

    }
void close()
    {

            pcap->release();

    }
};

#endif // CVIDEODEVICE_H
