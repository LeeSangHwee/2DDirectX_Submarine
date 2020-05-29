
#include <malloc.h>
#include <sys/stat.h>
#include "cMp3.h"

#define WIN32_LEAN_AND_MEAN

DS_SOUND_CHANNEL::DS_SOUND_CHANNEL()
{
	// COM 객체 초기화.
	CoInitialize(NULL);

	// 그래프 필터 객체 얻음.
	if(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC,
				IID_IGraphBuilder,reinterpret_cast<void**>(&graph_builder))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// 컨트롤 인터페이스를 얻어온다
	if(graph_builder->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&media_control))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// 탐색 인터페이스를 얻어온다
	if(graph_builder->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&media_seeking))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// 기본 오디오 인터페이스를 얻어온다
	if(graph_builder->QueryInterface(IID_IBasicAudio,reinterpret_cast<void**>(&basic_audio))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// 미디어 이벤트 인터페이스를 얻어온다
	if(graph_builder->QueryInterface(IID_IMediaEvent,reinterpret_cast<void**>(&media_event))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}
	
	init_flag=TRUE;
}

DS_SOUND_CHANNEL::~DS_SOUND_CHANNEL ()
{
	// 모든 필터들을 delete한다
	CleanGraph ();

	// 인터페이스들을 릴리즈 한다
	if(basic_audio) basic_audio->Release(); basic_audio=NULL;
	if(media_seeking) media_seeking->Release(); media_seeking=NULL;
	if(media_control) media_control->Release(); media_control=NULL;
	if(graph_builder) graph_builder->Release(); graph_builder=NULL;

	// COM을 uninitialize한다
	CoUninitialize ();
}

BOOL DS_SOUND_CHANNEL::CleanGraph(void)
{
	IEnumFilters*pFilterEnum;
	IBaseFilter**ppFilters;
	
	if(!init_flag) return FALSE;

	// 연주를 멈춘다
	if(media_control) media_control->Stop();
	
	// 그래프 내의 모든 필터를 얻기 위해 EnumFilters 포인터를 얻는다
	if(graph_builder->EnumFilters (&pFilterEnum)!=S_OK)	return FALSE;

	int iFiltCount=0,iPos=0;

	// 필터의 갯수를 파악
	while(S_OK==pFilterEnum->Skip(1)) iFiltCount++;

	ppFilters=reinterpret_cast<IBaseFilter**>(_alloca(sizeof(IBaseFilter*)*iFiltCount));
	
	pFilterEnum->Reset();

	// 필터에 대한 포인터를 모두 얻어온다
	while(S_OK==pFilterEnum->Next(1,&(ppFilters[iPos++]),NULL));
	if(pFilterEnum) pFilterEnum->Release(); pFilterEnum=NULL;

	// 모든 필터를 그래프에서 제거한다
	for(iPos=0;iPos<iFiltCount;iPos++)
	{
		graph_builder->RemoveFilter(ppFilters[iPos]);
		while(ppFilters[iPos]->Release()!=0);	// ref count 수 만큼 한다
	}

	return TRUE;
}

void DS_SOUND_CHANNEL::InitClass(void)
{
	// 모든 필터들을 delete한다
	CleanGraph();

	// 인터페이스들을 릴리즈 한다
	if(basic_audio) basic_audio->Release(); basic_audio=NULL;
	if(media_seeking) media_seeking->Release(); media_seeking=NULL;
	if(media_control) media_control->Release(); media_control=NULL;
	if(graph_builder) graph_builder->Release(); graph_builder=NULL;

	// COM을 uninitialize한다
	CoUninitialize();

	DS_SOUND_CHANNEL();
}

BOOL DS_SOUND_CHANNEL::HasFilter(IBaseFilter *filter)
{
	BOOL found=FALSE;

	IEnumFilters*pFilterEnum;
	IBaseFilter **ppFilters;
	
	if(!init_flag) return FALSE;

	// 그래프 내의 모든 필터를 얻기 위해 EnumFilters 포인터를 얻는다
	if(graph_builder->EnumFilters(&pFilterEnum)!=S_OK) return FALSE;

	int iFiltCount=0,iPos=0;

	// 필터의 갯수를 파악
	while(S_OK==pFilterEnum->Skip(1)) iFiltCount++;

	ppFilters=reinterpret_cast<IBaseFilter**>(_alloca(sizeof(IBaseFilter*)*iFiltCount));
	
	pFilterEnum->Reset();

	// 필터에 대한 포인터를 모두 얻어온다
	while(S_OK==pFilterEnum->Next(1,&(ppFilters[iPos++]),NULL));
	if(pFilterEnum) pFilterEnum->Release(); pFilterEnum=NULL;

	// 모든 필터를 검색한다
	for(iPos=0;iPos<iFiltCount;iPos++)
	{
		if(filter==ppFilters[iPos])	found=TRUE;
		while(ppFilters[iPos]->Release()!=0);   // ref count 수 만큼 한다
	}

	return found;
}

