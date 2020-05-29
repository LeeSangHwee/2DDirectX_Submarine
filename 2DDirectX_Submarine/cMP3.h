/*
 * 주의:윈도우 메시지 받지 않는 중에는 OnEvent()를 호출 해줘야 함,
 *		하지 않을 경우 mp3연주가 끝났을 때 반복(LOOPPING)이 되지 않음
 *	    amstrmid.lib 링크할것
 */
#ifndef _3DDIVER_MP3_PLAYER_H
#define _3DDIVER_MP3_PLAYER_H

#include <dshow.h>

//#define SAFE_RELEASE(p){if(p){(p)->Release();(p)=NULL;}}

// 사운드 필터 그래프 하나에 대한 관리를 수행하는 클래스
class DS_SOUND_CHANNEL
{
public:
	
	enum {};
	
protected:
	
	IGraphBuilder*graph_builder;
	IMediaControl*media_control;
	IMediaSeeking*media_seeking;
	IBasicAudio*basic_audio;
	IMediaEvent*media_event;

	BOOL init_flag;

public:
	
	DS_SOUND_CHANNEL();
	~DS_SOUND_CHANNEL();

	void InitClass(void);
	BOOL CleanGraph(void);
	BOOL HasFilter(IBaseFilter*filter);

	FILTER_STATE GetState(void) const;

	void OnEvent(void);

	BOOL IsInitialized(void){return init_flag;}
	IGraphBuilder*GetGraphBuilder(void){return graph_builder;}

	HRESULT GetVolume(long*vol){return basic_audio->get_Volume(vol);}
	HRESULT GetBalance(long*bal){return basic_audio->get_Balance(bal);}
	HRESULT SetVolume(const long vol){return basic_audio->put_Volume(vol);}
	HRESULT SetBalance(const long bal){return basic_audio->put_Balance(bal);}

	HRESULT Run(void){return media_control->Run();}
	HRESULT Stop(void){return media_control->Stop();}
	HRESULT Pause(void){return media_control->Pause();}
	HRESULT SetPosition(LONGLONG pos)
	{
		return media_seeking->SetPositions(&pos,AM_SEEKING_AbsolutePositioning,
										   &pos,AM_SEEKING_NoPositioning);
	}
	
};

// 실재 MP3를 플레이 하는 클래스
class cMP3
{
public:
	
	enum {DEF_CHANNELS=5,MIN_CHANNELS=1,ISERROR=-1};

protected:
	
	DS_SOUND_CHANNEL*channels;

	BOOL init_flag;

	int cur_channel;
	int channel_num;
	
public:

	cMP3(int channel_num=DEF_CHANNELS);

	// delete 순서, delete []channels; delete []sounds;
	~cMP3();

	void InitClass(int channel_num = DEF_CHANNELS);

	void __fastcall LoadSound(const char *path, const BOOL BGM_flag=FALSE);	// 효과음용 채널 1~4
	void __fastcall LoadBGM(const char *path){LoadSound(path, TRUE);}		// 스트림 연주용 0

	void OnEvent(void);														// 스트림 연주시 LOOPPING을 위함

	// 공통 컨트롤
	HRESULT RunAll(void);					// 모든 채널 연주
	HRESULT StopAll(void);					// 모든 채널 정지
	HRESULT PauseAll(void);					// 모든 채널 일시정지

	// 스트림 연주를 제외한 로딩된 모든 사운드 연주, 정지, 일시정지
	HRESULT RunSounds(void);
	HRESULT StopSounds(void);
	HRESULT PauseSounds(void);

	// 스트림 연주 제어
	HRESULT RunBGM(void){return channels[0].Run();}
	HRESULT StopBGM(void){return channels[0].Stop();}
	HRESULT PauseBGM(void){return channels[0].Pause();}

	// 채널 번호를 이용한 컨트롤
	HRESULT Run(const int which){return channels[which].Run();}
	HRESULT Stop(const int which){return channels[which].Stop();}
	HRESULT Pause(const int which){return channels[which].Pause();}

	HRESULT SetEntVolume(const long vol) const;
	HRESULT SetEntBalance(const long bal) const;

	HRESULT SetVolume(const int which,const long vol) const
			{return channels[which].SetVolume(vol);}
	HRESULT SetBalance(const int which,const long bal) const
			{return channels[which].SetBalance (bal);}

	FILTER_STATE GetState(const int which) const
	{return channels[which].GetState();}
	long GetVolume(const int which) const
	{long vol; channels[which].GetVolume(&vol); return vol;}
	long GetBalance(const int which) const
	{long bal; channels[which].GetBalance(&bal); return bal;}

	BOOL IsInitialized(void){return init_flag;}
	DS_SOUND_CHANNEL*GetChannel(const int which){return channels+which;}

};

#endif
