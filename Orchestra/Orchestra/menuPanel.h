#include "wx/wx.h"
#include "wx/treectrl.h"

class MenuPanel : public wxTreeCtrl
{
public:
	MenuPanel(wxWindow* win, 
			  const wxWindowID& id, 
			  const wxPoint& pos, 
			  const wxSize& size, 
			  const wxString& currentFolderPath);

	~MenuPanel();

private:
	//wxTreeCtrl* _treeMenu;
	wxString _rootPath;
};