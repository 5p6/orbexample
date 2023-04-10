#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H
#include<opencv2/video.hpp>
#include<opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include"calib.hpp"
#include<string>
namespace Rectify
{

    class Video{
        public:
        Video();
        Video(std::string videoname,std::string param);
        bool calcmap();
        bool on();
        bool isColor();
        bool isOpen();
        private:
        cv::VideoCapture cap;
        cv::Size imageSize;
        cv::FileStorage paramfile;
        cv::Mat mapx[2],mapy[2];
    };
    void showVideo(std::string videoname,std::string param);
} // namespace









#endif