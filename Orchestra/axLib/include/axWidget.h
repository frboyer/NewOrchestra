/*******************************************************************************
 * Copyright (c) 2015 Alexandre Arsenault.
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
#ifndef __AX_WIDGET__
#define __AX_WIDGET__

#include "axPanel.h"

enum axParameterType
{
    axINT, axUINT, axLONG, axULONG,
    axFLOAT, axDOUBLE, axBOOL, axSTRING,
    axCOLOR, axRECT, axPOINT, axSIZE
};



class axInfo
{
public:
    axInfo()
    {
        
    }

	typedef std::shared_ptr<axInfo> Ptr;
    
    axInfo(const std::string& path):
    _path(path)
    {
        if(_path.size())
        {
            _isEditable = true;
        }
    }
    
    std::string GetPath() const
    {
        return _path;
    }
    
    bool IsEditable() const;
    
    virtual axStringVector GetParamNameList() const
    {
        return axStringVector{};
    }
    
    virtual std::vector<axParameterType> GetParamTypeList() const
    {
        return std::vector<axParameterType>{};
    }
    
    virtual void SetAttribute(const axStringPair& attribute)
    {
		(attribute);
    }
   
    
    virtual void SetAttributes(const axVectorPairString& attributes)
    {
        for(auto& n : attributes)
        {
            SetAttribute(n);
        }
    }
    
    virtual std::string GetAttributeValue(const std::string& name)
    {
		(name);
        return "";
    }
    
private:
    std::string _path;
    bool _isEditable;
};




class axWidget : public axPanel
{
public:
    axWidget(axWindow* parent, const axRect& rect, axInfo* info);
	axWidget(axWindow* parent, const axRect& rect,std::shared_ptr<axInfo> info);

    axWidget(int f, axWindow* parent, const axRect& rect);
    virtual ~axWidget();
    
    bool IsEditable() const;
    bool IsInfoEditable() const;
    bool AcceptChild() const;
    
    virtual void SetInfo(const axVectorPairString& attributes);
    
    axInfo* GetInfo();

protected:
    std::shared_ptr<axInfo> _info;
    
private:
    bool _isEditable, _isInfoEditable, _acceptChild;
    std::string _name;
    
    
};

#endif //__AX_WIDGET__
