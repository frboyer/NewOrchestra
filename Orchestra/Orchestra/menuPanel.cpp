#include "menuPanel.h"

MenuPanel::MenuPanel(wxWindow* win,
					 const wxWindowID& id,
					 const wxPoint& pos,
					 const wxSize& size,
					 const wxString& currentFolderPath):
					 // Heritage
					 wxTreeCtrl(win, id, pos, size, 91, wxDefaultValidator, wxT("Menu")),
					 // Members
					 _rootPath(currentFolderPath)
{
	//SetBackgroundColour(wxColor(80, 80, 80));
	wxTreeItemId root = AddRoot(wxT("Root"));
	AppendItem(root, wxT("Lesson 1"));
	AppendItem(root, wxT("Lesson 2"));
	AppendItem(root, wxT("Lesson 3"));
}

MenuPanel::~MenuPanel()
{

}