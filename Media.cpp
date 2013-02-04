/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	2000/07/20 - This implemented Media play using DirectShow (In DirectMedia)
				 This support almost all audio-media (like windows media player)
*/


// Media.cpp: implementation of the CMedia class.
//
//////////////////////////////////////////////////////////////////////

#include "Media.h"
#include "main.h"
#include <dshow.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMedia::CMedia()
{
    ChangeStateTo( Uninitialized );

    media.hGraphNotifyEvent = NULL;
    media.pGraph = NULL;
}

CMedia::~CMedia()
{
    if (media.pGraph != NULL) {
	media.pGraph->Release();
	media.pGraph = NULL;
    }

    // this event is owned by the filter graph and is thus invalid
    media.hGraphNotifyEvent = NULL;
}

BOOL CMedia::RenderFile(LPSTR szFileName)
{
    HRESULT hr;
    WCHAR wPath[MAX_PATH];

    DeleteContents();

    if ( !CreateFilterGraph() ) {
	InitFail(hWnd, FALSE, "CreateFilterGrapy() : ERROR");	
	return FALSE;
    }

    MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wPath, MAX_PATH );

    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
    hr = media.pGraph->RenderFile(wPath, NULL);
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );

    if (FAILED( hr )) {
		InitFail(hWnd, FALSE, "RenderFile() : ERROR");
	return FALSE;
    }
    return TRUE;
}

BOOL CMedia::CreateFilterGraph()
{
    CoInitialize(NULL); 

    HRESULT hr; 

    hr = CoCreateInstance(CLSID_FilterGraph,           // CLSID of object
			  NULL,                         // Outer unknown
			  CLSCTX_INPROC_SERVER,         // Type of server
			  IID_IGraphBuilder,           // Interface wanted
			  (void **) &media.pGraph);     // Returned object
    if (FAILED(hr)){
	media.pGraph = NULL;
	return FALSE;
    }

	hr = media.pGraph->QueryInterface (IID_IMediaEvent, (void **) &media.pME);

    if (FAILED(hr)) { 
        return FALSE; 
    } 

    hr = media.pME->GetEventHandle((OAEVENT*) &media.hGraphNotifyEvent); 

    if (FAILED(hr)) { 
       return FALSE; 
    } 

	hr = media.pGraph->QueryInterface( IID_IMediaPosition, (void**)&media.pMP); 

	if (FAILED(hr)) { 
	   DeleteContents(); 
	   return FALSE; 
	} 

	CoUninitialize(); 

	return TRUE; 

}

#undef REWIND
#define FROM_START


void CMedia::OnMediaPlay()
{
    if( CanPlay() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
#ifdef REWIND
	    IMediaPosition * pMP;
	    hr = media.pGraph->QueryInterface(media.pGraph,
		                                      &IID_IMediaPosition,
		                                      (void**) &pMP);
	    if (SUCCEEDED(hr)) {
		// start from last position, but rewind if near the end
		REFTIME tCurrent, tLength;
		hr = pMP->get_Duration(pMP, &tLength);
		if (SUCCEEDED(hr)) {
		    hr = pMP->get_CurrentPosition(pMP, &tCurrent);
		    if (SUCCEEDED(hr)) {
			// within 1sec of end? (or past end?)
			if ((tLength - tCurrent) < 1) {
			    pMP->put_CurrentPosition(pMP, 0);
			}
		    }
		}
		pMP->Release(pMP);
	    }
#endif
	    // Ask the filter graph to play and release the interface
	    hr = pMC->Run();
	    pMC->Release();

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Playing );
		return;
	    }
	}

	// Inform the user that an error occurred
	//PlayerMessageBox( IDS_CANT_PLAY );
	InitFail(hWnd, FALSE, "OnMediaPlay() : ERROR");

    }

}

