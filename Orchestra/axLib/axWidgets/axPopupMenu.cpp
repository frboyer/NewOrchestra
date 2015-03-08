/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axPopupMenu.h"

axPopupMenu::axPopupMenu(axWindow* parent,
                         const axRect& rect,
                         const axPopupMenuEvents& events,
                         const axPopupMenuInfo& info,
                         const vector<string>& labels,
                         std::string bgImg,
                         axFlag flag) :
// Heritage.
axPanel(3, nullptr, rect),
// members.
_events(events),
_info(info),
_flag(flag),
_index(-1),
_labels(labels),
_lastSelected(nullptr)
{
	axSize tog_size(rect.size.x, LABEL_HEIGHT);

    axToggle::Events tog_event;
    tog_event.button_click = GetOnButtonClick();
    
    axToggle::Info tog_info;
    tog_info.normal = axColor(0.8, 0.8, 0.8, 0.6);
    tog_info.hover = axColor(0.9, 0.9, 0.9, 0.6);
    tog_info.clicking = axColor(0.7, 0.7, 0.7, 0.6);
    
    tog_info.selected = axColor(0.8, 0.4, 0.4, 0.6);
    tog_info.selected_hover = axColor(0.9, 0.4, 0.4, 0.6);
    tog_info.selected_clicking = axColor(0.7, 0.4, 0.4, 0.6);
    
    tog_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    tog_info.font_color = axColor(0.0, 0.0, 0.0, 1.0);
    
	for (int i = 0; i < _labels.size(); i++)
	{
		_btns.push_back(new axToggle(this,
						axRect(axPoint(0, i * LABEL_HEIGHT), tog_size),
						tog_event, tog_info, bgImg, _labels[i],
                                     axToggle::Flags::CANT_UNSELECT_WITH_MOUSE,
                        _labels[i]));
	}

	SetSize(axSize(tog_size.x, (int)_btns.size() * LABEL_HEIGHT));
    SetShownRect(GetRect());
    
    if(_events.selection_change)
    {
        AddConnection(axPopupMenuEvents::SELECTION_CHANGE,
                      _events.selection_change);
    }
}

void axPopupMenu::SetSelectedIndex(const int& index)
{
    if(index >= 0 && index < _btns.size())
    {
        _btns[index]->SetSelected(true);
        
        if (_lastSelected == nullptr)
        {
            _lastSelected = _btns[index];
        }
        else if (_btns[index] != _lastSelected)
        {
            _lastSelected->SetSelected(false);
            _lastSelected = _btns[index];
        }
    }
    else
    {
        std::cerr << "ERROR : axPopupMenu : INDEX OUT OF RANGE" << std::endl;
    }

}

void axPopupMenu::OnButtonClick(const axToggle::Msg& msg)
{
	if (_lastSelected == nullptr)
    {
		_lastSelected = msg.GetSender();
    }
	else if (msg.GetSender() != _lastSelected)
	{
		_lastSelected->SetSelected(false);
		_lastSelected = msg.GetSender();
	}
    
    PushEvent(axPopupMenuEvents::SELECTION_CHANGE,
              new axPopupMenuMsg(msg.GetMsg()));
    
//    if(_events.selection_change)
//    {
//        _events.selection_change(axPopupMenuMsg(msg.GetMsg()));
//    }
}



void axPopupMenu::OnMouseMotion(const axPoint& pos)
{
	/*if (!IsGrabbed())
	{
		GrabMouse();
	}*/

	//if ()

	/*for (unsigned int i = 0; i < _labels.size(); ++i)
	{
		axRect rect(0, i * LABEL_HEIGHT, GetSize().x, LABEL_HEIGHT);
		axRect r(rect + GetAbsoluteRect().position);

		if (r.IsPointInside(pos))
		{
			if (_index != i)
			{
				_index = i;
				Update();
			}
			break;
		}
	}*/
}

void axPopupMenu::OnMouseLeftUp(const axPoint& pos)
{
	//UnGrabMouse();
	////m_parent->TriggerEvent(m_eventID.valueChange);
	//if (_events.selection_change)
	//{
	//	_events.selection_change(axPopupMenuMsg(_labels[_index]));
	//}
}

void axPopupMenu::OnPaint()
{
	//Resize(axSize(GetSize().x, m_labels.size() * LABEL_HEIGHT));

	///axGC gc(GetBackBuffer());
	axGC* gc = GetGC();
	//axSize size = GetSize();
	axRect rect0(axPoint(0, 0), GetRect().size);

	gc->SetColor(_info.bgColorNormal);
	gc->DrawRectangle(rect0);

	gc->SetColor(_info.contourColor);
	gc->DrawRectangleContour(rect0);

	//axSize label_size(size.x, LABEL_HEIGHT);
	//for (unsigned int i = 0, pos_y = 0; i < _labels.size(); ++i, pos_y += LABEL_HEIGHT)
	//{
	//	if (i == _index)
	//	{
	//		gc->SetColor(_info.bgColorSelected);
	//		axRect selected_rect(axPoint(2, pos_y + 2), label_size - axSize(4, 4));

	//		gc->DrawRectangle(selected_rect);

	//		gc->SetColor(_info.fontColor);
	//		gc->DrawStringAlignedCenter(_labels[i], selected_rect);
	//	}

	//	else
	//	{
	//		gc->SetColor(_info.bgColorNormal);
	//		axRect selected_rect(axPoint(2, pos_y + 2), label_size - axSize(4, 4));

	//		gc->DrawRectangle(selected_rect);

	//		gc->SetColor(_info.fontColor);
	//		gc->DrawStringAlignedCenter(_labels[i], 
	//			axRect(axPoint(0, pos_y), label_size));

	///*		gc.DrawTextAligned(m_labels[i], axTEXT_CENTER,
	//			m_info.bgColorNormal.GetColorRGB(),
	//			"8",
	//			axRect(axPoint(0, pos_y), label_size));*/
	//	}
	//}

	////FlipScreen(gc);
}
