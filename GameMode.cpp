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
