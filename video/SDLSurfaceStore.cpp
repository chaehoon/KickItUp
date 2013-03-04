/*
 * SDLSurfaceStore.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "SDLSurfaceStore.h"
#include "SDLSurface.h"

SDLSurfaceStore::SDLSurfaceStore(void)	:	_pSurface( 0 )
{
}

SDLSurfaceStore::~SDLSurfaceStore(void)
{
}

// initilze sdl.
bool	SDLSurfaceStore::Initialize()
{
	// SDL_FULLSCREEN
	_pSurface = SDL_SetVideoMode( 640, 480, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_DOUBLEBUF );

	if( _pSurface == 0 )
		return false;

	// Set title Name
	SDL_WM_SetCaption( "KickItUp ver 0.9", NULL );

	return true;
}

// Destroy sdl.
void	SDLSurfaceStore::Destroy()
{
	SurfaceStore::Destroy();

	if( _pSurface == 0 )
		return;

	SDL_FreeSurface( _pSurface );
	_pSurface = 0;
}

// create sdl surface.
//Surface *	SDLSurfaceStore::create()
//{
//	Surface * pSurface = new SDLSurface( _pSurface );
//	return pSurface;
//}

// flip the main surface.
bool	SDLSurfaceStore::Process( unsigned long delta )
{
	SurfaceStore::Process( delta );
	SDL_Flip( _pSurface );
	return true;
}