/*
   현재는 EC_COMPLETE가 BGM용 채널에만 전해지므로
   (정확하게는 호출되므로), 분간없이 무조건 BGM 채널이라는 전제 아래에서 처리한다
*/
void DS_SOUND_CHANNEL::OnEvent (void)
{
	long lEventCode,lParam1,lParam2; 

	// TimeOut을 10ms로 설정, 출마다 10ms씩 기다림
	if(media_event->GetEvent(&lEventCode,&lParam1,&lParam2,0)==S_OK)
	{ 
		// 끝까지 도달한 경우, BGM은 순환 연주한다
		if(lEventCode==EC_COMPLETE) 
		{ 
			Stop();
			SetPosition(0);
			Run();
		} 
	}
}

// 현재 채널의 상태를 알려준다
FILTER_STATE DS_SOUND_CHANNEL::GetState(void)const
{
	FILTER_STATE state;

	media_control->GetState(0,reinterpret_cast<long*>(&state));

	return state;
}

// 실제 mp3를 제어하는 클래스
cMP3::cMP3(int channel_num)
{
	if((channels=new DS_SOUND_CHANNEL[channel_num])==NULL||channel_num < MIN_CHANNELS)
	{
		init_flag=FALSE;
		return;
	}

	this->channel_num=channel_num;
	// 0번 채널은 배경 음악 전용으로 사용하기 때문에, 일반 출력을 위해 이 변수 값은 1로 셋팅한다.
	cur_channel=1;
	init_flag=TRUE;
}

cMP3::~cMP3()
{
	// 반드시 채널부터 해제해야 한다
	delete [] channels;
}

void cMP3::InitClass(int channel_num)
{
	if (channels != NULL)
	{
		delete [] channels;
		channels = NULL;
	}

//	cMP3::cMP3(channel_num);
}

void cMP3::LoadSound(const char* path, BOOL BGM_flag)
{
	if(!init_flag) return ;

	/*if (channels != NULL)
	{
		delete [] channels;
		channels = NULL;
	}*/

	int channel;
	IPin*pPin;
	IGraphBuilder*graph_builder;
	IBaseFilter*temp_filter;
	WCHAR wFileName[MAX_PATH];
	struct _stat temp;

	// 배경용 음악인지, 효과음용 음악인지 구분
	if(BGM_flag==TRUE) channel=0;
	else
	if(channel_num<2) return ;
	else
	channel=cur_channel;

	// 그래프 빌더에 대한 포인터를 얻어온다
	if((graph_builder=channels[channel].GetGraphBuilder())==NULL) return ;

	// 인자로 전달된 파일이 없으면 종료
	if(_stat(path,&temp)!=0) return ;

	#ifndef UNICODE	
	MultiByteToWideChar(CP_ACP,0,path,-1,wFileName,MAX_PATH);
	#else	
	lstrcpy(wFileName,path);
	#endif

	// 채널을 정지시킨다
	if(channels[channel].Stop()!=S_OK) return ;

	// 그래프 내의 다른 필터를 초기화 한다.
	if(channels[channel].CleanGraph()==FALSE) return ;

	// 필터를 그래프에 포함, 로드한다
	if(graph_builder->AddSourceFilter(wFileName,wFileName,&temp_filter)!=S_OK) return ;

	// 사용할 사운드의 출력 핀을 얻어온다
	if(temp_filter->FindPin(L"Output",&pPin)!=S_OK) return ;
	
	// 사운드파일 랜더링
	if(graph_builder->Render(pPin)!=S_OK)
	{
		if(pPin) pPin->Release(); pPin=NULL;
		return ;
	}
	
	if(pPin) pPin->Release(); pPin=NULL;
	if(temp_filter) temp_filter->Release(); temp_filter=NULL;
}

void cMP3::OnEvent(void)
{
	// 배경음 전용
	channels[0].OnEvent();
}

HRESULT cMP3::RunAll(void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Run())!=S_OK) hr=temp_hr;
	}

	return hr;
}

HRESULT cMP3::StopAll(void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Stop())!=S_OK) hr=temp_hr;
	}

	return hr;
}

HRESULT cMP3::PauseAll (void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Pause())!=S_OK) hr=temp_hr;
	}
	return hr;
}

HRESULT cMP3:: RunSounds (void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=1;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Run())!=S_OK) hr=temp_hr;
	}

	return hr;
}

HRESULT cMP3:: StopSounds (void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=1;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Stop())!=S_OK) hr=temp_hr;
	}

	return hr;
}

HRESULT cMP3::PauseSounds(void)
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=1;i<channel_num;i++)
	{
		if((temp_hr=channels[i].Pause())!=S_OK)	hr=temp_hr;
	}

	return hr;
}

// 볼륨 값 (-10000 ~ 0), 전체 채널에 대해 같은 볼륨값을 적용한다
HRESULT cMP3:: SetEntVolume (const long vol) const
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].SetVolume(vol))!=S_OK) hr=temp_hr;
	}

	return hr;
}

//  볼륨 값(-10000[왼쪽] ~ 10000[오른쪽]), 전체 채널에 대해 같은 밸런스값(좌우 음향 균형)을 적용한다
//------------------------------------------------------------------------------
HRESULT cMP3::SetEntBalance(const long bal) const
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].SetBalance(bal))!=S_OK)	hr=temp_hr;
	}
	
	return hr;
}