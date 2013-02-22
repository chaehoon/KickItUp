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
	mMode |= mode;
}

void GameMode::UnSet(const Mode mode)
{
	mMode &= (~mode);
}

bool GameMode::IsSet(const Mode mode) const
{
	return mMode & mode;
}

