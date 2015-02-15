#include "Partition.h"

BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
EVT_MOTION(BasicGLPane::mouseMoved)
EVT_LEFT_DOWN(BasicGLPane::mouseDown)
EVT_LEFT_UP(BasicGLPane::mouseReleased)
EVT_RIGHT_DOWN(BasicGLPane::rightClick)
EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
EVT_SIZE(BasicGLPane::resized)
EVT_KEY_DOWN(BasicGLPane::keyPressed)
EVT_KEY_UP(BasicGLPane::keyReleased)
EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
EVT_PAINT(BasicGLPane::render)
EVT_BUTTON(1000, BasicGLPane::OnEvent)
END_EVENT_TABLE()


// some useful events to use
void BasicGLPane::mouseMoved(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	//std::cout << "Mouse pos : " << pos.x << " " << pos.y << std::endl;
	axApp::MainInstance->GetWindowManager()->OnMouseMotion(pos);
}

void BasicGLPane::mouseDown(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	if (event.LeftDown())
	{
		axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(pos);
	}
}

void BasicGLPane::mouseWheelMoved(wxMouseEvent& event)
{

}

void BasicGLPane::mouseReleased(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	if (event.LeftUp())
	{
		axApp::MainInstance->GetWindowManager()->OnMouseLeftUp(pos);
	}
}

void BasicGLPane::rightClick(wxMouseEvent& event)
{
}

void BasicGLPane::mouseLeftWindow(wxMouseEvent& event)
{
}

void BasicGLPane::keyPressed(wxKeyEvent& event)
{
}

void BasicGLPane::keyReleased(wxKeyEvent& event)
{
}

void BasicGLPane::OnEvent(wxCommandEvent& event)
{
	std::cout << "Event" << std::endl;
	axEventManager::GetInstance()->CallNext();
}

BasicGLPane::BasicGLPane(wxPanel* parent, 
						 const wxPoint& pos, 
						 const wxSize& size, 
						 int* args) :
wxGLCanvas(parent, wxID_ANY, args, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);
	wxGLCanvas::SetCurrent(*m_context);
	
	axEventManager::GetInstance();
	axApp* app = axApp::CreateApp(axSize(size.x, size.y));
	static_cast<axCoreWxWidgets*>(app->GetCore())->SetWxGlCanvas(this);

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

BasicGLPane::~BasicGLPane()
{
	delete m_context;
}

void BasicGLPane::resized(wxSizeEvent& evt)
{
	Refresh();
}

int BasicGLPane::getWidth()
{
	return GetSize().x;
}

int BasicGLPane::getHeight()
{
	return GetSize().y;
}

void BasicGLPane::render(wxPaintEvent& evt)
{
	if (!IsShown())
	{
		return;
	}

	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	axCore* core = axApp::MainInstance->GetCore();

	wxSize size(GetSize());
	core->ResizeGLScene(size.x, size.y, 0);
	
	core->DrawGLScene();

	glFlush();
	SwapBuffers();
}
