/*==========================================================================
 *
 *  Copyright (C) 1995-1999 Microsoft Corporation. All Rights Reserved.
 *			  (c) 2000 mahalo, the nogada worker, Amigo~*. All Rights Reserved.
 *	                t (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.
 *
 *  File:       dsutil.cpp
 *  Content:    Routines for dealing with sounds from resources
 *
 *
 *	Modified 2000/05/xx : DSGetWaveResource() Modified. (support wave file, not resource)
 *
 ***************************************************************************/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#define _HSNDOBJ_DEFINED
#include "dsutil.h"


///////////////////////////////////////////////////////////////////////////////
//
// DSLoadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer *DSLoadSoundBuffer(IDirectSound *pDS, LPCTSTR lpName)
{
    IDirectSoundBuffer *pDSB = NULL;
    DSBUFFERDESC dsBD = {0};
    BYTE *pbWaveData;

	void *pvRes_=NULL;

    if (DSGetWaveResource(NULL, lpName, &dsBD.lpwfxFormat, &pbWaveData, &dsBD.dwBufferBytes, pvRes_))
    {
        dsBD.dwSize  = sizeof(dsBD);
        dsBD.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | 
                       DSBCAPS_CTRLFREQUENCY | DSBCAPS_GETCURRENTPOSITION2;

        if (SUCCEEDED(IDirectSound_CreateSoundBuffer(pDS, &dsBD, &pDSB, NULL)))
        {
            if (!DSFillSoundBuffer(pDSB, pbWaveData, dsBD.dwBufferBytes))
            {
            IDirectSoundBuffer_Release(pDSB);
            pDSB = NULL;
            }
        }
        else
        {
            pDSB = NULL;
        }
	}

	if(pvRes_)free(pvRes_);

    return pDSB;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSReloadSoundBuffer
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSReloadSoundBuffer(IDirectSoundBuffer *pDSB, LPCTSTR lpName)
{
    BOOL result=FALSE;
    BYTE *pbWaveData;
    DWORD cbWaveSize;

	void *pvRes=NULL;

    if (DSGetWaveResource(NULL, lpName, NULL, &pbWaveData, &cbWaveSize, pvRes))
    {
    if (SUCCEEDED(IDirectSoundBuffer_Restore(pDSB)) &&
        DSFillSoundBuffer(pDSB, pbWaveData, cbWaveSize))
    {
        result = TRUE;
    }
    }

	if(pvRes)free(pvRes);

    return result;
}

///////////////////////////////////////////////////////////////////////////////
//
// DSGetWaveResource
//
///////////////////////////////////////////////////////////////////////////////

BOOL DSGetWaveResource(HMODULE hModule, LPCTSTR lpName,
    WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData, DWORD *pcbWaveSize, void *pvRes)
{
	long size;

	FILE *F;

	F=fopen(lpName,"rb");
	if(F==NULL)return FALSE;

	fseek(F,0,SEEK_END);
	size=ftell(F);

	fseek(F,0,SEEK_SET);

	pvRes=(char *)malloc(size);
	fread(pvRes,1,size,F);

	//pvRes_=ftell(F);
	
	DSParseWaveResource(pvRes, ppWaveHeader, ppbWaveData, pcbWaveSize);

	fclose(F);

	return	TRUE;
}


BOOL DSFillSoundBuffer(IDirectSoundBuffer *pDSB, BYTE *pbWaveData, DWORD cbWaveSize)
{
    if (pDSB && pbWaveData && cbWaveSize)
    {
    LPVOID pMem1, pMem2;
    DWORD dwSize1, dwSize2;

    if (SUCCEEDED(IDirectSoundBuffer_Lock(pDSB, 0, cbWaveSize,
        &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
    {
        CopyMemory(pMem1, pbWaveData, dwSize1);

        if ( 0 != dwSize2 )
        CopyMemory(pMem2, pbWaveData+dwSize1, dwSize2);

        IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
        return TRUE;
    }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL DSParseWaveResource(void *pvRes, WAVEFORMATEX **ppWaveHeader, BYTE **ppbWaveData,DWORD *pcbWaveSize)
{
    DWORD *pdw;
    DWORD *pdwEnd;
    DWORD dwRiff;
    DWORD dwType;
    DWORD dwLength;

    if (ppWaveHeader)
    *ppWaveHeader = NULL;

    if (ppbWaveData)
    *ppbWaveData = NULL;

    if (pcbWaveSize)
    *pcbWaveSize = 0;

    pdw = (DWORD *)pvRes;
    dwRiff = *pdw++;
    dwLength = *pdw++;
    dwType = *pdw++;

    if (dwRiff != mmioFOURCC('R', 'I', 'F', 'F'))
    goto exit;      // not even RIFF

    if (dwType != mmioFOURCC('W', 'A', 'V', 'E'))
    goto exit;      // not a WAV

    pdwEnd = (DWORD *)((BYTE *)pdw + dwLength-4);

    while (pdw < pdwEnd)
    {
    dwType = *pdw++;
    dwLength = *pdw++;

    switch (dwType)
    {
    case mmioFOURCC('f', 'm', 't', ' '):
        if (ppWaveHeader && !*ppWaveHeader)
        {
        if (dwLength < sizeof(WAVEFORMAT))
            goto exit;      // not a WAV

        *ppWaveHeader = (WAVEFORMATEX *)pdw;

        if ((!ppbWaveData || *ppbWaveData) &&
            (!pcbWaveSize || *pcbWaveSize))
        {
            return TRUE;
        }
        }
        break;

    case mmioFOURCC('d', 'a', 't', 'a'):
        if ((ppbWaveData && !*ppbWaveData) ||
        (pcbWaveSize && !*pcbWaveSize))
        {
        if (ppbWaveData)
            *ppbWaveData = (LPBYTE)pdw;

        if (pcbWaveSize)
            *pcbWaveSize = dwLength;

        if (!ppWaveHeader || *ppWaveHeader)
            return TRUE;
        }
        break;
    }

    pdw = (DWORD *)((BYTE *)pdw + ((dwLength+1)&~1));
    }

exit:
    return FALSE;
}
