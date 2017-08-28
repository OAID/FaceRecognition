#include <iostream>
#include <opencv2/opencv.hpp>

extern "C" {
   #include "scale_angle.h"
}

int get_aligned_face(const cv::Mat& img, float* landmark, int landmark_number, int desired_size,cv::Mat& out)
{
   
   float scale;
   float angle;
   float from_center[2];
   float to_center[2];


   if(cal_scale_and_angle(landmark,landmark_number,desired_size,&scale,&angle)<0)
   {
        return -1;
   }

   to_center[0]=desired_size*0.4;
   to_center[1]=desired_size*0.5;

   from_center[0]=(landmark[0]+landmark[1])/2;
   from_center[1]=(landmark[5]+landmark[6])/2;

   cv::Mat rot_mat=cv::getRotationMatrix2D(cv::Point2f(from_center[0],from_center[1]),-1*angle,scale);

   float ex=to_center[0]-from_center[0];
   float ey=to_center[1]-from_center[1];

   rot_mat.at<double>( 0,2)+= ex;
   rot_mat.at<double>(1,2) += ey;

   cv::warpAffine(img,out,rot_mat,cv::Size(desired_size,desired_size));

   return 0;

}
