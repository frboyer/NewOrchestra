#include "ScoreGL.h"

//*****************************************************************************
// ScorePlayer
//*****************************************************************************
ScorePlayer::ScorePlayer():
_markerData(nullptr),
_nMarker(0),
_selectedMarker(-1),
_currentImageIndex(-1)
{

}

ScorePlayer::~ScorePlayer()
{
	delete _markerData;
}

bool ScorePlayer::loadInfo(const std::string& data_path, 
						   const axStringVector& list)
{
	if (loadMarkerInfo(data_path))
	{
		std::cerr << "Error ScorePLayer can't read marker data." << std::endl;
		return true;
	}

	setImageList(list);

	return false;
}

bool ScorePlayer::loadMarkerInfo(std::string path)
{
	// Read markers from binary file.
	ifstream data(path, ios::binary);

	if (data.fail())
	{
		return true;
	}
	else
	{
		if (_markerData != nullptr)
		{
			delete[] _markerData;
			_markerData = nullptr;
			_nMarker = 0;
		}

		// Number of Marker in file.
		data.seekg(0);
		data.read((char*)&_nMarker, sizeof(int));
		_markerData = new ScoreMarkerInfo[_nMarker];

		// Read all markers data in one big chunk.
		data.read((char*)_markerData, sizeof(ScoreMarkerInfo)* _nMarker);
		data.close();

		_selectedMarker = -1;
		_currentImageIndex = 0;
	}

	return false;
}

void ScorePlayer::setImageList(const axStringVector& imgPathList)
{
	_imgPathList = imgPathList;
}

bool ScorePlayer::compareMarkerTime(const double& a, 
								    const ScoreMarkerInfo& b)
{
	return a < b.time;
}

double ScorePlayer::getSelectedMarkerTimeMs()
{
	return _markerData[_selectedMarker].time * 1000.0;
}

int ScorePlayer::findFirstMarkerOfImage(const int& imgNum)
{
	for (int i = 0; i < _nMarker; ++i)
	{
		if (_markerData[i].numImg == imgNum)
		{
			return i;
		}	
	}

	return -1;
}

bool ScorePlayer::changeTime(const double& timeMs)
{
	double sec = timeMs / 1000.0;

	// Fast test for when time is still in same region.
	//if (_markerData[_selectedMarker].time <= sec &&
	//	sec <= _markerData[_selectedMarker + 1].time)
	//{
	//	return false;
	//}
	
	// Returns the index to the first marker in markers data 
	// that is greater than timeMs.
	//int currentMarker = std::upper_bound(_markerData, _markerData + _nMarker, timeMs,
	//	[](const double& a, const ScoreMarkerInfo& b)
	//{ 
	//	return a < b.time; 
	//}) - _markerData - 1;

	int currentMarker = -5;

	//std::cout << "time : " << sec << std::endl;
	for (int i = 0; i < _nMarker; i++)
	{
		if (_markerData[i].time > sec)
		{
			currentMarker = i;
			
			break;
		}
	}
	
	if (currentMarker >= 0)
	{
		// Need to change image.
		if (_markerData[currentMarker].numImg != _markerData[_selectedMarker].numImg)
		{
			_currentImageIndex = _markerData[currentMarker].numImg;
		}

		// Call marker change event.
		if (_selectedMarker != currentMarker)
		{
			std::cout << "Current marker : " << currentMarker << std::endl;
			_selectedMarker = currentMarker;
			//std::cout << "Change marker : " << _selectedMarker << std::endl;
			// TODO : Call change marker event.
			return true;
		}

		return false;
	}

	return false;
}

int ScorePlayer::getCurrentImageIndex() const
{
	return _currentImageIndex;
}

int ScorePlayer::getNumberOfMarker() const
{
	return _nMarker;
}

int ScorePlayer::getSelectedMarker() const
{
	return _selectedMarker;
}

std::string ScorePlayer::getCurrentImagePath() const
{
	if (_selectedMarker != -1 && _selectedMarker < _nMarker)
	{
		return _imgPathList[_selectedMarker];
	}
	
	return "";
}

const ScoreMarkerInfo* ScorePlayer::getMarkerData() const
{
	return _markerData;
}



//*****************************************************************************
// ScoreGL
//*****************************************************************************
ScoreGL::ScoreGL(axWindow* parent, const axRect& rect, const ScoreGL::Events& events) :
axPanel(parent, rect),
_mouseHover(false),
_mouseHoverMarker(0),
_currentImageIndex(-1),
_isPlaying(false)
{
	if (events.bar_change)
	{
		axObject::AddConnection(events.BAR_CHANGE, events.bar_change);
	}
}

ScoreGL::~ScoreGL()
{

}

void ScoreGL::setPlaying(const bool& playing)
{
	_isPlaying = playing;
	Update();
}

bool ScoreGL::loadInfo(const std::string& data_path,
					   const axStringVector& list)
{
	bool err = _player.loadInfo(data_path, list);

	if (!err)
	{
		_player.setImageList(list);

		// Delete images if not empty.
		if (_images.size())
		{
			for (auto& n : _images)
			{
				delete n;
			}
			_images.empty();
		}

		// Load all images in memory.
		for (auto& path : list)
		{
			_images.push_back(new axImage(path));
		}

		std::cout << "IMG index" << std::endl;
		_currentImageIndex = 0;

		// No error.
		return false;
	}

	// Error.
	return true;

}

