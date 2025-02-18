/*
    
  This file is a part of EMIPLIB, the EDM Media over IP Library.
  
  Copyright (C) 2006-2016  Hasselt University - Expertise Centre for
                      Digital Media (EDM) (http://www.edm.uhasselt.be)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA

*/

/**
 * \file miptime.h
 */

#ifndef MIPTIME_H

#define MIPTIME_H

#include "mipconfig.h"
#include "miptypes.h"
#include "mipcompat.h"
#include <stdio.h>
#include <time.h>
#include <string>

// We're going to use some things from RTPTime
#include <jrtplib3/rtptimeutilities.h>

/** This class is used for timing purposes.
 *  This class provides some time handling functions.
 */
class EMIPLIB_IMPORTEXPORT MIPTime
{
public:
	/** Returns a MIPTime object containing the current time. */
	static MIPTime getCurrentTime();

	/** Pauses the current thread for the time contained in \c delay. */
	static void wait(const MIPTime &delay);
	
	/** Creates a time object containing the time corresponding to \c t. */
	MIPTime(real_t t = 0.0)								{ m_time = t; }

	/** Creates a time object containing the time corresponding to the two parameters. */
	MIPTime(int64_t seconds, int64_t microSeconds)					{ m_time = (((real_t)seconds)+(((real_t)microSeconds)/1000000.0)); }

	/** Returns the number of seconds contained in the time object. */
	int64_t getSeconds() const							{ return (int64_t)m_time; }

	/** Returns the number of microseconds contained in the time object. */
	int64_t getMicroSeconds() const;

	/** Returns a real value describing the time contained in this object. */
	real_t getValue() const 							{ return m_time; }

	MIPTime &operator-=(const MIPTime &t);
	MIPTime &operator+=(const MIPTime &t);
	bool operator<(const MIPTime &t) const;
	bool operator>(const MIPTime &t) const;
	bool operator<=(const MIPTime &t) const;
	bool operator>=(const MIPTime &t) const;
	std::string getString() const;
private:
	real_t m_time;
};

inline MIPTime MIPTime::getCurrentTime()
{
	// we'll use the RTPTime for this
	jrtplib::RTPTime tv = jrtplib::RTPTime::CurrentTime();
	return MIPTime((int64_t)tv.GetSeconds(), (int64_t)tv.GetMicroSeconds());
}

inline void MIPTime::wait(const MIPTime &delay)
{
	if (delay.m_time < 0)
		return;
	jrtplib::RTPTime::Wait(jrtplib::RTPTime(delay.m_time));
}

inline int64_t MIPTime::getMicroSeconds() const
{
	real_t t = m_time;
	if (t < 0)
		t = -t;
	t -= (real_t)((int64_t)t);
	t *= 1000000.0;
	return (int64_t)t;
}

inline MIPTime &MIPTime::operator-=(const MIPTime &t)
{ 
	m_time -= t.m_time;
	return *this;
}

inline MIPTime &MIPTime::operator+=(const MIPTime &t)
{ 
	m_time += t.m_time;
	return *this;
}

inline bool MIPTime::operator<(const MIPTime &t) const
{
	if (m_time < t.m_time)
		return true;
	return false;
}

inline bool MIPTime::operator>(const MIPTime &t) const
{
	if (m_time > t.m_time)
		return true;
	return false;
}

inline bool MIPTime::operator<=(const MIPTime &t) const
{
	if (m_time <= t.m_time)
		return true;
	return false;
}

inline bool MIPTime::operator>=(const MIPTime &t) const
{
	if (m_time >= t.m_time)
		return true;
	return false;
}

#include <inttypes.h>

inline std::string MIPTime::getString() const
{
	char str[256];

	MIP_SNPRINTF(str, 255, "%" PRId64 ".%06d", getSeconds(), (int)getMicroSeconds());
	
	return std::string(str);
}

#endif // MIPTIME_H

