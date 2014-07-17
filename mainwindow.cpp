#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <cwebcam.h>

#define BEFORE_CPU_COUNT  double duration;duration = static_cast<double>(cv::getTickCount());
#define AFTER_CPU_COUNT    duration = static_cast<double>(cv::getTickCount())-duration;duration /= cv::getTickFrequency();qDebug()<<"exe time is"<<duration;


using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    this->bilateral_kernel_len=7;
    this->cascade.load("../haarcascades/haarcascade_frontalface_alt.xml");
   // this->nestedCascade.load("../haarcascades/haarcascade_eye.xml");
}

MainWindow::~MainWindow()
{
    delete this->pwebcam;
    delete ui;
    delete pcap;
}



void MainWindow::on_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("sun open image"),".",tr("Images files(*.png *.jpg *.jpeg *.bmp)"));
    image=cv::imread(fileName.toStdString());
    //cv::cvCvtColor(&img,&img,CV_BGR2RGB);//#include<opencv2/imgproc/imgproc.hpp>
    this->rst=image.clone();
    displayInImg(image);

}

void MainWindow::displayInImg(cv::Mat& image)
{
    //QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);//without the step parameter,the image couldn't be displayed right
    QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_RGB888);
    QImage dstImg = img.rgbSwapped();
    // display on label
    ui->label_input->setPixmap(QPixmap::fromImage(dstImg));
    // resize the label to fit the image
    ui->label_input->resize(ui->label_input->pixmap()->size());

    if(image.data)
    {
        ui->pushButton_2->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    //CepsilonFilter hh;
    //hh.process(image,dst);
    /************************************************/
    BEFORE_CPU_COUNT
    bilateralFilter(image,rst,this->bilateral_kernel_len,bilateral_kernel_len*2,bilateral_kernel_len/2);
    AFTER_CPU_COUNT
    /**************************************************/
    //cvSmooth(image,rst,cv::CV_BILATERAL);
    //cvSmooth(const CvArr* src, CvArr* dst, int smoothtype=CV_BILATERAL, int size1=3, int size2=0, double    sigma1=0, double sigma2=0);
    //pstrategy=static_cast<CImgProc*>(new CepsilonFilter());
    //pstrategy->process(image,image);
    qDebug("process Ok");
    displayOutImg(&rst);
    //salt(image,3000);
    //sharpen(image,image);
    //cv::flip(image,image,1);
    //change color channel ordering
    //cv::cvtColor(image,image,CV_BGR2RGB);
}

void MainWindow::displayOutImg(cv::Mat* prst)
{

    //QImage
    QImage img= QImage((const unsigned char*)(prst->data),prst->cols,prst->rows,prst->step,QImage::Format_RGB888);
    QImage dstImg = img.rgbSwapped();
    // display on label
    ui->label_output->setPixmap(QPixmap::fromImage(dstImg));
    // resize the label to fit the image
    ui->label_output->resize(ui->label_output->pixmap()->size());


    qDebug()<<"display ok";

}


int demo_asm(char* model_name, char* cascade_name);

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

    qDebug("comboBox");

    // open the default camera
    pcap=new VideoCapture(0);
    if(!pcap->isOpened())  // check if we succeeded
        return;

    this->rst=this->image.clone();

    //this->exec();
    QTimer *timer=new QTimer(this);
    timer->setInterval(100);
    connect(timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    timer->start();

    //demo_main();


}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    this->bilateral_kernel_len=ui->horizontalSlider->value()/4;
    /************************************************/
    //BEFORE_CPU_COUNT
    //bilateralFilter(image,rst,bilateral_kernel_len,bilateral_kernel_len*2,bilateral_kernel_len/2);
    //AFTER_CPU_COUNT
    /**************************************************/
    //displayOutImg(&rst);
    qDebug()<<"slider"<<bilateral_kernel_len;
}

void MainWindow::nextFrame()
{



          *pcap >> (this->image);//frame; // get a new frame from camera
           //qDebug()<<image.rows;
           //qDebug()<<image.cols;


           displayInImg(this->image);


           BEFORE_CPU_COUNT
           double scale=1;
           bool tryflip=false;

           detectAndDraw( this->image, cascade, nestedCascade, scale, tryflip );
           //bilateralFilter(image,rst,bilateral_kernel_len,2*bilateral_kernel_len,bilateral_kernel_len/2);
           //detectAndDraw( frameCopy, cascade, nestedCascade, scale, tryflip );
           //displayInImg(*(this->psharedImg));

           //cvNamedWindow("ASM-Search",1);
           //cv::imshow("ASM-Search",this->image);
           displayOutImg(&(this->rst));
           AFTER_CPU_COUNT





}


/*
class MyThread:public QThread
{
   public:

}*/



void MainWindow::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
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



#include "asmfitting.h"
#include "vjfacedetect.h"
#include "video_camera.h"




using namespace std;



static void DrawResult(IplImage* image, const asm_shape& shape)
{
    for(int j = 0; j < shape.NPoints(); j++)
        cvCircle(image, cvPoint(shape[j].x, shape[j].y), 2, CV_RGB(255, 0, 0));
}

int demo_asm(char* model_name, char* cascade_name)
{
    asmfitting fit_asm;
    int camera_idx = 0;


    double t = (double)cvGetTickCount();
    if(fit_asm.Read(model_name) == false) return -1;
/***************************************************************************************/
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

      //  cvNamedWindow("ASM-Search",1);


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
           // flag = fit_asm.ASMSeqSearch(shape, image, j, true, n_iteration);
            int a;
            int b;
            shape.GetLeftRight(a,b);
            //If success, we draw and show its result
           // if(flag) DrawResult(image, shape);
show2:
           // cvShowImage("ASM-Search", image);
           // cvWaitKey(1);
            int iii=1;


        close_camera();


    return 0;
}



