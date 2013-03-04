#include "Stage.h"

Stage::Stage( Surface & screen )	: m_screen( screen ),
	m_loaded( false )
{
}

Stage::~Stage(void)
{
	Release();
}

void	Stage::Release()
{
}

Surface *	Stage::MakeSurface()
{
	return 0;
}

Surface *	Stage::GetScreen()
{
	return & m_screen;
}


bool	Stage::IsLoaded() const
{
	return m_loaded;
}

void	Stage::SetLoadState(const bool state /* = true */ )
{
	m_loaded = state;
}

