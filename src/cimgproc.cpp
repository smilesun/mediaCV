#include "cimgproc.h"
#include "cdetecttrack.h"
#include "cimgcontroller.h"
QMutex mutex;//syncronize CImgProc and
cv::Mat sharedImg;//syyncr
bool flag_begin=false;

CImgProc::CImgProc()
{
    // t_dt.start();
}

void CImgProc::doEnhanceOnHeadROI(const cv::Mat &image, cv::Mat &result)
{
    //YcbcrEqualize(image,result);
    //RgbEqualize(image,result);
     cv::Mat temp=image.clone();
     cv::Rect roi;
     if(!c_d_t.detectAndDraw(temp,1,true,roi)) return;

     qDebug()<<"roi is "<<roi.x<<" "<<roi.y<<" "<<roi.width<<" "<<roi.height;
     cv::Mat mroi=temp(roi);
     cv::Mat rroi=result(roi);


     //Mat rroi(result, roi);

     //updateBrightnessContrast(temp,CImgController::getInstance()->context._constrast,CImgController::getInstance()->context._brightness,result);

     updateBrightnessContrast(mroi,CImgController::getInstance()->context._constrast,CImgController::getInstance()->context._brightness,rroi);
    //rroi = Scalar(0, 255, 0);

    //updateBrightnessContrast(mroi,200,190,rroi);

    //result(roi)=rroi;
     //RgbEqualize(mroi,rroi);

    qDebug()<<"over"<<endl;


}

void CImgProc::processOnePicture(const cv::Mat &image, cv::Mat &result)
{

   doEnhanceOnHeadROI(image,result);
}

void CImgProc::YcbcrEqualize(const cv::Mat &image, cv::Mat &result)
{

    cv::Mat hsv;

    cv::cvtColor(image,hsv,CV_BGR2YCrCb);

    std::vector<cv::Mat> sbgr(hsv.channels());
    std::vector<cv::Mat> eq(hsv.channels());

    cv::split(hsv,sbgr);

    //for(int i=0;i<image.channels();i++)
      //  cv::equalizeHist(sbgr[i],eq[i]);
      eq[1]=sbgr[1].clone();
      eq[2]=sbgr[2].clone();
      cv::equalizeHist(sbgr[0],eq[0]);

    cv::Mat hsv2;
    cv::merge(eq,hsv2);
    cv::cvtColor(hsv2,result,CV_YCrCb2BGR);

}
void CImgProc::RgbEqualize(const cv::Mat &image, cv::Mat &result)
{


    std::vector<cv::Mat> sbgr(image.channels());
    std::vector<cv::Mat> eq(image.channels());
    cv::split(image,sbgr);
    for(int i=0;i<image.channels();i++)
        cv::equalizeHist(sbgr[i],eq[i]);
    cv::merge(eq,result);

}
void CImgProc::HsvEqualize(const cv::Mat &image, cv::Mat &result)
{

    cv::Mat hsv;

    cv::cvtColor(image,hsv,CV_BGR2HSV);

    std::vector<cv::Mat> sbgr(hsv.channels());
    std::vector<cv::Mat> eq(hsv.channels());

    cv::split(hsv,sbgr);

    //for(int i=0;i<image.channels();i++)
      //  cv::equalizeHist(sbgr[i],eq[i]);
      eq[1]=sbgr[1].clone();
      eq[2]=sbgr[2].clone();
      cv::equalizeHist(sbgr[0],eq[0]);

    cv::Mat hsv2;
    cv::merge(eq,hsv2);
    cv::cvtColor(hsv2,result,CV_HSV2BGR);

}

void CImgProc::processStream(const cv::Mat &image, cv::Mat &result)
{
    mutex.lock();
    qDebug()<<"in mutex in thread process";
        sharedImg=image.clone();
        flag_begin=true;
    mutex.unlock();
    qDebug()<<"out of  mutex in thread process";


}


void CImgProc::getColorRange()
{
    cv::Mat_<cv::Vec3b>::iterator it=
    inMat.begin<cv::Vec3b>();
    // obtain end position
    cv::Mat_<cv::Vec3b>::iterator itend=
    inMat.end<cv::Vec3b>();
    // loop over all pixels
    for ( ; it!= itend; ++it) {}
    // process each pixel ---------------------(*it)[0]= (*it)[0]/div*div + div/2;
}

