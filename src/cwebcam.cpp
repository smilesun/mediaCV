#include "cwebcam.h"
/*
CWebCam::CWebCam()
{
    isCamera=false;
}

void CWebCam::nextFrame()
{

    //this->Matframe = cvQueryFrame(capture);
    //this->pwin->displayInImg(*Matframe);

}

void CWebCam::exec()
{
    /*VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
           return;
    for(;;)
    {
           Mat frame;
           cap >> frame; // get a new frame from camera
           Mat rst=frame.clone();

           double duration;
           duration = static_cast<double>(cv::getTickCount());
           int r=40;

           bilateralFilter(frame,rst,r,2*r,r/2);

           duration = static_cast<double>(cv::getTickCount())-duration;
           duration /= cv::getTickFrequency(); // the elapsed time in ms
           qDebug()<<"exe time is"<<duration;

    }

}
*/
/*
void CWebCam::exec()
{
    capture = cvCaptureFromCAM(0);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);

    if(capture)
    {
        QMessageBox::information(this,"Information","successful!");
        if (capture)
        {
            this->Matframe = cvQueryFrame(capture);
            timer = new QTimer(this);
            timer->setInterval(30);
            connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
            timer->start();
            isCamera = true;
        }
    }
    else
    {
        QMessageBox::information(this,"Information","Sorry,fail!");
        isCamera = false;
    }
}

void CWebCam::store()
{
    capture = cvCaptureFromCAM(0);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);
    cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);
    if(capture)
    {
        QMessageBox::information(this,"Information","successful!");
        if (capture)
        {
            frame = cvQueryFrame(capture);
            if (frame)
                this->resize(frame->width,frame->height);
            qImg = new QImage(QSize(frame->width,frame->height),QImage::Format_RGB888);

            iplImg = cvCreateImageHeader(cvSize(frame->width,frame->height),8,3);
            iplImg->imageData = (char*)qImg->bits();
            timer = new QTimer(this);
            timer->setInterval(30);
            connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
            timer->start();
            writer = cvCreateVideoWriter("out.avi",CV_FOURCC('D', 'I', 'V', 'X'),10,cvSize(frame->width,frame->height),1);
            isCamera = true;
        }
    }
    else
    {
        QMessageBox::information(this,"Information","Sorry,fail!");
        isCamera = false;
    }
}

void CWebCam::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(isCamera == true)
    {
        painter.drawImage(QPoint(0,0),*qImg);
    }
    else
    {

    }
}

void CWebCam::writeNextFrame()
{

    frame = cvQueryFrame(capture);

    if (frame)
    {
        if (frame->origin == IPL_ORIGIN_TL)
        {
            cvCopy(frame,iplImg,0);
        }
        else
        {
            cvFlip(frame,iplImg,0);
        }
        //cv::cvtColor(iplImg,iplImg,CV_BGR2RGB);
        cvWriteFrame(writer,frame);
        this->update();
    }

}
*/

