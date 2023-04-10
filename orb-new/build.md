## how to build own dataset

---
### 1.calibration
just make use of the Calibration code to calibrate your camera

you can get the parameters from the opencv Calibration , the parameters from the Calibration involved :
* K1 the left camera matrix
* D1 the distortion about left camera 
* K2 the right camera matrix
* D2 the distortion about right camera 
* R the rotation matrix of transformation about the left camera coordinate to rigth camera coordinate
* t the translation vector about the left camera coordinate to rigth camera coordinate
* E the essential matrix about the left camera  to rigth camera 
* F the fundamental matrix about the left camera to rigth camera
* Q

---
### 2. write the parameters yaml about orbslam1.0-norm

example :
``` yaml{.line-numbers}
%YAML:1.0

#--------------------------------------------------------------------------------------------
# System config
#--------------------------------------------------------------------------------------------

# When the variables are commented, the system doesn't load a previous session or not store the current one

# If the LoadFile doesn't exist, the system give a message and create a new Atlas from scratch
#System.LoadAtlasFromFile: "Session_MH01_MH02_MH03_Stereo60_Pseudo"

# The store file is created from the current session, if a file with the same name exists it is deleted
#System.SaveAtlasToFile: "Session_MH01_MH02_MH03_Stereo60_Pseudo"

#--------------------------------------------------------------------------------------------
# Camera Parameters. Adjust them!
#--------------------------------------------------------------------------------------------
File.version: "1.0"

Camera.type: "PinHole"

# Camera calibration and distortion parameters (OpenCV) 
Camera1.fx: 365.374
Camera1.fy: 364.157
Camera1.cx: 335.52
Camera1.cy: 205.24
#[-0.1446462283166632, -0.0474532692455173, 0.003920278866616698, 0.003349838009598529, 0.08225231353801284]
Camera1.k1: -0.141507
Camera1.k2: -0.0186639
Camera1.p1:  0.00379964
Camera1.p2:  0.00248127
Camera1.k3:  0.026936


Camera2.fx: 369.117
Camera2.fy: 367.656
Camera2.cx: 349.744
Camera2.cy: 208.525
#[-0.1465437848833285, -0.01285400697180922, 0.002248340963218758, 0.001819119858090119, 0.0223178960508894]
Camera2.k1: -0.1415437848833285
Camera2.k2: -0.0186639
Camera2.p1:  0.00379964
Camera2.p2:  0.00248127
Camera2.k3:  0.026936

Camera.width: 672
Camera.height: 356

# Camera frames per second 
Camera.fps: 30

# Color order of the images (0: BGR, 1: RGB. It is ignored if images are grayscale)
Camera.RGB: 0



Stereo.ThDepth: 60.0
Stereo.T_c1_c2: !!opencv-matrix
  rows: 4
  cols: 4
  dt: f
  data: [0.999432,0.00867583,-0.0325503,1.19914,
        -0.00899332,0.999913,-0.00962004,-0.00363446,
         0.032464,0.00990731,0.999424,0.0359696,
         0.0,0.0,0.0,1.000000]



#--------------------------------------------------------------------------------------------
# ORB Parameters
#--------------------------------------------------------------------------------------------

# ORB Extractor: Number of features per image
ORBextractor.nFeatures: 2000

# ORB Extractor: Scale factor between levels in the scale pyramid 	
ORBextractor.scaleFactor: 1.414

# ORB Extractor: Number of levels in the scale pyramid	
ORBextractor.nLevels: 7

# ORB Extractor: Fast threshold
# Image is divided in a grid. At each cell FAST are extracted imposing a minimum response.
# Firstly we impose iniThFAST. If no corners are detected we impose a lower value minThFAST
# You can lower these values if your images have low contrast			
ORBextractor.iniThFAST: 30
ORBextractor.minThFAST: 10
#--------------------------------------------------------------------------------------------
# Viewer Parameters
#--------------------------------------------------------------------------------------------
Viewer.KeyFrameSize: 0.05
Viewer.KeyFrameLineWidth: 1.0
Viewer.GraphLineWidth: 0.9
Viewer.PointSize: 2.0
Viewer.CameraSize: 0.08
Viewer.CameraLineWidth: 3.0
Viewer.ViewpointX: 0.0
Viewer.ViewpointY: -0.7
Viewer.ViewpointZ: -1.8
Viewer.ViewpointF: 500.0
Viewer.imageViewScale: 1.0
```
pay attention to the camera parameters and distortion parameters ,just write it;
$\quad$ the vital set is the **Stereo.T_c1_c2**;what we get from the opencv calibration is the R and v;the **Stereo.T_c1_c2** is consisted of the rotation matrix **R** and translation vector **t** :
$$ T = \begin{bmatrix}
    R^T && t \\
    0^T && 1
\end{bmatrix}$$
we can write **Stereo.T_c1_c2** as :
$$ StereoT = T^{-1} = \begin{bmatrix}
    R && -R^Tt \\
    0^T && 1
\end{bmatrix}$$

### 3.
replace the CMakeLists.txt file with this ***CMakeLists.txt*** , pay attention that the first CMakeLists.txt is in the ORB-SLAM3 and the second is in the current directory.then just
```shell
cd /.../ORB_SLAM3
sh ./myexample/test.sh
./stereo  data_dir
```




