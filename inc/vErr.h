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
// vErr.h: interface for the CvErr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERR_H__B1EE181E_0C8A_493F_8B03_62BE970C7CA9__INCLUDED_)
#define AFX_VERR_H__B1EE181E_0C8A_493F_8B03_62BE970C7CA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _vLOGOBJ		( CvErr::m_errlog == NULL ? *( CvErr::m_errlog = _PTR_NEW CErrLog ) : *CvErr::m_errlog )
#define _vLOG			_LOGTO( _vLOGOBJ )
#define _vDUMPLOG()		CvErr::CreateErrorLog( FALSE, FALSE )
#define _vTESTCRASH()	CvErr::CreateErrorLog( TRUE, TRUE )
#define _vLOGDEL()		( _PTR_DELETE( CvErr::m_errlog ) )

//==================================================================
// CvErr
//
/// Provides error logging interface
/**
	Provides error logging	
*/
//==================================================================
class CvErr  
{
public:

	//==============================================================
	// Succeeded()
	//==============================================================
	/// Call to log success
	/**						
		\param [in] hr		-	Error code
		
		\return 
	
		\see 
	*/
	static HRESULT Succeeded( HRESULT hr );

	//==============================================================
	// Failed()
	//==============================================================
	/// Call to log failure
	/**
		\param [in] hr		-	Error code
		
		\return 
	
		\see 
	*/
	static HRESULT Failed( HRESULT hr );

	/// Constructor
	CvErr();

	/// Destructor
	virtual ~CvErr();

	//==============================================================
	// ExceptionHandler()
	//==============================================================
	/// Windows Exception API callback function
	/**
		\param [in] ExceptionInfo	-	Exception information structure
	*/
	static LONG WINAPI ExceptionHandler( struct _EXCEPTION_POINTERS *ExceptionInfo );

	//==============================================================
	// CreateErrorLog()
	//==============================================================
	/// Creates error logs
	/**
		\param [in] bCrash	-	Non-zero if you want crash logs written
		\param [in] bTest	-	Non-zero to flag as test
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL CreateErrorLog( BOOL bCrash, BOOL bTest );

	//==============================================================
	// SetDefaultExceptionHandler()
	//==============================================================
	/// Call to use the default error and crash reporting
	/**		
		\return Non-zero if handler was installed 
	
		\see 
	*/
	static BOOL SetDefaultExceptionHandler();

public:
	
	/// Static error log instance
	static CErrLog		*m_errlog;

};

#endif // !defined(AFX_VERR_H__B1EE181E_0C8A_493F_8B03_62BE970C7CA9__INCLUDED_)
