/*
 * ASMLibrary(Active Shape Model Library) -- A compact SDK for face alignment
 *
 * Copyright (c) 2008-2011, Yao Wei <njustyw@gmail.com>,
 * http://visionopen.com/members/yaowei/, All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

/**@file 
A demo show how to do image alignment (face tracking) using active shape model 

Please cite the following or equivalent reference in any publicly available 
text that uses asmlibrary: 

YAO Wei. Research on Facial Expression Recognition and Synthesis.
<EM> Master Thesis, Department of Computer Science and Technology, 
Nanjing University</EM>, Feb 2009. \url http://code.google.com/p/asmlibrary

@version 6.0-2010-5-30
*/

#include <vector>
#include <string>
#include <iostream>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include "asmfitting.h"
#include "vjfacedetect.h"
#include "video_camera.h"

using namespace std;

#ifdef WIN32
#define HAS_GSTREAMER
#endif


#define HAS_GSTREAMER


static void print_version()
{
	printf("\n\n"
		   "/**************************************************************/\n"
		   "/*      ASMLibrary -- A compact SDK for face alignment        */\n"
		   "/*  Copyright (c) 2008-2011 by Yao Wei, all rights reserved.  */\n"
		   "/*                    Contact: njustyw@gmail.com              */\n"
		   "/**************************************************************/\n"
		   "\n\n");
}

static void usage_fit()
{
	printf("Usage: fit -m model_file -h cascade_file "
		"{-i image_file | -v video_file | -c camera_idx} -n n_iteration\n\n\n");
	exit(0);
}


static void DrawResult(IplImage* image, const asm_shape& shape)
{
	for(int j = 0; j < shape.NPoints(); j++)
		cvCircle(image, cvPoint(shape[j].x, shape[j].y), 2, CV_RGB(255, 0, 0));
}

int demo_main(char* model_name, char* cascade_name)
{
	asmfitting fit_asm;


	char* filename = NULL;
	int use_camera = 0;
	int image_or_video = -1;
	int i;
	int n_iteration = 20;
	int camera_idx = 0;


	double t = (double)cvGetTickCount();
	if(fit_asm.Read(model_name) == false)
		return -1;
	t = ((double)cvGetTickCount() -  t )/  (cvGetTickFrequency()*1000.);
	printf("ASM model file read time cost: %.2f millisec\n", t);
	
	t = (double)cvGetTickCount();
	if(init_detect_cascade(cascade_name) == false)
		return -1;
	t = ((double)cvGetTickCount() -  t )/  (cvGetTickFrequency()*1000.);
	printf("Opencv haar-like file read time cost: %.2f millisec\n", t);
	


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
		{
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
			
			//If success, we draw and show its result
			if(flag) DrawResult(image, shape);
show2:
			cvShowImage("ASM-Search", image);
			cvWaitKey(1);
		}

		close_camera();


    return 0;
}



