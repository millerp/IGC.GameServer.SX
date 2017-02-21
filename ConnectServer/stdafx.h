// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <WinSock2.h>

// C RunTime Header Files
#include <stdlib.h>
#include <cstdio>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cstring>
#include <time.h>

#define CONNECTSERVER_VERSION "10.0.0.1"

#include <pugixml.hpp>


#pragma comment(lib, "ws2_32.lib")


// TODO: reference additional headers your program requires here
