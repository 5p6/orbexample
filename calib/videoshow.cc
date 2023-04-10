#include"videoshow.h"



namespace Rectify
{
        Video::Video(){}
        Video::Video(std::string videoname,std::string param){
            cap.open(videoname.c_str());
            paramfile.open(param.c_str(),cv::FileStorage::READ);
            imageSize = cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH)/2,cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        }

        bool Video::calcmap(){
            cv::Mat K1,K2,D1,D2;
            cv::Mat R1,R2,P1,P2;
            cv::Mat Q;
            if(!paramfile.isOpened()){ 
                std::cout<<"the param file is not opened"<<std::endl;
                return false;
            }
            K1 = paramfile["data"]["K1"].mat();
            D1 = paramfile["data"]["D1"].mat();
            K2 = paramfile["data"]["K2"].mat();
            D2 = paramfile["data"]["D1"].mat();
            R1 = paramfile["data"]["R1"].mat();
            R2 = paramfile["data"]["R2"].mat();
            P1 = paramfile["data"]["P1"].mat();
            P2 = paramfile["data"]["P2"].mat();
            std::cout<<"The K1:\n"<<K1<<std::endl;
            std::cout<<"The D1:\n"<<D1<<std::endl;
            std::cout<<"The K2:\n"<<K2<<std::endl;
            std::cout<<"The D2:\n"<<D2<<std::endl;
            std::cout<<"The R1:\n"<<R1<<std::endl;
            std::cout<<"The R2:\n"<<R2<<std::endl;
            std::cout<<"The P1:\n"<<P1<<std::endl;
            std::cout<<"The P2:\n"<<P2<<std::endl;
            std::cout<<"The image size:"<< imageSize<<std::endl;
            cv::initUndistortRectifyMap(K1,D1,R1,P1,imageSize,CV_32FC1,mapx[0],mapy[0]);
            cv::initUndistortRectifyMap(K2,D2,R2,P2,imageSize,CV_32FC1,mapx[1],mapy[1]);
            return true;
        }

        bool  Video::on(){
            if(!isOpen()){ 
            std::cout<<"Video open failed"<<std::endl;
            return false;
            }
            cv::Mat image,left,right,catimage;
            if(!calcmap()){
                std::cout<<"the calculation of map is failed"<<std::endl;
                return false;
            }
            while(true){
                cap >> image;
                if(image.empty()){
                    std::cout<<"the image empty"<<std::endl;
                    break;
                }
            
                left = image.colRange(cv::Range(0,imageSize.width)).clone();
                right = image.colRange(cv::Range(imageSize.width,2*imageSize.width)).clone();
                cv::remap(left,left,mapx[0],mapy[0],cv::INTER_AREA);
                cv::remap(right,right,mapx[1],mapy[1],cv::INTER_AREA);
                concat(left,right,catimage);
                cv::imshow("left",left);
                cv::imshow("right",right);
                cv::imshow("concat",catimage);
#ifdef SHOWWHILE
                while(true){
                    if(cv::waitKey(1)==27)break;   
                }
#else
                if(cv::waitKey(30)==27)break;   
#endif
            }

            image.release();
            left.release();
            right.release();
            catimage.release();
            cap.release();

            return true;
        }

        bool  Video::isColor(){
            return (cap.get(cv::CAP_PROP_CHANNEL) ==3);
        }
        
        bool  Video::isOpen(){
            return cap.isOpened();
        }


        void showVideo(std::string videoname,std::string param){
            std::cout<<"the video path : "<<videoname<<" the params name : "<<param<<std::endl;
            cv::Ptr<Video> video(new Video(videoname,param));
            video->on();
        }
} // namespace



