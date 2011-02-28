/*******************************************************************
// Copyright (c) 2002, Robert Umbehant ( www.wheresjames.com )
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
// Ru.h: Library
//
//////////////////////////////////////////////////////////////////////
/*! \mainpage rulib - General purpose library

  \section sec_intro Introduction

	A C++ library for the Windows platform containing general 
	purpose classes for files, images, sockets, registry, 
	video capture, MIME, and other basic purposes.

  \subsection sub_sockets

	Unfortunately, MS didn't make using the Socket 2 interface very
	straight forward.  You have to disable the original socket 
	interface by including the following line of code <b>before</b>
	you include <b>Windows.h</b>.	

	\code
	// Turn off socket 1 interface
	#define _MSWSOCK_
	\endcode

	Then include the Windows Socket 2 interface using something to the
	effecto of the following.

	\code
	/// Socket 2 interface
	#include <WinSock2.h>
	#pragma comment( lib, "WS2_32.lib" )
	\endcode

	I usually stick this in my <b>StdAfx.h</b> file.  To see an example, check
	the StdAfx.h file in <b>rulib.h</b>.

  \subsection sub_folders Folder Structure

		The following example folder structure is assumed.
		You may need to make modifications if your structure 
		does not match.  If the folder is not mentioned, it's
		location (probably) won't matter.

		-	code
			-	lib
				-	CxImage
				-	jpeg
				-	libpng
				-	OpenCV
				-	<b>rulib</b>
					-	dox
						-	doxygen.cfg

			-	MyApps ( or other catagory )
				-	2005_06 ( or appropriate date)
					-	App1
						-	App1.dsw
					-	App2
						-	App2.dsw


  \subsection sub_linking Linking to your project

		-	It is recommended you add something to the following
			to your projects main header or StdAfx.h file.  If your
			application path does not match the above, you will need
			to modify the paths in the code section below

		\code
		
	#include "../../../lib/rulib/inc/rulib.h"

	#ifdef _DEBUG
	#pragma comment( lib, "../../../lib/rulib/lib/db_rulib.lib" )
	#else
	#pragma comment( lib, "../../../lib/rulib/lib/rulib.lib" )
	#endif

		\endcode

		-	You could also add the <b> rulib </b> folder to
			your include path.  Manually add the libs, and
			add something to the effect of this to your headers.

		\code
	
	#include "rulib.h"	

		\endcode

  \subsection sub_compile Compiling this library.

	To compile this library you will need the following

		- Microsoft Developer Studio Visual C++ 6 Service Pack 6 or better

			or

		- Intel Compiler
			-#	http://www.intelcompiler.com/

		- Microsoft Platform SDK
			-#	Install and update include paths
			-#	http://en.wikipedia.org/wiki/Platform_SDK/

		- Doxygen
			-#	http://sourceforge.net/projects/doxygen/

		- CxImage
			-#	http://www.xdp.it/forum.htm

		- jpeg
			-#	http://www.jpeg.org/

		- libpng
			-#	http://www.libpng.org/pub/png/

		- OpenCV
			-#	http://sourceforge.net/projects/opencvlibrary/
  
		- rulib
			-#	http://sourceforge.net/projects/rulib/		

  \subsection sub_tools Useful Tools

		-	Tortoise CVS
			-#	http://www.tortoisecvs.org/

		-	Winmerge
			-#	http://winmerge.sourceforge.net/

		-	CVSNT
			-#	http://www.cvsnt.org/

		-	Subversion
			-#	http://subversion.tigris.org/

		-	TortoiseSVN / TortoiseMerge
			-#	http://tortoisesvn.tigris.org/

*/

#ifndef RULIB_2002_12_12_INCLUDED
#define RULIB_2002_12_12_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// To enable the image support, bmp, jpg, gif, png, etc...
#define ENABLE_WINIMAGE
//#undef ENABLE_WINIMAGE

// If you want support for OpenCV
//#define ENABLE_OPENCV
//#undef ENABLE_OPENCV

// To enable socket old socket classes
//#define ENABLE_SOCKETS
//#undef ENABLE_SOCKETS

