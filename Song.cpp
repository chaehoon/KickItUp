/*
	1999-2000 Copyright (c) mahalo, the nogada worker.. (Amigo~*). All rights reserved.
	          Copyright (c) Hyung-seok Choi, the Graphics Designer. All rights reserved.

	This implemented First time Song load.
*/


// SONG.cpp: implementation of the SONG class.
//
//////////////////////////////////////////////////////////////////////

#include "Song.h"
#include <stdio.h>
// #include <direct.h>
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

bool	ReadSTF(const char *fileName, STEP * pStep)
{
	char tmpStep[MAX_DATA][14];

	FILE *F = fopen(fileName,"rb");
	fread(pStep, sizeof(*pStep), 1, F);

	double bpmcount = 60 / pStep->BPM * 100;

	double tmpStart = pStep->start;

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

	pStep->start = (int)tmpStart;
	memcpy(&pStep->step, &tmpStep, sizeof(pStep->step));
	
	return true;
}

bool	ReadKSF(const char *fileName, STEP_NEW * pStep)
{
	STEP_NEW & step = *pStep;
	char TEMP[50];

	char TempStep[MAX_DATA][14];
	
	unsigned int i,j;
	double bpmcount,TempStart;

	step.start2=0;
	step.start3=0;
	step.BPM=0;
	step.BPM2=0;
	step.BPM3=0;
	step.bunki=0;
	step.bunki2=0;

	for(i=0;i<2048;i++)
		strcpy(step.step[i],"0000000000000");

	
	FILE * f = fopen(fileName,"rt");
	if(f == NULL)	{
		MsgBox( fileName,"SONG", NULL);
		return false;
	}

	char	szStr[256];
	while(!feof(f)) {
		if(fgetc(f)=='#'){
			for(i=0;;i++) {
				szStr[i]=fgetc(f);
				if(szStr[i]==':') {
					szStr[i]='\0';
					break;
				}
			}

			// #TITLE:Title Name;
			if(strcmp(szStr,"TITLE")==0) {
				for(i=0;;i++) {
					step.name[i]=fgetc(f);
					if(step.name[i]==';') {
						step.name[i]='\0';
						break;
					}
				}
			} else if(strcmp(szStr,"TICKCOUNT")==0) {	// #TICKCOUNT:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.tick=atoi(TEMP);
			} else if(strcmp(szStr,"BUNKI")==0) {	// #BUNKI:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.bunki=atoi(TEMP);
			} else if(strcmp(szStr,"BUNKI2")==0) {	// #BUNKI2:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.bunki2=atoi(TEMP);
			} else if(strcmp(szStr,"BPM")==0) {		// #BPM:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.BPM=(float)atof(TEMP);
			} else if(strcmp(szStr,"BPM2")==0) {	// #BPM2:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.BPM2=(float)atof(TEMP);
			} else if(strcmp(szStr,"BPM3")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.BPM3=(float)atof(TEMP);
			} else if(strcmp(szStr,"DIFFCULTY")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step._dummy=atoi(TEMP);
			} else if(strcmp(szStr,"MADI")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.madi=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.start=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME2")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.start2=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME3")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(f);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				step.start3=atoi(TEMP);
			} else if(strcmp(szStr,"STEP")==0) {
				for(i=0;i<2048;i++) {
					fscanf(f,"%s\n",step.step[i]);
					if(strcmp(step.step[i],"2222222222222")==0)break;
				}
			}
		}
	}
	
	bpmcount=60/step.BPM;
	bpmcount*=100;

	TempStart=step.start;

	if(step.tick==2) {
		for(i=0;;i+=2) {
			if(TempStart<bpmcount)break;
			TempStart-=bpmcount;
			strcpy(TempStep[i],"0000000000000");
			strcpy(TempStep[i+1],"0000000000000");
		}
	} else if(step.tick==4) {
		for(i=0;;i+=4) {
			if(TempStart<bpmcount)break;
			TempStart-=bpmcount;
			strcpy(TempStep[i],"0000000000000");
			strcpy(TempStep[i+1],"0000000000000");
			strcpy(TempStep[i+2],"0000000000000");
			strcpy(TempStep[i+3],"0000000000000");
		}
	}
	
	for(j=0;j<MAX_DATA-i;j++) {
		strcpy(TempStep[i+j],step.step[j]);
	}

	step.start=(int)TempStart;
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
	STEP step;

	ReadSTF(fileName, &step);

	HaveCrazy=true;
	bpm=step.BPM;

	Crazy_Start=step.start;
	Crazy_Tick=step.tick;

	sprintf(SongTitle,"%s", step.name);
	memcpy(&Data_Crazy, &step.step, sizeof(step.step));

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
	STEP_NEW step;

	ReadKSF(fileName, &step);

	HaveCrazy=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Crazy_Diff=step._dummy;
	Crazy_Start=step.start;
	Crazy_Start2=step.start2;
	Crazy_Start3=step.start3;
	Crazy_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Crazy,&step.step,sizeof(step.step));

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
	STEP step;

	ReadSTF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;

	Crazy_Start=step.start;
	Crazy_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Crazy1,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	ReadKSF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Crazy_Start=step.start;
	Crazy_Start2=step.start2;
	Crazy_Start3=step.start3;
	Crazy_Diff=step._dummy;
	Crazy_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Crazy1,&step.step,sizeof(step.step));

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
	STEP step;
	
	ReadSTF(fileName, &step);

	HaveHard=true;
	bpm=step.BPM;

	Hard_Start=step.start;
	Hard_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Hard,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	
	ReadKSF(fileName, &step);

	HaveHard=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Hard_Diff=step._dummy;
	Hard_Start=step.start;
	Hard_Start2=step.start2;
	Hard_Start3=step.start3;
	Hard_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Hard,&step.step,sizeof(step.step));

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
	STEP step;
	
	ReadSTF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;

	Hard_Start=step.start;
	Hard_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Hard1,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	
	ReadKSF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Hard_Diff=step._dummy;
	Hard_Start=step.start;
	Hard_Start2=step.start2;
	Hard_Start3=step.start3;
	Hard_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Hard1,&step.step,sizeof(step.step));

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
	STEP step;
	
	ReadSTF(fileName, &step);

	HaveEasy=true;
	bpm=step.BPM;

	Easy_Start=step.start;
	Easy_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Easy,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	
	ReadKSF(fileName, &step);

	HaveEasy=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Easy_Diff=step._dummy;
	Easy_Start=step.start;
	Easy_Start2=step.start2;
	Easy_Start3=step.start3;
	Easy_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Easy,&step.step,sizeof(step.step));

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
	STEP step;
	
	ReadSTF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;

	Easy_Start=step.start;
	Easy_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Easy1,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	
	ReadKSF(fileName, &step);

	HaveCouple=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Easy_Diff=step._dummy;
	Easy_Start=step.start;
	Easy_Start2=step.start2;
	Easy_Start3=step.start3;
	Easy_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Easy1,&step.step,sizeof(step.step));
	
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
	STEP step;
	
	ReadSTF(fileName, &step);

	HaveDouble=true;
	bpm=step.BPM;

	Double_Start=step.start;
	Double_Tick=step.tick;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Double,&step.step,sizeof(step.step));

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
	STEP_NEW step;
	
	ReadKSF(fileName, &step);

	HaveDouble=true;
	bpm=step.BPM;
	bpm2=step.BPM2;
	bpm3=step.BPM3;

	Double_Diff=step._dummy;
	Double_Start=step.start;
	Double_Start2=step.start2;
	Double_Start3=step.start3;
	Double_Tick=step.tick;

	Bunki=step.bunki;
	Bunki2=step.bunki2;

	sprintf(SongTitle,"%s",step.name);
	memcpy(&Data_Double,&step.step,sizeof(step.step));
	
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
