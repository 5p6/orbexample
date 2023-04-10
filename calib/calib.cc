#include"calib.hpp"

bool picure_source(std::string prefix_dir,std::string save_dir,int number,int colsize){
    std::cout<<"the prefix dir"<<prefix_dir<<std::endl;
    std::cout<<"the save path:"<<save_dir<<std::endl;
    std::cout<<"the left image save path"<<(save_dir + "/left")<<std::endl;
    std::cout<<"the right image save path"<<(save_dir + "/right")<<std::endl;
    std::vector<std::string> left;
    std::vector<std::string> right;
    // qiege the image into left and right image
    for(int i = 1 ; i < number + 1;i++){
        //the image path must be  prefix_dir/calib*.png
        std::string picure = prefix_dir + "/calib"+ std::to_string(i) + ".png";
        // the left(right) image path must be left(right)_path/left/calib*.png
        std::string left_path = save_dir +"/left/calib" + std::to_string(i) + ".png";
        std::string right_path = save_dir +"/right/calib" + std::to_string(i) + ".png";
        
        if(!qiege(picure,colsize,left_path,right_path)){
        std::cout<<"the qiege is not right"<<std::endl;
            return false;
        }
        left.push_back(left_path);
        right.push_back(right_path);
    }


    // txt file path
    std::ofstream leftofs((save_dir + "/left.txt").c_str(),std::ios::out);
    std::ofstream rightofs((save_dir + "/right.txt").c_str(),std::ios::out);
    if((!leftofs.is_open()) || (!rightofs.is_open())){
        std::cout<<"the save txt file is not open"<<std::endl;
        return false;
    }
    std::cout<<"the left picure path saved in "<<(save_dir + "/left.txt")<<std::endl;
    std::cout<<"the right picure path saved in "<<(save_dir + "/right.txt")<<std::endl;
    
    std::cout<<"the left path all:\n";
    for(auto i:left){
        std::cout<<i<<std::endl;
        leftofs<< i <<"\n";
    }
    std::cout<<"the right path all:\n";
    for(auto i:right){
        std::cout<<i<<std::endl;
        rightofs<< i <<"\n";
    }
    leftofs.close();
    rightofs.close();
    return true;
}

bool qiege(std::string picure,int size,std::string path1,std::string path2){
    cv::Mat image = cv::imread(picure,cv::IMREAD_UNCHANGED);
    if(image.empty()){
        std::cout<<"the picure is empty,please check your image path ,error happening in qiege code"<<std::endl;
        return false;
    }
    cv::Mat left = image.colRange(0,size);
    cv::Mat right = image.colRange(size,2*size);
    cv::imwrite(path1,left);
    cv::imwrite(path2,right);
    return true;
}

void concat(cv::Mat& _image1,cv::Mat& _image2,cv::Mat& concat){
	CV_Assert(_image1.rows == _image2.rows);
	cv::Mat src1,src2;
	src1 = _image1.clone();
	src2 = _image2.clone();
	if(src1.empty()|| src2.empty()){
		std::cout<<"the source data is empty"<<std::endl;
		return;
	}

	concat.release();
	concat.create(cv::Size(2 * src1.size().width,src1.size().height),src1.type());
	uchar* ptr1 = NULL;
	uchar* ptr2 = NULL;
	uchar* con1 = NULL;
	for(int i = 0 ; i<src1.rows;i++){
		ptr1 = src1.ptr<uchar>(i);
		ptr2 = src2.ptr<uchar>(i);
		con1 = concat.ptr<uchar>(i);
		for(int j =0;j<src1.cols;j++){
			if(src1.channels() == 1){
				if(i%30 == 0){
				*(con1 + j) = 0;
				*(con1 + src1.cols + j) = 0;
				continue;
				}
				*(con1 + j) = *(ptr1 + j);
				*(con1 + src1.cols + j) = *(ptr2 + j);
				continue;
			}
			if(i%30 == 0){
				*(con1 + 3 * j) = 15;
				*(con1 + 3 * j + 1) = 255;
				*(con1 + 3 * j + 2) = 0;
				*(con1 + 3 * (src1.cols + j)) = 15;
				*(con1 + 3 * (src1.cols + j) + 1) = 255;
				*(con1 + 3 * (src1.cols + j) + 2) = 0;
				continue;
			}
			*(con1 + 3 * j) = *(ptr1 + 3 * j);
			*(con1 + 3 * j + 1) = *(ptr1 + 3 * j + 1);
			*(con1 + 3 * j + 2) = *(ptr1 + 3 * j + 2);
			*(con1 + 3 * (src1.cols + j)) = *(ptr2 + 3 * j);
			*(con1 + 3 * (src1.cols + j) + 1) = *(ptr2 + 3 * j + 1);
			*(con1 + 3 * (src1.cols + j) + 2) = *(ptr2 + 3 * j + 2);
			}

		}
}


