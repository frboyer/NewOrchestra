#include "Partition.h"
#include <fstream>
#include <iostream>
#include "Debug.h"

BEGIN_EVENT_TABLE(Partition, wxPanel)
	EVT_SIZE(Partition::OnSize)
	//EVT_LEFT_DOWN(Partition::OnMouseLeftDown)
	//EVT_LEFT_UP(Partition::OnMouseLeftUp)
	EVT_MOTION(Partition::OnMouseMotion)
	//EVT_BUTTON(PARTITION_SCROLL_BAR_ID, Partition::OnScrollBar)
	//EVT_MOUSE_CAPTURE_LOST(Partition::OnMouseCaptureLost)
	//EVT_LEFT_DCLICK(Partition::OnMouseDClick)
	//EVT_ENTER_WINDOW(Partition::OnMouseMotion)
	EVT_PAINT(Partition::OnPaint)
	//EVT_MOUSE_EVENTS(Partition::OnMouseMotion)
END_EVENT_TABLE()

Partition::Partition(wxWindow* win, wxWindowID id, 
					 const wxPoint& pos, const wxSize& size) : 
		   // Heritage
		   wxPanel(win, id, pos, size),
		   // Members
		   //m_id(id), 
		   _mouseHover(false), 
		   _selectedMarker(false), 
		   _markerData(nullptr),
		   //pastResizeTick(0), 
		   _isPlaying(false)
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	wxImage::AddHandler(new wxPNGHandler);
	
	DEBUG_STREAM << "Init Partition" << endl;
}

void Partition::setPlaying(const bool& playing)
{
	_isPlaying = playing;
	Refresh();
}

bool Partition::loadInfo(const wxString& data_path, const vector<wxString>& list)
{
	if (!loadMarkerInfo(data_path))
	{
		return false;
	}

	setImageList(list);

	return true;
}

bool Partition::loadMarkerInfo(wxString path)
{
	ifstream data(path.char_str(), ios::binary);

	if (data.fail())
	{
		DEBUG_STREAM << "ERROR : Load data in Partition" << endl;
		return false;
	}

	else
	{
		data.seekg(0);
		_nbMarker = 0;
		data.read((char*)&_nbMarker, sizeof(int));
		_markerData = new t_data[_nbMarker];

		DEBUG_STREAM << "NB CUE : " << _nbMarker << endl;

		data.read((char*)_markerData, sizeof(t_data)* _nbMarker);
		data.close();
	}

	return true;
}

void Partition::setImageList(const vector<wxString>& list)
{
	_imgList = list;
	_currentImg = wxImage(_imgList[0]);
	//scaleImage(0);

	int height = 0;
	for (int i = 0; i < _imgList.size(); ++i)
	{
		height += wxImage(_imgList[i]).GetHeight();
	}

	//m_totalHeightOfAllImages = height;
	//m_scrollBar->setInputInfo(size.y, m_totalHeightOfAllImages, 0);

	Refresh();
}

bool Partition::changeTime(double timeMs)
{
	timeMs /= 1000.0;

	if (timeMs > _markerData[_nbMarker - 1].time)
	{
		_selectedMarker = _nbMarker - 1;
		scaleImage(_markerData[_nbMarker - 1].numImg);
		_currentMarkerImg = createSelectedMarkerImage(_selectedMarker, wxColor(180, 180, 180));
		Refresh();

		// Scrollbar.
		//_scrollBar->setInputTopDelta(markerData[selectedMarker].numImg * m_totalHeightOfAllImages / imgList.size());

		return true;
	}

	for (int i = 0; i < _nbMarker - 1; ++i)
	{
		if (_markerData[i].time <= timeMs && _markerData[i + 1].time >= timeMs)
		{
			if (_markerData[i].numImg != _markerData[_selectedMarker].numImg)
			{
				scaleImage(_markerData[i].numImg);
			}

			if (_selectedMarker != i)
			{
				_selectedMarker = i;
				_currentMarkerImg = createSelectedMarkerImage(_selectedMarker, wxColor(180, 180, 180));
				Refresh();
			}

			//m_scrollBar->setSliderPositionZeroToOne(markerData[selectedMarker].numImg / (imgList.size() * 1.0));
			return true;
		}
	}

	return false;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Too slow.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
wxImage Partition::createSelectedMarkerImage(const int& selected_marker, const wxColor& color, const int& alpha)
{
	t_size size = _markerData[selected_marker].size;

	int x = size.x * _resizeRatio_x;
	int y = size.y * _resizeRatio_y;

	wxImage img(x, y);
	img.InitAlpha();

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < y; ++j)
		{
			img.SetAlpha(i, j, alpha);
		}	
	}

	img.SetRGB(wxRect(0, 0, x, y), color.Red(), color.Green(), color.Blue());

	return img;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Too slow.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void Partition::scaleImage(const int& imagelistPosition)
{
	DEBUG_STREAM << "Image scaling" << endl;
	int x = GetSize().x - 100, y = GetSize().y - 100;
	_currentImg = wxImage(_imgList[imagelistPosition]);

	_resizeRatio_x = double(x) / double(_currentImg.GetWidth());
	_resizeRatio_y = double(y) / double(_currentImg.GetHeight());

	//wxIMAGE_QUALITY_NORMAL
	//wxIMAGE_QUALITY_HIGH
	_currentImg.Rescale(x, y, wxIMAGE_QUALITY_HIGH);
}

