#ifndef _HELPER_FUNCTION_H_
#define _HELPER_FUNCTION_H_

#include <string>
#include <sstream>


#ifdef _WIN32
#include <WinBase.h>




inline
	std::string FormatErrorMessage( int32 errorNum )
{
	static char buffer[1024];
	FormatMessageA(
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorNum,
		MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),
		(LPSTR)buffer,
		sizeof(buffer) / sizeof(char) - 1,
		NULL
		);

	std::ostringstream outMsg;
	outMsg << "errno: " << errorNum << ",msg: " << buffer;

	return outMsg.str();
}


#elif defined(__GNUC__)
#include <errno.h>



inline
	std::string FormatErrorMessage( int32 errorNum )
{
	std::ostringstream outMsg;
	outMsg << "errno: " << errorNum << ",msg: " << strerror( errorNum );
	return outMsg.str();
}

}
#endif

#endif