#ifndef _G_TIMER_H_
#define _G_TIMER_H_

#include "Types.h"

class GTimer
{
public:
	/// Same as GetTimeMS
	/// Holds the time in either a 32 or 64 bit variable, depending on __GET_TIME_64BIT
	static uint32 GetTime( void );

	/// Return the time as 32 bit
	/// \note The maximum delta between returned calls is 1 second - however, RakNet calls this constantly anyway. See NormalizeTime() in the cpp.
	static uint32 GetTimeMS( void );

	/// Return the time as 64 bit
	/// \note The maximum delta between returned calls is 1 second - however, RakNet calls this constantly anyway. See NormalizeTime() in the cpp.
	static uint64 GetTimeUS( void );

protected:
	static bool initialized;
};

#endif