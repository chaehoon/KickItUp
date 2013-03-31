/*
 * SurfaceStore.cpp
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#include "SurfaceStore.h"
#include "Surface.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

SurfaceStore *   g_pSurfaceStore = 0;

int	SurfaceStore::_alpha = 0;

SurfaceStore::SurfaceStore(void)	:	_nameIndex( 0 )
{
}

SurfaceStore::~SurfaceStore(void)
{
	Destroy();
}

// Order NoName Surface.
Surface * SurfaceStore::Order()
{
	ostringstream	o;
	o << _nameIndex;
	++_nameIndex;

	return Order( o.str() );
}
// Order New Surface.
Surface * SurfaceStore::Order( const std::string & name )
{
	// make to lower
	string lowerName;
	lowerName.resize(name.size());
	transform(name.begin(), name.end(), lowerName.begin(), ::tolower);

	// Find old Surface.
	Surface * pSurface  = Find( lowerName );

	if( pSurface == 0 ) {
		// crate new Surface.
		pSurface = create();

		assert( pSurface != 0 );

		if( pSurface )
			_surfaceList.insert( SurfaceList::value_type( lowerName, pSurface ) );
	}
	return pSurface;
}

// Destroy.
void	SurfaceStore::Destroy()
{
	// std::for_each( m_surfaceList.begin(), m_surfaceList.end(), std::mem_fun( &Surface::Free ) );
	for( SurfaceList::iterator i = _surfaceList.begin() ; i != _surfaceList.end() ; ++i ) {
		Surface * pSurface = i->second;
		pSurface->Free();
		delete pSurface;
	}

	_surfaceList.clear();
}

// find the surface.
Surface *	SurfaceStore::Find( const std::string & name )
{
	SurfaceIter	i = _surfaceList.find( name );
	if( i != _surfaceList.end() )
		return i->second;

	return 0;
}

bool SurfaceStore::Process( unsigned long delta )
{
	_updateAlpha( delta );
	return true;
}


void SurfaceStore::_updateAlpha( unsigned long delta )
{
	static unsigned long	s_timer = 0;
	static int	alphaInc = 10;

	s_timer += delta;

	if( 5 < s_timer ) {
		s_timer = 0;
		_alpha += alphaInc;
		if( _alpha < 0 ) {
			_alpha = 0;
			alphaInc = 5;
		}

		if( 255 < _alpha ) {
			_alpha = 255;
			alphaInc = -5;
		}
	}
}
