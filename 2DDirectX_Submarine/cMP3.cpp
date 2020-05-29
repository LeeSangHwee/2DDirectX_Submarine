
#include <malloc.h>
#include <sys/stat.h>
#include "cMp3.h"

#define WIN32_LEAN_AND_MEAN

DS_SOUND_CHANNEL::DS_SOUND_CHANNEL()
{
	// COM ��ü �ʱ�ȭ.
	CoInitialize(NULL);

	// �׷��� ���� ��ü ����.
	if(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC,
				IID_IGraphBuilder,reinterpret_cast<void**>(&graph_builder))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// ��Ʈ�� �������̽��� ���´�
	if(graph_builder->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&media_control))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// Ž�� �������̽��� ���´�
	if(graph_builder->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&media_seeking))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// �⺻ ����� �������̽��� ���´�
	if(graph_builder->QueryInterface(IID_IBasicAudio,reinterpret_cast<void**>(&basic_audio))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}

	// �̵�� �̺�Ʈ �������̽��� ���´�
	if(graph_builder->QueryInterface(IID_IMediaEvent,reinterpret_cast<void**>(&media_event))!=S_OK)
	{
		init_flag=FALSE;
		return;
	}
	
	init_flag=TRUE;
}

DS_SOUND_CHANNEL::~DS_SOUND_CHANNEL ()
{
	// ��� ���͵��� delete�Ѵ�
	CleanGraph ();

	// �������̽����� ������ �Ѵ�
	if(basic_audio) basic_audio->Release(); basic_audio=NULL;
	if(media_seeking) media_seeking->Release(); media_seeking=NULL;
	if(media_control) media_control->Release(); media_control=NULL;
	if(graph_builder) graph_builder->Release(); graph_builder=NULL;

	// COM�� uninitialize�Ѵ�
	CoUninitialize ();
}

BOOL DS_SOUND_CHANNEL::CleanGraph(void)
{
	IEnumFilters*pFilterEnum;
	IBaseFilter**ppFilters;
	
	if(!init_flag) return FALSE;

	// ���ָ� �����
	if(media_control) media_control->Stop();
	
	// �׷��� ���� ��� ���͸� ��� ���� EnumFilters �����͸� ��´�
	if(graph_builder->EnumFilters (&pFilterEnum)!=S_OK)	return FALSE;

	int iFiltCount=0,iPos=0;

	// ������ ������ �ľ�
	while(S_OK==pFilterEnum->Skip(1)) iFiltCount++;

	ppFilters=reinterpret_cast<IBaseFilter**>(_alloca(sizeof(IBaseFilter*)*iFiltCount));
	
	pFilterEnum->Reset();

	// ���Ϳ� ���� �����͸� ��� ���´�
	while(S_OK==pFilterEnum->Next(1,&(ppFilters[iPos++]),NULL));
	if(pFilterEnum) pFilterEnum->Release(); pFilterEnum=NULL;

	// ��� ���͸� �׷������� �����Ѵ�
	for(iPos=0;iPos<iFiltCount;iPos++)
	{
		graph_builder->RemoveFilter(ppFilters[iPos]);
		while(ppFilters[iPos]->Release()!=0);	// ref count �� ��ŭ �Ѵ�
	}

	return TRUE;
}

void DS_SOUND_CHANNEL::InitClass(void)
{
	// ��� ���͵��� delete�Ѵ�
	CleanGraph();

	// �������̽����� ������ �Ѵ�
	if(basic_audio) basic_audio->Release(); basic_audio=NULL;
	if(media_seeking) media_seeking->Release(); media_seeking=NULL;
	if(media_control) media_control->Release(); media_control=NULL;
	if(graph_builder) graph_builder->Release(); graph_builder=NULL;

	// COM�� uninitialize�Ѵ�
	CoUninitialize();

	DS_SOUND_CHANNEL();
}

