/*
 * GameMode.h
 *
 *  Created on: 2013. 2. 22.
 *      Author: chaehoon-win
 */

#ifndef GAMEMODE_H_
#define GAMEMODE_H_

class GameMode {
public:
	enum Mode {
		eMODE_NONE		= 0,
		eMODE_MIRROR	= 1 << 1,
		eMODE_NONSTOP	= 1 << 2,
		eMODE_UNION		= 1 << 3,
		eMODE_RAMDOM	= 1 << 4,
		eMODE_MIX		= 1 << 5,
		eMODE_VANISH	= 1 << 6,
		eMODE_RAMDOMS	= 1 << 7,
		eMODE_SUDDENR	= 1 << 8,
		eMODE_DOUBLE	= 1 << 8,
		eMODE_SYNCHRO	= 1 << 9,
	};
private:
	unsigned int	mMode;
	int				mSpeed;		// 배속

	struct	st_ModeStr {
		char	str[16];
		Mode	mode;
	};

public:
	GameMode();
	virtual ~GameMode();

	void Reset();

	void Set(const Mode mode);
	void UnSet(const Mode mode);
	bool IsSet(const Mode mode) const;
	Mode str2Mode(const char * str) const;

private:
	void _set(const Mode mode);
	void _unSet(const Mode mode);

};

#endif /* GAMEMODE_H_ */
