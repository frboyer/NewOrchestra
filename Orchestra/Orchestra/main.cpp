#include "main.h"
#include "Debug.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
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
	EVT_BUTTON(PLAY_BTN, MainFrame::OnStartTimer)
	//EVT_BUTTON(STOP_BTN, MainFrame::OnPauseStopTimer)
	//EVT_BUTTON(PAUSE_BTN, MainFrame::OnPauseStopTimer)
END_EVENT_TABLE()

// My frame constructor
MainFrame::MainFrame(wxFrame *frame, 
				 const wxString& title, 
				 const wxPoint& pos, 
			     const wxSize& size) :
				 wxFrame(frame, wxID_ANY, title, pos, size, 
						 wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE)
{
	_panel = new wxPanel(this, -1, wxPoint(0, 0), GetSize() );
	
	wxSize psize = _panel->GetSize();
	_menuPanel = new MenuPanel(_panel, wxID_ANY, 
							   wxPoint(0, 0), 
							   wxSize(150, psize.y), wxT(""));

	int x = (size.x - 150) * 0.5;
	_mediaPanel = new MediaPlayer(_panel, wxID_ANY, 
								  wxPoint(150, 0), 
								  wxSize(x, psize.y));

	char* path = "Resources/rien.mp4";
	_mediaPanel->loadMedia(path);

	_partition = new Partition(_panel, wxID_ANY,
							   wxPoint(150 + x, 0),
							   wxSize(x, psize.y));

	///bool loadInfo(const wxString& data_path, const vector<wxString>& list);
	wxString data_path = wxT("Ressources/Lesson1/output.data");
	vector<wxString> img_list;
	img_list.push_back("Ressources/Lesson1/test000.png");
	img_list.push_back("Ressources/Lesson1/test001.png");
	img_list.push_back("Ressources/Lesson1/test002.png");
	img_list.push_back("Ressources/Lesson1/test003.png");
	img_list.push_back("Ressources/Lesson1/test004.png");
	img_list.push_back("Ressources/Lesson1/test005.png");

	_partition->loadInfo(data_path, img_list);

	_partitionTimer = new wxTimer(this, EVENT_PARTITION_TIMER_ID);

	Maximize(true);
}

void MainFrame::Resize()
{
	_panel->SetSize(GetSize());
	wxSize psize = _panel->GetSize();
	_menuPanel->SetSize(wxSize(150, psize.y));

	int x = (psize.x - 150) * 0.5;
	_mediaPanel->SetPosition(wxPoint(150, 0));
	_mediaPanel->SetSize(wxSize(x, psize.y));

	_partition->SetPosition(wxPoint(150+x, 0));
	_partition->SetSize(wxSize(x, psize.y));

	//DEBUG_STREAM << _partition->GetSize().x << " : " << _partition->GetSize().y << endl;
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
	// DEBUG_STREAM << "clock " << VlcVideoPlayer::getClockTime() << endl;
	double ms = _mediaPanel->getVideoTimeMs();
	_partition->changeTime(ms);
	_mediaPanel->_device3D->timerEvent(ms);
}

void MainFrame::OnStartTimer(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
	/// @todo Make sure play and pause are sync with control bar from media player.
	if (!_partitionTimer->IsRunning())
	{
		//DEBUG_STREAM << "Start Timer" << endl;
		_partitionTimer->Start(16);
		_partition->setPlaying(true);
	}
}

void MainFrame::OnPauseStopTimer(wxCommandEvent& event)
{
	UNREFERENCED_PARAMETER(event);
	if (_partitionTimer->IsRunning())
	{
		_partitionTimer->Stop();
		_partition->setPlaying(false);
	}
}
