#ifndef _KICKITUP_SURFACE_H_
#define _KICKITUP_SURFACE_H_

#include "../common.h"
#include <string>

class Surface
{
public:
	Surface(void)	{}
	virtual ~Surface(void)	{}

	virtual bool    Load( const std::string & fileName ) = 0;		///< Load imagefile.
    virtual Surface *   ZoomSurface( const double zoom, const double angle = 0.0 ) = 0;
	virtual void    Free() = 0;										///< free surface.

	virtual bool    IsLoaded() const = 0;							///< check loaded image.

	virtual void    Blit( int x, int y, SDL_Rect * clip = 0 ) = 0;	///< Blit the surface to main surface.
	virtual void    Blit( int x, int y, Surface & destination, SDL_Rect * clip = 0 ) = 0;	///< Blit the surface.

	virtual unsigned long
					Getpixel( int x, int y ) = 0;					///< Getting the value of a pixel.
    virtual Dim     GetDim() const = 0;                        ///< Get dimension.

	virtual bool    SetAlpha( const unsigned char alpha ) = 0;		///< Adjust the alpha properties of a surface
	virtual bool	FillRect( const SDL_Rect * dstrect, const unsigned long color ) = 0;
																	///< performs a fast fill of the given rectangle with some color.
	// Sets the color key (transparent pixel) in a blittable surface and RLE acceleration.
	virtual bool SetColorKey( unsigned long key ) = 0;

	// Set the color key(0, 0 pixel)
	virtual bool SetColorKey() = 0;
};

#endif // SURFACE_H_
