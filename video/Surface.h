#ifndef _KICKITUP_SURFACE_H_
#define _KICKITUP_SURFACE_H_

#include <SDL/SDL.h>
#include <string>

class Surface
{
public:
	Surface(void);
	virtual ~Surface(void);

	bool SetVideoMode(int width, int height, int bpp, Uint32 flags = SDL_RESIZABLE);
	bool LoadBmp(std::string filename);
	void Free()	{	if( mpSurface) SDL_FreeSurface( mpSurface );	mpSurface = 0;}
	void Blit( int x, int y, Surface & destination, const SDL_Rect * clip = 0 );
	void Flip(void);

    bool IsLoaded() const   {   return mpSurface != 0;  }
	
	// Getting the value of a pixel
	Uint32 Getpixel(int x, int y);
	// Sets the color key (transparent pixel) in a blittable surface and RLE acceleration.
	bool SetColorKey(Uint32 flag, Uint32 key)	{	return SDL_SetColorKey( mpSurface, flag, key ) == 0;	}

	// Set the color key(0, 0 pixel)
	bool SetColorKey()	{	return SetColorKey( SDL_SRCCOLORKEY, Getpixel( 0, 0) );	}

	// Adjust the alpha properties of a surface
	bool SetAlpha(const Uint8 alpha);

    // This function performs a fast fill of the given rectangle with some color 
    bool FillRect( SDL_Rect * dstrect, Uint32 color );

private:
	SDL_Surface *	mpSurface;
};

#endif // SURFACE_H_

