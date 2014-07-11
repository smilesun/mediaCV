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

signals:

public slots:

};

#endif // CIMGPROC_H
