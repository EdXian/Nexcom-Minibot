#  Nexcom - Minibot
[NexGMC 下載](https://aiotcloud.net/market/singl.php?PID=132)

## 執行環境
 * Windows 10 64-bit i5-7600/8G  
 * Windows 7  32-bit Intel® Celeron 1900 

## Visual Studio C++ 2017
* 必須先安裝NexMotion SDK 和包含的C++ 2015 x86的轉發套件，後才安裝Visual Studio 2017。如果已經裝過套件，則可以直接按裝MSVC。
* 如果已安裝Visual Studio，則先移除C++ 2013 x86以下的轉發套件，在安裝NexMotion SDK，並於完成後補回先前刪除的套件。

## CMake GUI 3.8以上
* 有關Cmake的配置，可以參考目錄下的`CMakLists.txt`

[Windows win64-x64 Installer](https://cmake.org/files/v3.11/cmake-3.11.1-win64-x64.msi)

[Windows win32-x86 Installer](https://cmake.org/files/v3.11/cmake-3.11.1-win32-x86.msi)

## Opencv 包含 Opencv-Contrib 版本3.3.1
[opencv](https://github.com/opencv/opencv/archive/3.3.1.zip)

[opencv-contrib](https://github.com/opencv/opencv_contrib/archive/3.3.1.zip)

## 編譯
 1. 解壓縮後開啟CMAKE GUI
 2. 以CMAKE作配置 Configure >> generation >>open project
 3. 點選open project 打開專案 ，並直接做編譯。
 
 
## 執行
可以到程式中選擇"模擬"或是真實硬體，選擇模擬則啟動後會自行啟動模擬;選擇真實硬體則啟動後會自行運作。
* Nexmo.cpp
```
 
  robot1.launch(DEVICE_SIMULATOR , 0 );  GroupId =0;
  robot1.launch(DEVICE_REAL , 0 );      GroupId =0;

```

## 自訂議程式庫
修改成可以相容C++的程式庫，可依照需求，修改程式庫中的程式，達成想要的功能。 
  * motion_task .cpp/.h
```
  bool launch(I32_T devType, I32_T  devIndex);  會自動完成一系列的初始化 
  bool terminate();                             關閉系統
  bool gotohome();                              執行手臂歸位
  bool setvelratio(float percetage);            設定手臂速率
  bool LINE_Until_Task(Pos_T & target);         執行3度空間線運動，函數會一直執行，直到到達目標
  bool PTP_Until_Task(Pos_T & target);          執行3度空間點對點運動，函數會一直執行，直到到達目標
  .
  .
  .
```

## 視覺功能 
* 在系統中加入視覺辨識功能，支援opencv-contrib擴展的aruco功能，使用前先確認系統環境變數中已經加入了OpenCV_DIR。
* Apriltag二維條碼。
* 使用普通的USB網路攝影機即可。

### 校正
* [相機校正](https://docs.opencv.org/3.1.0/dc/dbb/tutorial_py_calibration.html)
* [aruco校正](https://docs.opencv.org/3.1.0/d5/dae/tutorial_aruco_detection.html)

### 二維條碼產生
* [Aruco Maker](https://docs.opencv.org/3.1.0/d5/dae/tutorial_aruco_detection.html)
* [Apriltag Tag36h11](https://robot2016.mit.edu/sites/default/files/documents/project_apriltag36h11.pdf)

## TODO



