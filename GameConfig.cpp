#include "GameConfig.h"
#include <algorithm>

GameConfig	g_GameConfig;

GameConfig::GameConfig(void)
{
	std::fill( &m_bStart[0], &m_bStart[eP_Max], false );
}

GameConfig::~GameConfig(void)
{
}