int calibrate(std::string left_txt, std::string right_txt,cv::Size chessboard,std::string xml_path,std::vector<int> params)
{
	if (left_txt.empty() || right_txt.empty())
	{
		std::cout << "txt文件为空" << std::endl;
		return 0;
	}
	else if (left_txt.rfind(".txt") == std::string::npos || right_txt.rfind(".txt") == std::string::npos)
	{
		std::wcout << "无txt文件可能输入文件错误" << std::endl;
		return 0;
	}
	if (xml_path.rfind(".xml") == std::string::npos)
	{
		std::cout << "xml文件路径错误" << std::endl;
		return 0;
	}
	//左右路径的文件名
	std::string left_filename(left_txt), right_filename(right_txt);
	//左右路径的文件
	std::ifstream left_ifs(left_filename, std::ios::in);
	std::ifstream right_ifs(right_filename, std::ios::in);
	//获得棋盘格的尺寸
	int rows = chessboard.height;
	int cols = chessboard.width;
	int board_n = rows * cols;
	//左右图路径读取
	std::vector<std::string> left_path;
	std::vector<std::string> right_path;
	//世界点
	std::vector<std::vector<cv::Point3f>> objpoints;
	std::vector<cv::Point3f> points;
	//图像角点
	std::vector<std::vector<cv::Point2f>> left_points;
	std::vector<std::vector<cv::Point2f>> right_points;
	std::vector<cv::Point2f> left_corners;
	std::vector<cv::Point2f> right_corners;
	//图像路径
	std::string left_name;
	std::string right_name;
	//记录找到角点的图像个数
	int total = 0;
	//读取图像和灰度图
	cv::Mat left_img, right_img;
	cv::Mat left_gray, right_gray;
	//是否有角点
	bool left_ret, right_ret;
	//是否显示
	bool is_show = params[0];
	cv::namedWindow("left",cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("right",cv::WINDOW_GUI_NORMAL);
	while (std::getline(left_ifs, left_name) && std::getline(right_ifs, right_name))
	{
		//读图片
		left_img = cv::imread(left_name);
		right_img = cv::imread(right_name);
        //save path
        left_path.push_back(left_name);
        right_path.push_back(right_name);
		cv::cvtColor(left_img, left_gray, cv::COLOR_BGR2GRAY);
		cv::cvtColor(right_img, right_gray, cv::COLOR_BGR2GRAY);
		std::cout<<"the path:"<<left_name<<std::endl;
		//找角点
		left_ret = cv::findChessboardCorners(left_gray, chessboard, left_corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FAST_CHECK);
		right_ret = cv::findChessboardCorners(right_gray, chessboard, right_corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_FAST_CHECK);
		if (!(left_ret & right_ret))
		{
			std::cout << "无角点" << std::endl;
			continue;
		}
		//利用粗角点寻找亚角点
		total++;
		cv::cornerSubPix(left_gray, left_corners, cv::Size(9,9), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 100, 1e-12));
		cv::cornerSubPix(right_gray, right_corners, cv::Size(9,9), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::Type::EPS + cv::TermCriteria::Type::MAX_ITER, 100, 1e-12));
		if (is_show)
		{
            cv::drawChessboardCorners(left_img, chessboard, left_corners, left_ret);
			cv::drawChessboardCorners(right_img, chessboard, right_corners, right_ret);
			cv::imshow("left", left_img);
			cv::imshow("right", right_img);
#ifdef SHOWWHILE
            while(true){
			if(cv::waitKey(1)==27)break;
            }
#else
            cv::waitKey(500);
#endif
        }
		//填入角点
		left_points.push_back(left_corners);
		right_points.push_back(right_corners);
	}
    cv::destroyAllWindows();
	//标志
	int flags =
		cv::CALIB_FIX_PRINCIPAL_POINT  + cv::CALIB_USE_INTRINSIC_GUESS;
	//图像大小
	cv::Size imagesize = left_img.size();
	//世界点填入
	for (int index = 0; index < total; index++)
	{
		points.clear();
		(points).swap(points);
		for (int i = 0; i < board_n; i++)
		{
			int x = i % cols;
			cv::Point3f  point(x * 14, (i - x) / cols * 14, 0);
			points.push_back(point);
		}
		objpoints.push_back(points);
	}
	//内参矩阵
	cv::Mat cameraMatrix1, cameraMatrix2;
	//畸变系数
	cv::Mat distcoeffs1, distcoeffs2;
	//"开始标定"
	std::vector<cv::Mat> rvecs[2];
	std::vector<cv::Mat> tvecs[2];
    cameraMatrix1 = cv::initCameraMatrix2D(objpoints,left_points,imagesize);
    cameraMatrix2 = cv::initCameraMatrix2D(objpoints,right_points,imagesize);
	//计算内参和畸变系数
	double rms1 = cv::calibrateCamera(objpoints, left_points, imagesize, cameraMatrix1, distcoeffs1, rvecs[0], tvecs[0], 0, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-16));
	double rms2 = cv::calibrateCamera(objpoints, right_points, imagesize, cameraMatrix2, distcoeffs2, rvecs[1], tvecs[1], 0, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-16));
	std::cout<<"the left monocular camera rms: "<<rms1<<std::endl;
    std::cout<<"the right monocular camera rms: "<<rms2<<std::endl;
    std::cout<<"initial K1:\n"<<cameraMatrix1<<std::endl;
    std::cout<<"initial D1:\n"<<distcoeffs1<<std::endl;
    std::cout<<"initial K2:\n"<<cameraMatrix2<<std::endl;
    std::cout<<"initial D2:\n"<<distcoeffs2<<std::endl;
	//旋转矩阵和平移向量
	cv::Mat R, T;
	//基本矩阵和本征矩阵
	cv::Mat E, F;
	//计算R，T，E，E
	double rms = cv::stereoCalibrate(
		objpoints,
		left_points,
		right_points,
		cameraMatrix1,
		distcoeffs1,
		cameraMatrix2,
		distcoeffs2,
		imagesize,
		R,
		T,
		E,
		F,
		0
	);
	std::cout<<"\nstereo calibration params : \n";
    std::cout<<"the stereo rms :"<<rms<<std::endl;
    std::cout<<"the stereo K1:\n"<<cameraMatrix1<<std::endl;
    std::cout<<"the stereo D1:\n"<<distcoeffs1<<std::endl;
    std::cout<<"the stereo K2:\n"<<cameraMatrix2<<std::endl;
    std::cout<<"the stereo D2:\n"<<distcoeffs2<<std::endl;
    std::cout<<"the Rotation matrix from camera 1 to camera 2:\n"<<R<<std::endl;
    std::cout<<"the translation vector matrix from camera 1 to camera 2:\n"<<T<<std::endl;
    std::cout<<"the essential matrix:\n"<<E<<std::endl;
    std::cout<<"the fundanmental matrix:\n"<<F<<std::endl;
	//R1,R2:旋转矩阵
	//P1,P2:相机映射矩阵
	//Q:重投影矩阵
	cv::Mat R1, R2, P1, P2, Q;
	cv::stereoRectify(
		cameraMatrix1,
		distcoeffs1,
		cameraMatrix2,
		distcoeffs2,
		imagesize,
		R, T,
		R1, R2, P1, P2, Q
	);
	std::cout<<"\nThe Rectify calibration params : \n";
    std::cout<<"the Rectify rms :"<<rms<<std::endl;
    std::cout<<"the Rectify K1:\n"<<cameraMatrix1<<std::endl;
    std::cout<<"the Rectify D1:\n"<<distcoeffs1<<std::endl;
    std::cout<<"the Rectify K2:\n"<<cameraMatrix2<<std::endl;
    std::cout<<"the Rectify D2:\n"<<distcoeffs2<<std::endl;
	std::cout<<"the Rectify R1:\n"<<R1<<std::endl;
    std::cout<<"the Rectify R2:\n"<<R2<<std::endl;
    std::cout<<"the Rectify P1:\n"<<P1<<std::endl;
    std::cout<<"the Rectify P2:\n"<<P2<<std::endl;
	//关闭文件
	left_ifs.close();
	right_ifs.close();

	//是否保存数据
	bool is_save = params[1];
	if (is_save) {
		//保存数据
		cv::FileStorage file(xml_path, cv::FileStorage::WRITE);
		//写入数据
		file << "data" << "{"
			<< "K1" << cameraMatrix1
			<< "D1" << distcoeffs1
			<< "R1" << R1
			<< "P1" << P1
			<< "K2" << cameraMatrix2
			<< "D2" << distcoeffs2
			<< "R2" << R2
			<< "P2" << P2
			<< "Q" << Q
			<< "}";
		std::cout << "xml已保存" << std::endl;
		file.release();
	}else{
        std::cout << "xml not save" << std::endl;
	}



	std::cout<<"the reundistort showing : "<<std::endl;
	cv::Mat left = cv::imread(left_path[0]);
    cv::Mat right = cv::imread(right_path[0]);
    cv::Mat leftmap[2],rightmap[2];
    cv::initUndistortRectifyMap(cameraMatrix1,distcoeffs1,R1,P1,left.size(),CV_32FC1,leftmap[0],leftmap[1]);
    cv::initUndistortRectifyMap(cameraMatrix2,distcoeffs2,R2,P2,left.size(),CV_32FC1,rightmap[0],rightmap[1]);
    cv::remap(left,left,leftmap[0],leftmap[1],cv::INTER_AREA);
    cv::remap(right,right,leftmap[0],leftmap[1],cv::INTER_AREA);

	cv::namedWindow("left",cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("right",cv::WINDOW_GUI_NORMAL);
	cv::namedWindow("the concat picure",cv::WINDOW_GUI_NORMAL);
    cv::imshow("left",left);
    cv::imshow("right",right);
	cv::Mat picure;
	concat(left,right,picure);
	cv::imshow("the concat picure",picure);
	std::cout<<"just press the esc to exit"<<std::endl;
    while(true){
        if(cv::waitKey(1)==27) break;
    }
	return 1;
}


