#ifndef	_KICKITUP_CONFIG_H
#define	_KICKITUP_CONFIG_H

#define	NORMAL		0
#define	FUSION		1

#define	DDR			0
#define	KOINS		1

struct KIUCONFIG
{
	bool	bcDead;
	bool	auto1_1p;
	bool	auto3_1p;
	bool	auto5_1p;
	bool	auto7_1p;
	bool	auto9_1p;
	bool	auto1_2p;
	bool	auto3_2p;
	bool    auto5_2p;
	bool    auto7_2p;
	bool    auto9_2p;

	int		KeySet;
	bool	OptJudge;
	int		JoySet;

};

void Configuration(void);
void CFGInitialize();
void CFGWrite();

#endif	// _CONFIG_H
