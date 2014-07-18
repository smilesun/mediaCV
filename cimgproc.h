#ifndef CIMGPROC_H
#define CIMGPROC_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
class CImgProc : public QObject
{
    Q_OBJECT

public:
    void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );
    explicit CImgProc(QObject *parent = 0);
    virtual void process(const cv::Mat &image, cv::Mat &result);
    void salt(cv::Mat &,int);
    void convert_video(char*,char* );
    void sharpen(const cv::Mat &image, cv::Mat &result);//laplasian
    void getColorRange();

signals:

public slots:
private:
    cv::Mat inMat;
    cv::Mat rstMat;
    cv::CascadeClassifier cascade;
    cv::CascadeClassifier nestedCascade;


};

#endif // CIMGPROC_H
