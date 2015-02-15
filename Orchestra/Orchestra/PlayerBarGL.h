#ifndef __PLAYER_BAR_GL__
#define __PLAYER_BAR_GL__

#include "axLib.h"

class PlayerBarGL : public axPanel
{
public:
	class Events
	{
	public:
		Events()
		{
		}

		axEventFunction backward_click;
		axEventFunction play_pause_click;
		axEventFunction stop_click;
		axEventFunction forward_click;
		axEventFunction toggle_animation;
		axEventFunction toggle_video;
		axEventFunction toggle_score;
	};

	PlayerBarGL(axWindow* parent, const axRect& rect, const Events& events);

	void SetBar(const int& bar);
	void SetPage(const int& page);

	void SetScrollSliderValue(const double& value, const double& ms);
	void SetVideoLength(const double& ms);

	bool GetHasVideoLength() const
	{
		return _hasVideoLenght;
	}

private:
	axNumberBox* _pageBox;
	axNumberBox* _barBox;
	axSlider* _scrollSlider;
	axLabel* _timeLabel;
	axLabel* _videoLengthLabel;
	bool _hasVideoLenght;
	axToggle* _animToggle;
	axToggle* _videoToggle;
	axToggle* _scoreToggle;
	// axEvents.
	virtual void OnPaint();
};

#endif //__PLAYER_BAR_GL__