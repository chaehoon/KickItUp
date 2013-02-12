#include "GameTitleStage.h"

GameTitleStage::GameTitleStage( Surface & screen )	: Stage( screen )
{
}

GameTitleStage::~GameTitleStage(void)
{
}


bool GameTitleStage::Load()
{
	if( IsLoaded() )
		return true;

	m_GameTitle = MakeSurface();
	if( !m_GameTitle->LoadBmp( "images/title.bmp" ) )
		return false;

	m_StateComment = MakeSurface();
	if( !m_StateComment->LoadBmp( "images/c_font.bmp" ) )
		return false;
	m_StateComment->SetColorKey();

	m_SmallFont = MakeSurface();
	if( !m_SmallFont->LoadBmp( "images/s_font.bmp" ) )
		return false;
	m_SmallFont->SetColorKey();

	// SetLoadState();

	return true;
}