// To enable the use of Socket 2 interface
#define ENABLE_SOCKETS2
//#undef ENABLE_SOCKETS2

// Enable compression classes
#define ENABLE_ZLIB
//#undef ENABLE_ZLIB

// Enable Squirrel script / SqPlus
//#define ENABLE_SQUIRREL
//#undef ENABLE_SQUIRREL

// Enables error logging
#define ENABLE_ERRORLOGGING
//#undef ENABLE_ERRORLOGGING

// Enable exception handling
//#define RULIB_EXCEPTION_HANDLING
//#undef RULIB_EXCEPTION_HANDLING

// Enable automatic library imports
//#define RULIB_LIBS
//#undef RULIB_LIBS
			   
// Enable stack tracing
#ifdef _DEBUG
#	define ENABLE_STACK_TRACING
#	define ENABLE_STACK_HISTORY
#	undef ENABLE_STACK_HISTORY_TIME // wjr 1/19/07
#	undef ENABLE_STACK_HISTORY_TIME_AUTO // wjr 1/19/07
#	undef ENABLE_STACK_PROFILE
#else
#	define ENABLE_STACK_TRACING
#	define ENABLE_STACK_HISTORY
#	undef ENABLE_STACK_HISTORY_TIME // wjr 1/19/07
#	undef ENABLE_STACK_HISTORY_TIME_AUTO // wjr 1/19/07
#	undef ENABLE_STACK_PROFILE
#endif

// Use this in Exe's, this is the fastest but doesn't work in DLL's
//#define CStackTrace_USING_TLS

// Define this in DLL's or Exe's.  For Exe only, define CStackTrace_USING_TLS instead
#define CStackTrace_USING_TLSAPI

#ifdef __INTEL_COMPILER
#   pragma warning( disable : 1786 )
#endif

#include <WinInet.h>
#include <commdlg.h>
#include <StdIO.h>
#include <ShellApi.h>
#include <time.h>
#include <math.h>
#include <string>
#include <vector>

// This only works if your paths match the above mentioned structure
#ifdef RULIB_LIBS

#pragma comment( lib, "Wininet.lib" )

#ifdef ENABLE_SQUIRREL
#   include "../../SqPlus/SqPlus/SqPlus.h"
#endif

#	ifdef ENABLE_WINIMAGE

#		ifdef _DEBUG
#			pragma comment( lib, "../../../lib/rulib/lib/db_rulib.lib" )
#			pragma comment( lib, "../../../lib/cximage/Debug/cximage.lib" )
#			pragma comment( lib, "../../../lib/png/Debug/png.lib" )
#			pragma comment( lib, "../../../lib/jpeg/Debug/jpeg.lib" )
#			pragma comment( lib, "../../../lib/zlib/Debug/zlib.lib" )
#		else
#			pragma comment( lib, "../../../lib/rulib/lib/rulib.lib" )
#			pragma comment( lib, "../../../lib/cximage/Release/cximage.lib" )
#			pragma comment( lib, "../../../lib/png/Release/png.lib" )
#			pragma comment( lib, "../../../lib/jpeg/Release/jpeg.lib" )
#			pragma comment( lib, "../../../lib/zlib/Release/zlib.lib" )
#		endif

#	endif

#	ifdef ENABLE_OPENCV
#		ifdef _DEBUG
#			pragma comment( lib, "../../../lib/OpenCV/lib/s_cvd.lib" )
#			pragma comment( lib, "../../../lib/OpenCV/lib/s_cxcored.lib" )
#		else
#			pragma comment( lib, "../../../lib/OpenCV/lib/s_cv.lib" )
#			pragma comment( lib, "../../../lib/OpenCV/lib/s_cxcore.lib" )
#		endif
#	endif

#	ifdef ENABLE_SQUIRREL
#		ifdef _DEBUG
#          	pragma comment( lib, "../../../lib/SqPlus/lib/squirrelD.lib" )
#          	pragma comment( lib, "../../../lib/SqPlus/lib/sqplusD.lib" )
#           pragma comment( lib, "../../../lib/SqPlus/lib/sqstdlibD.lib" )
#		else
#       	pragma comment( lib, "../../../lib/SqPlus/lib/squirrel.lib" )
#	        pragma comment( lib, "../../../lib/SqPlus/lib/sqplus.lib" )
#	        pragma comment( lib, "../../../lib/SqPlus/lib/sqstdlib.lib" )
#		endif

