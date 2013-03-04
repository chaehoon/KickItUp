// Song.h: interface for the SONG class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	_KICKITUP_SONG_H
#define	_KICKITUP_SONG_H

#include "Main.h"
#include "video/Surface.h"
#include "Common.h"

#define PATH_LEN    255

extern Surface  gNoDISC;

/** KickItUp Step Info
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

	KIUStep() : bpm1(0.f), bpm2(0.f), bpm3(0.f), madi(0), tick(0), diffculty(0), track(0),
			start1(0), start2(0), start3(0),bunki1(0), bunki2(0){
	}
};

/**
 * 노래와 Step정보를 저장 관리하는 클래스
 * TODO: 리펙토링이 필요하다.
 */
class Song  
{
private:
	KIUStep	mStep[2];	// crazy1, crazy2, heard1, heard2, ....

public:
	/**
	 * 현재 한곡당 2개의 스탭까지 제공할수 있는데 정확한 정보는 잘 모르겠다.
	 */
	const KIUStep & getStep1() {
		return mStep[0];
	}

	/**
	 * 현재 한곡당 2개의 스탭까지 제공할수 있는데 정확한 정보는 잘 모르겠다.
	 */
	const KIUStep & getStep2() {
		return mStep[1];
	}

	/**
	 *  Creazy1 스텝을 mStep[0]에 저장한다.
	 */
	void ReadCrazy_1(const char *fileName);

	/**
	 *  Creazy2 스텝을 mStep[1]에 저장한다.
	 */
	void ReadCrazy_2(const char *fileName);

	/**
	 *  Hard1 스텝을 mStep[0]에 저장한다.
	 */
	void ReadHard_1(const char *fileName);

	/**
	 *  Hard2 스텝을 mStep[1]에 저장한다.
	 */
	void ReadHard_2(const char *fileName);

	/**
	 *  Easy1 스텝을 mStep[0]에 저장한다.
	 */
	void ReadEasy_1(const char *fileName);

	/**
	 *  Easy2 스텝을 mStep[1]에 저장한다.
	 */
	void ReadEasy_2(const char *fileName);
	
	/**
	 *  Double 스텝을 mStep[0]에 저장한다.
	 */
	void ReadDouble(const char *fileName);

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
	char IntroPath[PATH_LEN+1];		// intro wav or mp3 file.
	
	char DiscImagePath[PATH_LEN+1];
	char BgImgPath[PATH_LEN+1];
	char MSDPath[PATH_LEN+1];
	
	char SongTitle[PATH_LEN+1];
	char PlayPath[PATH_LEN+1];
	// char PlayMpgPath[PATH_LEN+1];

	Song();
	virtual ~Song();

private:
    void _loadDiskImage();
    void _readStep(const char *fileName, KIUStep * pStep);
};

#endif // _SONG_H
