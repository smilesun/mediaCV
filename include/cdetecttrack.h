#ifndef CDETECTTRACK_H
#define CDETECTTRACK_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <QString>



class CDetectTrack
{


public:
    CDetectTrack();
    void detectAndDraw(cv::Mat& img,double scale, bool tryflip );
    bool detectAndDraw(cv::Mat& img,double scale, bool tryflip,cv::Rect& );
    int  demo_asm(char* model_name, char* cascade_name);
private:
    cv::CascadeClassifier cascade;
    cv::CascadeClassifier nestedCascade;

};



#endif // CDETECTTRACK_H
