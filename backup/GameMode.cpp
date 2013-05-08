/*
 * GameMode.cpp
 *
 *  Created on: 2013. 2. 22.
 *      Author: chaehoon-win
 */

#include "GameMode.h"

GameMode::GameMode()
{
	Reset();
}

GameMode::~GameMode()
{
}

void GameMode::Reset()
{
	mMode = 0;
}

void GameMode::Set(const Mode mode)
{
	// Vanish 모드와 SUDDENR 모드는 같이 있을수 없다.
	if(mode == eMODE_VANISH) {
		_unSet(eMODE_SUDDENR);
	}
	else if(mode == eMODE_SUDDENR) {
		_unSet(eMODE_VANISH);
	}
	_set(mode);
}

void GameMode::UnSet(const Mode mode)
{
	_unSet(mode);
}

bool GameMode::IsSet(const Mode mode) const
{
	return mMode & mode;
}


void GameMode::_set(const Mode mode)
{
	mMode |= mode;
}

void GameMode::_unSet(const Mode mode)
{
	mMode &= (~mode);
}


Mode GameMode::str2Mode(const char * str) const
{
	static const st_ModeStr	modeStr[] = {
			{"55975315", eMODE_VANISH},
			{"55159357", eMODE_MIRROR},
			{"51535957", eMODE_RANDOM},
			{"55797955", eMODE_SYNCHRO},
			{"79579579", eMODE_NONSTEP},
			{"13573159", eMODE_UNION},
			{"55555555", eMODE_CANCEL},
			{"55955575", eMODE_4DMIX},
			{"79513579", eMODE_SUDDENR},
			{"17931793", eMODE_RANDOMS}
	};

	for(int i = 0 ; i < sizeof(modeStr) / sizeif(modeStr[0]) ; ++i) {
		if( strcmp ( strmodeStr[i].str, str) == 0 ) {
			return strmodeStr[i].mode;
		}
	}

	return eMODE_NONE;
}
