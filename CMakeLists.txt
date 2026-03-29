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
set(CV_VERSION "/opencv412/build/install")
endif()
#D:\LIBRARY\IMAGE_PROCESSING\OPENCV\opencv412\opencv\build\x64\vc16\lib

#set(PKG_CONFIG_EXECUTABLE "C:/ProgramData/chocolatey/bin/pkg-config.exe")
set(PKG_CONFIG_PATH "D:/LIBRARY/IMAGE_PROCESSING/GSTREAMER/1.0/msvc_x86_64/lib/pkgconfig")

find_package(PkgConfig REQUIRED)
#pkg_check_modules(GstreamerLib REQUIRED IMPORTED_TARGET "gstreamer-1.0")

pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)
pkg_search_module(gstreamer-rtsp-server REQUIRED IMPORTED_TARGET gstreamer-rtsp-server-1.0>=1.4)

set(QT_LIB_DIR "${LIBRARY_DIR}/Qt683/6.8.3/${COMPILER}")
set(OPENCV_LIB_DIR "${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/${CV_VERSION}/")

message("SET QT     LIBRARY - ${Qt6_DIR}")
message("SET OPENCV LIBRARY - ${LIBRARY_DIR}/IMAGE_PROCESSING/OPENCV/${CV_VERSION}")
#=========================FIND LIBRARIES=====================

list(APPEND CMAKE_PREFIX_PATH ${QT_LIB_DIR})
list(APPEND CMAKE_PREFIX_PATH ${OPENCV_LIB_DIR})

find_package(Qt6 COMPONENTS Widgets Network Core Xml PrintSupport SerialPort SerialBus Graphs Quick QuickWidgets Network REQUIRED)

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
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION_MODULES/NETWORK_INTERFACES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION_MODULES/DATA_STRUCTURES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION_MODULES/RING_BUFFER")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION_MODULES/GENERIC_MESSAGES")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TRANSMISSION_MODULES/RING_BUFFER_ADAPTERS")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/GRAPHICS_WINDOW_GRAPH")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/IMAGE_PROCESSING")

include_directories("D:/LIBRARY/IMAGE_PROCESSING/GSTREAMER/1.0/msvc_x86_64/lib/gstreamer-1.0/include")
include_directories("D:/PROGRAMS/IDE/VisualStudio2022/VC/Tools/MSVC/14.44.35207/include")
include_directories("D:/LIBRARY/Qt683/6.8.3/msvc2022_64/include")




#======================LINK PROJECT===========================
add_executable (testCameraRTSP
        "main.cpp"

        "./GUI/CameraWindow.cpp"
        "./GUI/widget_complex_interface.cpp"
        "./GUI/widget_complex_interface.ui"

        "./GUI/widget_device_control.cpp"

        "./GUI/widget_main_control.cpp"

        "./GUI/widget_rotary_control.cpp"

        "./GUI/widget_block_status.cpp"
        "./GUI/widget_block_status.ui"

        "./GUI/widget_adjustable.cpp"
        "./GUI/label_active_image.cpp"
        "./GUI/Images.qrc"
        "./DEVICES/device_rotary_interface.cpp"
        "./GRAPHICS_WINDOW_GRAPH/linegraph.cpp"
        "./GRAPHICS_WINDOW_GRAPH/widget_line_graph.cpp"

        "./AIMING_MODULES/nodes_track_approximation.cpp"

        "./UTILITIES/debug_output_filter.cpp"
        "./UTILITIES/sinus_generator_class.cpp"
        "./UTILITIES/engine_keyfilter.cpp"

        "./UTILITIES/register_settings.cpp"
        "./CAMERA_RTSP_INTERFACE/interface_camera_rtsp.cpp"

        "./CAMERA_RTSP_INTERFACE/interface_image_source.cpp"
        "./TRANSMISSION_MODULES/NETWORK_INTERFACES/engine_tcp_interface.cpp"
        "./TRANSMISSION_MODULES/NETWORK_INTERFACES/engine_udp_interface.cpp"
        "./TRANSMISSION_MODULES/NETWORK_INTERFACES/engine_uart_interface.cpp"
        "./TRANSMISSION_MODULES/DATA_STRUCTURES/message_command_structures_rotary.cpp"
        )

target_link_libraries(testCameraRTSP
                PkgConfig::gstreamer
                PkgConfig::gstreamer-sdp
                PkgConfig::gstreamer-app
                PkgConfig::gstreamer-rtsp-server
                PkgConfig::gstreamer-video)
        
target_link_libraries(testCameraRTSP ${OpenCV_LIBS})
target_link_libraries(testCameraRTSP Qt6::Core)
target_link_libraries(testCameraRTSP Qt6::Widgets)
target_link_libraries(testCameraRTSP Qt6::Xml)
target_link_libraries(testCameraRTSP Qt6::Network)
target_link_libraries(testCameraRTSP Qt6::PrintSupport)
target_link_libraries(testCameraRTSP Qt6::SerialPort)
target_link_libraries(testCameraRTSP Qt6::SerialBus)
target_link_libraries(testCameraRTSP Qt6::Graphs)
target_link_libraries(testCameraRTSP Qt6::Quick)
target_link_libraries(testCameraRTSP Qt6::QuickWidgets)
target_link_libraries(testCameraRTSP Qt6::Network)
target_link_libraries(testCameraRTSP "${CMAKE_SOURCE_DIR}/DEVICE_ZOOM_LIB_WIN/controlPTZCamera.lib")




#==========================================================
