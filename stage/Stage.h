#ifndef _STAGE_H
#define _STAGE_H

#include "State.h"

class Stage	:	public State
{
private:
    bool    _bQuitStage;

protected:
    Stage() : _bQuitStage( false ) {}

    void    SetQuitStage( const bool bQuit )    {   _bQuitStage = bQuit;   }
    bool    GetQuitStage() const                {   return _bQuitStage;    }

public:
	virtual ~Stage()	{}

	virtual bool 		Initialize()		{return true;}
	virtual void 		Destroy()	{}
	virtual bool 		Process( unsigned long delta );

	virtual void		GetIn() = 0;	// get in the stage.
	virtual void		GetOut() = 0;	// get out the stage.

private:
	virtual bool		_think( unsigned long delta )		{	return true;	}	// 화면에 출력하기전 어떤 내용을 계산하는 함수.
	virtual bool		_render( unsigned long delta )		{	return true;	}	// 화면에 출력.

	bool 		_process( unsigned long delta );
};

// 처리함수.
inline	bool	Stage::_process( unsigned long delta )
{
    if( GetQuitStage() ) {
        GoToNextStage();
        return true;
    }

	// 선 작업 후
	if( _think( delta ) == false )
		return false;

	// 화면에 출력
	if( _render( delta ) == false )
		return false;

	return true;
}

inline bool Stage::Process(unsigned long delta )
{
	return _process(delta);
}

#endif // _STAGE_H
