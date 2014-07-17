#ifndef CIMGPROC_H
#define CIMGPROC_H

#include <QObject>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class CImgProc : public QObject
{
    Q_OBJECT

public:
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


};

#endif // CIMGPROC_H
