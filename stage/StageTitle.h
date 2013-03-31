#ifndef _KICKITUP_STAGETITLE_H
#define _KICKITUP_STAGETITLE_H

#include "Stage.h"
#include "../Input/iInputHandler.h"

class Surface;
class Sound;
class SoundStore;
class SurfaceStore;
class InputStore;
class Input;
class Context;

class StageTitle	:	public Stage , public iInputHandler
{
private:
	Context &			m_context;

	Surface *	m_pGameTitle;
	Surface *	m_pStateComment;
	Sound *		m_pOpening;

public:
	StageTitle( Context & context );
	virtual ~StageTitle(void);

	virtual bool 		Initialize();
	virtual void 		Destroy();
	virtual bool		Think( unsigned long delta )	{	return true;	}
    virtual bool        Render( unsigned long delta );

	virtual void		GetIn();
	virtual void		GetOut();

	virtual void		GoNextStage();
	virtual void		GoPreStage();

	virtual bool	InputHandleMessage( const eInputName name, const eInputState is );
};

#endif // _KICKITUP_STAGETITLE_H