bool ScoreGL::changeTime(const double& timeMs)
{
	bool need_to_refresh = _player.changeTime(timeMs);

	if (need_to_refresh)
	{
		axObject::PushEvent(Events::BAR_CHANGE, 
			new Msg(_player.getCurrentImageIndex() + 1, _player.getSelectedMarker()));
		Update();
	}

	return need_to_refresh;
}

void ScoreGL::OnMouseLeftDoubleClick(const axPoint& pos)
{
	//axPoint pt = pos - GetAbsoluteRect().position;

	////int i = _player.findFirstMarkerOfImage();
	//int i = findFirstMarkerOfImage(_markerData[_selectedMarker].numImg);

	//while (_markerData[i].numImg == _markerData[_selectedMarker].numImg)
	//{
	//	int pos_x = _markerData[i].point.x * _resizeRatio_x,
	//		size_x = _markerData[i].size.x * _resizeRatio_x,
	//		pos_y = _markerData[i].point.y * _resizeRatio_y,
	//		size_y = _markerData[i].size.y * _resizeRatio_y;

	//	if (pt.x >= BORDER + pos_x && pt.x < BORDER + pos_x + size_x &&
	//		pt.y >= BORDER + pos_y && pt.y < BORDER + pos_y + size_y)
	//	{
	//		//changeTime( markerData[i].time );
	//		_selectedMarker = i;
	//		//_currentMarkerImg = createSelectedMarkerImage(_selectedMarker,
	//			//wxColor(180, 180, 180));
	//		Update();

	//		//if (m_id != wxID_ANY)
	//		//{
	//		//	wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, m_id);
	//		//	wxPostEvent(this, btnEvent);
	//		//}
	//		break;
	//	}
	//	++i;
	//}
}

void ScoreGL::OnMouseMotion(const axPoint& pos)
{
	//const int index = _player.getSelectedMarker();
	int img_index = _player.getCurrentImageIndex();

	if (img_index != -1)
	{
		//std::cout << "hh" << std::endl;
		axPoint pt = pos - GetAbsoluteRect().position;

		const ScoreMarkerInfo* markerInfo = _player.getMarkerData();

		int x = GetSize().x - 35, y = GetSize().y - 40;
		_resizeRatio_x = x / double(_images[img_index]->GetWidth());
		_resizeRatio_y = y / double(_images[img_index]->GetHeight());

		bool mouseHover = false;

		int i = _player.findFirstMarkerOfImage(markerInfo[_player.getSelectedMarker()].numImg);

		// Go through all marker rectangle in image.
		while (markerInfo[i].numImg == img_index)
		{
			const  ScoreMarkerInfo& marker = markerInfo[i];

			axRect rect(marker.point.x * _resizeRatio_x,
				marker.size.x * _resizeRatio_x,
				marker.point.y * _resizeRatio_y,
				marker.size.y * _resizeRatio_y);

			rect.position += axPoint(BORDER, BORDER);

			if (rect.IsPointInside(pt))
			{
				mouseHover = true;
				// Update only if mouse marker is different.
				if (i != _mouseHoverMarker)
				{
					std::cout << "MOUSE INSIDE MARKER INDEX : " << i << std::endl;
					_mouseHoverMarker = i;
					Update();
				}
				break;
			}
			++i;
		}

		if (mouseHover != _mouseHover)
		{
			_mouseHover = mouseHover;
			Update();
		}

	}
}

void ScoreGL::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetDrawingRect());

	gc->SetColor(axColor(1.0, 1.0, 1.0));
	gc->DrawRectangle(rect);

	int img_index = _player.getCurrentImageIndex();
	if (img_index != -1 &&
		_images[img_index]->IsImageReady())
	{
		gc->DrawImageResize(_images[img_index],
							axPoint(10, 10),
							axSize(rect.size.x - 35, rect.size.y - 40));
	}


	// Selected bar.
	if (_mouseHover)
	{
		const ScoreMarkerInfo& selectedMarker = _player.getMarkerData()[_mouseHoverMarker];
		t_point pt = selectedMarker.point;
		t_size s = selectedMarker.size;
		gc->SetColor(axColor(0.6, 0.6, 1.0, 0.5));
		gc->DrawRectangle(axRect(50, 50));
		gc->DrawRectangle(axRect(10 + pt.x * _resizeRatio_x, 10 + pt.y * _resizeRatio_y,
						  s.x * _resizeRatio_x, s.y * _resizeRatio_y));
	}

	// Current playing bar.
	if (_player.getSelectedMarker() != -1)
	{
		const ScoreMarkerInfo& playingMarker = _player.getMarkerData()[_player.getSelectedMarker()];
		t_point pt = playingMarker.point;
		t_size s = playingMarker.size;
		gc->SetColor(axColor(0.6, 0.6, 0.6, 0.5));
		gc->DrawRectangle(axRect(50, 50));
		gc->DrawRectangle(axRect(10 + pt.x * _resizeRatio_x, 10 + pt.y * _resizeRatio_y,
								 s.x * _resizeRatio_x, s.y * _resizeRatio_y));
	}

}