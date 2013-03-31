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

protected:
	virtual bool		Think( unsigned long delta ) = 0;	// 화면에 출력하기전 어떤 내용을 계산하는 함수.
	virtual bool		Render( unsigned long delta ) = 0;	// 화면에 출력.

};

inline bool Stage::Process(unsigned long delta )
{
    if( GetQuitStage() ) {
        GoToNextStage();
        return true;
    }

	// 선 작업 후
	if( Think( delta ) == false )
		return false;

	// 화면에 출력
	if( Render( delta ) == false )
		return false;

	return true;
}

#endif // _STAGE_H
