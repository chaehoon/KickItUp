/*
 * SDLSurfaceStore.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef SDLSURFACESTORE_H_
#define SDLSURFACESTORE_H_

#include "../common.h"
#include "SurfaceStore.h"

class Surface;

/// @brief	SDLSurfaceStore class. (factory pattern)
class SDLSurfaceStore :	public SurfaceStore
{
private:
	SDL_Surface *	_pSurface;			///< SDL surface.

public:
	SDLSurfaceStore(void);
	virtual ~SDLSurfaceStore(void);

	virtual bool	Initialize();		///< Initialize sdl.
	virtual void	Destroy();			///< Destroy sdl
	virtual bool	Process( unsigned long delta);	///< Update.

protected:
	virtual Surface *	create();
};


#endif /* SDLSURFACESTORE_H_ */
