#  Nexcom - Minibot

## 執行環境
 * Windows 10 64-bit i5-7600/8G  
 * Windows 7  32-bit Intel® Celeron 1900 

## CMake GUI 3.8以上
[Windows win64-x64 Installer](https://cmake.org/files/v3.11/cmake-3.11.1-win64-x64.msi)

[Windows win32-x86 Installer](https://cmake.org/files/v3.11/cmake-3.11.1-win32-x86.msi)

## Opencv 包含 Opencv-Contrib 版本3.3.1
[opencv](https://github.com/opencv/opencv/archive/3.3.1.zip)

[opencv-contrib](https://github.com/opencv/opencv_contrib/archive/3.3.1.zip)

## Visual Studio C++ 2017
必須先安裝NexMotion SDK 和包含的C++ 2015 x86的轉發套件，後才安裝Visual Studio 2017。如果已經裝過套件，則可以直接按裝MSVC


## 編譯
 1. 解壓縮後開啟CMAKE GUI
 2. 以CMAKE作配置
 3. 確認系統環境變數中已經加入了，OpenCV_DIR
 4. 點選open project 打開專案 ，並直接做編譯。
 
 
## 執行
可以到程式中選擇"模擬"或是真實硬體，選擇模擬則啟動後會自行啟動模擬;選擇真實硬體則啟動後會自行運作
* Nexmo.cpp
```
 
  robot1.launch(DEVICE_SIMULATOR , 0 );  GroupId =0;
  robot1.launch(DEVICE_REAL , 0 );      GroupId =0;

```

## 自訂議程式庫
可依照需求，修改程式庫中的程式，達成想要的功能。 
  * motion_task .cpp/.h
```

  bool launch(I32_T devType, I32_T  devIndex);
  bool terminate();
  bool gotohome();
  bool setvelratio(float percetage);
  bool LINE_Until_Task(Pos_T & target);
  bool PTP_Until_Task(Pos_T & target);

```


