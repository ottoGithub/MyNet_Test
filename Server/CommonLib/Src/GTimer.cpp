#include "PreComp.h"
#include "GTimer.h"

using namespace std;

#ifdef _WIN32
#else
#include <sys/time.h>
#include <unistd.h>
uint64 initialTime;
#endif


bool GTimer::initialized=false;

uint32 GTimer::GetTime( void )
{
	return (uint32)(GetTimeUS()/1000);
}

uint32 GTimer::GetTimeMS( void )
{
	return (uint32)(GetTimeUS()/1000);
}

#ifdef _WIN32
uint64 GTimer::GetTimeUS( void )
{
	if ( initialized == false)
	{
		initialized = true;
	}	

	// 9/26/2010 In China running LuDaShi, QueryPerformanceFrequency has to be called every time because CPU clock speeds can be different
	uint64 curTime;
	LARGE_INTEGER PerfVal;
	LARGE_INTEGER yo1;

	QueryPerformanceFrequency( &yo1 );
	QueryPerformanceCounter( &PerfVal );

	__int64 quotient, remainder;
	quotient=((PerfVal.QuadPart) / yo1.QuadPart);
	remainder=((PerfVal.QuadPart) % yo1.QuadPart);
	curTime = (uint64) quotient*(uint64)1000000 + (remainder*(uint64)1000000 / yo1.QuadPart);
	return curTime;
}
#else
uint64 GTimer::GetTimeUS( void )
{
	timeval tp;
	if ( initialized == false)
	{
		gettimeofday( &tp, 0 );
		initialized=true;
		// I do this because otherwise RakNet::Time in milliseconds won't work as it will underflow when dividing by 1000 to do the conversion
		initialTime = ( tp.tv_sec ) * (uint64) 1000000 + ( tp.tv_usec );
	}

	// GCC
	uint64 curTime;
	gettimeofday( &tp, 0 );
	curTime = ( tp.tv_sec ) * (uint64) 1000000 + ( tp.tv_usec );
	return curTime - initialTime;
}
#endif
