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
#ifndef __AX_WIDGET_BUILDER__
#define __AX_WIDGET_BUILDER__

#include "axC++.h"
#include "axWidget.h"

/// @defgroup Widget
/// @{

class axWidgetBuilder
{
public:
    axWidgetBuilder(axWindow* parent);
    
//    virtual axWidget* Create(const std::string& path) = 0;
    
    virtual axWidget* Create(const axVectorPairString& attributes) = 0;
    
    axWindow* GetParent();
    
    void SetParent(axWindow* parent);
    
private:
    axWindow* _parent;
};

/// @}
#endif //__AX_WIDGET_BUILDER__

