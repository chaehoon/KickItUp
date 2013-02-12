// Song.h: interface for the SONG class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	_KICKITUP_SONG_H
#define	_KICKITUP_SONG_H

#include "Main.h"
#include "Surface.h"
#include "Common.h"

#define PATH_LEN    255

extern Surface  gNoDISC;

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

	Uint32 Bunki;
	Uint32 Bunki2;

	int Crazy_Tick;
	int Hard_Tick;
	int Easy_Tick;
	int Double_Tick;

	int Crazy_Diff;
	int Hard_Diff;
	int Easy_Diff;
	int Double_Diff;

	void ReadCrazy_1_STF(char *fileName);
	void ReadCrazy_1_KSF(char *fileName);
	void ReadCrazy_2_STF(char *fileName);
	void ReadCrazy_2_KSF(char *fileName);

	void ReadHard_1_STF(char *fileName);
	void ReadHard_1_KSF(char *fileName);
	void ReadHard_2_STF(char *fileName);
	void ReadHard_2_KSF(char *fileName);
	
	void ReadEasy_1_STF(char *fileName);
	void ReadEasy_1_KSF(char *fileName);
	void ReadEasy_2_STF(char *fileName);
	void ReadEasy_2_KSF(char *fileName);
	
	void ReadDouble_STF(char *fileName);
	void ReadDouble_KSF(char *fileName);

	bool HaveCrazy;
	bool HaveHard;
	bool HaveEasy;
	bool HaveCouple;
	bool HaveDouble;


	char TitleImgPath[PATH_LEN+1];
	char IntroWavPath[PATH_LEN+1];	// intro wav file.
	char IntroMp3Path[PATH_LEN+1];	// intro mp3 file.
	
	char Data_Hard[MAX_DATA][14];
	char Data_Hard1[MAX_DATA][14];

	char Data_Easy[MAX_DATA][14];
	char Data_Easy1[MAX_DATA][14];

	char Data_Crazy[MAX_DATA][14];
	char Data_Crazy1[MAX_DATA][14];

	char Data_Double[MAX_DATA][14];
	
	char DiscImagePath[PATH_LEN+1];
	char BgImgPath[PATH_LEN+1];
	char MSDPath[PATH_LEN+1];
	
    Surface *   mpDiskImage;
    Surface     mThisSongDiskImage;

	char SongTitle[PATH_LEN+1];
	char PlayWavPath[PATH_LEN+1];
	char PlayMp3Path[PATH_LEN+1];
	char PlayMpgPath[PATH_LEN+1];

	Uint32	Next;
	Uint32	Prev;

	SONG();
	virtual ~SONG();

private:
    void LoadDiskImage();
};

#endif // _SONG_H
