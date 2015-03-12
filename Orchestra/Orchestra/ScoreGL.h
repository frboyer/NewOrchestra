#ifndef __SCORE_GL__
#define __SCORE_GL__

#include "axLib.h"

struct t_point { short x, y; };
struct t_size { short x, y; };

// Is use for markers data info.
struct ScoreMarkerInfo
{
	int posInFile;
	t_point point;
	t_size size;
	short numImg;
	float time;
};

class ScorePlayer
{
public:
	ScorePlayer();

	~ScorePlayer();

	bool loadInfo(const std::string& data_path, const axStringVector& list);

	bool loadMarkerInfo(std::string path);

	void setImageList(const axStringVector& imgPathList);

	void setPlaying(const bool& playing);

	bool compareMarkerTime(const double& a, const ScoreMarkerInfo& b);

	bool changeTime(const double& timeMs);

	double getSelectedMarkerTimeMs();

	int findFirstMarkerOfImage(const int& imgNum);

	int getNumberOfMarker() const;

	int getSelectedMarker() const;

	int getCurrentImageIndex() const;

	std::string getCurrentImagePath() const;

	const ScoreMarkerInfo* getMarkerData() const;

private:
	axStringVector _imgPathList;
	ScoreMarkerInfo* _markerData;
	int _nMarker, _selectedMarker, _currentImageIndex;
};

class ScoreGL : public axPanel
{
public:
	/***************************************************************************
	* ScoreGL::Msg.
	**************************************************************************/
	class Msg : public axMsg
	{
	public:
		Msg(const int& page, const int& bar)
		{
			_page = page;
			_bar = bar;
		}

		int GetPage() const
		{
			return _page;
		}

		int GetBar() const
		{
			return _bar;
		}

		axMsg* GetCopy()
		{
			return new_ Msg(*this);
		}

	private:
		int _page, _bar;
	};

	/***************************************************************************
	* ScoreGL::Events.
	**************************************************************************/
	class Events
	{
	public:
		enum : axEventId { BAR_CHANGE };

		Events(){}
		Events(const axEventFunction& fct){ bar_change = fct; }

		axEventFunction bar_change;
	};

	/***************************************************************************
	* ScoreGL::ScoreGL
	**************************************************************************/
	ScoreGL(axWindow* parent, const axRect& rect, const ScoreGL::Events& events);

	~ScoreGL();

	void setPlaying(const bool& playing);

	bool loadInfo(const std::string& data_path, const axStringVector& list);

	bool changeTime(const double& timeMs);

private:
	ScorePlayer _player;
	//std::vector<std::unique_ptr<axImage> > _images;
	std::vector<std::unique_ptr<axBigImage>> _images;
	int _currentImageIndex;
	bool _isPlaying, _mouseHover;
	int _mouseHoverMarker;
	double _resizeRatio_x, _resizeRatio_y;
	static const int BORDER = 10;

	// axEvents.
	virtual void OnMouseLeftDoubleClick(const axPoint& pos);
	virtual void OnMouseMotion(const axPoint& pos);
	virtual void OnPaint();
};


#endif // __SCORE_GL__