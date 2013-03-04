/*
 * SDLSurface.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "SDLSurface.h"

SDLSurface::SDLSurface( SDL_Surface * pMainSurface )	: _pMainSurface( pMainSurface ) ,
	_pSurface( 0 )
{
}

SDLSurface::~SDLSurface(void)
{
}

bool SDLSurface::Load( const std::string & fileName )
{
	Free();

	//Temporary storage for the image that's loaded
	SDL_Surface* pLoadedImage = 0;

	//Load the image
	pLoadedImage = SDL_LoadBMP( fileName.c_str() );

	assert( pLoadedImage );

	if( pLoadedImage == 0 )
		return false;

	//Create an optimized image
	_pSurface = SDL_DisplayFormat( pLoadedImage );

	if( !_pSurface )
		return false;

	//Free the old image
	SDL_FreeSurface( pLoadedImage );

	return true;
}

Surface * SDLSurface::ZoomSurface( const double zoom, const double angle /* = 0.0 */ )
{
	/* TODO: zoom
    SDL_Surface * pSurface = rotozoomSurface( m_pSurface , angle, zoom, 0);
    SDLSurface * pRet( 0 );
    if( pSurface ) {
        pRet = new SDLSurface( m_pMainSurface );
        pRet->m_pSurface = pSurface;
    }
    return pRet;
    */
	return 0;
}

// free the Surface.
void SDLSurface::Free()
{
	if( _pSurface)
		SDL_FreeSurface( _pSurface );
	_pSurface = 0;
}

bool SDLSurface::IsLoaded() const
{
	if( _pSurface )
		return true;
	return false;
}

// Blit the surface to main surface.
void SDLSurface::Blit( int x, int y, SDL_Rect * clip /* = 0 */ )
{
	_blit( x, y, _pMainSurface, clip );
}

// Blit the surface.
void SDLSurface::Blit( int x, int y, Surface & destination, SDL_Rect * clip /* = 0 */ )
{

	// TODO: fix bug
//	SDL_Surface * pDestSurface = static_cast<SDL_Surface&>(destination)._pSurface;
//
//	//Blit the surface
//	_blit( x, y, pDestSurface, clip );
}

// Blit the surface.
void SDLSurface::_blit( int x, int y, SDL_Surface * pDestination, SDL_Rect * clip /* = 0 */ )
{
	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface( _pSurface, clip, pDestination, &offset );
}

// Getting the value of a pixel
unsigned long SDLSurface::Getpixel(int x, int y)
{
	int bpp = _pSurface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)_pSurface->pixels + y * _pSurface->pitch + x * bpp;
	Uint32	ret = 0;
	switch(bpp) {
	case 1:
		ret = *p;
		break;

	case 2:
		ret = *(Uint16 *)p;
		break;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			ret = p[0] << 16 | p[1] << 8 | p[2];
		else
			ret= p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		ret = *(Uint32 *)p;
		break;

	default:
		break;
	}

	return ret;       /* shouldn't happen, but avoids warnings */
}

// Get Surface Dimension
Dim SDLSurface::GetDim() const
{
    Dim ret;
    if( _pSurface ) {
        ret.setH( _pSurface->h );
        ret.setW( _pSurface->w );
    }
    return ret;
}

// This function performs a fast fill of the given rectangle with some color
bool SDLSurface::FillRect( const SDL_Rect * dstrect, const unsigned long color )
{
	return SDL_FillRect( _pSurface, const_cast<SDL_Rect *>(dstrect), color ) == 0;
}

// Adjust the alpha properties of a surface
bool SDLSurface::SetAlpha(const unsigned char alpha)
{
	return SDL_SetAlpha( _pSurface, SDL_SRCALPHA, alpha ) == 0;
}


// Sets the color key (transparent pixel) in a blittable surface and RLE acceleration.
bool SDLSurface::SetColorKey( unsigned long key )
{
	return SDL_SetColorKey( _pSurface, SDL_SRCCOLORKEY, key ) == 0;
}

// Set the color key(0, 0 pixel)
bool SDLSurface::SetColorKey()
{
	return SetColorKey( Getpixel( 0, 0) );
}
