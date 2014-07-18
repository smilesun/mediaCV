#include "cimgproc.h"

CImgProc::CImgProc(QObject *parent) :
    QObject(parent)
{
    this->cascade.load("../haarcascades/haarcascade_frontalface_alt.xml");
    this->nestedCascade.load("../haarcascades/haarcascade_eye.xml");
}

void CImgProc::process(const cv::Mat &image, cv::Mat &result)
{

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

/*
void CImgProc::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
    using namespace std;
    #include <stdio.h>
    int i = 0;
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    cv::resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
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

    Rect rec;


    vector<Rect>::const_iterator r = faces.begin();
    //for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    if(r!=faces.end())
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
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


//        cvRound(r->x*scale), cvRound(r->y*scale),cvRound(r->width-1)*scale, cvRound(r->height-1)*scale;

       cv::Mat roi=this->image(cv::Rect(r->x,r->y,r->width,r->height));
       cv::Mat rstRoi=this->rst(cv::Rect(r->x,r->y,r->width,r->height));

       bilateralFilter(roi,rstRoi,bilateral_kernel_len,2*bilateral_kernel_len,bilateral_kernel_len/2);
       qDebug()<<"bilateral over";
       //*psharedImg=this->rst.clone();
       //cvShowImage("ASM-Search", rst);



    }
 qDebug()<<"detect and draw over";

}
*/

#include "asmfitting.h"
#include "vjfacedetect.h"
#include "video_camera.h"




using namespace std;



static void DrawResult(IplImage* image, const asm_shape& shape)
{
    for(int j = 0; j < shape.NPoints(); j++)
        cvCircle(image, cvPoint(shape[j].x, shape[j].y), 2, CV_RGB(255, 0, 0));
}

int CImgProc::demo_asm(char* model_name, char* cascade_name)
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


            // NOTE: when the parameter is set 1, we can read from camera
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


        close_camera();


    return 0;
}



