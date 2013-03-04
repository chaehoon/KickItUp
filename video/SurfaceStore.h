/*
 * SurfaceStore.h
 *
 *  Created on: 2013. 3. 4.
 *      Author: chaehoon-win
 */

#ifndef SURFACESTORE_H_
#define SURFACESTORE_H_

#include "../common/common.h"
#include <map>
#include <string>

class Surface;

/// @brief	Surface factory(factory pattern)
class SurfaceStore
{
private:
	typedef std::map<const std::string, Surface *>	SurfaceList;	///< Surface list type.
	typedef SurfaceList::iterator					SurfaceIter;	///< Surface iterator type.
	SurfaceList			_surfaceList;		///< Surface List.

	static int			_alpha;

	int					_nameIndex;		// Order 호출시 이름이 없는 것을 만들때 사용.

public:
	virtual ~SurfaceStore(void);

	virtual bool		Initialize() = 0;		///< Initialize.
	virtual void		Destroy();				///< Destroy.

	Surface *			Order( const std::string & name );	///< Order New Surface.
	Surface *			Order();							///< Order NoName Surface.
	Surface *			Find( const std::string & name );	///< Find the Surface.
	virtual bool		Process( unsigned long delta);		///< Update.

	int					GetFlashAlphaValue() const	{	return _alpha;	}

private:
	void				_updateAlpha( unsigned long delta );

protected:
	virtual	Surface *	create() = 0;						/// create Surface.

	SurfaceStore(void);
};

extern SurfaceStore *   g_pSurfaceStore;

#endif /* SURFACESTORE_H_ */
