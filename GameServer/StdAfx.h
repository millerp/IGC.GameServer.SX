// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning ( disable : 4786 )	// Disable Warning of Large Debuf Strings ( truncated to 255 len )

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x0601
#define WINVER 0x0601

//#define VLD_FORCE_ENABLE		// Visual Leak Detector
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#define NOMINMAX

#include <vector>
#include <set>
#include <stack>
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
#include <random>
#include <tchar.h>
#include <strsafe.h>
#include <time.h>
#include <Rpc.h>
#include <strtk.hpp>

// Windows Header Files:
#include <Windows.h>
#include <WinSock2.h>
#include <MMSystem.h>
#include <CommCtrl.h>
#include <cstdlib>
#include <map>
#include <string>

//#include <vld.h>
// Local Header Files
#include "Lang.h"
#include "IniReader.h"

// Crypto++
#include <sha.h>
#include <md4.h>
#include <md5.h>
#include <ripemd.h>
#include <rsa.h>
#include <3way.h>
#include <cast.h>
#include <rc5.h>
#include <rc6.h>
#include <mars.h>
#include <idea.h>
#include <gost.h>
#include <tea.h>
#include <twofish.h>
#include <des.h>
#include <skipjack.h>
#include <safer.h>
#include <rsa.h>
#include <osrng.h>
#include <rijndael.h>
#include <integer.h>

// RSA
#include <mpir.h>
#include <gmp.h>
// XML
#include <pugixml.hpp>

// LUA
#include <lua.hpp>

// muParser
#include <muParser.h>

// Boost
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#pragma comment(lib, "cryptlib.lib")
#pragma comment(lib, "lua52.lib")
#pragma comment(lib, "iniparser.lib")
#pragma comment(lib, "muparser.lib")
#pragma comment(lib, "winmm.lib")


#define CRYPTOPP_
//#define AWHOST
//#define EMU_NOCRYPT
//#define C3C4_DISABLECRYPT

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "mpir64.lib")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
