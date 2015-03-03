//
//  MenuPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__MenuPanel__
#define __Demo__MenuPanel__

#include <axLib/axLib.h>
//#include "axLib.h"

class MenuPanel : public axPanel
{
public:
    MenuPanel(axWindow* parent,
                const axRect& rect);
    
    axEVENT_ACCESSOR(axButton::Msg, OnPopupMenu);
    axEVENT_ACCESSOR(axButton::Msg, OnTestBtnUnder);
    axEVENT_ACCESSOR(axPopupMenuMsg, OnPopupMenuChoice);
    axEVENT_ACCESSOR(axDropMenuMsg, OnDropMenuChoice);
    
//    axEVENT_ACCESSOR(axDropMenuMsg, OnDropMenuChoice);
    
private:
    virtual void OnPaint();
    
    void OnPopupMenu(const axButton::Msg& msg);
    void OnPopupMenuChoice(const axPopupMenuMsg& msg);
    
    void OnDropMenuChoice(const axDropMenuMsg& msg);
    
    void OnMouseLeftDown(const axPoint& pos);
    
    void OnTestBtnUnder(const axButton::Msg& msg);
    
    axButton* _btnPopMenu;
    axPopupMenu* _popMenu;
    axDropMenu* _dropMenu;
    
    
    axTextBox* _txtCtrl;
    
};

#endif /* defined(__Demo__MenuPanel__) */
