/*******************************************************************
// Copyright (c) 2000, Robert Umbehant
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
// Win32.h: interface for the CWin32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN32_H__B54269B4_47CD_479A_AEF0_05C9466D1C3E__INCLUDED_)
#define AFX_WIN32_H__B54269B4_47CD_479A_AEF0_05C9466D1C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VER_FULL( maj, min, build ) ( ( ( maj & 0xff ) << 24 ) | ( ( min & 0xff ) << 16 ) | ( build & 0xffff ) )
#define VER_MAJOR( ver ) ( ( ver >> 24 ) & 0xff )
#define VER_MINOR( ver ) ( ( ver >> 16 ) & 0xff )
#define VER_BUILD( ver ) ( ver & 0xffff )

/// Information about the current machine
/**
	\see CWin32
*/
typedef struct tagACTIVESYSPARAMS
{
	double			cpu;

	BOOL			bValid;
	double			oldIdle; 
	double			oldSys;

	DWORD			dwPageSize;
	DWORD			dwTotalPhysicalPages;
	DWORD			dwAllocGranularity;
	DWORD			dwProcessors;

	LARGE_INTEGER	liIdleTime;
	LARGE_INTEGER	liSystemTime;

} ACTIVESYSPARAMS, *LPACTIVESYSPARAMS;

#ifndef SHACF_DEFAULT
#define SHACF_DEFAULT                   0x00000000
#define SHACF_FILESYSTEM                0x00000001
#define SHACF_URLALL                    (SHACF_URLHISTORY | SHACF_URLMRU)
#define SHACF_URLHISTORY                0x00000002
#define SHACF_URLMRU                    0x00000004
#define SHACF_USETAB                    0x00000008
#define SHACF_FILESYS_ONLY              0x00000010

#define SHACF_AUTOSUGGEST_FORCE_ON      0x10000000
#define SHACF_AUTOSUGGEST_FORCE_OFF     0x20000000
#define SHACF_AUTOAPPEND_FORCE_ON       0x40000000
#define SHACF_AUTOAPPEND_FORCE_OFF      0x80000000
#endif


//==================================================================
// CWin32
//
/// Generic Windows functions
/**
	This class provides a few generic Windows functions.  These are
	provided through static functions so there is no need to create
	an instance of this class.
*/
//==================================================================
class CWin32  
{
public:

	/// Contains information about a Windows service
	typedef struct __WINDOWS_SERVICE_STATUS 
	{
		/// Service type
		DWORD dwServiceType; 

		/// Current execution state of the service
		DWORD dwCurrentState; 

		/// Control codes that are accepted
		DWORD dwControlsAccepted; 

		/// Service startup and/or shutdown error code
		DWORD dwWin32ExitCode; 

		/// Error code causing service error
		DWORD dwServiceSpecificExitCode; 

		/// Service increments during lengthy opertaions to show it's still working.
		DWORD dwCheckPoint; 

		/// Estimated amount of time a pending operation will take in milliseconds.
		DWORD dwWaitHint; 

	} WINDOWS_SERVICE_STATUS, *LPWINDOWS_SERVICE_STATUS;


public:


	//==============================================================
	// PopupMenu()
	//==============================================================
	/// Shows a popup menu
	/**
		\param [in] x			-	Horizontal offset for popup menu
		\param [in] y			-	Vertical offset for popup menu
		\param [in] pMenu		-	Menu resource ID
		\param [in] hWnd		-	Handle to window that receives
									the menu commands.
		\param [in] hInstance	-	Application instance handle
									containing the popup menu resource.
		\param [in] lSub		-	If >= 0, specifies the sub menu
									index to use.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL PopupMenu( long x, long y, LPCTSTR pMenu, HWND hWnd, HMODULE hInstance, long lSub = -1 );

	//==============================================================
	// ControlService()
	//==============================================================
	/// Executes specified command on named service
	/**
		\param [in] pServiceName	-	Service name
		\param [in] dwControl		-	Service control function
										- SERVICE_CONTROL_STOP -
											Stops the service
										- SERVICE_CONTROL_PAUSE -
											Pauses the service
										- SERVICE_CONTROL_CONTINUE -
											Continues a paused service
										- SERVICE_CONTROL_INTERROGATE -
											Retrieves service status
											information in pStatus.											
										- SERVICE_CONTROL_SHUTDOWN -
											Function fails if this
											command is specified.								
											
		\param [in] pStatus			-	Receives the latest service 
										status information.  Can be
										NULL.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ControlService( LPCTSTR pServiceName, DWORD dwControl, LPWINDOWS_SERVICE_STATUS pStatus = NULL );

	//==============================================================
	// StartService()
	//==============================================================
	/// Starts the specified service
	/**
		\param [in] pServiceName	-	Name of service
		
		\return Non-zero if service was started
	
		\see 
	*/
	static BOOL StartService( LPCTSTR pServiceName );

