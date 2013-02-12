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

STEP	ReadSTF(char *fileName)
{
	STEP STP;

	char tmpStep[MAX_DATA][14];

	FILE *F = fopen(fileName,"rb");
	fread(&STP,sizeof(STP),1,F);

	double bpmcount = 60 / STP.BPM * 100;

	double tmpStart = STP.start;

	unsigned int i;
	if(STP.tick==2) {
		for(i=0;;i+=2) {
			if(tmpStart<bpmcount)
				break;
			tmpStart-=bpmcount;
			strcpy(tmpStep[i],"0000000000000");
			strcpy(tmpStep[i+1],"0000000000000");
		}
	} else if(STP.tick==4) {
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
		strcpy(tmpStep[i+j], STP.step[j]);
	}

	STP.start = (int)tmpStart;
	memcpy(&STP.step, &tmpStep,sizeof(STP.step));
	
	return STP;
}

STEP_NEW	ReadKSF(char *fileName)
{
	STEP_NEW STP;
	char TEMP[50];

	char TempStep[MAX_DATA][14];
	
	unsigned int i,j;
	double bpmcount,TempStart;

	STP.start2=0;
	STP.start3=0;
	STP.BPM=0;
	STP.BPM2=0;
	STP.BPM3=0;
	STP.bunki=0;
	STP.bunki2=0;

	for(i=0;i<2048;i++)
		strcpy(STP.step[i],"0000000000000");

	
	FILE *F = fopen(fileName,"rt");
	if(F==NULL)	{
		MsgBox( fileName,"SONG",NULL);
	}

	char	szStr[256];
	for(;;) {
		if(feof(F))
			break;
		if(fgetc(F)=='#'){
			for(i=0;;i++) {
				szStr[i]=fgetc(F);
				if(szStr[i]==':') {
					szStr[i]='\0';
					break;
				}
			}

			// #TITLE:Title Name;
			if(strcmp(szStr,"TITLE")==0) {
				for(i=0;;i++) {
					STP.name[i]=fgetc(F);
					if(STP.name[i]==';') {
						STP.name[i]='\0';
						break;
					}
				}
			} else if(strcmp(szStr,"TICKCOUNT")==0) {	// #TICKCOUNT:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.tick=atoi(TEMP);
			} else if(strcmp(szStr,"BUNKI")==0) {	// #BUNKI:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.bunki=atoi(TEMP);
			} else if(strcmp(szStr,"BUNKI2")==0) {	// #BUNKI2:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.bunki2=atoi(TEMP);
			} else if(strcmp(szStr,"BPM")==0) {		// #BPM:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.BPM=(float)atof(TEMP);
			} else if(strcmp(szStr,"BPM2")==0) {	// #BPM2:100;
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.BPM2=(float)atof(TEMP);
			} else if(strcmp(szStr,"BPM3")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.BPM3=(float)atof(TEMP);
			} else if(strcmp(szStr,"DIFFCULTY")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP._dummy=atoi(TEMP);
			} else if(strcmp(szStr,"MADI")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.madi=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.start=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME2")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.start2=atoi(TEMP);
			} else if(strcmp(szStr,"STARTTIME3")==0) {
				for(i=0;;i++) {
					TEMP[i]=fgetc(F);
					if(TEMP[i]==';') {
						TEMP[i]='\0';
						break;
					}
				}
				STP.start3=atoi(TEMP);
			} else if(strcmp(szStr,"STEP")==0) {
				for(i=0;i<2048;i++) {
					fscanf(F,"%s\n",&STP.step[i]);
					if(strcmp(STP.step[i],"2222222222222")==0)break;
				}
			}
		}
	}
	
	bpmcount=60/STP.BPM;
	bpmcount*=100;

	TempStart=STP.start;

	if(STP.tick==2) {
		for(i=0;;i+=2) {
			if(TempStart<bpmcount)break;
			TempStart-=bpmcount;
			strcpy(TempStep[i],"0000000000000");
			strcpy(TempStep[i+1],"0000000000000");
		}
	} else if(STP.tick==4) {
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
		strcpy(TempStep[i+j],STP.step[j]);
	}

	STP.start=(int)TempStart;
	memcpy(&STP.step,&TempStep,sizeof(STP.step));
	
	return STP;
}

bool GetFullPathName( const char * fileName, char * outputDir, const size_t size )
{
    char    buff[PATH_LEN+1] = { 0, };
    if( getcwd( buff, sizeof( buff ) ) == NULL )
        return false;

    snprintf( outputDir, size, "%s/%s", buff, fileName );
    return true;

}

