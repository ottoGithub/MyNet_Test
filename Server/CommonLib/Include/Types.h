#ifndef _TYPES_H_
#define _TYPES_H_

#if defined(__GNUC__)
#elif defined(_WIN32)
#include <Windows.h>
#else
#error "Unknow Platform"
#endif

#if defined(__GNUC__)
	typedef long long int int64;
	typedef unsigned long long int uint64;
#elif defined(_WIN32)
	typedef __int64 int64;
	typedef unsigned __int64 uint64;
#else
#endif


typedef signed int int32;
typedef unsigned int uint32;
typedef signed short  int16;
typedef unsigned short uint16;
typedef signed char int8;
typedef unsigned char uint8;
typedef unsigned long int ulong_type;
typedef signed long int long_type;
typedef float	f32;
typedef double	f64;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



#ifdef _WIN32
//#define snprintf _snprintf sn
#define atoll(a) _atoi64(a)
#define atoull(a) _strtoui64(a, NULL, 10)
#define atoul(a) strtoul( a, NULL, 10)
#endif

#if defined(__GNUC__)
	typedef pthread_t ThreadID;
	typedef pid_t	ProcessID:
#elif defined(_WIN32)    
	typedef DWORD ThreadID;
	typedef DWORD ProcessID;
#else
#endif
           
#endif