	//==============================================================
	// SearchWindow()
	//==============================================================
	/// Searches for a particular window, allows wild cards
	/**
		\param [in] pClass	-	Class name to find or NULL for any
		\param [in] pName	-	Window text to find or NULL for any
		\param [in] hRoot	-	Root window, NULL for desktop 
		\param [in] bChild	-	Non-zero to search child windows
		
		\note It would be nice to have a way to continue the search for
		other matching windows.

		\return Handle of first window found
	
		\see 
	*/
	static HWND SearchWindow( LPCTSTR pClass, LPCTSTR pName, HWND hRoot = NULL, BOOL bChild = FALSE );

	//==============================================================
	// StringToGuid()
	//==============================================================
	/// Converts a string GUID to binary form
	/**
		\param [in] pGuid		-	Buffer that receives GUID
		\param [in] pString		-	String to convert
		
		\warning Must match the <b>exact</b> format of 
		XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX or function will fail.

		\return Value in pGuid or NULL if failure
	
		\see 
	*/
	static const GUID * StringToGuid( GUID *pGuid, LPCTSTR pString );

	//==============================================================
	// GuidToString()
	//==============================================================
	/// Converts a binary GUID into string representation
	/**
		\param [in] pStr	-	Receives GUID
		\param [in] pGuid	-	Binary GUID

		if pGuid is NULL, a unique GUID is returned in pStr generated
		by calling CoCreateGUID().
		
		\return Value in pStr or NULL if failure
	
		\see 
	*/
	static LPCTSTR GuidToString( LPSTR pStr, const GUID *pGuid = NULL );

	//==============================================================
	// GetWSAErrorMsg()
	//==============================================================
	/// Creates an error string for Windows Socket error codes
	/**
		\param [in] pMsg	-	Receives human readable error string
		\param [in] err		-	Windows Socket error code
		
		\return Non-zero if valid socket error code.
	
		\see 
	*/
	static BOOL GetWSAErrorMsg(LPSTR pMsg, DWORD err);

	//==============================================================
	// ShowError()
	//==============================================================
	/// Shows a dialog box displaying information about a Windows error.
	/**
		\param [in] hParent		-	Parent window for dialog box
		\param [in] dwError		-	Windows or Socket error code.
		\param [in] pTitle		-	Dialog box title
		\param [in] pAdd		-	String to add to the end of the message.
		\param [in] dwType		-	Type of dialog box to display
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ShowError( HWND hParent, DWORD dwError, LPCTSTR pTitle, LPCTSTR pAdd = NULL, DWORD dwType = MB_OK | MB_ICONEXCLAMATION );

	//==============================================================
	// GetSystemErrorMsg()
	//==============================================================
	/// Returns information about a Windows or Socket error code
	/**
		\param [in] dwErr		-	Error code
		\param [in] pMsg		-	Receives the error code string
		\param [in] pTemplate	-	Template that defines the message format.
		
		pTemplate is passed to wsprintf to format the error string.
		The default template is "%lu ( 0x%lX ) : %s".

		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetSystemErrorMsg( DWORD dwErr, LPSTR pMsg, LPCTSTR pTemplate = NULL );

	//==============================================================
	// AutoComplete()
	//==============================================================
	/// Enables web address auto complete on the specified edit box
	/**
		\param [in] hTarget		-	Edit box handle
		\param [in] dwFlags		-	Auto complete flags
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL AutoComplete(HWND hTarget, DWORD dwFlags);

	//==============================================================
	// GetSysInfo()
	//==============================================================
	/// Returns information about the system state
	/**
		\param [in] pAsp	-	Receives system state information
		
		\return Non-zero if success
	
		\see ACTIVESYSPARAMS
	*/
	static BOOL GetSysInfo( LPACTIVESYSPARAMS pAsp );

	//==============================================================
	// GetCpuLoad()
	//==============================================================
	/// Returns the current CPU load
	static double GetCpuLoad();
	
	//==============================================================
	// SetTransparency()
	//==============================================================
	/// Sets the transparency color key of a layerd window
	/**
		\param [in] hWnd			-	Window handle
		\param [in] rgbTransparent	-	Transparent color key
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetTransparency( HWND hWnd, COLORREF rgbTransparent );

	//==============================================================
	// SetTranslucency()
	//==============================================================
	/// Sets the translucency of a window
	/**
		\param [in] hWnd	-	Window handle
		\param [in] dwTl	-	Level of translucency ( 0 - 255 )		

		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetTranslucency( HWND hWnd, DWORD dwTl );

	//==============================================================
	// GetIEVersion()
	//==============================================================
	/// Returns the version of IE that is installed
	static DWORD GetIEVersion();

	//==============================================================
	// RunModalLoop()
	//==============================================================
	/// Creates a modal loop
	/**
		\param [in] hWnd	-	Window handle
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL RunModalLoop( HWND hWnd );

	//==============================================================
	// StartNewApp()
	//==============================================================
	/// Starts a new instance of this application
	/**
		\return Non-zero if success
	*/
	static BOOL StartNewApp();

