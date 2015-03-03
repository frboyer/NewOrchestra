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
#ifndef __AX_WIDGET_SELECTOR__
#define __AX_WIDGET_SELECTOR__

/// @defgroup Widgets
/// @{

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * axLabel.
 ******************************************************************************/
class axWidgetSelector: public axWidget
{
public:
    /***************************************************************************
     * axLabel::Info.
     **************************************************************************/
    class Info : public axInfo
    {
    public:
        Info();
        
        Info(const axColor& color);

        
        axColor _color;
    };

    /***************************************************************************
     * axWidgetSelector::axWidgetSelector.
     **************************************************************************/
    axWidgetSelector(axWindow* parent);
    
    void SetSelectedWidget(axWindow* win);

private:
    axWindow* _selectedWidget;
    
    // Events.
    virtual void OnPaint();
};

/// @}
/// @}
#endif // __AX_WIDGET_SELECTOR__.