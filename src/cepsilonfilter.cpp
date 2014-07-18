#include "cepsilonfilter.h"


CepsilonFilter::CepsilonFilter()
{



}


void CepsilonFilter::process(const cv::Mat &image, cv::Mat &result)
{
   // pimage=&image;
   // presult=&result;
   // epsilonFilter();
}



uchar getSquash(uchar input,uchar threshold)
{
   return (input<threshold)?input:0;
}


/*
 *11111
 *11211
 *12421
 *11211
 *11111
*/

uchar CepsilonFilter::getNeighborWeight(int row,int col,int k)
{


    cv::Mat kernel(5,5,CV_32F,cv::Scalar(1));
    kernel.at<float>(1,2)= 2.0/32;
    kernel.at<float>(2,1)= 2.0/32;
    kernel.at<float>(2,2)= 4.0/32;
    kernel.at<float>(2,3)= 2.0/32;
    kernel.at<float>(3,2)= 2.0/32;

    uchar epsilon=20;

    //

    //int N=kernel.size[1];
    int N=2;
    float temp=0;
    cv::Mat_<cv::Vec3b>im2=*pimage;
    cv::Mat_<cv::Vec3b>rst2=*presult;

    for(int k1=-N;k1<N;k1++)
        for(int k2=-N;k2<N;k2++)
        {
          temp+=kernel.at<float>(N+k1,N+k2)*getSquash(im2(row+k1,col+k2)[k]-im2(row,col)[k],epsilon);
        }
    return uchar(temp);
}

/*uchar CepsilonFilter::getNeighborWeight(int row,int col,int k)//this will generate magic,since it use laplacine
{

    cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
    kernel.at<float>(1,1)= 5.0;
    kernel.at<float>(0,1)= -1.0;
    kernel.at<float>(2,1)= -1.0;
    kernel.at<float>(1,0)= -1.0;
    kernel.at<float>(1,2)= -1.0;

    uchar epsilon=3;
    //
    int N=2;

    uchar temp=0;
    cv::Mat_<cv::Vec3b>im2=*pimage;
    cv::Mat_<cv::Vec3b>rst2=*presult;

    for(int k1=-N;k1<N;k1++)
        for(int k2=-N;k2<N;k2++)
        {
          temp+=kernel.at<float>(k1,k2)*getSquash(im2(row+k1,col+k2)[k]-im2(row,col)[k],epsilon);
        }
}
*/
int n=3;
void CepsilonFilter::epsilonFilter()//
{
    if(pimage->channels()==1)
    {
    }//if
    else if(pimage->channels()==3)
    {
        cv::Mat_<cv::Vec3b>im2=*pimage;
        cv::Mat_<cv::Vec3b>rst2=*presult;
        for (int j= n; j<im2.rows-n; j++) { // for all rows
                    for (int i=n; i<im2.cols-n; i++)   //iterate the row:
                    {
                            for(int k=0;k<3;k++)
                            {
                                uchar temp=getNeighborWeight(j,i,k);
                                rst2(j,i)[k]= im2(j,i)[k]+temp;//
                            }
                    }
            }
    }//elseif
    else;
}


