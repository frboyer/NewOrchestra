#include "PlayerBarGL.h"

PlayerBarGL::PlayerBarGL(axWindow* parent, const axRect& rect, const PlayerBarGL::Events& events) :
axPanel(parent, rect),
_hasVideoLenght(false)
{
	// Play pause button.
	axToggle::Info togInfo;
	togInfo.normal = axColor::axTransparentColor;
	togInfo.hover = axColor::axTransparentColor;
	togInfo.clicking = axColor::axTransparentColor;
	togInfo.selected = axColor::axTransparentColor;
	togInfo.selected_hover = axColor::axTransparentColor;
	togInfo.selected_clicking = axColor::axTransparentColor;
	togInfo.contour = axColor::axTransparentColor;
	togInfo.img = "MenuToggle.png";
	togInfo.single_img = false;

	_menuToggle = new_ axToggle(this,
		axRect(axPoint(5, 5), axSize(25, 25)),
		axToggle::Events(events.menu_toggle),
		togInfo);

	axLabel::Info labelInfo;
	labelInfo.normal = axColor::axTransparentColor;
	labelInfo.contour = axColor::axTransparentColor;;
	labelInfo.font_color = axColor::axBlackColor;;
	labelInfo.font_size = 12;
	labelInfo.font_name = "";
	labelInfo._alignement = axAlignement::axALIGN_CENTER;

	_timeLabel = new_ axLabel(this,
							  axRect(axPoint(30, 10),
							  axSize(50, 15)), labelInfo, "00:00");

	// Scroll slider.
	axSliderInfo sld_info;
	sld_info.img_path = std::string("sliderPlain.png");
	sld_info.btn_size = axSize(12, 12);
	sld_info.slider_width = 4;

	// Doesn't seem to work for now.
	sld_info.bgColorNormal = axColor(0.7, 0.7, 0.7, 0.2);
	sld_info.bgColorHover = axColor(0.9, 0.9, 0.9, 0.2);
	sld_info.bgColorClicked = axColor(0.6, 0.6, 0.6, 0.2);
	sld_info.contourColor = axColor(0.0, 0.0, 0.0);

	sld_info.sliderColorNormal = axColor(0.2, 0.2, 0.9);
	sld_info.sliderColorHover = axColor(0.2, 0.2, 1.0);
	sld_info.sliderColorClicked = axColor(0.2, 0.2, 0.6);
	sld_info.sliderContourColor = axColor(0.2, 0.2, 0.2);
	sld_info.backSliderColor = axColor(0.7, 0.7, 0.7);
	sld_info.backSliderContourColor = axColor(0.2, 0.2, 0.2);

	_scrollSlider = new_ axSlider(this, 
								 axRect(_timeLabel->GetNextPosRight(5), axSize(400, 15)),
								 axSliderEvents(), 
								 sld_info);

	_videoLengthLabel = new_ axLabel(this,
		axRect(_scrollSlider->GetNextPosRight(5),
		axSize(50, 15)), labelInfo, "00:00");

	axButton* left = new_ axButton(this,
		axRect(_videoLengthLabel->GetNextPosRight(10), axSize(25, 25)),
		axButton::Events(events.left_click),
		axBUTTON_TRANSPARENT,
		"Left.png");

	axButton* front = new_ axButton(this,
		axRect(left->GetNextPosRight(5), axSize(25, 25)),
		axButton::Events(events.front_click),
		axBUTTON_TRANSPARENT,
		"Front.png");

	axButton* right = new_ axButton(this,
		axRect(front->GetNextPosRight(5), axSize(25, 25)),
		axButton::Events(events.right_click),
		axBUTTON_TRANSPARENT,
		"Right.png");

	// Backward button.
	axButton* back = new_ axButton(this,
		axRect(right->GetNextPosRight(20) - axPoint(0, 5), axSize(25, 25)),
		axButton::Events(events.backward_click),
		axBUTTON_TRANSPARENT,
		"backwardBtn.png");


	togInfo.img = "playPauseBtn.png";
	_playPauseToggle = new_ axToggle(this,
		axRect(back->GetNextPosRight(5), axSize(25, 25)),
		axToggle::Events(events.play_pause_click),
		togInfo);

	// Stop button.
	axButton* stop = new_ axButton(this,
		axRect(_playPauseToggle->GetNextPosRight(5), axSize(25, 25)),
		axButton::Events(events.stop_click),
		axBUTTON_TRANSPARENT,
		"stopBtn.png");

	// Forward button.
	axButton* forw = new_ axButton(this,
		axRect(stop->GetNextPosRight(5), axSize(25, 25)),
		axButton::Events(events.forward_click),
		axBUTTON_TRANSPARENT,
		"forwardBtn.png");

	axToggle::Info togShowSelectionInfo;
	togShowSelectionInfo.normal = axColor::axTransparentColor;
	togShowSelectionInfo.hover = axColor::axTransparentColor;
	togShowSelectionInfo.clicking = axColor::axTransparentColor;

	togShowSelectionInfo.selected = axColor::axTransparentColor;
	togShowSelectionInfo.selected_hover = axColor::axTransparentColor;
	togShowSelectionInfo.selected_clicking = axColor::axTransparentColor;

	togShowSelectionInfo.contour = axColor::axTransparentColor;
	togShowSelectionInfo.img = "3DToggle.png";
	togShowSelectionInfo.single_img = false;

	_animToggle = new_ axToggle(this,
		axRect(forw->GetNextPosRight(25), axSize(25, 25)),
		axToggle::Events(events.toggle_animation),
		togShowSelectionInfo);

	togShowSelectionInfo.img = "VideoToggle.png";
	_videoToggle = new_ axToggle(this,
		axRect(_animToggle->GetNextPosRight(5), axSize(25, 25)),
		axToggle::Events(events.toggle_video),
		togShowSelectionInfo);

	//togShowSelectionInfo.single_img = true;
	togShowSelectionInfo.img = "NoteToggle.png";
	_scoreToggle = new_ axToggle(this,
		axRect(_videoToggle->GetNextPosRight(5), axSize(25, 25)),
		axToggle::Events(events.toggle_score),
		togShowSelectionInfo);

	_animToggle->SetSelected(true);
	_videoToggle->SetSelected(true);
	_scoreToggle->SetSelected(true);

	axLabel* pageLabel = new_ axLabel(this,
		axRect(_scoreToggle->GetNextPosRight(15) + axPoint(0, 5),
		axSize(50, 15)), labelInfo, "Page");

	axNumberBox::Info box_info(
		axColor(0.7, 0.7, 0.7, 4.0),
		axColor(0.75, 0.75, 0.75, 2.0),
		axColor(0.8, 0.8, 0.8, 4.0),
		axColor(0.2, 0.2, 0.0, 1.0),
		axColor(0.0, 0.0, 0.0, 1.0),
		axColor(0.0, 0.0, 0.0, 1.0),
		"", false);

	_pageBox = new_ axNumberBox(this,
		axRect(pageLabel->GetNextPosRight(5), axSize(40, 20)),
		axNumberBox::Events(),
		box_info, axNO_FLAG, 1,
		axFloatRange(1.0, 50), axCTRL_INT);

	axLabel* mesureLabel = new_ axLabel(this,
		axRect(_pageBox->GetNextPosRight(10),
		axSize(50, 15)), labelInfo, "Mesure");

	_barBox = new_ axNumberBox(this,
		axRect(mesureLabel->GetNextPosRight(5), axSize(40, 20)),
		axNumberBox::Events(),
		box_info, axNO_FLAG, 1,
		axFloatRange(1.0, 50), axCTRL_INT);

	// Volume label.
	axLabel* volumeLabel = new_ axLabel(this,
		axRect(_barBox->GetNextPosRight(10),
		axSize(50, 15)), labelInfo, "Volume");


	// Volume slider.
	axSlider* volume = new_ axSlider(this, axRect(volumeLabel->GetNextPosRight(5), axSize(50, 15)),
		axSliderEvents(), sld_info);
	(volume);
}

