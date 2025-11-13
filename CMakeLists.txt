cmake_minimum_required(VERSION 3.14)
project(testCameraRTSP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ./GUI)
#===========================================================


if(EXISTS "/home/broms/LIBRARY_LOC")
set(LIBRARY_DIR "/home/broms/LIBRARY_LOC")
set(COMPILER "gcc_64")
endif()

if(EXISTS "D:/LIBRARY")
set(LIBRARY_DIR "D:/LIBRARY")
set(COMPILER "msvc2022_64")
endif()

if(EXISTS "${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/opencv412")
set(CV_VERSION "/opencv412/build/")
endif()
#D:\LIBRARY\IMAGE_PROCESSING\OPENCV\opencv412\opencv\build\x64\vc16\lib

if(EXISTS "${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/opencv411")
set(CV_VERSION "opencv411")
endif()

set(QT_LIB_DIR "${LIBRARY_DIR}/Qt683/6.8.3/${COMPILER}")
set(OPENCV_LIB_DIR "${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/${CV_VERSION}/")

message("SET QT     LIBRARY - ${Qt6_DIR}")
message("SET OPENCV LIBRARY - ${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/${CV_VERSION}")
#=========================FIND LIBRARIES=====================

list(APPEND CMAKE_PREFIX_PATH ${QT_LIB_DIR})
list(APPEND CMAKE_PREFIX_PATH ${OPENCV_LIB_DIR})

find_package(Qt6 COMPONENTS Widgets Network Core Xml PrintSupport REQUIRED)

find_package(OpenCV REQUIRED )

message("OPENCV LIBS "${OpenCV_LIBS})

#=========================INCLUDES============================
include_directories("./GUI")
include_directories("./CAMERA_RTSP_INTERFACE")
include_directories("./PROCESSING")
include_directories("./DEVICE_ZOOM_LIB_WIN")
include_directories("./DEVICE_ROTARY")
include_directories("./UTILITIES")
include_directories("./DEVICES")
include_directories("./COMMON")
include_directories("./AIMING_MODULES")
include_directories("D:/LIBRARY/MATH_LIBRARY/eigen")


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/GUI")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION/TRANSMISSION_MODULES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION/DATA_STRUCTURES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION/EMBEDED_RING_BUFFER")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION/GENERIC_MESSAGES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION/RING_BUFFER_ADAPTERS")

#======================LINK PROJECT===========================
add_executable (testCameraRTSP
        "main.cpp"

        "./GUI/CameraWindow.cpp"
        "./GUI/widget_complex_interface.cpp"
        "./GUI/widget_complex_interface.ui"

        "./GUI/widget_control_block.cpp"
        "./GUI/widget_control_block.ui"

        "./GUI/widget_panel_switcher.cpp"
        "./GUI/widget_panel_switcher.ui"

        "./GUI/widget_laser_control.cpp"
        "./GUI/widget_laser_control.ui"

        "./GUI/widget_camera_control.cpp"
        "./GUI/widget_camera_control.ui"

        "./GUI/widget_lid_control.cpp"
        "./GUI/widget_lid_control.ui"

        "./GUI/widget_rotary_platform_control.cpp"
        "./GUI/widget_rotary_platform_control.ui"

        "./GUI/widget_focus_ranger_control.cpp"
        "./GUI/widget_focus_ranger_control.ui"

        "./GUI/widget_block_status.cpp"
        "./GUI/widget_block_status.ui"

        "./GUI/widget_adjustable.cpp"
        "./GUI/label_active_image.cpp"
        "./GUI/Images.qrc"
        "./DEVICES/device_rotary_interface.cpp"

        "./UTILITIES/debug_output_filter.cpp"

        "./UTILITIES/register_settings.cpp"
        "./CAMERA_RTSP_INTERFACE/interface_camera_rtsp.cpp"
        "./CAMERA_RTSP_INTERFACE/interface_image_source.cpp"
        "./TRANSMISSION/TRANSMISSION_MODULES/engine_tcp_interface.cpp"
        "./TRANSMISSION/TRANSMISSION_MODULES/engine_udp_interface.cpp"
        )

        
target_link_libraries(testCameraRTSP ${OpenCV_LIBS})
target_link_libraries(testCameraRTSP Qt6::Core)
target_link_libraries(testCameraRTSP Qt6::Widgets)
target_link_libraries(testCameraRTSP Qt6::Xml)
target_link_libraries(testCameraRTSP Qt6::Network)
target_link_libraries(testCameraRTSP Qt6::PrintSupport)
target_link_libraries(testCameraRTSP "${CMAKE_SOURCE_DIR}/DEVICE_ZOOM_LIB_WIN/controlPTZCamera.lib")




#==========================================================