void CImgProc::convert_video(char* infilename,char* outfilename)
{

    CvCapture* capture=0;
    capture=cvCreateFileCapture(infilename);
    if(!capture)
    {
        return;
    }
    IplImage *bgr_frame=cvQueryFrame(capture);//init the video read
    double fps=cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
    CvSize size=cvSize((int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH),cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT));
    CvVideoWriter *writer=cvCreateVideoWriter(outfilename,CV_FOURCC('M','J','P','G'),fps,size);//choose 'MJPG'codec,choose replay rate to be fps2
    IplImage* logpolar_frame=cvCreateImage(size,IPL_DEPTH_8U,3);
    while((bgr_frame=cvQueryFrame(capture))!=NULL)
    {

        //cv::cvLogPolar(bgr_frame,logpolar_frame,cvPoint2D32f(bgr_frame->width/2,bgr_frame->height/2),40,cv::CV_INTER_LINEAR+cv::CV_WARP_FILL_OUTLIERS);
        cvWriteFrame(writer,bgr_frame);
    }
    cvReleaseVideoWriter(&writer);
    cvReleaseImage(&logpolar_frame);
    cvReleaseCapture(&capture);
    return;
}



void CImgProc::salt(cv::Mat &image,int n)
{
    for(int k=0;k<n;k++)
      {
        int i =qrand()%image.cols;
        int j=qrand()%image.rows;
        if(image.channels()==1)
        {
            image.at<uchar>(j,i)=255;

        }
        else if(image.channels()==3)
        {
            image.at<cv::Vec3b>(j,i)[0]=255;
            image.at<cv::Vec3b>(j,i)[1]=255;
            image.at<cv::Vec3b>(j,i)[2]=255;
        }

    }

}

void CImgProc::sharpen(const cv::Mat &image, cv::Mat &result)//laplasian

{

    // allocate if necessary
    if((&result)!=(&image))
    {
        result.create(image.size(), image.type());
    }
    if(image.channels()==1)
    {
    for (int j= 1; j<image.rows-1; j++) { // for all rows
                // (except first and last)
                const uchar* previous=
                        image.ptr<const uchar>(j-1); // previous row
                const uchar* current=
                        image.ptr<const uchar>(j); // current row
                const uchar* next=
                        image.ptr<const uchar>(j+1); // next row

                // output at
                uchar* output= result.ptr<uchar>(j); // output row

                for (int i=1; i<image.cols-1; i++) {//iterate the row:
                    *output++= cv::saturate_cast<uchar>(5*current[i]-current[i-1]-current[i+1]-previous[i]-next[i]);
                }
        }
        // Set the unprocess pixels to 0
        result.row(0).setTo(cv::Scalar(0));
        result.row(result.rows-1).setTo(cv::Scalar(0));
        result.col(0).setTo(cv::Scalar(0));
        result.col(result.cols-1).setTo(cv::Scalar(0));

    }//if
    else if(image.channels()==3)
    {

        cv::Mat_<cv::Vec3b>im2=image;
        cv::Mat_<cv::Vec3b>rst2=result;
        for (int j= 1; j<image.rows-1; j++) { // for all rows
                    for (int i=1; i<image.cols-1; i++) {//iterate the row:
                        for(int k=0;k<3;k++)
                        rst2(j,i)[k]= cv::saturate_cast<uchar>(5*im2(j,i)[k]-im2(j,i-1)[k]-im2(j,i+1)[k]-im2(j-1,i)[k]-im2(j+1,i)[k]);

                    }
            }
    }//elseif
    else;

}


/*void CImgProc::colorEnhanceBrightnessContrast()
{
    std::vector<cv::Mat> sbgr(image.channels());
    std::vector<cv::Mat> eq(image.channels());
    cv::split(image,sbgr);
    for(int i=0;i<image.channels();i++)
          updateBrightnessContrast(sbgr[i],);
    cv::merge(eq,result);
}*/


void CImgProc::updateBrightnessContrast(cv::Mat&image,int _brightness,int _contrast,cv::Mat& rst)
{
    int histSize = 64;
    int brightness = _brightness - 100;
    int contrast = _contrast - 100;

    /*
     * The algorithm is by Werner D. Streidt
     * (http://visca.com/ffactory/archives/5-99/msg00021.html)
     */
    double a, b;
    if( contrast > 0 )
    {
        double delta = 127.*contrast/100;
        a = 255./(255. - delta*2);
        b = a*(brightness - delta);
    }
    else
    {
        double delta = -128.*contrast/100;
        a = (256.-delta*2)/255.;
        b = a*brightness + delta;
    }

  //  Mat dst, hist;
    image.convertTo(rst, CV_8U, a, b);
    qDebug("convert ok");
   // rst=dst.clone();
  //  qDebug("clone ok");
 // a*x+b
   // imshow("image", dst);

  /*  calcHist(&dst, 1, 0, Mat(), hist, 1, &histSize, 0);
    Mat histImage = Mat::ones(200, 320, CV_8U)*255;

    normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);

    histImage = Scalar::all(255);
    int binW = cvRound((double)histImage.cols/histSize);

    for( int i = 0; i < histSize; i++ )
        rectangle( histImage, Point(i*binW, histImage.rows),
                   Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                   Scalar::all(0), -1, 8, 0 );
    imshow("histogram", histImage);
    */
}

