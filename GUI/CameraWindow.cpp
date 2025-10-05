#include "CameraWindow.h"
#include "interface_camera_rtsp.h"
#include "interface_image_source.h"
#include <QPainter>
#include <QPen>

using namespace std;
#define TAG "[ ROT_FIND ]" 

void WidgetProcessDisplay::LinkToSource(std::shared_ptr<SourceImageInterface> Source)
{
  ImageSource = Source;
  timerDisplayImage.start(30);
}

//void WidgetCameraControl::LinkToCamera(CameraInterfaceUniversal* Camera)
//{
//  CameraDevice = Camera;
//
//  connect(&butStartStopCamera, &QPushButton::toggled, [Camera](bool OnOff) 
//  {
//    if(OnOff) Camera->slotStartStream(); if(!OnOff) Camera->slotStopStream();
//  });
//
//  connect(&butSetExposure, &QPushButton::clicked, [Camera,this]() 
//  {
//    auto exposure = lineExposure.text().toInt(); Camera->CameraSetExposure(exposure);
//  });
//
//  connect(&butZoom1, &QPushButton::toggled, [Camera,this](bool OnOff) 
//  {
//    if(!OnOff) return;
//    int x_center = 720/2; int y_center = 540/2; int size = 160; 
//    Camera->CameraSetOffset(x_center - size/2, y_center - size/2);
//    Camera->CameraSetSize(size,size);
//  });
//  connect(&butZoom2, &QPushButton::toggled, [Camera,this](bool OnOff) 
//  {
//    if(!OnOff) return;
//    int x_center = 720/2; int y_center = 540/2; int size = 200; 
//    Camera->CameraSetOffset(x_center - size/2, y_center - size/2);
//    Camera->CameraSetSize(size,size);
//  });
//  connect(&butZoom3, &QPushButton::toggled, [Camera,this](bool OnOff) 
//  {
//    if(!OnOff) return;
//    int x_center = 720/2; int y_center = 540/2; int size = 300; 
//    Camera->CameraSetOffset(x_center - size/2, y_center - size/2);
//    Camera->CameraSetSize(size,size);
//  });
//  connect(&butZoom4, &QPushButton::toggled, [Camera,this](bool OnOff) 
//  {
//    if(!OnOff) return;
//    int x_center = 720/2; int y_center = 540/2; int size = 400; 
//    Camera->CameraSetOffset(x_center - size/2, y_center - size/2);
//    Camera->CameraSetSize(size,size);
//  });
//  connect(&butZoom5, &QPushButton::toggled, [Camera,this](bool OnOff) 
//  {
//    if(!OnOff) return;
//    int x_center = 720/2; int y_center = 540/2; int size = 500; 
//    Camera->CameraSetOffset(x_center - size/2, y_center - size/2);
//    Camera->CameraSetSize(size,size);
//  });
//}

QString CommonStyleSheet::style = {
    //" QWidget { background-color:rgb(81, 86, 89);"
    //" border: 2px solid #455364;"
    //" padding: 0px;"
    //" color: #E0E1E3;"
    //" selection-background-color: #346792;"
    //" selection-color: #E0E1E3; }"

    " QLabel { background-color:rgb(72, 70, 68);"
    " border: 1px solid black;"
    " padding: 0px;"
    " font: bold 16pt Ubuntu; "
    " color: #E0E1E3;"
    " selection-background-color: #346792;"
    " selection-color: #E0E1E3; }"

    " QLineEdit { background-color:rgb(155, 147, 136);"
    " border: 1px solid black;"
    " padding: 0px;"
    " font: bold 16pt Ubuntu; "
    " color:rgb(0, 0, 0);"
    " selection-background-color: #346792;"
    " selection-color: #E0E1E3; }"

    " QGroupBox { background-color:rgb(75, 82, 69);"
    " border: 1px solid black;"
    " padding: 0px;"
    " color: #E0E1E3;"
    " selection-background-color: #346792;"
    " selection-color: #E0E1E3; }"
};

QString CommonStyleSheet::style2 = {
    " QWidget { background-color:rgb(84, 77, 69);"
    " border: 2px solid #455364;"
    " padding: 0px;"
    " color:rgb(41, 41, 42);"
    " selection-background-color: #346792;"
    " selection-color: #E0E1E3; }"

    " QPushButton {	background-color: rgb(104, 111, 111); "
    "   color:rgb(43, 45, 47); "
    "   border-radius: 4px; "
    "   padding: 2px; "
    "   outline: none; "
    "   border: none; "
    " 	font: bold 14pt Ubuntu; } "

    " QPushButton:disabled { "
    "   background-color: #455364; "
    "   color: #9DA9B5; "
    "   border-radius: 4px; "
    "   padding: 2px;  } "

    " QPushButton:checked { "
    " 	background-color: rgb(5, 102, 12); "
    "   border-radius: 4px; "
    "   padding: 2px; "
    "   outline: none; "
    "   color:rgb(186, 192, 197); "
    " font: bold 14pt Ubuntu; } "

    " QPushButton:checked:disabled { "
    "   background-color: #60798B; "
    "   color: #9DA9B5; "
    "   border-radius: 4px; "
    "   padding: 2px; "
    "   outline: none; } "

    " QPushButton:checked:selected { "
    "   background: #60798B; } "

    " QPushButton:hover { "
    "   background-color: #54687A; "
    "   color: #E0E1E3; } "

    " QPushButton:pressed { "
    "   background-color: #60798B;  } "

    " QPushButton:selected { "
    "   background: #60798B; "
    "   color: #E0E1E3;  } "

    " QPushButton::menu-indicator { "
    "   subcontrol-origin: padding; "
    "   subcontrol-position: bottom right; "
    "   bottom: 4px; "
    " 	background-color: rgb(210, 204, 182); } "

    " QDialogButtonBox QPushButton { "
    "   min-width: 80px; }"

};
