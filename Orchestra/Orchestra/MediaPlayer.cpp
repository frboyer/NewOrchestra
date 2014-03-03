#include "MediaPlayer.h"
#include <iostream>
#include "Debug.h"


BEGIN_EVENT_TABLE(MediaPlayer, wxPanel)
	EVT_SIZE(MediaPlayer::OnSize)
	EVT_BUTTON(BACK_BTN, MediaPlayer::OnBackBtn)
	EVT_BUTTON(STOP_BTN, MediaPlayer::OnStopBtn)
	EVT_BUTTON(PLAY_BTN, MediaPlayer::OnPlayBtn)
	EVT_BUTTON(PAUSE_BTN, MediaPlayer::OnPauseBtn)
	EVT_BUTTON(FWRD_BTN, MediaPlayer::OnFwrdBtn)
	//EVT_BUTTON(SLIDER_LEFT_DOWN, MediaPlayer::OnSliderLeftDown)
	//EVT_BUTTON(SLIDER_LEFT_UP, MediaPlayer::OnSliderLeftUp)
	EVT_SLIDER(SLIDER_MOTION, MediaPlayer::OnSliderMotion)
	EVT_BUTTON(VIDEO_MOTION, MediaPlayer::OnVideoMotion)
END_EVENT_TABLE()

MediaPlayer::MediaPlayer(wxWindow* win, 
						 const wxWindowID& id, 
						 const wxPoint& pos, 
						 const wxSize& size):
						 // Heritage
						 wxPanel(win, id, pos, size)
{
	SetBackgroundColour(wxColor(80, 80, 80));

	DEBUG_STREAM << "Init MediaPlayer" << endl;

	_ctrlBar = new ControlBar(this, wxID_ANY, 
							  wxPoint(0, GetSize().y - CTRL_BAR_SIZE),
							  wxSize(GetSize().x, CTRL_BAR_SIZE));
	
	wxSize mediaSize(GetSize().x, (GetSize().y - CTRL_BAR_SIZE) * 0.5);
	
	_videoPlayer = new VlcVideoPlayer(this, VIDEO_MOTION, 
									  wxPoint(0, mediaSize.y), mediaSize);

	_device3D = new Device3D(this, wxPoint(0, 0), mediaSize);
}

bool MediaPlayer::loadMedia(const char* videoPath)
{
	return _videoPlayer->loadVideo(videoPath);
}

void MediaPlayer::OnSize(wxSizeEvent& event)
{
	SetSize(event.GetSize());

	wxSize mediaSize(GetSize().x, (GetSize().y - CTRL_BAR_SIZE) * 0.5);
	_videoPlayer->SetPosition(wxPoint(0, mediaSize.y));
	_videoPlayer->SetSize(mediaSize);

	//DEBUG_STREAM << "Size MediaPlayer" << endl;
	_device3D->SetPosition(wxPoint(0, 0));
	_device3D->SetSize(mediaSize);

	_ctrlBar->SetPosition(wxPoint(0, GetSize().y - CTRL_BAR_SIZE));
	_ctrlBar->SetSize(wxSize(GetSize().x, CTRL_BAR_SIZE));

}

void MediaPlayer::OnBackBtn(wxCommandEvent& event)
{
	_videoPlayer->backward();
	//videoInterface->backward();
	//DEBUG_STREAM << "MediaPlayer OnBackBtn" << endl;
}

void MediaPlayer::OnStopBtn(wxCommandEvent& event)
{
	_videoPlayer->stop();
	//controlBar->setSliderValue(0.0);
	//DEBUG_STREAM << "MediaPlayer OnStopBtn" << endl;
}

void MediaPlayer::OnPauseBtn(wxCommandEvent& event)
{
	//DEBUG_STREAM << "Pause Video" << endl;
	_videoPlayer->pause();
	event.Skip();
}

void MediaPlayer::OnPlayBtn(wxCommandEvent& event)
{
	//DEBUG_STREAM << "Play Video" << endl;
	_videoPlayer->play();
	event.Skip();
}

void MediaPlayer::OnFwrdBtn(wxCommandEvent& event)
{
	_videoPlayer->forward();
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnFwrdBtn" << endl;
}

void MediaPlayer::OnVideoMotion(wxCommandEvent& event)
{
	_ctrlBar->SetSliderValue(_videoPlayer->getPosition());
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnVideoMotion" << endl;
}

void MediaPlayer::OnSliderMotion(wxCommandEvent& event)
{
	_videoPlayer->navigate(_ctrlBar->GetSliderValue());
	//videoInterface->navigate(controlBar->getSliderValue());
	//_ctrlBar->SetSliderValue(_videoPlayer->getPosition());
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnSliderMotion" << endl;
}