#	endif

#	ifdef ENABLE_OPENCV
#		include "Cv.h"
#	endif

#else

#	ifdef ENABLE_OPENCV
#		include "opencv/cv.h"
#	endif

#	ifdef ENABLE_SQUIRREL
#		include "SqPlus.h"
#	endif

#endif


#ifdef ENABLE_OPENCV
#	define CV_RELEASE_IMAGE( img ) ( ( img ) ? ( cvReleaseImage( &img ), img = NULL ) : 0 )
#endif


#ifdef ENABLE_SQUIRREL
#	define SQCHAR SQChar
#else
#	define SQCHAR TCHAR
#endif


#ifdef RULIB_EXCEPTION_HANDLING
#define RULIB_TRY			try
#define RULIB_CATCH_ALL		catch( ... )
#else
#define RULIB_TRY			if ( 1 )
#define RULIB_CATCH_ALL		else if ( 0 )
#endif

/*******************************************************************
// A few handy macros
*******************************************************************/
#include "Macro.h"

// Base classes

/*******************************************************************
// CTimeout - Timeout class
*******************************************************************/
#include "Timeout.h"

/*******************************************************************
// CHqTimer - High accuracy timer class
*******************************************************************/
#include "HqTimer.h"

// Group A

/*******************************************************************
// CStrParse - String parsing functions
*******************************************************************/
#include "StrParse.h"

/*******************************************************************
// TMem - Auto pointer template * use STL instead
*******************************************************************/
#include "TMem.h"

/*******************************************************************
// CStr - Generic string class * use STL instead
*******************************************************************/
#include "Str.h"

/*******************************************************************
// CWinRect - Windows rectangle class
*******************************************************************/
#include "WinRect.h"

/*******************************************************************
// CWinPolygon - Polygon class
*******************************************************************/
#include "WinPolygon.h"

/*******************************************************************
// TThreadSafe	- Thread safe wrapper
// CTlLock		- Thread lock class
// CTlLocalLock	- Scope release wrapper for CTlLock
*******************************************************************/
#include "ThreadSafe.h"

/*******************************************************************
// linked_ptr.h - Linked pointer, hopefully one day a standard
*******************************************************************/
#include "linked_ptr.h"

/*******************************************************************
// CLList - Linked list class
*******************************************************************/
#include "LList.h"

/*******************************************************************
// CHList - Hash list
//		ext: CLList
*******************************************************************/
#include "HList.h"

/*******************************************************************
// TLList - Template class for linked lists
//		ext: CLList
*******************************************************************/
#include "TLList.h"

/*******************************************************************
// THList - Template class for hash lists
//		ext: CLList
*******************************************************************/
#include "THList.h"

/*******************************************************************
// CStackTrace - Stack trace class, #define RULIB_STACKTRACING
*******************************************************************/
#include "StackTrace.h"

/*******************************************************************
// CErrLog - Auto pointer class * use STL instead
*******************************************************************/
#include "ErrLog.h"

/*******************************************************************
// CvErr - Error reporting class (start of one anyway)
*******************************************************************/
#include "vErr.h"

/*******************************************************************
// CBase64 - For standard base-64 encoding and decoding 
*******************************************************************/
#include "Base64.h"

/*******************************************************************
// CMd5Rsa - The RFC 1321 RSA code with minimal changes
*******************************************************************/
#include "Md5Rsa.h"

/*******************************************************************
// CMd5 - Wrapper class for the RFC 1321
*******************************************************************/
#include "Md5.h"

/*******************************************************************
// CThread - For windows thread control
*******************************************************************/
#include "Thread.h"

/*******************************************************************
// CCircBuf - Thread-safe circular buffer class
*******************************************************************/
#include "CircBuf.h"

/*******************************************************************
// CDataPacket - A simple data packet class with CRC check
*******************************************************************/
#include "DataPacket.h"

