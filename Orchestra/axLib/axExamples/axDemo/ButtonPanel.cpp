//
//  ButtonPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "ButtonPanel.h"

ButtonPanel::ButtonPanel(axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(parent, rect)
{
    _selector = new axWidgetSelector(this);
    
    _drawingBuffer = new axDrawingBuffer(rect.size);
    _drawingBuffer->DrawRectangle();
    
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
    
    axButton::Info btn1_info;
    btn1_info.normal = axColor(1.0, 0.0, 0.0, 0.3);
    btn1_info.hover = axColor(0.9, 0.9, 0.9, 0.3);
    btn1_info.clicking = axColor(0.7, 0.7, 0.7, 0.3);
    btn1_info.contour = axColor(0.0, 0.0, 1.0, 0.5);
    btn1_info.selected = btn1_info.normal;
    btn1_info.round_corner_radius = 3;
    
    axButton::Events btn1_evts;
    btn1_evts.button_click = GetOnButtonWithEvtManager();
    
    axButton* btn1 = new axButton(this,
                                  axRect(40, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  "", "Btn1");
    
    axSize size(axApp::GetInstance()->GetCore()->GetScreenSize());
//    std::cout << "SIZE : " << size.x << " " << size.y << std::endl;

    
//    axButton* btn2 = new axButton(this,
//                                  axRect(110, 40, 60, 25),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("GrayButton.png"),
//                                  "Btn2",
//                                  axButton::Flags::SINGLE_IMG);
//    
//    axButton* btn3 = new axButton(this,
//                                  axRect(180, 40, 60, 25),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("btn2.png"),
//                                  "Btn3");
//    
//    std::string img_path = app_path + std::string("button.png");
//    axButton* btn4 = new axButton(this,
//                                  axRect(250, 40, 60, 25),
//                                  axButton::Events(),
//                                  btn1_info,
//                                  img_path,
//                                  "Btn4",
//								  axButton::Flags::SINGLE_IMG |
//								  axButton::Flags::IMG_RESIZE);
//    
//    axButton* btn5 = new axButton(this,
//                                  axRect(320, 40, 60, 25),
//                                  axButton::Events(),
//								  axButton::Info(app_path + std::string("axButtonBlue.axobj")),
//                                  img_path,
//                                  "Btn5",
//								  axButton::Flags::SINGLE_IMG |
//								  axButton::Flags::IMG_RESIZE);
    
    axButton::Info btn2_info;
    btn2_info.normal = axColor(1.0, 0.0, 0.0, 1.0);
    btn2_info.hover = axColor(0.0, 1.0, 0.0, 1.0);
    btn2_info.clicking = axColor(0.0, 0.0, 1.0, 1.0);
    btn2_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    btn2_info.selected = btn2_info.normal;
    btn2_info.round_corner_radius = 0;
    
    axButton* btn6 = new axButton(this,
                                  axRect(390, 40, 25, 25),
                                  axButton::Events(),
                                  btn2_info,
                                  app_path + std::string("playTest.png"),
                                  "",
								  axButton::Flags::IMG_RESIZE);
    
    axButton* btn7 = new axButton(this,
                                  axRect(390, 90, 25, 25),
                                  axButton::Events(),
                                  axBUTTON_TRANSPARENT,
                                  app_path + std::string("playTest.png"),
                                  "",
                                  axButton::Flags::IMG_RESIZE);
    
    _timer = new axTimer();
    _timer->AddConnection(0, GetOnTimerEvent());
    
    _colorTimer = axColor(1.0, 1.0, 1.0);
    
//    axButton* btn7 = new axButton(this,
//                                  axRect(200, 40, 12, 12),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("up.png"));
//    
//    axButton* btn8 = new axButton(this,
//                                  axRect(btn7->GetNextPosDown(0), axSize(12, 12)),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("down.png"));
    
    axButton* btn9 = new axButton(this,
                                  axRect(240, 40, 12, 12),
                                  axButton::Events(),
                                  axBUTTON_TRANSPARENT,
                                  app_path + std::string("upSquare.png"));
    
    axButton* btn10 = new axButton(this,
                                   axRect(btn9->GetNextPosDown(-2), axSize(12, 12)),
                                   axButton::Events(),
                                   axBUTTON_TRANSPARENT,
                                   app_path + std::string("downSquare.png"));
    
    // Tab button event.
    axToggle::Events btn_evts;
//    btn_evts.button_click = GetOnTabClick();
    
    // Tab buttons info.
    axToggle::Info btn_info;
    btn_info.normal = axColor(0.8, 0.8, 0.8, 0.0);
    btn_info.hover = axColor(0.9, 0.9, 0.9, 0.0);
    btn_info.clicking = axColor(0.7, 0.7, 0.7, 0.0);
    
    btn_info.selected = axColor(0.8, 0.4, 0.4, 0.0);
    btn_info.selected_hover = axColor(0.9, 0.4, 0.4, 0.0);
    btn_info.selected_clicking = axColor(0.7, 0.4, 0.4, 0.0);
    
    btn_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    btn_info.font_color = axColor(0.0, 0.0, 0.0, 0.0);
    btn_info.img = "radio.png";
    
    axToggle* togg = new axToggle(this,
                                 axRect(axPoint(150, 200), axSize(15, 15)),
                                 btn_evts,
                                 btn_info,
                                 "");
    
    btn_info.img = "checkbox.png";
    axToggle* togg2 = new axToggle(this,
                                   axRect(axPoint(150, 250), axSize(15, 15)),
                                   btn_evts,
                                   btn_info,
                                   "");
    
    axLabel::Info labelInfo;
    labelInfo.normal = axColor(0.8, 0.8, 0.8, 0.0);
    labelInfo.contour = axColor(0.9, 0.9, 0.9, 0.0);
    labelInfo.font_color = axColor(0.0, 0.0, 0.0, 1.0);
    labelInfo.font_size = 12;
    labelInfo.font_name = "FreeSans.ttf";
    labelInfo._alignement = axALIGN_CENTER;
    
    axLabel* label = new axLabel(this, axRect(100, 200, 70, 30),
                                 labelInfo, "Radio");
    
    axLabel* label2 = new axLabel(this, axRect(20, 250, 70, 30),
                                 labelInfo, "Checkbox");
    
    
    _font = new axFont(0);
    
//    axAnimatedButton* animBtn = new axAnimatedButton(this,
//                                                     axRect(50, 160, 60, 25),
//                                                     axButton::Events(),
//                                                     "axButtonImg.png",
//                                                     "Test");
}

void ButtonPanel::OnButtonWithEvtManager(const axButton::Msg& msg)
{
    //_selector->SetSelectedWidget(msg.GetSender());
//    std::cout << "Button event." << std::endl;
//    _timer->StartTimer(20, 800);
    Update();
}

void ButtonPanel::OnTimerEvent(const axTimerMsg& msg)
{
//    std::cout << "Timer : " << msg.GetTime() << std::endl;
    double timer_ratio = msg.GetTime() / double(800 - 20) * 0.4;
    _colorTimer = axColor(timer_ratio, timer_ratio, 0.4);
    Update();
}

void ButtonPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
//    gc->SetColor(_colorTimer, 1.0);
    gc->SetColor(axColor::axWhiteColor);
    gc->DrawRectangle(rect0);
    
//    _drawingBuffer->DrawRectangle();
    gc->DrawBuffer(_drawingBuffer);
    
//    gc->SetColor(axColor::axRedColor);
//    gc->DrawRoundedRectangle(axRect(40, 200, 50, 25), 12);
//
//    gc->SetColor(axColor::axBlackColor);
//    gc->DrawRoundedRectangleContour(axRect(40, 200, 50, 25), 12);
//    
////    gc->SetColor(axColor::axRedColor);
////    gc->DrawRoundedRectangle(axRect(200, 200, 50, 50), 5);
//    
//    gc->SetColor(axColor::axBlackColor);
//    gc->DrawRoundedRectangleContour(axRect(200, 200, 50, 50), 5);
//    
////    gc->SetColor(axColor::axRedColor);
////    gc->DrawRoundedRectangle(axRect(290, 200, 50, 50), 10);
//    
//    gc->SetColor(axColor::axBlackColor);
//    gc->DrawRoundedRectangleContourSmooth(axRect(290, 200, 50, 50), 10);
//    
//    
////    gc->SetColor(axColor::axRedColor);
////    gc->DrawRoundedRectangle(axRect(370, 200, 50, 50), 4);
//    
//    gc->SetColor(axColor::axBlackColor);
//    gc->DrawRoundedRectangleContour(axRect(370, 200, 50, 50), 4);
//    
//    gc->SetColor(axColor::axRedColor);
//    gc->DrawLine(axPoint(150.0, 120.0), axPoint(200, 150.0));
//    
//    gc->DrawSmouthLine(axPoint(170.0, 120.0), axPoint(220, 150.0), 3);
//
//    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
//    gc->DrawRectangleContour(rect0);
}