#ifndef CTHREADWORKER_H
#define CTHREADWORKER_H
#include <QThread>
#include "cdetecttrack.h"
#include "cimgcontroller.h"
/*
class ThreadWorker:public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        QString result="this a a separate thread";
        // ... here is the expensive or blocking operation ...
        while(1)
        {
            mutex.lock();

            mutex.unlock();
            cd.detectAndDraw(this->image,1, true);

        }
        emit resultReady(result);
    }
public:

    ThreadWorker(cv::Mat img)
    {

    }
    void setImage(cv::Mat& img)
    {
        this->mutex.lock();
        this->image=img.clone();//deep copy!!!
        this->mutex.unlock();
    }
    void setImage(const cv::Mat& img)
    {
        //this->image=img;
        this->image=img.clone();
    }
private:

    cv::Mat image;
    CDetectTrack cd;
    QMutex mutex;
signals:
    void resultReady(const QString &s);
};
*/
#endif // CTHREADWORKER_H
