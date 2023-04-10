#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<chrono>
#include<opencv2/core/core.hpp>
#include<System.h>
#include<assert.h>



/*
这里有两种使用方法：
	第一种就是把你没有切割的视频直接放进去就可以:
	@param argv[1] first
	@param argv[2] 视频路径
	@param argv[3] 切割的宽
	@param argv[4] 参数路径
	@param argv[5] 词典路径
	第二种就是用左右视频分开的路径输入:
	@param argv[1] second
	@param argv[2] 左视频路径
	@param argv[3] 右视频路径
	@param argv[4] 参数路径
	@param argv[5] 词典路径
同时有默认参数:
1. example first :

./stereo first /home/robot/dataset/zed/video/data.avi (672 ./param.xml ./Vocabulary/orbvoc.txt)

2. example second :

./stereo second /home/robot/dataset/zed/video/leftdata.avi   /home/robot/dataset/zed/video/rightdata.avi (./param.xml ./Vocabulary/orbvoc.txt)

附记：
()中的是可选参数 ,你可以调整默认参数来减少输入的参数量.
*/
//切割默认参数
int cols = 672;
//其他默认参数
std::string videopath;
std::string leftvideopath,rightvideopath;
//共有默认参数
std::string param_file = "./testsmall.yaml";
std::string vocfile = "./Vocabulary/ORBvoc.txt";


/*
这里主要是视频流获取
*/
cv::VideoCapture cap;
cv::VideoCapture leftcap;
cv::VideoCapture rightcap;


int show_first(int argc,char* argv[]){
	
	assert(argc>2);
	std::string vocpath = vocfile;
	std::string parampath = param_file ;
	int colsize = cols;
	cap.open(argv[2]);
	if(argc == 6){
		colsize = atoi(argv[3]);
		parampath = std::string(argv[4]);
		vocpath = std::string(argv[5]);
	}else if(argc == 5){
		colsize = atoi(argv[3]);
		parampath = std::string(argv[4]);
	}else if(argc == 4){
		colsize = atoi(argv[3]);
	}

	std::cout<<"the video path : "<< argv[2]<<std::endl;
	std::cout<<"the qiege col size : "<< colsize <<std::endl;
	std::cout<<"the param path : "<< parampath <<std::endl;
	std::cout<<"the voc path : "<< vocpath <<std::endl;

	if(!cap.isOpened()){
	std::cout<<"the cap is not opened"<<std::endl;
	return 1;}
	ORB_SLAM3::System SLAM(vocpath,parampath,ORB_SLAM3::System::STEREO,true);
	
	auto start = chrono::system_clock::now();
	
	cv::Mat image,left,right;
	int index = 0;
	
	while(cap.isOpened()){
		cap>>image;
		index ++;
		if(image.empty()){
			std::cout<<"the video is end"<<std::endl;
			break;
		}
		if(cv::waitKey(1)==27){
			SLAM.Shutdown();
			break;
		}
		left = image.colRange(cv::Range(0,colsize)).clone();
		right = image.colRange(cv::Range(colsize,2*colsize)).clone();
		//cv::rotate(left,left,1);
		//cv::rotate(right,right,1);
		//cv::imshow("left",left);
		auto now = std::chrono::system_clock::now();
		auto timestamp=std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		SLAM.TrackStereo(left,right,double(timestamp.count())/1000.0);
	}
	
	cap.release();
	left.release();
	right.release();
	image.release();
	std::cout<<"just press the exc that you can exit"<<std::endl;
	while(true){
	if(cv::waitKey(1)==27) {
	SLAM.Shutdown();
	break;
	}
	}
	return 1;
}

int show_second(int argc,char* argv[]){
	assert(argc>3);
	std::string vocpath  = vocfile;
	std::string parampath = param_file;
	leftcap.open(argv[2]);
	rightcap.open(argv[3]);
	if(argc == 6){
		parampath = std::string(argv[4]);
		vocpath = std::string(argv[5]);
	}else if(argc == 5){
		parampath = std::string(argv[4]);
	}
	std::cout<<"the left video path : "<< argv[2]<<std::endl;
	std::cout<<"the right video path : "<< argv[3]<<std::endl;
	std::cout<<"the param path : "<< parampath <<std::endl;
	std::cout<<"the voc path : "<< vocpath <<std::endl;
	/*
	(!A)+(!B) = !(AB) 摩根定律
	*/
	if(!(leftcap.isOpened() && rightcap.isOpened())){
	std::cout<<"the cap is not opened"<<std::endl;
	return 1;
	}

	ORB_SLAM3::System SLAM(vocpath,parampath,ORB_SLAM3::System::STEREO,true);
	auto start = chrono::system_clock::now();
	cv::Mat left,right;
	int index = 0;
	
	while(true){
		leftcap>>left;
		rightcap>>right;
		cv::imshow("left",left);
		cv::imshow("right",right);
		if(cv::waitKey(1)==27)break;
		index++;
		if(left.empty() || right.empty()){
			std::cout<<"the video is end"<<std::endl;
			break;
		}
		auto now = std::chrono::system_clock::now();
		auto timestamp=std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		SLAM.TrackStereo(left,right,double(timestamp.count())/1000.0);
	}
	
	cap.release();
	left.release();
	right.release();
	std::cout<<"just press the exc that you can exit"<<std::endl;
	while(true){
	if(cv::waitKey(1)==27) {
	SLAM.Shutdown();
	break;
	}
	}
	return 1;
}


int issecond = 0;
int main(int argc,char* argv[]){

	if(argc<2) return 0;
	if(std::string(argv[1])=="second") issecond = 1;

	switch (issecond)
	{
		case 0:{
		std::cout<<"the pattern is set to the first pattern"<<std::endl;
		show_first(argc,argv);
		break;
		}
		case 1:	{
		std::cout<<"the pattern is set to the second pattern"<<std::endl;
		show_second(argc,argv);
			break;
		}
		default:break;
	}
	return 1;
}

