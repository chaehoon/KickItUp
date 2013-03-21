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
#include <strings.h>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Song::Song() : 	mThisSongDiskImage(0),
		Next(0), Prev(0),
	HaveCrazy(false),
	HaveHard(false),
	HaveEasy(false),
	HaveCouple(false),
	HaveDouble(false),

{
    mpDiskImage = & mThisSongDiskImage;
}

Song::~Song()
{
}

/**
 * context에 있는 내용에서 key값에 해당되는 값을 반환한다.
 *
 * 예) _getValue("#TITLE:Hello world;", "#TITLE:", val) --> "Hello world"
 *
 * @param	context	키값과 값을 가지고 있는 몸체
 * @param	key		찿을 키값
 * @param	value	반환 값
 * @return	찾았는지 여부
 */
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

/**
 * context에 있는 내용에서 key값에 해당되는 값을 반환한다.
 *
 * 예) _getValue("#TITLE:Hello world;", "#TITLE:", val) --> "Hello world"
 *
 * @param	context	키값과 값을 가지고 있는 몸체
 * @param	key		찿을 키값
 * @param	value	반환 string
 * @param	size	value size
 * @return	찾았는지 여부
 */
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

/**
 * context에 있는 내용에서 key값에 해당되는 값을 반환한다.
 *
 * 예) _getValue("#TITLE:Hello world;", "#TITLE:", val) --> "Hello world"
 *
 * @param	context	키값과 값을 가지고 있는 몸체
 * @param	key		찿을 키값
 * @param	value	반환 값
 * @return	찾았는지 여부
 */
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

/**
 * context에 있는 내용에서 key값에 해당되는 값을 반환한다.
 *
 * 예) _getValue("#TITLE:Hello world;", "#TITLE:", val) --> "Hello world"
 *
 * @param	context	키값과 값을 가지고 있는 몸체
 * @param	key		찿을 키값
 * @param	value	반환 값
 * @return	찾았는지 여부
 */
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

/**
 * 파일에서 KickItUp Step(stf Step) Data를 읽어 들인다.
 *
 * @param	fileName
 * @param	pStep		반환 할 Step정보
 */
bool _readSTF(const char *fileName, KIUStep * pStep)
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

/**
 * 파일에서 KickItUp Step(ksf Step) Data를 읽어 들인다.
 *
 * @param	fileName
 * @param	pStep		반환 할 Step정보
 */
bool _readKSF(const char *fileName, KIUStep * pStep)
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

/**
 * 파일이름에 전체 경로를 알아낸다.
 *
 * @param	fileName
 * @param	outputDir	반환 할 파일전체 경로
 * @param	size		outputDir size
 */
bool _getFullPathName( const char * fileName, char * outputDir, const size_t size )
{
    char    buff[PATH_LEN+1] = { 0, };
    if( getcwd( buff, sizeof( buff ) ) == NULL )
        return false;

    char realFileName[64] = {0, };

    int ret = false;
    if(GetRealFileName(fileName, realFileName, sizeof(realFileName))) {
    	snprintf(outputDir, size, "%s/%s", buff, realFileName);
    	ret = true;
    }
    else {
    	outputDir[0] = '\0';
    }
    return ret;

}

/**
 * 파일에서 KickItUp Crazy Mode Step Data를 읽어 들인다.
 */
void Song::ReadCrazy_1(const char *fileName)
{
	HaveCrazy=true;
	_readStep(fileName, &mStep[0]);
}

/**
 * 파일에서 KickItUp Crazy Mode Step Data를 읽어 들인다.
 */
void Song::ReadCrazy_2(const char *fileName)
{
	HaveCouple=true;
	_readStep(fileName, &mStep[1]);
}

/**
 * 파일에서 KickItUp Hard Mode Step Data를 읽어 들인다.
 */
void Song::ReadHard_1(const char *fileName)
{
	HaveHard=true;
	_readStep(fileName, &mStep[0]);
}

/**
 * 파일에서 KickItUp Hard Mode Step Data를 읽어 들인다.
 */
void Song::ReadHard_2(const char *fileName)
{
	HaveCouple=true;
	_readStep(fileName, &mStep[1]);
}

/**
 * 파일에서 KickItUp Easy Mode Step Data를 읽어 들인다.
 */
void Song::ReadEasy_1(const char *fileName)
{
	HaveEasy=true;
	_readStep(fileName, &mStep[0]);
}

/**
 * 파일에서 KickItUp Easy Mode Step Data를 읽어 들인다.
 */
void Song::ReadEasy_2(const char *fileName)
{
	HaveCouple=true;
	_readStep(fileName, &mStep[1]);
}

/**
 * 파일에서 KickItUp Double Mode Step Data를 읽어 들인다.
 */
void Song::ReadDouble(const char *fileName)
{
	HaveDouble=true;
	_readStep(fileName, &mStep[0]);
}

/**
 * 화면에 보여줄 노래의 disk image data를 읽어들인다.
 */
void Song::_loadDiskImage()
{
    if( mThisSongDiskImage.Load( "Disc.bmp" ) ) {
        mThisSongDiskImage.SetColorKey();
        mpDiskImage = &mThisSongDiskImage;
    }
    else {
        mpDiskImage = & gNoDISC;
    }
}

void Song::_readStep(const char *fileName, KIUStep * pStep)
{
	// 확장자 얻기
	const char * extPtr = fileName + strlen(fileName) - 3;

	// 확장자 구분.	".stf", ".ksf"
	if(strcasecmp(extPtr, "ksf") == 0) {
		_readKSF(fileName, pStep);
	} else {
		_readSTF(fileName, pStep);
	}

	strncpy(SongTitle, pStep->name, sizeof(SongTitle));
	_getFullPathName("title.bmp", TitleImgPath, PATH_LEN);
	_getFullPathName("back.bmp", BgImgPath, PATH_LEN);

	if(!_getFullPathName("song.wav", PlayPath, PATH_LEN)) {
		_getFullPathName("song.mp3", PlayPath, PATH_LEN);
		// _getFullPathName("song.mpg", PlayMpgPath, PATH_LEN);
	}

	if(!_getFullPathName("intro.wav", IntroPath, PATH_LEN) ) {
		_getFullPathName("intro.mp3", IntroPath, PATH_LEN);
	}

    _loadDiskImage();
}
