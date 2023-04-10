#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>
std::string param_file="./Examples/Monocular/EuRoC.yaml";
std::string vocfile="./Vocabulary/ORBvoc.txt";
cv::VideoCapture cap;


int main(int argc,char* argv[]){
	ORB_SLAM3::System SLAM(vocfile,param_file,ORB_SLAM3::System::MONOCULAR,true);
	if(argc<=1) {std::cout<<"the input param is not enough"<<std::endl; return 0;}
	
	cap.open(argv[1]);
	auto start = chrono::system_clock::now();
	int index = 0;
	while(true){
		cv::Mat image;
		cap>>image;
		if(image.empty()){
			std::cout<<"the video is end"<<std::endl;
			break;
		}
		cv::resize(image,image,cv::Size(752,480));
		cv::rotate(image,image,cv::ROTATE_90_CLOCKWISE);
		auto now = std::chrono::system_clock::now();
		auto timestamp=std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		SLAM.TrackMonocular(image,double(timestamp.count())/1000.0);
	}
	
	while(true){
	if(cv::waitKey(1)==27) break;
	}
	SLAM.Shutdown();
	return 1;
}

