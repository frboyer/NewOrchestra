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

// Get the rectangle of height bottomRectHeight, removing it from the bottom of rect.
static wxRect rectAtBottom(wxRect& rect, int bottomRectHeight)
{
	rect.height -= bottomRectHeight;
	return wxRect(rect.GetLeft(), rect.GetBottom() + 1, rect.GetWidth(), bottomRectHeight);
}
// To call functions that take a position and a size instead of a rectangle.
#define POSITION_AND_SIZE(RECT) (RECT).GetPosition(), (RECT).GetSize()

MediaPlayer::MediaPlayer(wxWindow* win, 
						 const wxWindowID& id, 
						 const wxPoint& pos, 
						 const wxSize& size):
						 // Heritage
						 wxPanel(win, id, pos, size)
{
	SetBackgroundColour(wxColor(80, 80, 80));

	DEBUG_STREAM << "Init MediaPlayer" << endl;

	wxRect remainingRect(pos, size);
	wxRect controlBarRect = rectAtBottom(remainingRect, CTRL_BAR_HEIGHT);
	wxRect videoRect = rectAtBottom(remainingRect, remainingRect.height / 2);

	#define NEW_SUB_PANEL(TYPE, ID, RECT, ...) new TYPE(this, ID, POSITION_AND_SIZE(RECT), __VA_ARGS__)
	static const auto noId = wxID_ANY;
	_ctrlBar     = NEW_SUB_PANEL(ControlBar,     noId,         controlBarRect);
	_videoPlayer = NEW_SUB_PANEL(VlcVideoPlayer, VIDEO_MOTION, videoRect);
	_device3D    = NEW_SUB_PANEL(Device3D,       noId,         remainingRect);
	#undef NEW_SUB_PANEL
}

bool MediaPlayer::loadMedia(const char* videoPath)
{
	return _videoPlayer->loadVideo(videoPath);
}

void MediaPlayer::OnSize(wxSizeEvent& event)
{
	SetSize(event.GetSize());

	wxSize mediaSize(GetSize().x, (GetSize().y - CTRL_BAR_HEIGHT) * 0.5);
	_videoPlayer->SetPosition(wxPoint(0, mediaSize.y));
	_videoPlayer->SetSize(mediaSize);

	_device3D->SetPosition(wxPoint(0, 0));
	_device3D->SetSize(mediaSize);
	//DEBUG_STREAM << "Size MediaPlayer" << endl;

	_ctrlBar->SetPosition(wxPoint(0, GetSize().y - CTRL_BAR_HEIGHT));
	_ctrlBar->SetSize(wxSize(GetSize().x, CTRL_BAR_HEIGHT));



}

void MediaPlayer::OnBackBtn(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
	_videoPlayer->backward();
	//videoInterface->backward();
	//DEBUG_STREAM << "MediaPlayer OnBackBtn" << endl;
}

void MediaPlayer::OnStopBtn(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
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
	UNREFERENCED_PARAMETER(event);
	_videoPlayer->forward();
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnFwrdBtn" << endl;
}

void MediaPlayer::OnVideoMotion(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
	_ctrlBar->SetSliderValue(_videoPlayer->getChunkPosition());
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnVideoMotion" << endl;
}

void MediaPlayer::OnSliderMotion(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
	_videoPlayer->navigate(_ctrlBar->GetSliderValue());
	//videoInterface->navigate(controlBar->getSliderValue());
	//_ctrlBar->SetSliderValue(_videoPlayer->getPosition());
	//videoInterface->forward();
	//DEBUG_STREAM << "MediaPlayer OnSliderMotion" << endl;
}