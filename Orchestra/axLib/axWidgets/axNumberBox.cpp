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
#include "axNumberBox.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * axNumberBox::Flags.
 ******************************************************************************/
const axFlag axNumberBox::Flags::SINGLE_IMG = axFLAG_1;
const axFlag axNumberBox::Flags::NO_IMG_RESIZE = axFLAG_2;
const axFlag axNumberBox::Flags::LABEL = axFLAG_3;
const axFlag axNumberBox::Flags::SLIDER = axFLAG_4;
const axFlag axNumberBox::Flags::NO_MOUSE = axFLAG_5;

/*******************************************************************************
 * axNumberBox::Msg.
 ******************************************************************************/
axNumberBox::Msg::Msg(const double& value):
_value(value)
{
}

double axNumberBox::Msg::GetValue() const
{
    return _value;
}

axMsg* axNumberBox::Msg::GetCopy()
{
    return new_ Msg(*this);
}

/*******************************************************************************
 * axNumberBox::Info.
 ******************************************************************************/
axNumberBox::Info::Info()
{
    
}

axNumberBox::Info::Info(const axColor& normal_color,
                        const axColor& hover_color,
                        const axColor& clicked_color,
                        const axColor& selected_color,
                        const axColor& contour_color,
                        const axColor& font_color_,
                        const std::string& imgPath,
                        const bool& singleImg) :
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
contour(contour_color),
font_color(font_color_),
img(imgPath),
single_img(singleImg)
{
    
}

axNumberBox::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axNumberBox::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axNumberBox::Info::GetParamNameList() const
{
    return axStringVector{"normal", "hover", "clicking",
        "selected", "contour", "font_color", "img", "single_img"};
}

std::string axNumberBox::Info::GetAttributeValue(const std::string& name)
{
    if(name == "normal")
    {
        return normal.ToString();
    }
    else if(name == "hover")
    {
        return hover.ToString();
    }
    else if(name == "clicking")
    {
        return clicking.ToString();
    }
    else if(name == "selected")
    {
        return selected.ToString();
    }
    else if(name == "contour")
    {
        return contour.ToString();
    }
    else if(name == "font_color")
    {
        return font_color.ToString();
    }
    else if(name == "img")
    {
        return img;
    }
    else if(name == "single_img")
    {
        return to_string(single_img);
    }
    
    return "";
}

void axNumberBox::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "normal")
    {
        normal.LoadFromString(attribute.second);
    }
    else if(attribute.first == "hover")
    {
        hover.LoadFromString(attribute.second);
    }
    else if(attribute.first == "clicking")
    {
        clicking.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected")
    {
        selected.LoadFromString(attribute.second);
    }
    else if(attribute.first == "contour")
    {
        contour.LoadFromString(attribute.second);
    }
    else if(attribute.first == "font_color")
    {
        font_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "img")
    {
        img = attribute.second;
    }
    else if(attribute.first == "single_img")
    {
        single_img = stoi(attribute.second) == 0 ? false : true;
    }
}

/*******************************************************************************
 * axNumberBox::Builder.
 ******************************************************************************/
axNumberBox::Builder::Builder(axWindow* win):
_parent(win),
_past(nullptr)
{
    
}

axNumberBox* axNumberBox::Builder::Create(axVectorPairString attributes)
{
    std::string name;
    axPoint pos;
    axNumberBox::Events evts;
    axFloatRange range(0.0, 1.0);
    axControlType ctrltype = axControlType::axCTRL_FLOAT;
    for(auto& s : attributes)
    {
        if(s.first == "name")
        {
            name = s.second;
        }
        else if(s.first == "rect")
        {
            axStringVector strVec;
            strVec = GetVectorFromStringDelimiter(s.second, ",");
            
            pos = axPoint(stoi(strVec[0]),
                          stoi(strVec[1]));
            
            _size = axSize(stoi(strVec[2]),
                           stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info = axNumberBox::Info(s.second);
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second) == 0 ? false : true;
        }
        else if(s.first == std::string("event"))
        {
            
            evts.value_change = _parent->GetEventFunction(s.second);
        }
        else if(s.first == "range")
        {
            axStringVector strVec;
            strVec = GetVectorFromStringDelimiter(s.second, ",");
            
            range = axFloatRange(stod(strVec[0]),
                                 stod(strVec[1]));
        }
        else if(s.first == std::string("ctrl_type"))
        {
            if(s.second == "float")
            {
                ctrltype = axControlType::axCTRL_FLOAT;
            }
            else if(s.second == "int")
            {
                ctrltype = axControlType::axCTRL_INT;
            }
        }
        else if(s.first == std::string("event"))
        {
            evts.value_change = _parent->GetEventFunction(s.second);
        }
        
    }
    
    axNumberBox* box = new_ axNumberBox(_parent, axRect(pos, _size),
                                       evts, _info, 0, 0.0, range,
                                       ctrltype);
    
    _parent->GetResourceManager()->Add(name, box);
    return box;
}

/*******************************************************************************
 * axNumberBox::axNumberBox.
 ******************************************************************************/
