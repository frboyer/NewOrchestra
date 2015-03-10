#include "main.h"
#include "Debug.h"
#include "VlcPlayer.h"
#include "Panel3D.h"
#include "OrchestraConstValue.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	// Create the main frame window
	_frame = new MainFrame(NULL, wxT("Orchestra"), wxDefaultPosition, wxSize(640, 480));

	CreateMenu();

	// Show the frame
	_frame->Show(true);
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
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnCloseWindow)
	EVT_MENU(wxID_EXIT, MainFrame::OnExit)
	EVT_SIZE(MainFrame::OnSize)
	EVT_TIMER(EVENT_PARTITION_TIMER_ID, MainFrame::OnPartitionTimer)
	EVT_TIMER(EVENT_ANIMATION_TIMER_ID, MainFrame::OnAnimationTimer)
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
	_menuActive = false;

	_panel = new wxPanel(this, -1, wxPoint(0, 0), GetSize());
	
	wxSize psize = _panel->GetSize();

	wxRect remainingRect(pos, wxSize(size.x * 0.5, size.y));
	wxRect videoRect = rectAtBottom(remainingRect, remainingRect.height / 2);
	_videoPlayer = new VlcVideoPlayer(_panel, wxID_ANY, videoRect.GetPosition(), videoRect.GetSize());
	_device3D = new Device3D(_panel, wxID_ANY, remainingRect.GetPosition(), remainingRect.GetSize());
	
	_videoPath.front = "C:\\Users\\Alexandre Arsenault\\Desktop\\rien.mp4";
	_videoPath.left = "C:\\Users\\Alexandre Arsenault\\Desktop\\Varese Arcana Gauche.mp4";
	_videoPath.right = "C:\\Users\\Alexandre Arsenault\\Desktop\\Varese Arcana Droite.mp4";
	
	//char* videoPath = "C:\\Users\\Alexandre Arsenault\\Desktop\\rien.mp4";//"Ressources/rien.mp4";
	//char* videoPath = "Ressources/rien.mp4";
	_videoPlayer->loadVideo(_videoPath.front.c_str());

	// Arguments for wxGLCanvas.
	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 }; 
	
	// Instance wxOpenGL window and axLib wraper.
	_axWrapper = new axWxPanel(_panel, wxPoint(0, 0), wxSize(psize.x, psize.y), args);
		 
//	//------------------------------------------------------------------------------------
//	// axLib code.
//	//------------------------------------------------------------------------------------
//	
	///bool loadInfo(const wxString& data_path, const vector<wxString>& list);
	std::string data_path("Ressources/Lesson1/output.data");
	axStringVector img_list;
	img_list.push_back("Ressources/Lesson1/test000.png");
	img_list.push_back("Ressources/Lesson1/test001.png");
	img_list.push_back("Ressources/Lesson1/test002.png");
	img_list.push_back("Ressources/Lesson1/test003.png");
	img_list.push_back("Ressources/Lesson1/test004.png");
	img_list.push_back("Ressources/Lesson1/test005.png");

	_axMainPanel = new axPanel(nullptr, axRect(0, 0, psize.x, psize.y));
	//std::cout << "axLib : TOP PARENT PANEL ID : " << _axMainPanel->GetId() << std::endl;
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
	playBarEvents.left_click = GetOnLeftButton();
	playBarEvents.front_click = GetOnFrontButton();
	playBarEvents.right_click = GetOnRightButton();
	playBarEvents.menu_toggle = GetOnMenuToggle();

	_playerBar = new PlayerBarGL(_axMainPanel, 
								 axRect(0, size.y - PLAYER_BAR_HEIGHT, size.x * 0.5, PLAYER_BAR_HEIGHT),
								 playBarEvents);

	
	//_playerBar->SetVideoLength(_videoPlayer->getMovieLength() / 1000.0);

	_score->loadInfo(data_path, img_list);


	std::vector<LessonInfo> folder1_lessons_info = 
	{ 
		LessonInfo(1, "Nom de la piece", "02:26"),
		LessonInfo(2, "Nom de la piece2", "04:21")
	};

	std::vector<LessonInfo> folder2_lessons_info =
	{
		LessonInfo(1, "Nom de la piece", "01:20"),
		LessonInfo(2, "Nom de la piece2", "03:47"),
		LessonInfo(3, "Nom de la piece3", "00:58")
	};

	std::vector<LessonInfo> folder3_lessons_info =
	{
		LessonInfo(1, "Blalala", "06:52"),
		LessonInfo(2, "Nom de la piece2", "04:07")
	};

	std::vector<FolderInfo> folders_info = 
	{
		FolderInfo("Piece 1", "Alexandre Arsenault", folder1_lessons_info),
		FolderInfo("Piece 2", "Peter Smith", folder2_lessons_info),
		FolderInfo("Piece 3", "John Ferg", folder3_lessons_info)
	};

	_menu = new OrchestraMenu(_axMainPanel, 
		axRect(0, 0, size.y - PLAYER_BAR_HEIGHT, 200), folders_info);
		
	_menu->Hide();
