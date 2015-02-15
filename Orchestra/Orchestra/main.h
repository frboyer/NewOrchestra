#include <iostream>
#include "wx/wx.h"

#include "VlcPlayer.h"
#include "Panel3D.h"
#include "Partition.h"
#include "ScoreGL.h"
#include "PlayerBarGL.h"

class VlcVideoPlayer;
class Device3D;

using namespace std;

enum MainFrameEventID
{
	EVENT_PARTITION_TIMER_ID = 9000,
};

class MainFrame : public wxFrame
{
public:
	MainFrame(wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size);


private:
	wxPanel* _panel;

	BasicGLPane* _axWrapper;

	VlcVideoPlayer* _videoPlayer;
	Device3D* _device3D;
	libvlc_time_t _lastTime;
	static const int _maxTimeJumpForInterpolation = 1000; // in milliseconds

	//--------------------
	ScoreGL* _score;
	axPanel* _axMainPanel;
	PlayerBarGL* _playerBar;
	//--------------------
	wxTimer* _partitionTimer;

	void OnSize(wxSizeEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnPartitionTimer(wxTimerEvent& event);

	void Resize();

	axEVENT_ACCESSOR(axButton::Msg, OnBackwardButton);
	void OnBackwardButton(const axButton::Msg& msg);

	axEVENT_ACCESSOR(axToggle::Msg, OnPlayPauseButton);
	void OnPlayPauseButton(const axToggle::Msg& msg);

	axEVENT_ACCESSOR(axButton::Msg, OnStopButton);
	void OnStopButton(const axButton::Msg& msg);

	axEVENT_ACCESSOR(axButton::Msg, OnForwardButton);
	void OnForwardButton(const axButton::Msg& msg);

	axEVENT_ACCESSOR(ScoreGL::Msg, OnScoreBarChange);
	void OnScoreBarChange(const ScoreGL::Msg& msg);

	axEVENT_ACCESSOR(axToggle::Msg, OnToggleAnimation);
	void OnToggleAnimation(const axToggle::Msg& msg);

	axEVENT_ACCESSOR(axToggle::Msg, OnToggleVideo);
	void OnToggleVideo(const axToggle::Msg& msg);

	axEVENT_ACCESSOR(axToggle::Msg, OnToggleScore);
	void OnToggleScore(const axToggle::Msg& msg);

	DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
public:
	bool OnInit();

private:
	wxMenuBar* _menu;
	wxFrame* _frame;

	void CreateMenu();
};

