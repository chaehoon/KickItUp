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

/** Step Info
 */
struct KIUStep
{
	char name[256];		///< Step Title
	float bpm1;			///< Step BPM1
	float bpm2;			///< Step BPM2
	float bpm3;			///< Step BPM3
	int madi;			///< Step MADI ?
	int tick;			///< Tick count
	int diffculty;		///< DIFFCULTY - Not Use
	int track;			///< track
	int start1;			///< start time1
	int start2;			///< start time2
	int start3;			///< start time3
	int bunki1;			///< bunki1 ?
	int bunki2;			///< bunki2 ?

	char step[MAX_DATA][14];	///< Step Data
};

class SONG  
{
private:
	KIUStep	mStep[2];	// crazy1, crazy2, heard1, heard2, ....

public:
	const KIUStep & getStep1() {
		return mStep[0];
	}

	const KIUStep & getStep2() {
		return mStep[1];
	}

	void ReadCrazy_1_STF(const char *fileName);
	void ReadCrazy_1_KSF(const char *fileName);
	void ReadCrazy_2_STF(const char *fileName);
	void ReadCrazy_2_KSF(const char *fileName);

	void ReadHard_1_STF(const char *fileName);
	void ReadHard_1_KSF(const char *fileName);
	void ReadHard_2_STF(const char *fileName);
	void ReadHard_2_KSF(const char *fileName);
	
	void ReadEasy_1_STF(const char *fileName);
	void ReadEasy_1_KSF(const char *fileName);
	void ReadEasy_2_STF(const char *fileName);
	void ReadEasy_2_KSF(const char *fileName);
	
	void ReadDouble_STF(const char *fileName);
	void ReadDouble_KSF(const char *fileName);

    Surface *   mpDiskImage;
    Surface     mThisSongDiskImage;

	Uint32	Next;
	Uint32	Prev;

	bool HaveCrazy;
	bool HaveHard;
	bool HaveEasy;
	bool HaveCouple;
	bool HaveDouble;

	char TitleImgPath[PATH_LEN+1];
	char IntroWavPath[PATH_LEN+1];	// intro wav file.
	char IntroMp3Path[PATH_LEN+1];	// intro mp3 file.
	
	char DiscImagePath[PATH_LEN+1];
	char BgImgPath[PATH_LEN+1];
	char MSDPath[PATH_LEN+1];
	
	char SongTitle[PATH_LEN+1];
	char PlayWavPath[PATH_LEN+1];
	char PlayMp3Path[PATH_LEN+1];
	char PlayMpgPath[PATH_LEN+1];

	SONG();
	virtual ~SONG();

private:
    void LoadDiskImage();
};

#endif // _SONG_H
