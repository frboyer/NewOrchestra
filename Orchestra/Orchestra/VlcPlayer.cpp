#include "VlcPlayer.h"
#include "Debug.h"
#define DO_EVENTS_TRACE
#ifdef DO_EVENTS_TRACE
	#define IF_EVENTS_TRACE(x) x
#else
	#define IF_EVENTS_TRACE(x)
#endif

IF_EVENTS_TRACE(
	static VlcVideoPlayer::Time timeDifference() {
		static VlcVideoPlayer::Time last = 0;
		VlcVideoPlayer::Time current = VlcVideoPlayer::getClockTime();
		VlcVideoPlayer::Time difference = current - last;
		last = current;
		return difference;
	}
)

VlcVideoPlayer::VlcVideoPlayer(wxWindow* win, const wxWindowID& id, wxPoint pt, wxSize size) :
				// Heritage
				wxPanel(win, id, pt, size), 
				// Members
				_lastPos(0.0),
				_isPlaying(false),
				_vlcMedia(nullptr),
				_vlcInstance(nullptr),
				_vlcPlayer(nullptr)
{
	SetBackgroundColour(wxColor(0, 0, 0));

	
	char const* vlcOptions[] = {
		"--no-video-title-show", // Hide filename.
		"--clock-source=perf" // Possible clock sources are (and their resolution on my Win8 system): interrupt (not working), tick (16ms), multimedia (1ms), perf (1us), wall (1ms)
	}; 

	assert(true);
	// Create new VLC instance.
	(_vlcInstance = libvlc_new(ARRAYSIZE(vlcOptions), vlcOptions))     != nullptr || fatalVlc("Can't open VLC instance");
	// Create VLC player.
	(_vlcPlayer = libvlc_media_player_new(_vlcInstance))               != nullptr || fatalVlc("Can't create VLC player");
	// Create VLC EventManager.
	(_vlcEventManager = libvlc_media_player_event_manager(_vlcPlayer)) != nullptr || fatalVlc("Can't create VLC event manager");

	// libVLC events and callback.
	struct { libvlc_event_type_t eventType; libvlc_callback_t function; }
	eventCallbacks[] = {
		{ libvlc_MediaPlayerPositionChanged, _positionChangedCallback },
		{ libvlc_MediaPlayerTimeChanged,     _timeChangedCallback },
		{ libvlc_MediaPlayerPlaying,         _playingCallback },
		{ libvlc_MediaPlayerPaused,          _pausedCallback },
		{ libvlc_MediaPlayerStopped,         _stoppedCallback },
	};
	for (auto& callback : eventCallbacks)
		libvlc_event_attach(_vlcEventManager, callback.eventType, callback.function, this);

	IF_EVENTS_TRACE(
		Time startTime = getClockTime();
		Time currentTime;
		while ((currentTime = getClockTime()) == startTime) { }
		DEBUG_STREAM << "Clock resolution: " << currentTime - startTime << endl;
	)
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

void VlcVideoPlayer::navigate(double pos)
{
	_lastEvent = { libvlc_clock(), int64_t(libvlc_media_player_get_length(_vlcPlayer)*pos * 1000) };
	setPosition(pos);
	//libvlc_media_player_set_position(_vlcPlayer, float(pos));
}

bool VlcVideoPlayer::loadVideo(const char* path)
{
	(path);
//TOCHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	const char* t = path;///*"Ressources\\Lesson1\\die.wav";*/ /*"Ressources\\test.mp4"*/ /*"Ressources\\Stravinsky Sacre 1avant142 a 149.mp3"*/ "Ressources\\Varese Arcana.mp3";
	//NOTE: VLC import of an Audition .mp4 export (using FAAC) has a very perceptible delay (audio is too late); .ogg audio seems a bit too soon; .mp3 seems ok.  Delay is probably constant, so we could compensate.
	
	if ((_vlcMedia = libvlc_media_new_path(_vlcInstance, t)) != nullptr)
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

void VlcVideoPlayer::positionChangedCallback(float newPosition)
{
	// From experience, this callback is called approximately each 300ms.  It is called at the same time as timeChangedCallback.
	IF_EVENTS_TRACE(
		DEBUG_STREAM << "positionChanged " << timeDifference() << " " << int64_t(getChunkPlaybackTime()) << " " << newPosition << endl;
	)

	float vMax = 100;

	float pos = libvlc_media_player_get_position(_vlcPlayer);

	if (_lastPos != int(pos * vMax))
	{
		_lastPos = int(pos * vMax);

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// BUG with wxCommandEvent and wxPostEvent.
		// Too much data in event queue when is called too often.
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		wxPostEvent(this, wxCommandEvent(wxEVT_COMMAND_BUTTON_CLICKED, GetId()));
	}
}

void VlcVideoPlayer::timeChangedCallback(libvlc_time_t newTime)
{
	// From experience, this callback is called approximately each 300ms.
	// While playing, libvlc_media_player_get_time increments by exactly 1000 * newTime.
	IF_EVENTS_TRACE( DEBUG_STREAM << "videoTime " << timeDifference() << " " << getChunkPlaybackTime() << " " << newTime << endl; )

	_lastEvent = { libvlc_clock(), newTime * millisecondsToTime };
}

void VlcVideoPlayer::playingCallback()
{
	IF_EVENTS_TRACE( DEBUG_STREAM << "playing " << timeDifference() << " " << getChunkPlaybackTime() << endl; )

	_lastEvent = { libvlc_clock(), getChunkPlaybackTime() };
	_isPlaying = true;
}

void VlcVideoPlayer::pausedCallback()
{
	IF_EVENTS_TRACE( DEBUG_STREAM << "paused " << timeDifference() << " " << getChunkPlaybackTime() << endl; )

	_lastEvent = getCurrentTimePoint();
	_isPlaying = false;
}

void VlcVideoPlayer::stoppedCallback()
{
	IF_EVENTS_TRACE( DEBUG_STREAM << "stopped " << timeDifference() << " " << getChunkPlaybackTime() << endl; )

	_lastEvent = getCurrentTimePoint();
	_isPlaying = false;
}
