/*
 * SDLSurface.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef SDLSURFACE_H_
#define SDLSURFACE_H_

#include "../common/common.h"
#include <string>

class Surface;

class SDLSurface {
private:
	SDL_Surface *	_pMainSurface;		///< main screen surface.
	SDL_Surface *	_pSurface;			///< surface.

public:
	SDLSurface( SDL_Surface * pMainSurface );
	virtual ~SDLSurface(void);

	virtual bool    Load( const std::string & fileName );		///< Load imagefile.
    virtual Surface *   ZoomSurface( const double zoom, const double angle = 0.0 );
	virtual void    Free();										///< free surface.

	virtual bool    IsLoaded() const;							///< check loaded image.

	virtual void    Blit( int x, int y, SDL_Rect * clip = 0 );	///< Blit the surface to main surface.
	virtual void    Blit( int x, int y, Surface & destination, SDL_Rect * clip = 0 );	///< Blit the surface.

	virtual unsigned long
		Getpixel(int x, int y);									///< Getting the value of a pixel.
    virtual Dim     GetDim() const;

	virtual bool    SetAlpha( const unsigned char alpha );		///< Adjust the alpha properties of a surface
	virtual bool	FillRect( const SDL_Rect * dstrect, const unsigned long color );

	// Sets the color key (transparent pixel) in a blittable surface and RLE acceleration.
	virtual bool SetColorKey( unsigned long key );

	// Set the color key(0, 0 pixel)
	virtual bool SetColorKey();

private:
	void	_blit( int x, int y, SDL_Surface * pDestination, SDL_Rect * clip = 0 );
};

#endif /* SDLSURFACE_H_ */