	//==============================================================
	// Reboot()
	//==============================================================
	///	Reboots Windows
	/**
		\return Non-zero if success
	*/
	static BOOL Reboot();

	//==============================================================
	// ahtoudw()
	//==============================================================
	/// Converts ASCII hex value to DWORD
	/**
		\param [in] pdwNum			-	Receives DWORD value
		\param [in] pBuffer			-	Buffer containing ASCII hex string
		\param [in] ucBytes			-	Bytes in pBuffer to convert
		\param [in] bSkipInvalid	-	Non-zero to skip invalid characters
		
		if bSkipInvalid is set to zero and an invalid character is found,
		the function aborts the conversion and returns zero.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ahtoudw( LPDWORD pdwNum, LPCTSTR pBuffer, BYTE ucBytes, BOOL bSkipInvalid = FALSE );

	//==============================================================
	// RedrawWindow()
	//==============================================================
	/// Redraws the specified window
	/**
		\param [in] hWnd	-	Handle to window
		\param [in] pRect	-	Area of window to redraw, NULL for all
		\param [in] hRgn	-	Region of window to redraw, NULL for all
		\param [in] uFlags	-	Redraw flags
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL RedrawWindow(	HWND hWnd, const RECT *pRect = NULL,
								HRGN hRgn = NULL,
								UINT uFlags =	RDW_ERASE | RDW_INVALIDATE |
												RDW_ERASENOW | RDW_UPDATENOW |
												RDW_ALLCHILDREN )
	{	if ( !hWnd || !::IsWindow( hWnd ) ) return FALSE;
		return ::RedrawWindow( hWnd, pRect, hRgn, uFlags ); 
	}


	//==============================================================
	// Md5Hash()
	//==============================================================
	/// Quick way to create a GUID from a data buffer
	/**
		\param [in] pGuid	-	Receives the GUID
		\param [in] pBuf	-	Buffer containing data to hash
		\param [in] dwSize	-	Number of bytes in pBuf.  If zero,
								a NULL terminated string is assumed.
		
		\return Value in pGuid
	*/
	static const GUID* Md5Hash( GUID *pGuid, LPVOID pBuf, DWORD dwSize )
	{	CMd5Rsa::MD5( (LPBYTE)pGuid, (LPBYTE)pBuf, dwSize );
		return pGuid;
	}

	//==============================================================
	// Md5Hash()
	//==============================================================
	/// Quick way to create a GUID from a string
	/**
		\param [in] pGuid	-	Recieves the new GUID
		\param [in] pStr	-	NULL terminated string to hash.
		
		\return Value in pGuid
	*/
	static const GUID* Md5Hash( GUID *pGuid, LPCTSTR pStr )
	{	CMd5Rsa::MD5( (LPBYTE)pGuid, (LPBYTE)pStr, 0 );
		return pGuid;
	}

	//==============================================================
	// GetInstanceHandle()
	//==============================================================
	/// Returns the instance handle for the current module
    /*
        This is correct even for DLL's
    */
    static HMODULE GetInstanceHandle();

	//==============================================================
	// sMD5_guid()
	//==============================================================
	/// Retuns MD5 hash of the input string in GUID format
	/**
		\param [in] pStr		-	String to hash
	*/
	static std::string sMD5_guid(LPCTSTR pStr)
	{	if ( !pStr || !*pStr ) return "";
		GUID guidId;
		char szId[ 1024 ] = { 0 };
		CMd5Rsa::MD5( (LPBYTE)&guidId, pStr, strlen( pStr ) );
		CWin32::GuidToString( szId, &guidId );			
		return szId;
	}

	//==============================================================
	// sMD5_guid()
	//==============================================================
	/// Retuns MD5 hash of the input string in base 64 format
	/**
		\param [in] pStr		-	String to hash
	*/
	static std::string sMD5_base64(LPCTSTR pStr)
	{	if ( !pStr || !*pStr ) return "";
		GUID guidId;
		char szId[ 1024 ] = { 0 };
		CMd5Rsa::MD5( (LPBYTE)&guidId, pStr, strlen( pStr ) );
		CBase64 b64;
		szId[ b64.Encode( szId, sizeof( szId ) - 1, (LPBYTE)&guidId, sizeof( guidId ) ) ] = 0;
		return szId;
	}

	/// Constructor
	CWin32();

	/// Destructor
	virtual ~CWin32();

};

#endif // !defined(AFX_WIN32_H__B54269B4_47CD_479A_AEF0_05C9466D1C3E__INCLUDED_)
