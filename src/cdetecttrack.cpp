#include "cdetecttrack.h"
#include <vector>
#include <stdio.h>
#include <QDebug>

using namespace std;
using namespace cv;

CDetectTrack::CDetectTrack()
{
    this->cascade.load("../Resources/ASMLib/haarcascades/haarcascade_frontalface_alt.xml");
    this->nestedCascade.load("../Resources/ASMLib/haarcascades/haarcascade_eye.xml");

}


const static Scalar colors[] =  { CV_RGB(0,0,255),
    CV_RGB(0,128,255),
    CV_RGB(0,255,255),
    CV_RGB(0,255,0),
    CV_RGB(255,128,0),
    CV_RGB(255,255,0),
    CV_RGB(255,0,0),
    CV_RGB(255,0,255)} ;

void CDetectTrack::detectAndDraw(cv::Mat& image,double scale, bool tryflip )
{


    int i = 0;
    double t = 0;
    vector<Rect> faces, faces2;
    cv::Mat img=image.clone();

    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    ///smallSize

    cv::resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );

    equalizeHist( smallImg, smallImg );


    t = (double)cvGetTickCount();

    /*1.1 means decrease the size of the image to 1/1.1 each time,2 means 3 times detected count(varying window)*/
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );

    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 Size(30, 30) );


        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }

    t = (double)cvGetTickCount() - t;

    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    printf( "face num is  %d", faces.size());

    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];// choose different color
        int radius;

        double aspect_ratio = (double)r->width/r->height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                       cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                       color, 3, 8, 0);

    }
    qDebug()<<"detect and draw over";
    image=img.clone();
    cv::imshow("face",img);

}

#include "asmfitting.h"
#include "vjfacedetect.h"
#include "video_camera.h"
using namespace std;

static void DrawResult(IplImage* image, const asm_shape& shape)
{
    for(int j = 0; j < shape.NPoints(); j++)
        cvCircle(image, cvPoint(shape[j].x, shape[j].y), 2, CV_RGB(255, 0, 0));
}

int CDetectTrack::demo_asm(char* model_name, char* cascade_name)
{
    asmfitting fit_asm;
    int camera_idx = 0;//default camera
    int n_iteration=20;
/***************************************************************************************/
    double t = (double)cvGetTickCount();
    if(fit_asm.Read(model_name) == false) return -1;
    t = ((double)cvGetTickCount() -  t )/  (cvGetTickFrequency()*1000.);
    printf("ASM model file read time cost: %.2f millisec\n", t);
    t = (double)cvGetTickCount();
    if(init_detect_cascade(cascade_name) == false)return -1;
    t = ((double)cvGetTickCount() -  t )/  (cvGetTickFrequency()*1000.);
    printf("Opencv haar-like file read time cost: %.2f millisec\n", t);
/************************************************************************************************/


        printf("hi,going to takepicture");
        asm_shape shape, detshape;

        bool flag = false;
        IplImage* image;
        int j = 0;

      if(open_camera(camera_idx) == false)
        {
            fprintf(stderr, "Can not open camera [%d]\n", camera_idx);
            exit(0);
        }

       cvNamedWindow("ASM-Search",1);

while(1)
       {     // NOTE: when the parameter is set 1, we can read from camera
            image = read_from_camera();

            if(flag == false)
            {
                //Firstly, we detect face by using Viola_jones haarlike-detector
                flag = detect_one_face(detshape, image);

                //Secondly, we initialize shape from the detected box
                if(flag)
                {
                    InitShapeFromDetBox(shape, detshape, fit_asm.GetMappingDetShape(), fit_asm.GetMeanFaceWidth());
                    j ++;
                }
                else
                    goto show2;
            }

            //Thirdly, we do image alignment
            flag = fit_asm.ASMSeqSearch(shape, image, j, true, n_iteration);

            if(flag) DrawResult(image, shape); //If success, we draw and show its result
show2:
            cvShowImage("ASM-Search", image);
            cvWaitKey(1);
}

        close_camera();


    return 0;
}
