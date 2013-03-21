#ifndef _KICKITUP_KSF_H
#define _KICKITUP_KSF_H

#include <istream>
#include <string>
#include <vector>
#include <algorithm>

using std::istream;
using std::string;
using std::vector;

struct StepData_t
{
    StepData_t()    {   std::fill( &step[0], &step[14], 0 );  }
    char    step[14];
};

// Kickitup Step Format.
class Ksf
{
private:
	const static	int	MAX_DATA	=	2048;

	std::string		m_title;
    double		m_bpm[3];
    int			m_madi;
    int			m_tick;
    int			m_dummy;
    int			m_track;
    int			m_start[3];
    int			m_bunki[2];
    vector<StepData_t>  m_step;

public:
	Ksf(void);
	~Ksf(void);

	bool	ReadFromStrem( istream & is );

	std::string		GetTitle()	const	{	return	m_title;	}
	double	GetBPM( int index )	{	return m_bpm[index];	}
	int	GetMadi() const		{	return m_madi;	}
	int	GetTick() const		{	return m_tick;	}
	int	GetDummy() const	{	return m_dummy;	}
	int	GetTrack() const	{	return m_track;	}
	int	GetStart( const int index ) const	{	return m_start[index];	}
	int	GetBunki( const int index ) const	{	return m_bunki[index];	}
	const char * GetStep( const int index ) const {	return m_step[index].step;	}
    bool isEndStep( const int index ) const  {   return m_step.size() <= static_cast<size_t>(index); }
    int GetStepSize() const                 {   return m_step.size();   }

private:
    bool _getValue( const string & context, const string & key, string & value );
    void _addTick();
};

#endif // _KICKITUP_KSF_H
