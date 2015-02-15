#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include <vector>
#include "wx/glcanvas.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

#include "axLib.h"

class BasicGLPane : public wxGLCanvas, public axObject
{
public:
	/***************************************************************************
	* BasicGLPane::BasicGLPane.
	**************************************************************************/
	BasicGLPane(wxPanel* parent,
		const wxPoint& pos,
		const wxSize& size,
		int* args);

	virtual ~BasicGLPane();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);

	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	void OnEvent(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxGLContext* m_context;
};
