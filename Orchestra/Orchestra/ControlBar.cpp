#include "ControlBar.h"
#include "Debug.h"
#include "wx/timectrl.h"

BEGIN_EVENT_TABLE(ControlBar, wxPanel)
	EVT_SIZE(ControlBar::OnSize)
	EVT_BUTTON(BACK_BTN, ControlBar::OnBackBtn)
	EVT_BUTTON(STOP_BTN, ControlBar::OnStopBtn)
	EVT_BUTTON(PLAY_BTN, ControlBar::OnPlayBtn)
	EVT_BUTTON(PAUSE_BTN, ControlBar::OnPauseBtn)
	EVT_BUTTON(FWRD_BTN, ControlBar::OnFwrdBtn)
	EVT_BUTTON(SLIDER_LEFT_DOWN, ControlBar::OnSliderLeftDown)
	EVT_BUTTON(SLIDER_LEFT_UP, ControlBar::OnSliderLeftUp)
	EVT_SLIDER(SLIDER_MOTION, ControlBar::OnSliderMotion)
END_EVENT_TABLE()

ControlBar::ControlBar(wxWindow* win,
					   const wxWindowID& id,
					   const wxPoint& pos,
					   const wxSize& size):
					   // Heritage
					   wxPanel(win, id, pos, size)
{
	wxImage::AddHandler(new wxPNGHandler);

	SetBackgroundColour(wxColor(240, 240, 240));

	_slider = new wxSlider(this, SLIDER_MOTION, 0, 0, 1000, wxPoint(4, 2), wxSize(size.x - 8, 25));

	int y = 30, x = (size.x - 150) * 0.5;
	_backBtn = new wxBitmapButton(this, BACK_BTN,
								  wxBitmap("Ressources/backward.png", wxBITMAP_TYPE_PNG),
								  wxPoint(x+=25, y), wxSize(25, 25));// , wxBORDER_NONE);

	_pauseBtn = new wxBitmapButton(this, PAUSE_BTN,
							       wxBitmap("Ressources/pause.png", wxBITMAP_TYPE_PNG),
							       wxPoint(x += 25, y), wxSize(25, 25));

	_playBtn = new wxBitmapButton(this, PLAY_BTN,
							      wxBitmap("Ressources/play.png", wxBITMAP_TYPE_PNG),
							      wxPoint(x += 25, y), wxSize(25, 25));

	_stopBtn = new wxBitmapButton(this, STOP_BTN,
								  wxBitmap("Ressources/stop.png", wxBITMAP_TYPE_PNG),
								  wxPoint(x += 25, y), wxSize(25, 25));// , wxBORDER_NONE);

	_forwBtn = new wxBitmapButton(this, FWRD_BTN,
								  wxBitmap("Ressources/forward.png", wxBITMAP_TYPE_PNG),
								  wxPoint(x += 25, y), wxSize(25, 25));// , wxBORDER_NONE);
}

double ControlBar::GetSliderValue()
{
	return _slider->GetValue() / 1000.0;
}

void ControlBar::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	SetSize(size);

	_slider->SetPosition(wxPoint(4, 2));
	_slider->SetSize(wxSize(size.x - 8, 25));
	
	int y = 30, x = (size.x - 150) * 0.5;
	_backBtn->SetPosition(wxPoint(x += 25, y));
	_pauseBtn->SetPosition(wxPoint(x += 25, y));
	_playBtn->SetPosition(wxPoint(x += 25, y));
	_stopBtn->SetPosition(wxPoint(x += 25, y));
	_forwBtn->SetPosition(wxPoint(x += 25, y));
}

void ControlBar::OnBackBtn(wxCommandEvent& event)
{
	event.Skip();
	DEBUG_STREAM << "ControlBar OnBackBtn" << endl;
}

void ControlBar::SetSliderValue(const double& v)
{
	_slider->SetValue(v * 1000.0);
}

void ControlBar::OnStopBtn(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnPauseBtn(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnPlayBtn(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnFwrdBtn(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnSliderLeftDown(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnSliderLeftUp(wxCommandEvent& event)
{
	event.Skip();
}

void ControlBar::OnSliderMotion(wxCommandEvent& event)
{
	//DEBUG_STREAM << "TEST" << endl;
	event.Skip();
}