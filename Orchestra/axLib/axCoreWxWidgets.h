#ifndef __AX_CORE_WX_WIDGETS__
#define __AX_CORE_WX_WIDGETS__


#include "axCore.h"

class wxGLCanvas;

class axCoreWxWidgets : public axCore
{
public:
	axCoreWxWidgets();

	virtual void Init(const axSize& frame_size);

	virtual bool CreatePopupWindow(const char* title, int width, int height)
	{
		return false;
	}

	void SetWxGlCanvas(wxGLCanvas* canvas)
	{
		_wxGLCanvas = canvas;
	}

	virtual string OpenFileDialog()
	{
		return "";
	}

	virtual void PushEventOnSystemQueue();

	axPoint GetScreenRelativePosition()
	{

	}

	virtual void ResizeFrame(const axSize& size)
	{

	}
	
	virtual void UpdateAll();

	virtual axSize GetScreenSize()
	{
		return axSize(1000, 1000);
	}

	virtual axRect GetScreenRect()
	{
		return axRect(0, 0, 1000, 1000);
	}

	virtual int DrawGLPopScene()
	{
		return 0;
	}

	//virtual int DrawGLScene()
	//{
	//	return 0;
	//}

	std::string GetAppDirectory()
	{
		return "";
	}
	void DeletePopWindow()
	{

	}

protected:
	virtual void KillGLWindow()
	{

	}
	virtual void KillPopGLWindow()
	{

	}
	virtual bool CreateGLWindow(const char* title, int width, int height, int bits = 16)
	{
		return false;
	}
	virtual void MainLoop()
	{

	}

	wxGLCanvas* _wxGLCanvas;

};

#endif //__AX_CORE_WX_WIDGETS__