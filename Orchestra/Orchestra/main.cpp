#include "main.h"
#include "Debug.h"
#include "VlcPlayer.h"
#include "Panel3D.h"
#include "OrchestraConstValue.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	// Create the main frame window
	_frame = new MainFrame(NULL, wxT("Orchestra"), wxDefaultPosition, wxSize(640, 480));

	CreateMenu();
	return true;
}

void MyApp::CreateMenu()
{
	// Make a menubar
	wxMenu *file_menu = new wxMenu;
	file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT));
	_menu = new wxMenuBar;
	_menu->Append(file_menu, wxT("&File"));
	_frame->SetMenuBar(_menu);

	// Show the frame
	_frame->Show(true);
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnCloseWindow)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TIMER(EVENT_PARTITION_TIMER_ID, MainFrame::OnPartitionTimer)
END_EVENT_TABLE()

static wxRect rectAtBottom(wxRect& rect, int bottomRectHeight)
{
	rect.height -= bottomRectHeight;
	return wxRect(rect.GetLeft(), rect.GetBottom() + 1, rect.GetWidth(), bottomRectHeight);
}

MainFrame::MainFrame(wxFrame *frame, 
				 const wxString& title, 
				 const wxPoint& pos, 
			     const wxSize& size) :
				 wxFrame(frame, wxID_ANY, title, pos, size, 
						 wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE)
{
	_panel = new wxPanel(this, -1, wxPoint(0, 0), GetSize() );
	
	wxSize psize = _panel->GetSize();

	int x = (size.x - 150) * 0.5;

	wxRect remainingRect(pos, wxSize(size.x * 0.5, size.y));
	wxRect videoRect = rectAtBottom(remainingRect, remainingRect.height / 2);
	_videoPlayer = new VlcVideoPlayer(_panel, wxID_ANY, videoRect.GetPosition(), videoRect.GetSize());
	_device3D = new Device3D(_panel, wxID_ANY, remainingRect.GetPosition(), remainingRect.GetSize());

	char* videoPath = "Resources/rien.mp4";
	_videoPlayer->loadVideo(videoPath);

	// Arguments for wxGLCanvas.
	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 }; 
	
	// Instance wxOpenGL window and axLib wraper.
	_axWrapper = new BasicGLPane(_panel, wxPoint(150, 0), wxSize(size.x - 150, psize.y), args);
		 
	//------------------------------------------------------------------------------------
	// axLib code.
	//------------------------------------------------------------------------------------
	
	///bool loadInfo(const wxString& data_path, const vector<wxString>& list);
	std::string data_path("Ressources/Lesson1/output.data");
	axStringVector img_list;
	img_list.push_back("Ressources/Lesson1/test000.png");
	img_list.push_back("Ressources/Lesson1/test001.png");
	img_list.push_back("Ressources/Lesson1/test002.png");
	img_list.push_back("Ressources/Lesson1/test003.png");
	img_list.push_back("Ressources/Lesson1/test004.png");
	img_list.push_back("Ressources/Lesson1/test005.png");

	_axMainPanel = new axPanel(nullptr, axRect(0, 0, size.x, size.y));
	_axMainPanel->SetWindowColor(axColor(0.6));

	_score = new ScoreGL(_axMainPanel, 
						 axRect(size.x * 0.5, 0, size.x * 0.5, size.y),
						 ScoreGL::Events(GetOnScoreBarChange()));

	PlayerBarGL::Events playBarEvents;
	playBarEvents.backward_click = GetOnBackwardButton();
	playBarEvents.forward_click = GetOnForwardButton();
	playBarEvents.stop_click = GetOnStopButton();
	playBarEvents.play_pause_click = GetOnPlayPauseButton();
	playBarEvents.toggle_animation = GetOnToggleAnimation();
	playBarEvents.toggle_video = GetOnToggleVideo();
	playBarEvents.toggle_score = GetOnToggleScore();

	_playerBar = new PlayerBarGL(_axMainPanel, 
		axRect(0, size.y - PLAYER_BAR_HEIGHT, size.x * 0.5, PLAYER_BAR_HEIGHT),
								 playBarEvents);

	
	//_playerBar->SetVideoLength(_videoPlayer->getMovieLength() / 1000.0);

	_score->loadInfo(data_path, img_list);
	//-----------------------------------------------------------------------------------

	_partitionTimer = new wxTimer(this, EVENT_PARTITION_TIMER_ID);
	
	Maximize(true);
}

