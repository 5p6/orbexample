#include"calib.hpp"
#include"videoshow.h"






/*
@brief calibration code
@param argv[1] left picure path svae text
@param argv[2] right picure path svae text
@param argv[3] chessboard ==> x
@param argv[4] chessboard ==> y
@param argv[5] the xml file of saved camera params 
@param argv[6] option 1 involved in params,which could control the show of the finding corner pixel
@param argv[7] option 2 invovle in params,which could control whether save the camera params or not;

@brief for example:
1.
./main /home/robot/dataset/zed/test/left.txt /home/robot/dataset/zed/test/right.txt 9 9 ./params.xml
2.
./main /home/robot/dataset/zed/test/left.txt /home/robot/dataset/zed/test/right.txt 9 9 ./params.xml 1 1
3.
for example:
./main qiege /home/robot/dataset/zed /home/robot/dataset/zed/test 8 1280
explain:
the picures are /home/robot/dataset/zed/calib*.png
the left images are /home/robot/dataset/zed/test/left/calib*.png
the right images are /home/robot/dataset/zed/test/right/calib*.png
the left txt are /home/robot/dataset/zed/test/left.txt
the right txt are /home/robot/dataset/zed/test/right/.txt
*/
int main(int argc,char* argv[]){
    if(argc<2){
    std::cout<<"please check the calib picure file path"<<std::endl;
        return 0;
    }
    if(std::string(argv[1]) == "qiege"){
        if(argc<6){
            std::cout<<"the number of input params is incorrect(from qiege)"<<std::endl;
            return false;}
        picure_source(argv[2],argv[3],atoi(argv[4]),atoi(argv[5]));
        return 1;
    }
    if(std::string(argv[1]) == "videoshow"){
        if(argc<4){
            std::cout<<"the number of input params is incorrect(from video show)"<<std::endl;
            return false;}
        Rectify::showVideo(argv[2],argv[3]);
        return 1;
    }
    if(argc<6){
        std::cout<<"please check the calib picure file path(from the calib)"<<std::endl;
        return 0;
    }
    std::string left_text = argv[1];
    std::string right_text = argv[2];
    cv::Size chessboard(atoi(argv[3]),atoi(argv[4]));
    std::string xml_file = argv[5];
    std::vector<int> params{1,0};
    if(argc==8){
        params[0] = atoi(argv[6]);
        params[1] = atoi(argv[7]);
    }
    calibrate(left_text,right_text,chessboard,xml_file,params);
    return 1;
}