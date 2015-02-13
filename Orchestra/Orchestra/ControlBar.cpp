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

const wxSize ControlBar::buttonsSize(40, 28);

ControlBar::ControlBar(wxWindow* win,
					   const wxWindowID& id,
					   const wxPoint& pos,
					   const wxSize& size) :
	// Heritage
	wxPanel(win, id, pos, size),
	// Members
	buttons({ // This seems to be the only syntax supported by VisualStudio 2013; it compiles even though IntelliSense reports an error.  Without "Button" there is an "internal error".
		Button{ _backBtn,  BACK_BTN,  "Ressources/backward.png" },
		Button{ _pauseBtn, PAUSE_BTN, "Ressources/pause.png" },
		Button{ _playBtn,  PLAY_BTN,  "Ressources/play.png" },
		Button{ _stopBtn,  STOP_BTN,  "Ressources/stop.png" },
		Button{ _forwBtn,  FWRD_BTN,  "Ressources/forward.png" }
	})
{
	wxImage::AddHandler(new wxPNGHandler);

	SetBackgroundColour(wxColor(240, 240, 240));

	_slider = new wxSlider(this, SLIDER_MOTION, 0, 0, 1000);

	for (auto& button : buttons)
		button.buttonPointer = new wxBitmapButton(this,
			button.id, wxBitmap(button.pngFileName, wxBITMAP_TYPE_PNG),
			wxPoint(), buttonsSize
			);
	updateWidgetsPositions(size);
}

void ControlBar::updateWidgetsPositions(const wxSize& size)
{
	_slider->SetPosition(wxPoint(4, 2));
	_slider->SetSize(wxSize(size.x - 8, 25));
	
	int totalButtonsWidth = buttonsSize.GetWidth() * N_BUTTONS;
	int y = 26, x = (size.GetWidth() - totalButtonsWidth) / 2;
	for (auto& button : buttons)
		button.buttonPointer->SetPosition(wxPoint(x += buttonsSize.GetWidth(), y));
}

double ControlBar::GetSliderValue()
{
	return _slider->GetValue() / 1000.0;
}

void ControlBar::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	SetSize(size);
	updateWidgetsPositions(size);
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