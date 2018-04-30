cmake_minimum_required (VERSION 3.8)
project(aruco)


#選擇Release
set(CMAKE_CONFIGURATION_TYPES "Release")

#查看系統環境
cmake_host_system_information(RESULT ENV QUERY IS_64BIT)

#32位元和64位元環境選擇
IF (ENV)
message(STATUS  "Windows 64-bit environment")
SET(NEXMOTION_INCLUDE_DIR "C:\\Program Files (x86)\\NEXCOM\\NexGMC\\Header")
SET(NEXMOTION_LIBS  "C:\\Program Files (x86)\\NEXCOM\\NexGMC\\Lib" )

ELSE()
message(STATUS  "Windows 32-bit environment")
SET(NEXMOTION_INCLUDE_DIR "C:\\Program Files\\NEXCOM\\NexGMC\\Header")
SET(NEXMOTION_LIBS  "C:\\Program Files\\NEXCOM\\NexGMC\\Lib" )

ENDIF()



find_package(OpenCV REQUIRED)

#匯入Header文件
include_directories( 	include
			${NEXMOTION_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS} )			

IF(OpenCV_FOUND)
#message(STATUS  "OpenCV IS  FOUND.")
include_directories( include ${OpenCV_INCLUDE_DIRS} )

add_executable (aruco "aruco_test.cpp"  "aruco_test.h")
target_link_libraries(aruco ${OpenCV_LIBS})


ELSE()	
message(WARNING  "OpenCV IS NOT FOUND.")		    
# 將來源新增到此專案的可執行檔。
#add_executable (Nexmo "Nexmo.cpp" "Nexmo.h"  )
#將相依到程式庫匯入到執行檔
#target_link_libraries(Nexmo ${NEXMOTION_LIBS}/NexMotion.lib  motion_task )


ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)