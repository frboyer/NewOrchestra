#include "vlc.h"
#include "wx/wx.h"

class VlcVideoPlayer : public wxPanel
{
public:
	VlcVideoPlayer(wxWindow* win, const wxWindowID& id, wxPoint pt, wxSize size);

	~VlcVideoPlayer();

	 double getPosition() const;
	 void mSize(const wxSize& size);
	 long getTimeMs();
	 long getTotalTimeMs();
	 void setVolume(double volume);
	 void backward();
	 void stop();
	 void play();
	 void pause();
	 void forward();
	 void mute();
	 void unMute();
	 void navigate(double pos);
	 bool loadVideo(const char* path);
	 
private:
	libvlc_instance_t* _vlcInstance;
	libvlc_media_t* _vlcMedia;
	libvlc_media_player_t* _vlcPlayer;
	libvlc_event_manager_t* _vlcEventManager;

	float _lastPos;

	void videoTimeCallback();
	void videoMovedCallback();

	static void vlcPositionChanged(const libvlc_event_t *event, void* data);
	static void vlcTimeChanged(const libvlc_event_t *event, void* data);

	//DECLARE_EVENT_TABLE()
};