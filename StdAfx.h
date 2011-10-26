/*******************************************************************
// Copyright (c) 2002, Robert Umbehant
// mailto:rumbehant@wheresjames.com
//
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License, or (at your option) any later 
// version.
//
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free 
// Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
// MA 02111-1307 USA 
//
*******************************************************************/
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__736428F5_60C1_48EB_9BBA_1C4573198A61__INCLUDED_)
#define AFX_STDAFX_H__736428F5_60C1_48EB_9BBA_1C4573198A61__INCLUDED_

//+++ Ignore depreciated functions for now...
#pragma warning( disable : 4995 )
#pragma warning( disable : 4996 )

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Turn off socket 1 interface
#define _MSWSOCK_

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define STRICT

#include <Windows.h>
#include <Shlwapi.h>

// wjr 9/11/06...
// Used for PraesidiumAdmin dual monitor window placement
#if !defined(WINVER) || (WINVER < 0x0500)
#define SM_CXVIRTUALSCREEN      78
#define SM_CYVIRTUALSCREEN      79
#endif
// ...wjr


//#ifdef _DEBUG
//#	define ASSERT( v ) ( v ) ? 1 : _CrtDbgBreak()
//#else
#	define ASSERT( v )
//#endif 

#define DEBUG_NEW new

#include "comdef.h"
#include "comptr.h"

/// Socket 2 interface
#include <WinSock2.h>
#pragma comment( lib, "WS2_32.lib" )

#include "inc/rulib.h"

#ifndef _T
#	if defined( _UNICODE ) || defined( UNICODE )
#		define _T( t ) L##t
#	else
#		define _T( t ) t
#	endif
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__736428F5_60C1_48EB_9BBA_1C4573198A61__INCLUDED_)
