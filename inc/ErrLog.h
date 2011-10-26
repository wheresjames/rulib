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
// ErrLog.h: interface for the CErrLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRLOG_H__3FA5ABA7_742C_4EB1_923F_CBEB1EFF5A9D__INCLUDED_)
#define AFX_ERRLOG_H__3FA5ABA7_742C_4EB1_923F_CBEB1EFF5A9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Params
// Add( __FILE__, __LINE__, <severity>, <error code>, <description> );

// Actual call
// _vLOG( _ES_NOTICE, -1, "Unable to create AVI" );

#define _ERRINF					__FILE__, __LINE__, __FUNCTION__

#define _ES_NOTICE				_ERRINF, 0x00000001
#define _ES_WARNING				_ERRINF, 0x00000002
#define _ES_ERROR				_ERRINF, 0x00000003

#define _LOGTO( obj )			( obj ).Add

// return CErrLog::Add( _ERRINF, code, str, ... )

/// Error item information structure
typedef struct tagERRORITEMINFO : LLISTINFO
{
	/// String containing source file that generated the error
	CStr				*sFile;

	/// String containing the name of the function that generated the error
	CStr				*sFunction;

	/// Line number that generated the error
	DWORD				dwLine;

	/// Severity of error
	DWORD				dwSeverity;

	/// Error code
	DWORD				dwCode;

	/// Unix time of error
	DWORD				dwTime;

} ERRORITEMINFO, *LPERRORITEMINFO; // end typedef struct

//==================================================================
// CErrLog
//
/// This class provides error logging functionality
/**
	This class provides error logging functionality.  Use the error
	macros _vLog() to log each error event.	
*/
//==================================================================
class CErrLog : public CLList  
{

public:

	/// Constructor
	CErrLog();

	/// Destructor
	virtual ~CErrLog();

public:

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources
	virtual void Destroy();

	//==============================================================
	// InitObject()
	//==============================================================
	/// Initializes an error object
	/**
		\param [in] node	-	LPERRORITEMINFO to be initialized.
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL InitObject( void *node );

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Release any resources assigned to element
	/**
		\param [in] node	-	LPERRORITEMINFO to be initialized.
	*/
	virtual void DeleteObject( void *node );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Return the size of an individual error element.
	virtual DWORD GetObjSize() { return sizeof( ERRORITEMINFO ); }

	//==============================================================
	// Add()
	//==============================================================
	/// Adds error information to the list
	/**
		\param [in] pFile			-	The filename that generated the error
		\param [in] dwLine			-	Line number of the error
		\param [in] pFunction		-	Funtion name that generated error
		\param [in] dwSeverity		-	Severity level of the error
		\param [in] dwErrCode		-	Windows error code if any
		\param [in] pStr			-	String describing what caused the error.
		
		\return dwErrCode is returned
	
		\see 
	*/
	HRESULT _cdecl Add(	LPCTSTR pFile, DWORD dwLine, LPCTSTR pFunction,
						DWORD dwSeverity, DWORD dwErrCode, LPCTSTR pStr, ... );

};

#endif // !defined(AFX_ERRLOG_H__3FA5ABA7_742C_4EB1_923F_CBEB1EFF5A9D__INCLUDED_)
