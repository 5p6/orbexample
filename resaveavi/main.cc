
#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<vector>

cv::VideoCapture cap;
cv::VideoWriter leftwriter;
cv::VideoWriter rightwriter;
int width;
int height;
int iscolor = 1;
/*
@param argv[1]  the video path
@param argv[2]  the save video's file
@param argv[3]  width
@param argv[4]  height

@brief the left video is argv[2]/left.mp4 and right is argv[2]/right.mp4

example:
./main /home/robot/dataset/zed/video/test.avi /home/robot/dataset/zed/video/test 1280 752


*/

int main(int argc,char* argv[]){
    if(argc<5){
        std::cout<<"there is no file here"<<std::endl;
        return 0;
    }else if(argc==5){
        iscolor = atoi(argv[5]);
    }
    width =atoi(argv[3]);
    height = atoi(argv[4]);

    std::cout<<"the save image size is width:"<< width <<" height:"<<height<<std::endl;



    std::string savefile(argv[2]);
    std::string leftfile = savefile + "/left.avi";
    std::string rightfile = savefile + "/right.avi";


    cap.open(argv[1]);
    if(!cap.isOpened()){
        std::cout<<"cannot open video file"<<std::endl;
        return 0;
    }

    leftwriter.open(leftfile,cv::VideoWriter::fourcc('D','I','V','X'),30,cv::Size(width,height),iscolor);
    rightwriter.open(rightfile,cv::VideoWriter::fourcc('D','I','V','X'),30,cv::Size(width,height),iscolor);
    
    if((!leftwriter.isOpened()) || (!rightwriter.isOpened())){
        std::cout<<"the writer video is not open,please check the file path is right or not"<<std::endl;
    }else{
        std::cout<<"the left and right writer is opened"<<std::endl;
        std::cout<<"the left video path: "<< leftfile<<std::endl;
        std::cout<<"the right video path: "<<rightfile<<std::endl;
    }

    cv::Mat image;
    cv::Mat left;
    cv::Mat right;
    while(true){
        cap>>image;
        if(image.empty()){
            break;
        }
        left = cv::Mat(image,cv::Range(0,height),cv::Range(0,width));
        right = cv::Mat(image,cv::Range(0,height),cv::Range(width,2*width));
        // left = image.colRange(cv::Range(0,width)).clone();
        // right = image.colRange(cv::Range(width,2*width));
        // std::cout<<left.size()<<std::endl;
#ifdef SHOWPICURE
        cv::imshow("left",left);
        cv::imshow("right",right);
        if(cv::waitKey(1)==27)break;
#endif
        leftwriter << left;
        rightwriter << right;
        image.release();
        left.release();
        right.release();
    }
    std::cout<<"this code is over"<<std::endl;
    cap.release();
    leftwriter.release();
    rightwriter.release();
    return 1;
}


