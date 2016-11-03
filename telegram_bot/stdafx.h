// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "wldap32.lib" )


#if defined(_DEBUG)
#	pragma comment(lib, "libeay32MTd.lib")
#	pragma comment(lib, "ssleay32MTd.lib")
#	pragma comment(lib, "libcurld.lib")
#else
#	pragma comment(lib, "libcurl32MT.lib")
#endif

#include <iostream>
#include <string>


using namespace std;


typedef std::basic_string<TCHAR>	STRING;