/*******************************************************************
// CWin32 - A few handy windows functions
*******************************************************************/
#include "Win32.h"

/*******************************************************************
// CWinDc - Windows Device Context wrapper
*******************************************************************/
#include "WinDc.h"

/*******************************************************************
// WinDcQueue - Array of DC's 
*******************************************************************/
#include "WinDcQueue.h"

/*******************************************************************
// CWinFile - For windows file system manipulation
*******************************************************************/
#include "WinFile.h"

/*******************************************************************
// CPipe - Generic I/O base class
*******************************************************************/
#include "Pipe.h"

/*******************************************************************
// CPFile - Generic file stream class
//		ext: CPipe
*******************************************************************/
#include "PFile.h"

/*******************************************************************
// CVar - Generic script variable class
*******************************************************************/
#include "Var.h"

/*******************************************************************
// CWinTime - Time formating and clock/calendar drawing
*******************************************************************/
#include "WinTime.h"

/*******************************************************************
// CMessage - Windows message target wrapper
*******************************************************************/
#include "Message.h"

/*******************************************************************
// CMessageList - Windows message target list wrapper
*******************************************************************/
#include "MessageList.h"

/*******************************************************************
// CWin - Base class for HWND wrapper
*******************************************************************/
#include "Win.h"

/*******************************************************************
// CWinDlg - Base class for HWND Dialog
*******************************************************************/
#include "WinDlg.h"

/*******************************************************************
// CCfgFile - Manipulating ini like files * use CReg instead
*******************************************************************/
#include "CfgFile.h"

/*******************************************************************
// CScsSerialize - Provides simple data serializing / deserializing
*******************************************************************/
#include "ScsSerialize.h"

/*******************************************************************
// CRKey - Handles single registry keys and registry disk files
*******************************************************************/
#include "RKey.h"

/*******************************************************************
// CReg - Handles whole sections of the Windows Registry
*******************************************************************/
#include "Reg.h"

/*******************************************************************
// CClientDde - Implements the client side of a DDE connection
*******************************************************************/
#include "ClientDde.h"

/*******************************************************************
// CDdeSz - Wraps a DDE string
*******************************************************************/
#include "DdeSz.h"

// Group B

/*******************************************************************
// CAsyncWinSock - Windows socket API wrapper
*******************************************************************/
#ifdef ENABLE_SOCKETS
#include "AsyncWinSock.h"
#endif

/*******************************************************************
// CAutoReg - Windows registry class
*******************************************************************/
#include "AutoReg.h"

/*******************************************************************
// CBmp - Windows bitmap class (use CWinImg if possible)
*******************************************************************/
#include "Bmp.h"

/*******************************************************************
// CCapture - Video For Windows capture class
*******************************************************************/
#include "Capture.h"

/*******************************************************************
// CComCtrl - Static functions for Tree and List controls
*******************************************************************/
#include "ComCtrl.h"

#ifdef ENABLE_ZLIB

/*******************************************************************
// CComp2 - ZLib compression wrapper
*******************************************************************/
#include "Comp2.h"

/*******************************************************************
// CCmpFile - General purpose storage file.  Similar to zip
*******************************************************************/
#include "CmpFile.h"

#endif

/*******************************************************************
// CCsvFile - For parsing Comma Separated Value files
*******************************************************************/
#include "CsvFile.h"

/*******************************************************************
// CDlgMover - Moving windows and ShAppBar
*******************************************************************/
#include "DlgMover.h"

/*******************************************************************
// CFifoSync - Implements a packet based fifo buffer
*******************************************************************/
#include "FifoSync.h"

/*******************************************************************
// CFileSearch - Searches disk files for search patterns
*******************************************************************/
#include "FileSearch.h"

/*******************************************************************
// CFindFiles - Wrapper for the Windows file search API
*******************************************************************/
#include "FindFiles.h"

/*******************************************************************
// CGrBrush - Simple HBRUSH wrapper
*******************************************************************/
#include "GrBrush.h"

/*******************************************************************
// CGrPen - Simple HPEN wrapper
*******************************************************************/
#include "GrPen.h"

