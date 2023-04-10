#ifndef CALIB_HPP
#define CALIB_HPP
#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

#define Debug std::cout<<"Debug"<<std::endl;
/*@brief the 
* @param _image1 the first iuput image 
* @param _image2 the second input image
* @param concat  The fused image after merging two images.
*/ 
void concat(cv::Mat& _image1,cv::Mat& _image2,cv::Mat& concat);

int calibrate(std::string left_txt, std::string right_txt,cv::Size chessboard,std::string xml_path,std::vector<int> params);

bool qiege(std::string picure,int size,std::string path1,std::string path2);

/*
@param prefix_dir the path of images 
@param save_dir the save path of left and right images 
@param number the images quantities
@param colsize the size you need
*/
bool picure_source(std::string prefix_dir,std::string save_dir,int number,int colsize);


/*
for example:
./main qiege /home/robot/dataset/zed /home/robot/dataset/zed/test 8 1280
the picures are /home/robot/dataset/zed/calib*.png
the left images are /home/robot/dataset/zed/test/left/calib*.png
the right images are /home/robot/dataset/zed/test/right/calib*.png
the left txt are /home/robot/dataset/zed/test/left.txt
the right txt are /home/robot/dataset/zed/test/right/.txt
*/

#endif 