void CMedia::DeleteContents()
{
    if (media.pGraph != NULL) {
	media.pGraph->Release();
	media.pGraph = NULL;
    }

    // this event is owned by the filter graph and is thus invalid
    media.hGraphNotifyEvent = NULL;

    ChangeStateTo( Uninitialized );
}

BOOL CMedia::CanPlay()
{
    return (media.state == Stopped || media.state == Paused);
}

BOOL CMedia::CanStop()
{
    return (media.state == Playing || media.state == Paused);
}

void CMedia::ChangeStateTo(State newState)
{
    media.state = newState;
}

void CMedia::OpenMediaFile(LPSTR szFile)
{
	RenderFile(szFile);
	ChangeStateTo(Stopped);
}

void CMedia::OnMediaPause()
{
   if( CanPause() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
	    // Ask the filter graph to pause and release the interface
	    hr = pMC->Pause();
	    pMC->Release();

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Paused );
		return;
	    }
	}

	// Inform the user that an error occurred
	//PlayerMessageBox( IDS_CANT_PAUSE );
	InitFail(hWnd, FALSE, "OnMediaPause() : ERROR");
    }
}


void CMedia::OnMediaAbortStop()
{
    if(CanStop())
    {
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);

	if( SUCCEEDED(hr) ){
#ifdef FROM_START
	    IMediaPosition * pMP;
#endif
	    // Ask the filter graph to stop and release the interface
	    hr = pMC->Stop();
	    pMC->Release();

#ifdef FROM_START

	    // if we want always to play from the beginning
	    // then we should seek back to the start here
	    // (on app or user stop request, and also after EC_COMPLETE).
	    hr = media.pGraph->QueryInterface(
			IID_IMediaPosition,
			(void**) &pMP);
	    if (SUCCEEDED(hr)) {
		pMP->put_CurrentPosition(0);
		pMP->Release();
	    }

	    // no visible rewind or we will re-show the window!

#endif

	    if( SUCCEEDED(hr) ){
		ChangeStateTo( Stopped );
		return;
	    }
	}
	// Inform the user that an error occurred
	//PlayerMessageBox( IDS_CANT_STOP );
	InitFail(hWnd, FALSE, "OnMediaStop() : ERROR");
    }
}

void CMedia::OnMediaStop()
{
    if( CanStop() ){
	HRESULT hr;
	IMediaControl *pMC;

	// Obtain the interface to our filter graph
	hr = media.pGraph->QueryInterface(IID_IMediaControl, (void **) &pMC);
	if( SUCCEEDED(hr) ){

#ifdef FROM_START
	    IMediaPosition * pMP;
	    OAFilterState state;

	    // Ask the filter graph to pause
	    hr = pMC->Pause();

	    // if we want always to play from the beginning
	    // then we should seek back to the start here
	    // (on app or user stop request, and also after EC_COMPLETE).
	    hr = media.pGraph->QueryInterface(IID_IMediaPosition,
						      (void**) &pMP);
	    if (SUCCEEDED(hr)) {
		pMP->put_CurrentPosition(0);
		pMP->Release();
	    }

	    // wait for pause to complete
	    pMC->GetState(INFINITE, &state);
#endif

	    // now really do the stop
	    pMC->Stop();
	    pMC->Release();
	    ChangeStateTo( Stopped );
	    return;
	}

	// Inform the user that an error occurred
	//PlayerMessageBox( IDS_CANT_STOP );
	InitFail(hWnd, FALSE, "OnMediaStop() : ERROR");
    }
}

BOOL CMedia::CanPause()
{
    return (media.state == Playing || media.state == Paused);
}

REFTIME CMedia::GetCurrentPosition()
{
	HRESULT	hr;

	REFTIME	r_time;

	IMediaPosition * pMP;
	hr = media.pGraph->QueryInterface(IID_IMediaPosition,
						  (void**) &pMP);
	if (SUCCEEDED(hr)) {
	pMP->get_CurrentPosition(&r_time);
	pMP->Release();
	}

	return r_time;
}