void SONG::ReadCrazy_1_STF(char *fileName)
{
	STEP STP;

	STP=ReadSTF(fileName);

	HaveCrazy=true;
	bpm=STP.BPM;

	Crazy_Start=STP.start;
	Crazy_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Crazy,&STP.step,sizeof(STP.step));

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

void SONG::ReadCrazy_1_KSF(char *fileName)
{
	STEP_NEW STP;

	STP=ReadKSF(fileName);

	HaveCrazy=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Crazy_Diff=STP._dummy;
	Crazy_Start=STP.start;
	Crazy_Start2=STP.start2;
	Crazy_Start3=STP.start3;
	Crazy_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Crazy,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadCrazy_2_STF(char *fileName)
{
	STEP STP;

	STP=ReadSTF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;

	Crazy_Start=STP.start;
	Crazy_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Crazy1,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadCrazy_2_KSF(char *fileName)
{
	STEP_NEW STP;
	STP=ReadKSF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Crazy_Start=STP.start;
	Crazy_Start2=STP.start2;
	Crazy_Start3=STP.start3;
	Crazy_Diff=STP._dummy;
	Crazy_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Crazy1,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_1_STF(char *fileName)
{
	STEP STP;
	
	STP=ReadSTF(fileName);

	HaveHard=true;
	bpm=STP.BPM;

	Hard_Start=STP.start;
	Hard_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Hard,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_1_KSF(char *fileName)
{
	STEP_NEW STP;
	
	STP=ReadKSF(fileName);

	HaveHard=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Hard_Diff=STP._dummy;
	Hard_Start=STP.start;
	Hard_Start2=STP.start2;
	Hard_Start3=STP.start3;
	Hard_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Hard,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_2_STF(char *fileName)
{
	STEP STP;
	
	STP=ReadSTF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;

	Hard_Start=STP.start;
	Hard_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Hard1,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadHard_2_KSF(char *fileName)
{
	STEP_NEW STP;
	
	STP=ReadKSF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Hard_Diff=STP._dummy;
	Hard_Start=STP.start;
	Hard_Start2=STP.start2;
	Hard_Start3=STP.start3;
	Hard_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Hard1,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_1_STF(char *fileName)
{
	STEP STP;
	
	STP=ReadSTF(fileName);

	HaveEasy=true;
	bpm=STP.BPM;

	Easy_Start=STP.start;
	Easy_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Easy,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_1_KSF(char *fileName)
{
	STEP_NEW STP;
	
	STP=ReadKSF(fileName);

	HaveEasy=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Easy_Diff=STP._dummy;
	Easy_Start=STP.start;
	Easy_Start2=STP.start2;
	Easy_Start3=STP.start3;
	Easy_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Easy,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_2_STF(char *fileName)
{
	STEP STP;
	
	STP=ReadSTF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;

	Easy_Start=STP.start;
	Easy_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Easy1,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadEasy_2_KSF(char *fileName)
{
	STEP_NEW STP;
	
	STP=ReadKSF(fileName);

	HaveCouple=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Easy_Diff=STP._dummy;
	Easy_Start=STP.start;
	Easy_Start2=STP.start2;
	Easy_Start3=STP.start3;
	Easy_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Easy1,&STP.step,sizeof(STP.step));
	
	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}


void SONG::ReadDouble_STF(char *fileName)
{
	STEP STP;
	
	STP=ReadSTF(fileName);

	HaveDouble=true;
	bpm=STP.BPM;

	Double_Start=STP.start;
	Double_Tick=STP.tick;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Double,&STP.step,sizeof(STP.step));

	GetFullPathName("Title.bmp",TitleImgPath,PATH_LEN);
	GetFullPathName("Back.bmp",BgImgPath,PATH_LEN);
	GetFullPathName("Song.wav",PlayWavPath,PATH_LEN);
	GetFullPathName("Song.mp3",PlayMp3Path,PATH_LEN);
	GetFullPathName("Song.mpg",PlayMpgPath,PATH_LEN);
	GetFullPathName("Intro.wav",IntroWavPath,PATH_LEN);
	GetFullPathName("Intro.mp3",IntroMp3Path,PATH_LEN);
    LoadDiskImage();
}

void SONG::ReadDouble_KSF(char *fileName)
{
	STEP_NEW STP;
	
	STP=ReadKSF(fileName);

	HaveDouble=true;
	bpm=STP.BPM;
	bpm2=STP.BPM2;
	bpm3=STP.BPM3;

	Double_Diff=STP._dummy;
	Double_Start=STP.start;
	Double_Start2=STP.start2;
	Double_Start3=STP.start3;
	Double_Tick=STP.tick;

	Bunki=STP.bunki;
	Bunki2=STP.bunki2;

	sprintf(SongTitle,"%s",STP.name);
	memcpy(&Data_Double,&STP.step,sizeof(STP.step));
	
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
