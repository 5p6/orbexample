##  解释
该程序主要用于摄像机拍摄的棋盘格图片的分割和标定,该程序仅依赖opencv

---

### 1. 分割
在你编译结束后，你可以使用切割模块来切割你棋盘格的图像，图片仅横向切割，且每张图的命名是cailb_{i}.png的格式
示例
``` java{.line-numbers}
./main qiege /home/robot/dataset/zed /home/robot/dataset/zed/test 8 1280
```
* ./main 程序的exe文件
* qiege 代表使用切割模块
* /home/robot/dataset/zed 所有未分割文件的文件夹路径
* /home/robot/dataset/zed/test 分割图像的保存路径，分为left和right
* 8 图片分割的个数
* 1280 横向分割的像素个数
## 2.标定
你只需要把图片的路径放在一个txt文件里面就可以开始标定内参了,示例
``` java{.line-numbers}
example first:
./main /home/robot/dataset/zed/test/left.txt /home/robot/dataset/zed/test/right.txt 9 9 ./params.xml
example second:
./main /home/robot/dataset/zed/test/left.txt /home/robot/dataset/zed/test/right.txt 9 9 ./params.xml 1 1
```
* ./main 程序的exe文件
* /home/robot/dataset/zed/test/left.txt 左图像路径的txt文件
* /home/robot/dataset/zed/test/right.txt 右图像路径的txt文件
* 9 9 棋盘格规格
* ./params.xml 内参和外参保存的xml文件路径
可选:
* {0,1} {0,1} 第一个{0,1}是选择是否显示角点查找，第二个{0,1}是用于是否保存参数到xml里面

