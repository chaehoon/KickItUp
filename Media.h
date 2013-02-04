// Media.h: interface for the CMedia class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEDIA_H__287083C1_5E3F_11D4_AE52_00C026E0D554__INCLUDED_)
#define AFX_MEDIA_H__287083C1_5E3F_11D4_AE52_00C026E0D554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dinput.h>
#include <strmif.h>
#include <control.h>

enum State {Uninitialized, Stopped, Paused, Playing };

struct Media
{
	State state;
    IGraphBuilder *pGraph;
	IMediaPosition *pMP;
	IMediaEvent *pME;
	HANDLE hGraphNotifyEvent;
};

class CMedia  
{
public:
	REFTIME GetCurrentPosition(void);
	void OnMediaStop();
	void OnMediaAbortStop(void);
	void OnMediaPause(void);
	void OpenMediaFile(LPSTR szFile);
	void ChangeStateTo(State newState);
	BOOL CanStop(void);
	BOOL CanPlay(void);
	void OnMediaPlay(void);
	BOOL CreateFilterGraph(void);
	BOOL RenderFile(LPSTR szFileName);
	CMedia();
	virtual ~CMedia();

protected:
	BOOL CanPause(void);
	void DeleteContents(void);
	Media media;
};

#endif // !defined(AFX_MEDIA_H__287083C1_5E3F_11D4_AE52_00C026E0D554__INCLUDED_)