BOOL DS_SOUND_CHANNEL::HasFilter(IBaseFilter *filter)
{
	BOOL found=FALSE;

	IEnumFilters*pFilterEnum;
	IBaseFilter **ppFilters;
	
	if(!init_flag) return FALSE;

	// �׷��� ���� ��� ���͸� ��� ���� EnumFilters �����͸� ��´�
	if(graph_builder->EnumFilters(&pFilterEnum)!=S_OK) return FALSE;

	int iFiltCount=0,iPos=0;

	// ������ ������ �ľ�
	while(S_OK==pFilterEnum->Skip(1)) iFiltCount++;

	ppFilters=reinterpret_cast<IBaseFilter**>(_alloca(sizeof(IBaseFilter*)*iFiltCount));
	
	pFilterEnum->Reset();

	// ���Ϳ� ���� �����͸� ��� ���´�
	while(S_OK==pFilterEnum->Next(1,&(ppFilters[iPos++]),NULL));
	if(pFilterEnum) pFilterEnum->Release(); pFilterEnum=NULL;

	// ��� ���͸� �˻��Ѵ�
	for(iPos=0;iPos<iFiltCount;iPos++)
	{
		if(filter==ppFilters[iPos])	found=TRUE;
		while(ppFilters[iPos]->Release()!=0);   // ref count �� ��ŭ �Ѵ�
	}

	return found;
}

/*
   ����� EC_COMPLETE�� BGM�� ä�ο��� �������Ƿ�
   (��Ȯ�ϰԴ� ȣ��ǹǷ�), �а����� ������ BGM ä���̶�� ���� �Ʒ����� ó���Ѵ�
*/
void DS_SOUND_CHANNEL::OnEvent (void)
{
	long lEventCode,lParam1,lParam2; 

	// TimeOut�� 10ms�� ����, �⸶�� 10ms�� ��ٸ�
	if(media_event->GetEvent(&lEventCode,&lParam1,&lParam2,0)==S_OK)
	{ 
		// ������ ������ ���, BGM�� ��ȯ �����Ѵ�
		if(lEventCode==EC_COMPLETE) 
		{ 
			Stop();
			SetPosition(0);
			Run();
		} 
	}
}

// ���� ä���� ���¸� �˷��ش�
FILTER_STATE DS_SOUND_CHANNEL::GetState(void)const
{
	FILTER_STATE state;

	media_control->GetState(0,reinterpret_cast<long*>(&state));

	return state;
}

// ���� mp3�� �����ϴ� Ŭ����
cMP3::cMP3(int channel_num)
{
	if((channels=new DS_SOUND_CHANNEL[channel_num])==NULL||channel_num < MIN_CHANNELS)
	{
		init_flag=FALSE;
		return;
	}

	this->channel_num=channel_num;
	// 0�� ä���� ��� ���� �������� ����ϱ� ������, �Ϲ� ����� ���� �� ���� ���� 1�� �����Ѵ�.
	cur_channel=1;
	init_flag=TRUE;
}

cMP3::~cMP3()
{
	// �ݵ�� ä�κ��� �����ؾ� �Ѵ�
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

	// ���� ��������, ȿ������ �������� ����
	if(BGM_flag==TRUE) channel=0;
	else
	if(channel_num<2) return ;
	else
	channel=cur_channel;

	// �׷��� ������ ���� �����͸� ���´�
	if((graph_builder=channels[channel].GetGraphBuilder())==NULL) return ;

	// ���ڷ� ���޵� ������ ������ ����
	if(_stat(path,&temp)!=0) return ;

	#ifndef UNICODE	
	MultiByteToWideChar(CP_ACP,0,path,-1,wFileName,MAX_PATH);
	#else	
	lstrcpy(wFileName,path);
	#endif

	// ä���� ������Ų��
	if(channels[channel].Stop()!=S_OK) return ;

	// �׷��� ���� �ٸ� ���͸� �ʱ�ȭ �Ѵ�.
	if(channels[channel].CleanGraph()==FALSE) return ;

	// ���͸� �׷����� ����, �ε��Ѵ�
	if(graph_builder->AddSourceFilter(wFileName,wFileName,&temp_filter)!=S_OK) return ;

	// ����� ������ ��� ���� ���´�
	if(temp_filter->FindPin(L"Output",&pPin)!=S_OK) return ;
	
	// �������� ������
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
	// ����� ����
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

// ���� �� (-10000 ~ 0), ��ü ä�ο� ���� ���� �������� �����Ѵ�
HRESULT cMP3:: SetEntVolume (const long vol) const
{
	HRESULT hr=S_OK,temp_hr;

	for(int i=0;i<channel_num;i++)
	{
		if((temp_hr=channels[i].SetVolume(vol))!=S_OK) hr=temp_hr;
	}

	return hr;
}

//  ���� ��(-10000[����] ~ 10000[������]), ��ü ä�ο� ���� ���� �뷱����(�¿� ���� ����)�� �����Ѵ�
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