//	//-----------------------------------------------------------------------------------
//
	_partitionTimer = new wxTimer(this, EVENT_PARTITION_TIMER_ID);
	_animationTimer = new wxTimer(this, EVENT_ANIMATION_TIMER_ID);

	Maximize(true);
}

MainFrame::~MainFrame()
{

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
	_menu->Resize(axSize(200 - 5,h));

	bool videoShown = _videoPlayer->IsShown();
	bool animShown = _device3D->IsShown();
	bool scoreShown = _score->IsShown();

	int menu_width = 200;
	int left_pos_x = 0;
	int middle_pos_x = w;


	//axPrint("Middle position", middle_pos_x);
	if (_menuActive)
	{
		w = (psize.x - menu_width) * 0.5;
		left_pos_x = menu_width;
		middle_pos_x = menu_width + (psize.x - menu_width) * 0.5;
		psize.x -= menu_width;
	}

	if (videoShown && animShown && scoreShown)
	{
		_device3D->SetPosition(wxPoint(left_pos_x, 0));
		_device3D->SetSize(wxSize(w - 5, h * 0.5 - 5));

		_videoPlayer->SetPosition(wxPoint(left_pos_x, h * 0.5));
		_videoPlayer->SetSize(wxSize(w - 5, h * 0.5));

		_score->SetRect(axRect(middle_pos_x, 0, w, h));
	}
	else if (!videoShown && animShown && scoreShown)
	{
		_device3D->SetPosition(wxPoint(left_pos_x, 0));
		_device3D->SetSize(wxSize(w - 5, h));

		_score->SetRect(axRect(middle_pos_x, 0, w, h));
	}
	else if (videoShown && !animShown && scoreShown)
	{
		_videoPlayer->SetPosition(wxPoint(left_pos_x, 0));
		_videoPlayer->SetSize(wxSize(w - 5, h));

		_score->SetRect(axRect(middle_pos_x, 0, w, h));
	}
	else if (!videoShown && !animShown && scoreShown)
	{
		_score->SetRect(axRect(left_pos_x, 0, psize.x, h));
	}
	else if (videoShown && animShown && !scoreShown)
	{
		_device3D->SetPosition(wxPoint(left_pos_x, 0));
		_device3D->SetSize(wxSize(w - 5, h));

		_videoPlayer->SetPosition(wxPoint(middle_pos_x, 0));
		_videoPlayer->SetSize(wxSize(w, h));
	}
	else if (videoShown && !animShown && !scoreShown)
	{
		_videoPlayer->SetPosition(wxPoint(left_pos_x, 0));
		_videoPlayer->SetSize(wxSize(psize.x, h));
	}
	else if (!videoShown && animShown && !scoreShown)
	{
		_device3D->SetPosition(wxPoint(left_pos_x, 0));
		_device3D->SetSize(wxSize(psize.x, h));
	}

	_axWrapper->Refresh();
	_score->Update();
	_axMainPanel->Update();
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
	delete _animationTimer; _animationTimer = nullptr;
	Destroy();
	destroyed = true;
}

