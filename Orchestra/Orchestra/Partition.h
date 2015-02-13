#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include <vector>

using namespace std;

struct t_point { short x, y; };
struct t_size { short x, y; };

// Is use for markers data info.
struct t_data
{
	int posInFile;
	t_point point;
	t_size size;
	short numImg;
	double time;
};

/// @todo Change to GL Canvas to make resizeing faster.
class Partition : public wxPanel
{
public:
	Partition(wxWindow* win, wxWindowID id, 
			  const wxPoint& pos, const wxSize& size);
	~Partition();

	bool loadInfo(const wxString& data_path, const vector<wxString>& list);
	bool loadMarkerInfo(wxString path);
	void setImageList(const vector<wxString>& list);
	bool changeTime(double timeMs);
	void setPlaying(const bool& playing);
	int findFirstMarkerOfImage(const int& imgNum);
	double getSelectedMarkerTimeMs();

private:
	vector<wxString> _imgList;
	wxImage _currentImg, _currentMarkerImg, _mouseHoverImage;
	t_data* _markerData;
	bool _isPlaying, _mouseHover;
	int _nbMarker, _selectedMarker, _mouseHoverMarker;
	//int m_id, m_totalHeightOfAllImages;
	double _resizeRatio_x, _resizeRatio_y;
	static const int BORDER = 50;

	// Create and resize a transparent image for the selected marker.
	wxImage createSelectedMarkerImage(const int& selected_marker, 
								      const wxColor& color, const int& alpha = 110);

	void scaleImage(const int& imagelistPosition);

	// Events.
	void OnSize(wxSizeEvent& event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnMouseDClick(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()
};
