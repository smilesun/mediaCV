#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this,tr("sun open image"),".",tr("Images files(*.png *.jpg *.jpeg *.bmp)"));
    image=cv::imread(fileName.toStdString());
    //cv::cvCvtColor(&img,&img,CV_BGR2RGB);

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
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    CepsilonFilter hh;
    hh.process(image,image);
    //pstrategy=static_cast<CImgProc*>(new CepsilonFilter);
    //pstrategy->process(image,image);

    // salt(image,3000);
    //sharpen(image,image);
    // cv::flip(image,image,1);
    // change color channel ordering
    //cv::cvtColor(image,image,CV_BGR2RGB);

    //QImage
    QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_RGB888);
    QImage dstImg = img.rgbSwapped();
    // display on label
    ui->label_output->setPixmap(QPixmap::fromImage(dstImg));
    // resize the label to fit the image
    ui->label_output->resize(ui->label_output->pixmap()->size());
    //delete pstrategy;

}

void MainWindow::convert_video(char* infilename,char* outfilename)
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



void MainWindow::salt(cv::Mat &image,int n)
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

void MainWindow::sharpen(const cv::Mat &image, cv::Mat &result)//laplasian

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





void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

}
