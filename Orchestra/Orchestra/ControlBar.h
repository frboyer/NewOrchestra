#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "wx/bitmap.h"
#include "wx/timectrl.h"
#include "wx/spinctrl.h"
#include "wx/slider.h"

enum ControlBarID
{
	BACK_BTN = 7500,
    STOP_BTN,
	PLAY_BTN,
	PAUSE_BTN,
	FWRD_BTN,
	SLIDER_LEFT_DOWN,
	SLIDER_LEFT_UP,
	SLIDER_MOTION,
	VIDEO_MOTION,
	VIDEO_STOP
};

class ControlBar: public wxPanel
{
public:
	ControlBar(wxWindow* win, 
			   const wxWindowID& id, 
			   const wxPoint& pos, 
			   const wxSize& size);

	void SetSliderValue(const double& v);
	double GetSliderValue();

private:
	wxBitmapButton *_backBtn, *_forwBtn, *_stopBtn, *_pauseBtn, *_playBtn;
	wxSlider* _slider;
	wxSpinCtrl *_timeMin, *_timeSec;
	wxStaticText *_txtMin, *_txtSec;

	void OnSize(wxSizeEvent& event);
	void OnBackBtn(wxCommandEvent& event);
	void OnStopBtn(wxCommandEvent& event);
	void OnPlayBtn(wxCommandEvent& event);
	void OnPauseBtn(wxCommandEvent& event);
	void OnFwrdBtn(wxCommandEvent& event);
	void OnSliderLeftDown(wxCommandEvent& event);
	void OnSliderLeftUp(wxCommandEvent& event);
	void OnSliderMotion(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};