axNumberBox::axNumberBox(axWindow* parent,
                         const axRect& rect,
                         const axNumberBox::Events& events,
                         const axNumberBox::Info& info,
                         axFlag flags,
                         double value,
                         axFloatRange range,
                         axControlType type,
                         axControlUnit unit,
                         axControlInterpolation interpolation,
                         std::string label):
axWidget(parent, rect, axInfo::Ptr(new_ axNumberBox::Info(info))),
// Members.
_events(events),
//_info(info),
_flags(flags),
_range(range),
_type(type),
_unit(unit),
_interpolation(interpolation),
_currentColor(&GetInfo()->normal),
_nCurrentImg(axNUM_BOX_NORMAL),
_font(nullptr)
{
    _bgImg = toUnique(new_ axImage(GetInfo()->img));
    
    //double v = value;
    _value = axClamp<double>(value, _range.left, _range.right);
    
    _font = toUnique(new_ axFont(0));
    _font->SetFontSize(10);

    _zeroToOneValue = _range.GetZeroToOneValue(_value);
    
    if(_events.value_change)
    {
        AddConnection(Events::VALUE_CHANGE, _events.value_change);
    }
}

void axNumberBox::SetInfo(const axVectorPairString& attributes)
{
    _info->SetAttributes(attributes);
    
    /// @todo Major leak.
    std::string path = _info->GetAttributeValue("img");
    if(_bgImg->GetImagePath() != path)
    {
        _bgImg = toUnique(new_ axImage(path));
    }
    
    Update();
}

double axNumberBox::GetValue()
{
    return _value;
}

void axNumberBox::OnMouseEnter()
{
    _currentColor = &GetInfo()->hover;
    _nCurrentImg = axNUM_BOX_HOVER;
    Update();
}

void axNumberBox::OnMouseLeave()
{
    
    if(!IsGrabbed())
    {
        _currentColor = &GetInfo()->normal;
        _nCurrentImg = axNUM_BOX_NORMAL;
        Update();
    }
}

void axNumberBox::OnMouseLeftDown(const axPoint& pos)
{

    _clickPosY = (pos - GetAbsoluteRect().position).y;
    _nCurrentImg = axNUM_BOX_DOWN;
    _currentColor = &GetInfo()->clicking;
    GrabMouse();
    Update();

}

void axNumberBox::OnMouseLeftUp(const axPoint& pos)
{
	(pos);

    if(IsGrabbed())
    {
        UnGrabMouse();
        
        if(IsMouseHoverWindow())
        {
            _currentColor = &GetInfo()->hover;
            _nCurrentImg = axNUM_BOX_HOVER;
        }
        else
        {
            _currentColor = &GetInfo()->normal;
            _nCurrentImg = axNUM_BOX_NORMAL;
        }

        PushEvent(Events::VALUE_CHANGE, new_ Msg(_value));
        Update();
    }
}

void axNumberBox::OnMouseLeftDragging(const axPoint& pos)
{
	axPoint pt(GetAbsoluteRect().position);
    axPoint p = pos - pt;

    double delta = p.y - _clickPosY;
    
    _clickPosY = p.y;

    double v = -delta / 100.0;
    _zeroToOneValue += v;
    
    _zeroToOneValue = axClamp<double>(_zeroToOneValue, 0.0, 1.0);
    _value = _range.GetValueFromZeroToOne(_zeroToOneValue);

    PushEvent(Events::VALUE_CHANGE, new_ Msg(_value));
    
    Update();
}

void axNumberBox::OnPaint()
{
	axGC* gc = GetGC();
	axSize size = GetSize();
//	axRect rect0(0, 0, size.x, size.y);
    axRect rect0(GetDrawingRect());
    axRect rect(GetRect());

    gc->SetColor(*_currentColor);
    gc->DrawRectangle(rect0);

    if(_bgImg->IsImageReady())
    {
        if (GetInfo()->single_img)
        {
            if(IsFlag(Flags::NO_IMG_RESIZE, _flags))
            {
                gc->DrawImage(_bgImg.get(), axPoint(0, 0));
            }
            else
            {
                gc->DrawImageResize(_bgImg.get(), axPoint(0, 0), rect0.size);
            }
        }
        else
        {
            gc->DrawPartOfImage(_bgImg.get(), axPoint(0, _nCurrentImg * rect.size.y),
                                rect.size, axPoint(0, 0));
        }
    }

    gc->SetColor(GetInfo()->font_color);

    if(_type == axControlType::axCTRL_FLOAT)
    {
        std::string v = to_string(_value);
        if(_value < 0)
        {
            v.resize(5);
        }
        else
        {
            v.resize(4);
        }
        
        gc->DrawStringAlignedCenter(*_font, v, rect0);
    }
    else if(_type == axControlType::axCTRL_INT)
    {
        std::string v = to_string((int)_value);
        gc->DrawStringAlignedCenter(*_font, v, rect0);
    }

    
    gc->SetColor(GetInfo()->contour);
    gc->DrawRectangleContour(rect0);
}
