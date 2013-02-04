// SONG.h: interface for the SONG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SONG_H__9B34A601_B4DC_11D3_AB0D_444553540000__INCLUDED_)
#define AFX_SONG_H__9B34A601_B4DC_11D3_AB0D_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ddutil.h"
//#include "sound.h"
#include "dsutil.h"
#include "main.h"

extern LPDIRECTDRAWSURFACE NoDISC;

struct  STEP
{
	unsigned char name[256];
	float BPM;
	int madi;
	int tick;
	int _dummy;
	int track;
	int start;
	char step[2048][14];
};

struct STEP_NEW
{
	unsigned char name[256];
	float BPM;
	float BPM2;
	float BPM3;
	int madi;
	int tick;
	int _dummy;
	int track;
	int start;
	int start2;
	int start3;
	int bunki;
	int bunki2;

	char step[2048][14];
};

class SONG  
{
public:
	double	bpm;
	double	bpm2;
	double	bpm3;

	int Crazy_Start;
	int Hard_Start;
	int Easy_Start;
	int Double_Start;

	int Crazy_Start2;
	int Hard_Start2;
	int Easy_Start2;
	int Double_Start2;

	int Crazy_Start3;
	int Hard_Start3;
	int Easy_Start3;
	int Double_Start3;

	DWORD Bunki;
	DWORD Bunki2;

	int Crazy_Tick;
	int Hard_Tick;
	int Easy_Tick;
	int Double_Tick;

	int Crazy_Diff;
	int Hard_Diff;
	int Easy_Diff;
	int Double_Diff;

	void ReadCrazy_1_STF(char *Filename);
	void ReadCrazy_1_KSF(char *Filename);
	void ReadCrazy_2_STF(char *Filename);
	void ReadCrazy_2_KSF(char *Filename);

	void ReadHard_1_STF(char *Filename);
	void ReadHard_1_KSF(char *Filename);
	void ReadHard_2_STF(char *Filename);
	void ReadHard_2_KSF(char *Filename);
	
	void ReadEasy_1_STF(char *Filename);
	void ReadEasy_1_KSF(char *Filename);
	void ReadEasy_2_STF(char *Filename);
	void ReadEasy_2_KSF(char *Filename);
	
	void ReadDouble_STF(char *Filename);
	void ReadDouble_KSF(char *Filename);

	BOOL HaveCrazy;
	BOOL HaveHard;
	BOOL HaveEasy;
	BOOL HaveCouple;
	BOOL HaveDouble;


	LPDIRECTSOUNDBUFFER	Int_Song;

	char TitleImgPath[MAX_PATH+1];
	char IntroWavPath[MAX_PATH+1];	// intro wav file.
	char IntroMp3Path[MAX_PATH+1];	// intro mp3 file.
	
	char Data_Hard[MAX_DATA][14];
	char Data_Hard1[MAX_DATA][14];

	char Data_Easy[MAX_DATA][14];
	char Data_Easy1[MAX_DATA][14];

	char Data_Crazy[MAX_DATA][14];
	char Data_Crazy1[MAX_DATA][14];

	char Data_Double[MAX_DATA][14];
	
	char DiscImagePath[MAX_PATH+1];
	char BgImgPath[MAX_PATH+1];
	char MSDPath[MAX_PATH+1];
	
	LPDIRECTDRAWSURFACE DiskImage;

	char SongTitle[MAX_PATH+1];
	char PlayWavPath[MAX_PATH+1];
	char PlayMp3Path[MAX_PATH+1];
	char PlayMpgPath[MAX_PATH+1];

	DWORD	Next;
	DWORD	Prev;

	SONG();
	virtual ~SONG();

};

#endif // !defined(AFX_SONG_H__9B34A601_B4DC_11D3_AB0D_444553540000__INCLUDED_)
