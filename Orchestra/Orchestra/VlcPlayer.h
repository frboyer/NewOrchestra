#include "vlc.h"
#include "wx/wx.h"

class VlcVideoPlayer : public wxPanel
{
public:
	typedef int64_t Time;                        // In microseconds (as VLC clock is).
	static const Time millisecondsToTime = 1000; // Mutiply by this constant to convert libvlc_time_t to Time.

	VlcVideoPlayer(wxWindow* win, const wxWindowID& id, wxPoint pt, wxSize size);

	~VlcVideoPlayer();

	void mSize(const wxSize& size);

	// ----- Higher resolution versions of libvlc functions -----

	// Current playback time, as precise as clock is.
	Time getPlaybackTime()       { return getCurrentTimePoint().playbackTime; }

	// Position as percentage between 0.0 and 1.0 of movie length.
	double getPosition()         { return getPlaybackTime() / double(getMovieLength()); }

	// Sets position as percentage as percentage between 0.0 and 1.0 of movie length.
	void setPosition(double position) const { setPlaybackTime(getMovieLength() * position); }


	// ----- Wrapping of libvlc functions -----

	// Playtime at the start of the currently playing chunk (usually in 300ms chunks).
	Time getChunkPlaybackTime()     const { return libvlc_media_player_get_time(_vlcPlayer) * millisecondsToTime; } 

	// Sets the playback time.
	void setPlaybackTime(Time time) const { libvlc_media_player_set_time(_vlcPlayer, time / millisecondsToTime); }

	// Position of current chunk as a percentage between 0.0 and 1.0 of movie length.
	double getChunkPosition()       const { return double(libvlc_media_player_get_position(_vlcPlayer)); }

	// Total length of the movie.
	Time getMovieLength()           const { return libvlc_media_player_get_length(_vlcPlayer) * millisecondsToTime; }

	// Current clock time, as defined by VLC clock source.
	static Time getClockTime()            { return libvlc_clock(); }

	void play()                 const { libvlc_media_player_play(_vlcPlayer); }
	void stop()                 const { libvlc_media_player_stop(_vlcPlayer); }
	bool isPlaying()            const { return libvlc_media_player_is_playing(_vlcPlayer) == 1; }
	void setPause(bool doPause) const { return libvlc_media_player_set_pause(_vlcPlayer, doPause); }
	void pause()                const { setPause(isPlaying()); }

	void navigate(double pos);
	void forward()                    { setPosition(getPosition() + 0.10); }
	void backward()                   { setPosition(getPosition() - 0.10); }

	//void setVolume(double volume);
	void setMute(bool doMute)   const { libvlc_audio_set_mute(_vlcPlayer, doMute); }
	void mute()                 const { setMute(true); }
	void unMute()               const { setMute(false); }
	bool loadVideo(const char* path);
	 
private:
	libvlc_instance_t* _vlcInstance;
	libvlc_media_t* _vlcMedia;
	libvlc_media_player_t* _vlcPlayer;
	libvlc_event_manager_t* _vlcEventManager;

	float _lastPos;

	struct TimePoint {
		int64_t clockTime, playbackTime; // Both are in microseconds (as VLC clock is).
	};
	TimePoint _lastEvent;
	bool _isPlaying;

	TimePoint getCurrentTimePoint() {
		int64_t currentClockTime = libvlc_clock();
		int64_t playedSinceLastEvent = _isPlaying ? (currentClockTime - _lastEvent.clockTime) : 0;
		return TimePoint{ currentClockTime, _lastEvent.playbackTime + playedSinceLastEvent };
	}

	__declspec(noreturn) static bool fatalVlc(wxString message) { // Defined to return bool so it can be used in expressions.
		wxMessageBox(message << "\n" << libvlc_errmsg(), "Fatal error");
		exit(1);
	}

	// Callback functions:
	void positionChangedCallback(float newPosition);
	void timeChangedCallback(libvlc_time_t newTime);
	void playingCallback();
	void pausedCallback();
	void stoppedCallback();

	// Trampolines to call member functions:
	static VlcVideoPlayer* _this(void* data)                                      { return reinterpret_cast<VlcVideoPlayer*>(data); }
	static void _positionChangedCallback(const libvlc_event_t* event, void* data) { _this(data)->positionChangedCallback(event->u.media_player_position_changed.new_position); }
	static void _timeChangedCallback(const libvlc_event_t* event, void* data)     { _this(data)->timeChangedCallback(event->u.media_player_time_changed.new_time); }
	static void _playingCallback(const libvlc_event_t*, void* data)               { _this(data)->playingCallback(); }
	static void _pausedCallback(const libvlc_event_t*, void* data)                { _this(data)->pausedCallback(); }
	static void _stoppedCallback(const libvlc_event_t*, void* data)               { _this(data)->stoppedCallback(); }

	//DECLARE_EVENT_TABLE()
};