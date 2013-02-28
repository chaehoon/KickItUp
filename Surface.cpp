#include "Surface.h"

Surface::Surface(void)	: mpSurface(0)
{
}

Surface::~Surface(void)
{
	Free();	
}

bool Surface::SetVideoMode(int width, int height, int bpp, Uint32 flags /* = SDL_RESIZABLE */)
{
	mpSurface = SDL_SetVideoMode( width, height, bpp, flags );
	if( mpSurface )
		return true;
	return false;
}


bool Surface::LoadBmp(std::string filename)
{
    Free();

	//Temporary storage for the image that's loaded
	SDL_Surface* loadedImage = NULL;

	//Load the image
	loadedImage = SDL_LoadBMP( filename.c_str() );

	//If nothing went wrong in loading the image
	if( loadedImage != NULL )
	{
		Free();

		//Create an optimized image
		mpSurface = SDL_DisplayFormat( loadedImage );

		if( !mpSurface )
			return false;

		//Free the old image
		SDL_FreeSurface( loadedImage );

		return true;
	}

	//Return the optimized image
	return false;
}

void Surface::BltFast( int x, int y, Surface & destination, const SDL_Rect * clip /* = 0 */ )
{
	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface( mpSurface, const_cast<SDL_Rect *>(clip), destination.mpSurface, &offset );
}

void Surface::Flip(void)
{
	SDL_Flip( mpSurface );
}

// Getting the value of a pixel
Uint32 Surface::Getpixel(int x, int y)
{
	int bpp = mpSurface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)mpSurface->pixels + y * mpSurface->pitch + x * bpp;
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
// Adjust the alpha properties of a surface
bool Surface::SetAlpha(const Uint8 alpha)
{
	return SDL_SetAlpha( mpSurface, SDL_SRCALPHA, alpha ) == 0;	
}

// This function performs a fast fill of the given rectangle with some color 
bool Surface::FillRect( SDL_Rect * dstrect, Uint32 color )
{
    return SDL_FillRect( mpSurface, dstrect, color ) == 0;
}
