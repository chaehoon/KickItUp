/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	This implemented First time Song load.
*/


// SONG.cpp: implementation of the SONG class.
//
//////////////////////////////////////////////////////////////////////

#include "Song.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SONG::SONG()
{
    mpDiskImage = & mThisSongDiskImage;
}

SONG::~SONG()
{
}

bool _getValue( const string & context, const string & key, string * value )
{
    string::size_type startIdx;
    string::size_type endIdx;

    bool    ret = false;

    // value 얻기.
    startIdx = context.find( key );
    if( startIdx != string::npos ) {
        startIdx += key.length();
        endIdx = context.find_last_of( ";" );
        if( startIdx < endIdx && endIdx != string::npos ) {
            value->assign( context.begin()+startIdx, context.begin() + endIdx );
            ret = true;
        }
    }
    return ret;
}

bool _getValue( const string & context, const string & key, char * value, const int size )
{
	bool ret = false;
	string	strVal;
	if(_getValue(context, key, &strVal)) {
		strncpy(value, strVal.c_str(), size);
		ret = true;
	}
	return ret;

}

bool _getValue( const string & context, const string & key, int * value )
{
	bool ret = false;
	string	strVal;
	if(_getValue(context, key, &strVal)) {
		*value = atoi(strVal.c_str());
		ret = true;
	}
	return ret;
}

bool _getValue( const string & context, const string & key, float * value )
{
	bool ret = false;
	string	strVal;
	if(_getValue(context, key, &strVal)) {
		*value = atof(strVal.c_str());
		ret = true;
	}
	return ret;
}

bool	ReadSTF(const char *fileName, KIUStep * pStep)
{
	char tmpStep[MAX_DATA][14];

	FILE *F = fopen(fileName,"rb");
	fread(pStep, sizeof(*pStep), 1, F);

	double bpmcount = 60 / pStep->bpm1 * 100;

	double tmpStart = pStep->start1;

	unsigned int i = 0;
	if(pStep->tick==2) {
		for(i=0;;i+=2) {
			if(tmpStart<bpmcount)
				break;
			tmpStart-=bpmcount;
			strcpy(tmpStep[i],"0000000000000");
			strcpy(tmpStep[i+1],"0000000000000");
		}
	} else if(pStep->tick==4) {
		for(i=0;;i+=4) {
			if(tmpStart<bpmcount)break;
			tmpStart-=bpmcount;
			strcpy(tmpStep[i],"0000000000000");
			strcpy(tmpStep[i+1],"0000000000000");
			strcpy(tmpStep[i+2],"0000000000000");
			strcpy(tmpStep[i+3],"0000000000000");
		}
	}
	
	for(unsigned int j = 0 ; j < (MAX_DATA - i) ; j++) {
		strcpy(tmpStep[i+j], pStep->step[j]);
	}

	pStep->start1 = (int)tmpStart;
	memcpy(&pStep->step, &tmpStep, sizeof(pStep->step));
	
	return true;
}

