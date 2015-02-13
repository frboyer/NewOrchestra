#include <iostream>
#include "wx/wx.h"

#include "menuPanel.h"
#include "MediaPLayer.h"
#include "Partition.h"


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
	MenuPanel* _menuPanel;
	wxPanel* _panel;
	MediaPlayer* _mediaPanel;
	Partition* _partition;

	wxTimer* _partitionTimer;

	void OnSize(wxSizeEvent& event);
	void OnCloseWindow(wxCloseEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnPartitionTimer(wxTimerEvent& event);
	void OnStartTimer(wxCommandEvent& event);
	void OnPauseStopTimer(wxCommandEvent& event);
	void Resize();

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

