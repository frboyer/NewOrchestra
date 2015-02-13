#include "wx/wx.h"
#include "ControlBar.h"
#include "VlcPlayer.h"
#include "Panel3D.h"

class MediaPlayer : public wxPanel
{
public:
	MediaPlayer(wxWindow* win, const wxWindowID& id, const wxPoint& pos, const wxSize& size);

	bool loadMedia(const char* videoPath);
	double getVideoTimeMs() {
		return _videoPlayer->getPlaybackTime() / 1000.0;
	}

public: //TODO: should not be public 
	ControlBar* _ctrlBar;
	VlcVideoPlayer* _videoPlayer;
	Device3D* _device3D;
	libvlc_time_t _lastTime;
	static const int _maxTimeJumpForInterpolation = 1000; // in milliseconds

private:
	static const int CTRL_BAR_HEIGHT = 120;

	void OnSize(wxSizeEvent& event);
	void OnBackBtn(wxCommandEvent& event);
	void OnStopBtn(wxCommandEvent& event);
	void OnPauseBtn(wxCommandEvent& event);
	void OnPlayBtn(wxCommandEvent& event);
	void OnFwrdBtn(wxCommandEvent& event);
	void OnSliderLeftDown(wxCommandEvent& event);
	void OnSliderLeftUp(wxCommandEvent& event);
	void OnSliderMotion(wxCommandEvent& event);
	//void changeSlider(wxCommandEvent& event);
	void OnVideoMotion(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};