bool	ReadKSF(const char *fileName, KIUStep * pStep)
{
	KIUStep & step = *pStep;

	char TempStep[MAX_DATA][14];
	
	double bpmcount,tempStart;

	step.start2 = 0;
	step.start3 = 0;
	step.bpm1 = 0;
	step.bpm2 = 0;
	step.bpm3 = 0;
	step.bunki1 = 0;
	step.bunki2 = 0;

	for(int i = 0 ; i < MAX_DATA ; ++i) {
		strcpy(step.step[i],"0000000000000");
	}
	
	FILE * f = fopen(fileName,"rt");
	if(f == NULL)	{
		MsgBox( fileName,"SONG", NULL);
		return false;
	}

	char	line[1024];
	while(!feof(f)) {
		fgets(line, sizeof(line), f);
		if(_getValue(line, "#TITLE:", step.name, sizeof(step.name)) ||
				_getValue(line, "#TICKCOUNT:", &step.tick) ||
				_getValue(line, "#BUNKI:", &step.bunki1) ||
				_getValue(line, "#BUNKI2:", &step.bunki2) ||
				_getValue(line, "#BPM:", &step.bpm1) ||
				_getValue(line, "#BPM2:", &step.bpm2) ||
				_getValue(line, "#BPM3:", &step.bpm3) ||
				_getValue(line, "#DIFFCULTY:", &step.diffculty) ||
				_getValue(line, "#MADI:", &step.madi) ||
				_getValue(line, "#STARTTIME:", &step.start1) ||
				_getValue(line, "#STARTTIME2:", &step.start2) ||
				_getValue(line, "#STARTTIME3:", &step.start3) ) {
			continue;
		}

		// STEP 얻기
		if(strncmp(line,"#STEP:", 6) == 0) {
			for(int i=0 ; i < MAX_DATA ; ++i) {
				fscanf(f,"%s\n",step.step[i]);
				if(strcmp(step.step[i],"2222222222222")==0)
					break;
			}
		}
	}
	
	bpmcount = 60/step.bpm1;
	bpmcount *= 100;

	tempStart=step.start1;

	int startStepIndex = 0;
	if(step.tick==2) {
		for(startStepIndex = 0;; startStepIndex += 2) {
			if(tempStart<bpmcount)break;
			tempStart-=bpmcount;
			strcpy(TempStep[startStepIndex],"0000000000000");
			strcpy(TempStep[startStepIndex+1],"0000000000000");
		}
	} else if(step.tick==4) {
		for(startStepIndex = 0 ;; startStepIndex += 4) {
			if(tempStart<bpmcount)break;
			tempStart -= bpmcount;
			strcpy(TempStep[startStepIndex],"0000000000000");
			strcpy(TempStep[startStepIndex+1],"0000000000000");
			strcpy(TempStep[startStepIndex+2],"0000000000000");
			strcpy(TempStep[startStepIndex+3],"0000000000000");
		}
	}
	
	for(int j = 0 ; j < MAX_DATA - startStepIndex ; ++j) {
		strcpy(TempStep[startStepIndex+j],step.step[j]);
	}

	step.start1=(int)tempStart;
	memcpy(&step.step,&TempStep,sizeof(step.step));
	
	return true;
}

bool GetFullPathName( const char * fileName, char * outputDir, const size_t size )
{
    char    buff[PATH_LEN+1] = { 0, };
    if( getcwd( buff, sizeof( buff ) ) == NULL )
        return false;

    snprintf( outputDir, size, "%s/%s", buff, fileName );
    return true;

}

void SONG::ReadCrazy_1_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[0]);

	HaveCrazy = true;

	sprintf(SongTitle,"%s", mStep[0].name);

	char    buff[PATH_LEN-1] = { 0, };
	getcwd( buff, sizeof( buff ) );

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadCrazy_1_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[0]);

	HaveCrazy=true;

	sprintf(SongTitle,"%s",mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadCrazy_2_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadCrazy_2_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_1_STF(const char *fileName)
{
	ReadKSF(fileName, &mStep[0]);

	HaveHard=true;

	sprintf(SongTitle,"%s", mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_1_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[0]);

	HaveHard=true;

	sprintf(SongTitle,"%s",mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_2_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_2_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_1_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[0]);

	HaveEasy=true;

	sprintf(SongTitle,"%s",mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_1_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[0]);

	HaveEasy=true;

	sprintf(SongTitle,"%s",mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_2_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_2_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[1]);

	HaveCouple=true;

	sprintf(SongTitle,"%s",mStep[1].name);
	
	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}


void SONG::ReadDouble_STF(const char *fileName)
{
	ReadSTF(fileName, &mStep[0]);

	HaveDouble=true;

	sprintf(SongTitle,"%s",mStep[0].name);

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadDouble_KSF(const char *fileName)
{
	ReadKSF(fileName, &mStep[0]);

	HaveDouble=true;

	sprintf(SongTitle,"%s",mStep[0].name);
	
	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    
    LoadDiskImage();
}


void SONG::LoadDiskImage()
{
    if( mThisSongDiskImage.LoadBmp( "Disc.bmp" ) ) {
        mThisSongDiskImage.SetColorKey();
        mpDiskImage = &mThisSongDiskImage;
    }
    else {
        mpDiskImage = & gNoDISC;
    }
}