void PlayerBarGL::SetBar(const int& bar)
{
	_barBox->SetValue(bar);
}

void PlayerBarGL::SetPage(const int& page)
{
	_pageBox->SetValue(page);
}

void PlayerBarGL::SetScrollSliderValue(const double& value, const double& ms)
{
	if (value >= 0.0)
	{
		_scrollSlider->SetValue(value);

		int time_in_sec = int(ms / 1000.0);
		int min = (int)floor(time_in_sec / 60.0);
		int sec = time_in_sec % 60;

		std::string min_label = min < 10 ? std::string("0") + std::to_string(min) : std::to_string(min);
		std::string sec_label = sec < 10 ? std::string("0") + std::to_string(sec) : std::to_string(sec);
		std::string label_value = min_label + std::string(":") + sec_label;
		_timeLabel->SetLabel(label_value);
	}
}

void PlayerBarGL::SetVideoLength(const double& ms)
{
	if (ms > 0.0)
	{
		int time_in_sec = int(ms / 1000.0);
		int min = (int)floor(time_in_sec / 60.0);
		int sec = time_in_sec % 60;

		std::string min_label = min < 10 ? std::string("0") + std::to_string(min) : std::to_string(min);
		std::string sec_label = sec < 10 ? std::string("0") + std::to_string(sec) : std::to_string(sec);
		std::string label_value = min_label + std::string(":") + sec_label;
		_videoLengthLabel->SetLabel(label_value);

		_hasVideoLenght = true;
	}
}

void PlayerBarGL::HideMenu()
{
	_menuToggle->SetSelected(false);
}

void PlayerBarGL::SetPauseToggle()
{
	_playPauseToggle->SetSelected(false);
}

void PlayerBarGL::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetDrawingRect());

	gc->DrawRectangleColorFade(rect, axColor(0.4), axColor(0.8));

	gc->SetColor(axColor(0.2));
	gc->DrawRectangleContour(rect);
}