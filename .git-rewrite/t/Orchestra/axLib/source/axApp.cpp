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
#include "axApp.h"
#include "axResourceManager.h"

#include "axPanel.h"
#include "axToggle.h"

axApp* axApp::MainInstance = nullptr;

axResourceManager* axApp::_resourceManager = nullptr;

axApp::axApp():
_debugEditorActive(false)
{
#ifdef __linux__
	_core = new axCoreX11(this);
	_core->Init(axSize(0, 0));
#endif //__linux__


#ifdef _MSC_VER
	#if _axWxWidgetsCore_ == 1
	_core = new axCoreWxWidgets();
	_core->Init(axSize(0, 0));
	#else
		_core = new axCoreWin32();
		axCORE = _core;
		_core->Init(axSize(0, 0));
	#endif //_axWxWidgetsCore_

#endif //_MSC_VER
    
    
#ifdef __APPLE__
    
#ifdef _AX_VST_APP_
    _core = new axVstCoreMac();
    _core->Init(axSize(800, 273));
#else
    _core = new axCoreMac();
//    _core->Init(axSize(500, 500));
#endif // _AX_VST_APP_
    
#endif // __APPLE__
    
    
//------------------------------------------------------------------------------

#if _axDebugEditor_ == 1
    // @todo Fix this.
    MainInstance = this;

    /// @todo Change debugPanel position.
    axPanel* debugPanel = new axPanel(3, nullptr,
                                      axRect(500 - 20, 500 - 20, 20, 20));
    
    axToggle::Info btn_info;
    btn_info.normal = axColor(0.8, 0.8, 0.8, 0.0);
    btn_info.hover = axColor(0.9, 0.9, 0.9, 0.0);
    btn_info.clicking = axColor(0.7, 0.7, 0.7, 0.0);
    
    btn_info.selected = axColor(0.8, 0.4, 0.4, 0.0);
    btn_info.selected_hover = axColor(0.9, 0.4, 0.4, 0.0);
    btn_info.selected_clicking = axColor(0.7, 0.4, 0.4, 0.0);
    
    btn_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    btn_info.font_color = axColor(0.0, 0.0, 0.0, 0.0);
    
    btn_info.img = "settings.png";
    btn_info.single_img = true;
    
    axToggle* tog = new axToggle(debugPanel,
                                 axRect(axPoint(0, 0), axSize(20, 20)),
                                 axToggle::Events(GetOnDebugEditor()),
                                 btn_info,
                                 "",
                                 "",
                                 axToggle::Flags::SINGLE_IMG);
    tog->SetEditable(false);
#endif // _axDebugEditor_
//    std::cout << "Size : " << _core->GetGlobalSize().x << " " << _core->GetGlobalSize().y << std::endl;
//------------------------------------------------------------------------------
}

axApp::axApp(const axSize& frame_size):
_debugEditorActive(false)
{
#ifdef __linux__
	_core = new axCoreX11(this);
	_core->Init(frame_size);
#endif //__linux__

#ifdef _MSC_VER
	#if _axWxWidgetsCore_ == 1
		_core = new axCoreWxWidgets();
		_core->Init(frame_size);
	#else
		_core = new axCoreWin32();
		axCORE = _core;
		_core->Init(frame_size);
	#endif //_axWxWidgetsCore_

#endif // _MSC_VER
    
#ifdef __APPLE__
    
#ifdef _AX_VST_APP_
    _core = new axVstCoreMac();
    _core->Init(frame_size);
#else
    _core = new axCoreMac();
    //_core->Init(frame_size);
#endif // _AX_VST_APP_
    
#endif // __APPLE__
}

void axApp::AddMainEntry(std::function<void()> fct)
{
    _mainEntryFunction = fct;
}

void axApp::CallMainEntryFunction()
{
    if(_mainEntryFunction)
    {
        _mainEntryFunction();
    }
}

//------------------------------------------------------------------------------
void axApp::OnDebugEditor(const axMsg& msg)
{
//    std::cout << "Size : " << _core->GetGlobalSize().x << " " << _core->GetGlobalSize().y << std::endl;
//
//    editingToggle
    
    if(_debugEditorActive)
    {
        _debugEditorActive = false;
    }
    else
    {
        _debugEditorActive = true;
    }
}
//------------------------------------------------------------------------------

string axApp::OpenFileDialog()
{
	return _core->OpenFileDialog();
}

//bool axApp::CreatePopupWindow(const char* title, int x, int y)
//{
//	return _core->CreatePopupWindow("Popup", x, y);
//}

string axApp::GetAppDirectory()
{
	return _core->GetAppDirectory();
}

void axApp::MainLoop()
{
	//CallMainEntryFunction();
	_core->MainLoop();
}

void axApp::UpdateAll()
{
	_core->UpdateAll();
}

axManager* axApp::GetWindowManager()
{
	return _core->GetWindowManager();
}

axManager* axApp::GetPopupManager()
{
    return _core->GetPopupManager();
}

void axApp::AddWindow(axWindow* win)
{
	GetWindowManager()->Add(win);
}

void axApp::AddPopWindow(axWindow* win)
{
	_core->GetPopupManager()->Add(win);
}

void axApp::ActivateDebugEditor(const bool& active)
{
    _debugEditorActive = active;
    UpdateAll();
}

bool axApp::IsDebugEditorActive() const
{
    return _debugEditorActive;
}

axCore* axApp::GetCore()
{
	return _core;
}

axResourceManager* axApp::GetResourceManager() const
{
    return _resourceManager == nullptr ?
           _resourceManager = new axResourceManager() : _resourceManager;
}

void axApp::AddAfterGUILoadFunction(std::function<void()> fct)
{
    _afterGuiLoadFunction = fct;
}

void axApp::CallAfterGUILoadFunction()
{
    if(_afterGuiLoadFunction)
    {
        _afterGuiLoadFunction();
    }
}