void MainFrame::OnPartitionTimer(wxTimerEvent& event)
{
	UNREFERENCED_PARAMETER(event);

	double ms = _videoPlayer->getPlaybackTime() / 1000.0;

	// Change score time.
	_score->changeTime(ms);

	// Change playbar time.
	_playerBar->SetScrollSliderValue(_videoPlayer->getPosition(), ms);
								
	// This is there only because it doesn't seem to work when the 
	// video is not playing. It should be call only once.
	if (_playerBar->GetHasVideoLength() == false)
	{
		_playerBar->SetVideoLength(_videoPlayer->getMovieLength() / 1000.0);
	}
}

void MainFrame::OnAnimationTimer(wxTimerEvent& event)
{
	UNREFERENCED_PARAMETER(event);

	double ms = _videoPlayer->getPlaybackTime() / 1000.0;
	_device3D->timerEvent(ms);
}

void MainFrame::OnBackwardButton(const axButton::Msg& msg)
{
	(msg);
	_videoPlayer->backward();
}

void MainFrame::OnPlayPauseButton(const axToggle::Msg& msg)
{
	if (msg.GetSelected())
	{
		_videoPlayer->play();

		// Hide menu when playing.
		if (_menuActive)
		{
			_menu->Hide();
			_playerBar->HideMenu();
			_menuActive = false;
			Resize();
			Refresh();
		}

		if (!_partitionTimer->IsRunning())
		{
			_partitionTimer->Start(250);
			_score->setPlaying(true);
		}

		if (!_animationTimer->IsRunning())
		{
			_animationTimer->Start(16);
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

		if (_animationTimer->IsRunning())
		{
			_animationTimer->Stop();
		}
	}
}

void MainFrame::OnStopButton(const axButton::Msg& msg)
{
	(msg);
	_videoPlayer->stop();

	if (_partitionTimer->IsRunning())
	{
		_partitionTimer->Stop();
		_score->setPlaying(false);
	}

	if (_animationTimer->IsRunning())
	{
		_animationTimer->Stop();
	}
}

void MainFrame::OnForwardButton(const axButton::Msg& msg)
{
	(msg);
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


/// @todo Make sure the video's different before reload.
void  MainFrame::ChangeVideoAngle(const std::string& path)
{
	bool isPlaying = _videoPlayer->isPlaying();
	long long time = _videoPlayer->getPlaybackTime();
	_videoPlayer->loadVideo(path.c_str());

	if (isPlaying)
	{
		_videoPlayer->play();

		axPrint("is video playing right after play call : ", _videoPlayer->isPlaying());
		axPrint("!!! POSIBLE INFINITE LOOP !!! : ", __LINE__, " in main.cpp");
		while (_videoPlayer->isPlaying() == false)
		{
		}

		// From VLC doc, this is not supported for every formats.
		// And this has no effect if the media is not playing.
		_videoPlayer->setPlaybackTime(time);
		_videoPlayer->unMute();
	}
}

void MainFrame::OnLeftButton(const axButton::Msg& msg)
{
	(msg);
	_device3D->SetLeftAlign();
	ChangeVideoAngle(_videoPath.left);
}



void MainFrame::OnFrontButton(const axButton::Msg& msg)
{
	(msg);
	
	_device3D->SetFrontAlign();
	ChangeVideoAngle(_videoPath.front);
	
}


void MainFrame::OnRightButton(const axButton::Msg& msg)
{
	(msg);
	_device3D->SetRightAlign();
	ChangeVideoAngle(_videoPath.right);
}

void MainFrame::OnMenuToggle(const axToggle::Msg& msg)
{
	if (msg.GetSelected())
	{
		if (_videoPlayer->isPlaying())
		{
			_playerBar->SetPauseToggle();
			_videoPlayer->pause();

			if (_partitionTimer->IsRunning())
			{
				_partitionTimer->Stop();
				_score->setPlaying(false);
			}
		}

		_menuActive = true;
		_menu->Show();
		

	}
	else
	{
		//std::cout << "Menu off" << std::endl;
		_menuActive = false;
		_menu->Hide();
	}

	Resize();
}