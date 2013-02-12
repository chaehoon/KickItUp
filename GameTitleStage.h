#ifndef _GAMETITLESTAGE_H
#define _GAMETITLESTAGE_H

#include "Stage.h"

class GameTitleStage :
	public Stage
{
public:
	GameTitleStage( Surface & screen );
	virtual ~GameTitleStage(void);

	virtual bool Load();

private:
	Surface *	m_GameTitle;
	Surface *	m_StateComment;
	Surface *	m_SmallFont;
};

#endif // _GAMETITLESTAGE_H