void MainFrame::Resize()
{
	wxSize psize = GetSize();
	_panel->SetSize(psize);

	int w = psize.x * 0.5;
	int h = psize.y - PLAYER_BAR_HEIGHT;

	// axLib wrapper resize.
	_axWrapper->SetPosition(wxPoint(0, 0));
	_axWrapper->SetSize(wxSize(psize.x, psize.y));
	_axMainPanel->SetSize(axSize(psize.x, psize.y));
	_playerBar->SetRect(axRect(0, h, psize.x, PLAYER_BAR_HEIGHT));

	bool videoShown = _videoPlayer->IsShown();
	bool animShown = _device3D->IsShown();
	bool scoreShown = _score->IsShown();

	if (videoShown && animShown && scoreShown)
	{
		_device3D->SetPosition(wxPoint(0, 0));
		_device3D->SetSize(wxSize(w - 5, h * 0.5 - 5));

		_videoPlayer->SetPosition(wxPoint(0, h * 0.5));
		_videoPlayer->SetSize(wxSize(w - 5, h * 0.5));

		_score->SetRect(axRect(w, 0, w, h));
	}
	else if (!videoShown && animShown && scoreShown)
	{
		_device3D->SetPosition(wxPoint(0, 0));
		_device3D->SetSize(wxSize(w - 5, h));

		_score->SetRect(axRect(w, 0, w, h));
	}
	else if (videoShown && !animShown && scoreShown)
	{
		_videoPlayer->SetPosition(wxPoint(0, 0));
		_videoPlayer->SetSize(wxSize(w - 5, h));

		_score->SetRect(axRect(w, 0, w, h));
	}
	else if (!videoShown && !animShown && scoreShown)
	{
		_score->SetRect(axRect(0, 0, psize.x, h));
	}
	else if (videoShown && animShown && !scoreShown)
	{
		_device3D->SetPosition(wxPoint(0, 0));
		_device3D->SetSize(wxSize(w - 5, h));

		_videoPlayer->SetPosition(wxPoint(w, 0));
		_videoPlayer->SetSize(wxSize(w, h));
	}
	else if (videoShown && !animShown && !scoreShown)
	{
		_videoPlayer->SetPosition(wxPoint(0, 0));
		_videoPlayer->SetSize(wxSize(psize.x, h));
	}
	else if (!videoShown && animShown && !scoreShown)
	{
		_device3D->SetPosition(wxPoint(0, 0));
		_device3D->SetSize(wxSize(psize.x, h));
	}
}

void MainFrame::OnSize(wxSizeEvent& event)
{
	SetSize(event.GetSize());
	Resize();
}

// Intercept menu commands
void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	static bool destroyed = false;
	
	if (destroyed)
	{
		return;
	}
	
	delete _partitionTimer; _partitionTimer = nullptr;
	Destroy();
	destroyed = true;
}

void MainFrame::OnPartitionTimer(wxTimerEvent& event)
{
	UNREFERENCED_PARAMETER(event);

	double ms = _videoPlayer->getPlaybackTime() / 1000.0;
	_score->changeTime(ms);
	_device3D->timerEvent(ms);

	_playerBar->SetScrollSliderValue(_videoPlayer->getPosition(), 
									 ms);
	
	if (_playerBar->GetHasVideoLength() == false)
	{
		_playerBar->SetVideoLength(_videoPlayer->getMovieLength() / 1000.0);
	}
}

void MainFrame::OnBackwardButton(const axButton::Msg& msg)
{
	_videoPlayer->backward();
}

void MainFrame::OnPlayPauseButton(const axToggle::Msg& msg)
{
	if (msg.GetSelected())
	{
		_videoPlayer->play();

		if (!_partitionTimer->IsRunning())
		{
			_partitionTimer->Start(16);
			_score->setPlaying(true);
		}
	}
	else
	{
		_videoPlayer->pause();

		if (_partitionTimer->IsRunning())
		{
			_partitionTimer->Stop();
			_score->setPlaying(false);
		}
	}
}

void MainFrame::OnStopButton(const axButton::Msg& msg)
{
	_videoPlayer->stop();

	if (_partitionTimer->IsRunning())
	{
		_partitionTimer->Stop();
		_score->setPlaying(false);
	}
}

void MainFrame::OnForwardButton(const axButton::Msg& msg)
{
	_videoPlayer->forward();
}

void MainFrame::OnScoreBarChange(const ScoreGL::Msg& msg)
{
	_playerBar->SetPage(msg.GetPage());
	_playerBar->SetBar(msg.GetBar());
	
}

void MainFrame::OnToggleAnimation(const axToggle::Msg& msg)
{
	if (!msg.GetSelected() && !_videoPlayer->IsShown() && !_score->IsShown())
	{
		msg.GetSender()->SetSelected(true);
	}
	else
	{
		_device3D->Show(msg.GetSelected());
		Resize();
	}
}


void MainFrame::OnToggleVideo(const axToggle::Msg& msg)
{
	if (!msg.GetSelected() && !_device3D->IsShown() && !_score->IsShown())
	{
		msg.GetSender()->SetSelected(true);
	}
	else
	{
		_videoPlayer->Show(msg.GetSelected());
		Resize();
	}
}


void MainFrame::OnToggleScore(const axToggle::Msg& msg)
{
	if (!msg.GetSelected() && !_device3D->IsShown() && !_videoPlayer->IsShown())
	{
		msg.GetSender()->SetSelected(true);
	}
	else
	{
		if (_score->IsShown())
		{
			_score->Hide();
		}
		else
		{
			_score->Show();
		}

		Resize();
	}
}