/*******************************************************************
// CGrDC - HDC wrapper containing many static functions
*******************************************************************/
#include "GrDC.h"

/*******************************************************************
// CHookStdio - Hooks standard i/o streams
*******************************************************************/
#include "HookStdio.h"

/*******************************************************************
// TIncDev - Incremental division class
*******************************************************************/
#include "IncDev.h"

/*******************************************************************
// CMime	- MIME format reader/writer class
*******************************************************************/
#include "Mime.h"

/*******************************************************************
// CModule - For dynamic use of dll library's
*******************************************************************/
#include "Module.h"

/*******************************************************************
// CMovableObj - Generic 2D object manipulation class
*******************************************************************/
#include "MovableObj.h"

/*******************************************************************
// CMultiStore - Large data index
*******************************************************************/
#include "MultiStore.h"

/*******************************************************************
// CNetMsg - Messaging protocol derived from CDataPacket
*******************************************************************/
#include "NetMsg.h"

/*******************************************************************
// CNetCmd - Provides common command structure
*******************************************************************/
#include "NetCmd.h"

/*******************************************************************
// CNetCom - CNetMsg connection manager
*******************************************************************/
#include "NetCom.h"

/*******************************************************************
// CNetDispatch - Helper class for dispatching CNetCom messages
*******************************************************************/
#include "NetDispatch.h"

/*******************************************************************
// CNetMgr - Creates a network backbone
*******************************************************************/
#include "NetMgr.h"

/*******************************************************************
// CNetFile	- For downloading files and data from the internet
*******************************************************************/
#include "NetFile.h"

/*******************************************************************
// CParams - For serializing / deserializing parameter lists
*******************************************************************/
#include "Params.h"

/*******************************************************************
// CResType - Enumerating resources
*******************************************************************/
#include "ResType.h"

/*******************************************************************
// CPid - PID algorithm
*******************************************************************/
#include "Pid.h"

/*******************************************************************
// CStackReport - Generates stack reports
*******************************************************************/
#include "StackReport.h"

/*******************************************************************
// CTaskList - Class for retrieving a list of running processes
*******************************************************************/
#include "TaskList.h"

/*******************************************************************
// CText	- Text drawing and manipulation
*******************************************************************/
#include "Text.h"

/*******************************************************************
// CTooltip - For adding tool tips
*******************************************************************/
#include "Tooltip.h"

/*******************************************************************
// CTrayIcon - Tray icon manipulation
*******************************************************************/
#include "TrayIcon.h"

/*******************************************************************
// CWinAvi - Windows avi wrapper
*******************************************************************/
#include "WinAvi.h"

#ifdef ENABLE_SOCKETS2

/*******************************************************************
// CWinSocket - Windows socket API wrapper
*******************************************************************/
#include "WinSocket.h"

/*******************************************************************
// CWinAsyncSocket - Async socket interface
*******************************************************************/
#include "WinAsyncSocket.h"

/*******************************************************************
// CWspStream - Generic stream protocol for CWinAsyncSocket
*******************************************************************/
#include "WspStream.h"

#endif

/*******************************************************************
// CAviEncode - Thread wrapper for CWinAvi
*******************************************************************/
#include "AviEncode.h"

/*******************************************************************
// CWinDropTarget - The easy way to make a window a drop target
*******************************************************************/
#include "WinDropTarget.h"

#ifdef ENABLE_WINIMAGE

/*******************************************************************
// CWinImg	- Basically a wrapper for the xImage library
*******************************************************************/
#include "WinImg.h"

#endif

/*******************************************************************
// CWinPrint - Thin wrapper for the Windows printing API
*******************************************************************/
#include "WinPrint.h"

/*******************************************************************
// CWinTimer - Windows timer class
*******************************************************************/
#include "WinTimer.h"

/*******************************************************************
// CXpTheme - Dynamic linking to XP theme features
*******************************************************************/
#include "XpTheme.h"

/*******************************************************************
// CSqExe - Wraps the Squirrel script / SqPlus API (has to be last)
*******************************************************************/
#include "SqExe.h"

#endif RULIB_2002_12_12_INCLUDED
