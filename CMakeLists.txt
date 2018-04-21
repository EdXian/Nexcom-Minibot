cmake_minimum_required (VERSION 3.8)
project(nexmo)


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



SET(GLOB Headers_ include/*.h)
SET(GLOB Sources_ include/*.cpp)





#匯入Header文件
include_directories( 	include
			${NEXMOTION_INCLUDE_DIR} )			
#建立自訂議Lib					
add_library(motion_task  "include/motion_task.h" "include/motion_task.cpp")
#將相依到程式庫匯入到自訂議Lib
target_link_libraries(motion_task  ${NEXMOTION_LIBS}/NexMotion.lib    )


	
add_executable (Nexmo "Nexmo.cpp" "Nexmo.h"  )
target_link_libraries(Nexmo ${NEXMOTION_LIBS}/NexMotion.lib  motion_task )

find_package(OpenCV REQUIRED)

IF(OpenCV_FOUND)
message(STATUS  "OpenCV IS  FOUND.")
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