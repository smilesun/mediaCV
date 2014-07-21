/*sunxd*/
#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>


#include <cv.h>
#include <highgui.h>

#include "asmfitting.h"
#include "vjfacedetect.h"
#include "video_camera.h"

using namespace std;

#ifdef WIN32
#define HAS_GSTREAMER
#endif


#define HAS_GSTREAMER

#include <math.h>


const static cv::Scalar colors[] =  { CV_RGB(0,0,255),
	CV_RGB(0,128,255),
	CV_RGB(0,255,255),
	CV_RGB(0,255,0),
	CV_RGB(255,128,0),
	CV_RGB(255,255,0),
	CV_RGB(255,0,0),
	CV_RGB(255,0,255)} ;




static cv::Scalar color = colors[1];
static int sflag=false;
static asm_shape last_shape;


class CFacialBeauty
{

private:
	int bilateral_kernel_len;
        bool flag_draw_rect;
         
public:

        CFacialBeauty(){
      	bilateral_kernel_len=7;
        flag_draw_rect=true;
        

        }
        void setBilateral_kernel_len(int len)
        {
         if(len>0)  bilateral_kernel_len=len;
        }
        void disableShowRect(){flag_draw_rect=false;}
        void enableShowRect(){flag_draw_rect=true;}

	void beauty(asm_shape shape,cv::Mat &img,cv::Mat &rst,int bilateral_kernel_len,bool flag)// output,输入图像被处理完后一定要拷贝回输入图像

	{ 
		// should handle when shape is not defined;
		//shape.COG(&dx,&dy);
		int width=200; //preset value
		int height=300;//preset value
		int xmargin=50;
		int ymargin=200;
		int lx=50;
		int ly=100;


		if(flag) 
		{
			sflag=true;
			last_shape=shape;

		}

		/*************************************************************/ 
		if(sflag)
		{ 

			lx=last_shape[1].x;
			ly=last_shape[1].y;	
			int rx=last_shape[11].x;
			int ry=last_shape[11].y;
			int tx=last_shape[14].x;
			int ty=last_shape[14].y; 
			int bx=last_shape[6].x;
			int by=last_shape[6].y;
			width=rx-lx;            
			height=by-ty;
		}
		/**************************************************************/

		int rectWidth=abs(1.5*width);
		int rectHeight=abs(2*height);
		int rectLx=lx-xmargin;
		int rectLy=ly-ymargin;

		CvPoint cpTL;
		cpTL.x=rectLx;
		cpTL.y=rectLy;
		CvPoint cpBR;
		cpBR.x=rectLx+rectWidth;
		cpBR.y=rectLy+rectHeight;
		CvArr* s=(CvArr*)&img;
                if(flag_draw_rect) cv:rectangle(img, cpTL,cpBR,color,3,8,0);

   //           cvRectangle(s, cpTL,cpBR,color,3,8,0);

   printf("org face width is %d, org face height is %d",width,height);
   printf("rect width is %d, rect height is %d",rectWidth,rectHeight);
   printf("image width is %d, image height is %d",rst.cols,rst.rows);

   assert((rectHeight>0)&(rectWidth>0));

   if((rectLx)>0&&(rectLy>0)&&(rectLx+rectWidth<img.cols)&&(rectLy+rectHeight<img.rows))
	   bilateralFilter(img(cv::Rect(rectLx,rectLy,rectWidth,rectHeight)),rst(cv::Rect(rectLx,rectLy,rectWidth,rectHeight)),bilateral_kernel_len,2*bilateral_kernel_len,bilateral_kernel_len/2);

	}


	cv::Mat beautyWrapper12(asm_shape shape, IplImage* image,bool flag)
	{
		//cvSetImageROI(image,rect);
		// cv::rectangle();
		cv::Mat img(image);
		cv::Mat rst=img.clone();
		//IplImage ip;
		beauty(shape,img,rst,bilateral_kernel_len,flag);
		// ip=rst; //这样写会挂掉??why
		return rst;
	}




	IplImage beautyWrapper1(asm_shape shape, IplImage* image,bool flag)
	{
		//cvSetImageROI(image,rect);
		// cv::rectangle();
		cv::Mat img(image);
		cv::Mat rst=img.clone();
		IplImage ip;
		beauty(shape,img,rst,bilateral_kernel_len,flag);
		ip=rst; //这样写会挂掉??why
		return ip;
	}

	cv::Mat beautyWrapper2(asm_shape shape, cv::Mat img,bool flag)
	{
		//cvSetImageROI(image,rect);
		// cv::rectangle();
		cv::Mat rst=img.clone();
		//IplImage ip;
		//ip=rst;
		beauty(shape,img,rst,bilateral_kernel_len,flag);
		//*ip=rst; //这样写会挂掉??why
		return rst;
	}

};