int Partition::findFirstMarkerOfImage(const int& imgNum)
{
	for (int i = 0; i < _nbMarker; ++i)
	{
		if (_markerData[i].numImg == imgNum)
			return i;
	}

	return -1;
}

double Partition::getSelectedMarkerTimeMs()
{
	return _markerData[_selectedMarker].time * 1000.0;
}

void Partition::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	SetSize(size);

	scaleImage(_markerData[_selectedMarker].numImg);
	_currentMarkerImg = createSelectedMarkerImage(_selectedMarker, 
												  wxColor(180, 180, 180));
	Refresh();
}

void Partition::OnMouseDClick(wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();
	int i = findFirstMarkerOfImage(_markerData[_selectedMarker].numImg);

	while (_markerData[i].numImg == _markerData[_selectedMarker].numImg)
	{
		int pos_x = _markerData[i].point.x * _resizeRatio_x,
			size_x = _markerData[i].size.x * _resizeRatio_x,
			pos_y = _markerData[i].point.y * _resizeRatio_y,
			size_y = _markerData[i].size.y * _resizeRatio_y;

		if (pt.x >= BORDER + pos_x && pt.x < BORDER + pos_x + size_x &&
			pt.y >= BORDER + pos_y && pt.y < BORDER + pos_y + size_y)
		{
			//changeTime( markerData[i].time );
			_selectedMarker = i;
			_currentMarkerImg = createSelectedMarkerImage(_selectedMarker, 
														  wxColor(180, 180, 180));
			Refresh();

			//if (m_id != wxID_ANY)
			//{
			//	wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, m_id);
			//	wxPostEvent(this, btnEvent);
			//}
			break;
		}
		++i;
	}
}

void Partition::OnMouseMotion(wxMouseEvent& event)
{
	//DEBUG_STREAM << "MOUSE MOTION" << endl;
	wxPoint pt = event.GetPosition();
	int i = findFirstMarkerOfImage(_markerData[_selectedMarker].numImg);

	while (_markerData[i].numImg == _markerData[_selectedMarker].numImg)
	{
		int pos_x = _markerData[i].point.x * _resizeRatio_x,
			size_x = _markerData[i].size.x * _resizeRatio_x,
			pos_y = _markerData[i].point.y * _resizeRatio_y,
			size_y = _markerData[i].size.y * _resizeRatio_y;

		if (pt.x >= BORDER + pos_x && pt.x < BORDER + pos_x + size_x &&
			pt.y >= BORDER + pos_y && pt.y < BORDER + pos_y + size_y)
		{
			_mouseHoverMarker = i;
			_mouseHoverImage = createSelectedMarkerImage(i, wxColor(220, 220, 220), 80);
			_mouseHover = true;
			Refresh();
			break;
		}
		else _mouseHover = false;

		++i;
	}
	if (i != _mouseHoverMarker)
	{
		_mouseHover = false;
		Refresh();
	}
}

void Partition::OnPaint(wxPaintEvent& event)
{
	wxAutoBufferedPaintDC dc(this);
	wxSize size = this->GetSize();

	dc.SetPen(wxPen(wxColor(0, 0, 0), 2, wxSOLID));
	dc.SetBrush(wxBrush(wxColor(250, 250, 250)));
	//dc.SetBrush(wxBrush(wxColor(0, 0, 0)));
	dc.DrawRectangle(wxRect(0, 0, size.x, size.y));

	// Partition images.
	if (_currentImg.IsOk())
	{
		wxBitmap img(_currentImg);
		dc.DrawBitmap(img, 50, 50);
	}

	// Selected Bar.
	if (_currentMarkerImg.IsOk())// && markerData[selectedMarker].numImg ==  )
	{
		//_DEBUG_ DSTREAM << "MARKER IMG EXIST (OnPaint)" << endl;
		wxBitmap img(_currentMarkerImg);

		t_point pt = _markerData[_selectedMarker].point;
		dc.DrawBitmap(_currentMarkerImg, BORDER + pt.x * _resizeRatio_x, 
					  BORDER + pt.y * _resizeRatio_y);
	}

	// Mouse Hover.
	if (_mouseHover && _mouseHoverImage.IsOk())
	{
		DEBUG_STREAM << "MOUSE OVER ON PAINT" << endl;
		wxBitmap img(_mouseHoverImage);

		t_point pt = _markerData[_mouseHoverMarker].point;
		dc.DrawBitmap(_mouseHoverImage, BORDER + pt.x * _resizeRatio_x, 
					  BORDER + pt.y * _resizeRatio_y);
	}
}