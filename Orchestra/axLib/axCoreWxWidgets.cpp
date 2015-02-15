
#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "axCoreWxWidgets.h"
#include "axEventManager.h"
#include "axApp.h"
#include <Winuser.h>
#include <Windowsx.h>

axCoreWxWidgets::axCoreWxWidgets() : axCore()
{

}

void axCoreWxWidgets::Init(const axSize& frame_size)
{
	InitManagers();
	InitGL();
}

void axCoreWxWidgets::UpdateAll()
{
	if (_wxGLCanvas != nullptr)
	{
		_wxGLCanvas->Refresh();
	}
}

void axCoreWxWidgets::PushEventOnSystemQueue()
{
	wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, 1000);
	wxPostEvent(_wxGLCanvas, btnEvent);
}