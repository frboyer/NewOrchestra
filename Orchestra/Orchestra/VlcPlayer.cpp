#include "VlcPlayer.h"
#include "Debug.h"

VlcVideoPlayer::VlcVideoPlayer(wxWindow* win, const wxWindowID& id, wxPoint pt, wxSize size) :
				// Heritage
				wxPanel(win, id, pt, size), 
				// Members
				_lastPos(0.0),
				_vlcMedia(nullptr),
				_vlcInstance(nullptr),
				_vlcPlayer(nullptr)
{
	SetBackgroundColour(wxColor(0, 0, 0));

	// Hide filename.
	char const* vlcOptions[] = { "--no-video-title-show" }; 

	// Create new VLC instance.
	if (_vlcInstance = libvlc_new(1, vlcOptions) )
	{
		// Create VLC player.
		if (_vlcPlayer = libvlc_media_player_new(_vlcInstance)) 
		{
			// Create VLC EventManager.
			if (!(_vlcEventManager = libvlc_media_player_event_manager(_vlcPlayer)))
				DEBUG_STREAM << "Can't create VLC Event Manager" << endl;

		}

		else DEBUG_STREAM << "Can't create player from vlcMedia" << endl;
	}
	else DEBUG_STREAM << "Can't Open VLC instance" << endl;

	// libVLC events and callback.
	if (_vlcInstance && _vlcPlayer && _vlcEventManager)
	{
		libvlc_event_attach(_vlcEventManager,
							libvlc_MediaPlayerPositionChanged,
							VlcVideoPlayer::vlcPositionChanged,
							this);

		libvlc_event_attach(_vlcEventManager,
							libvlc_MediaPlayerTimeChanged,
							VlcVideoPlayer::vlcTimeChanged,
							this);
	}
}

VlcVideoPlayer::~VlcVideoPlayer()
{
	if (_vlcInstance)
	{
		if (_vlcPlayer != nullptr && libvlc_media_player_is_playing(_vlcPlayer))
		{
			libvlc_media_player_stop(_vlcPlayer);
		}
			
		// Destroy VLC instance.
		libvlc_release(_vlcInstance); 
		_vlcInstance = nullptr;
	}
}

double VlcVideoPlayer::getPosition() const
{
	return double(libvlc_media_player_get_position(_vlcPlayer));
}

long VlcVideoPlayer::getTimeMs()
{
	return libvlc_media_player_get_time(_vlcPlayer);
}

long VlcVideoPlayer::getTotalTimeMs()
{
	return libvlc_media_player_get_length(_vlcPlayer);
}

void VlcVideoPlayer::backward()
{
	// Backward 10%
	libvlc_media_player_set_position(_vlcPlayer, 
									 libvlc_media_player_get_position(_vlcPlayer) - 0.1); 
}

void VlcVideoPlayer::stop()
{
	libvlc_media_player_stop(_vlcPlayer);
}

void VlcVideoPlayer::pause()
{
	libvlc_media_player_set_pause(_vlcPlayer, 
								  libvlc_media_player_is_playing(_vlcPlayer));
}

void VlcVideoPlayer::play()
{
	libvlc_media_player_play(_vlcPlayer);
}

void VlcVideoPlayer::forward()
{
	// Forward 10%
	libvlc_media_player_set_position(_vlcPlayer, 
									 libvlc_media_player_get_position(_vlcPlayer) + 0.1); 
}

void VlcVideoPlayer::mute()
{
	libvlc_audio_set_mute(_vlcPlayer, true);
}

void VlcVideoPlayer::unMute()
{
	libvlc_audio_set_mute(_vlcPlayer, false);
}

void VlcVideoPlayer::navigate(double pos)
{
	libvlc_media_player_set_position(_vlcPlayer, float(pos));
}

bool VlcVideoPlayer::loadVideo(const char* path)
{
//TOCHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	char t[] = "Ressources\\test.mp4";
	
	if (_vlcMedia = libvlc_media_new_path(_vlcInstance, t))
	{
		libvlc_media_player_set_media(_vlcPlayer, _vlcMedia);
		libvlc_media_release(_vlcMedia);

		// Needed for mixing VLC and wxWidgets.
		// Needs to be after above calls, or else bug with stop button!
		libvlc_media_player_set_hwnd(_vlcPlayer, reinterpret_cast<void *> ((HWND)this->GetHandle()));

		DEBUG_STREAM << "VLC LOADED VIDEO FILE" << endl;
		
//TOCHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		libvlc_audio_set_mute(_vlcPlayer, true);
	}
	else
	{
		DEBUG_STREAM << "Can't load media from path" << endl;
		return false;
	}

	return true; // Didn't fails loading.
}

void VlcVideoPlayer::vlcPositionChanged(const libvlc_event_t *event, void* data)
{
	((VlcVideoPlayer*)data)->videoMovedCallback();
}

void VlcVideoPlayer::vlcTimeChanged(const libvlc_event_t *event, void* data)
{
	((VlcVideoPlayer*)data)->videoTimeCallback();
}

void VlcVideoPlayer::videoMovedCallback()
{
	float vMax = 100;

	if (_vlcInstance != nullptr)
	{
		float pos = libvlc_media_player_get_position(_vlcPlayer);

		if (_lastPos != int(pos * vMax))
		{
			_lastPos = int(pos * vMax);

			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// BUG with wxCommandEvent and wxPostEvent.
			// Too much data in event queue when is called too often.
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (GetId())
			{
				wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
				wxPostEvent(this, btnEvent);
			}
		}
	}
}

void VlcVideoPlayer::videoTimeCallback()
{
	//DEBUG_STREAM << "VIDEO TIME CALLBACK" << endl;
}