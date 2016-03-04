#ifndef _COMMONLIB_PRE_COMP_H_
#define _COMMONLIB_PRE_COMP_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
/*
 头文件Winsock2.h在内部包含了Windows.h的核心元素，
 因此在Winsock应用程序中通常不包含头文件Windows.h。
 如果需要一个#include行来包含Windows.h，那么就需要
 在前面包含#define WIN32_LEAN_AND_MEAN 宏。由于历
 史原因，头文件Windows.h默认地包含Windows Sockets 
 1.1版的头文件Winsock.h。在头文件Winsock.h中的声明
 会和Windows Socket2需要的头文件Winsock2.h中的声明
 冲突。WIN32_LEAN_AND_MEAN宏防止Windows.h包含Winso
 ck.h。
 */
#endif

#ifdef _WIN32
#include <WinSock2.h>
#endif

#include "Types.h"

#define SAFE_DELETE(p) { if(p){ delete p; p = NULL;} }

#endif