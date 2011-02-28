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
// StackReport.h: interface for the CStackReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKREPORT_H__57E699D4_12C6_4216_BA1D_0245D27BFAD5__INCLUDED_)
#define AFX_STACKREPORT_H__57E699D4_12C6_4216_BA1D_0245D27BFAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CStackReport
//
/// Generates a stack report
/**
	This class contains a few functions to generate a stack report
	from the specified CStackTrace object.	
*/
//==================================================================
class CStackReport  
{
public:

	/// Default Constructor
	CStackReport();

	/// Destructor
	virtual ~CStackReport();

	//==============================================================
	// CallStack()
	//==============================================================
	/// Generate a stack report
	/**
		\param [out] pReg	-	Receives the stack report
		\param [in] pSt		-	CStackTrace object pointer
		
		Upon return, the CReg object will contain a key entry for 
		each thread, and the current call stack for each thread will 
		be in the key value fields in order of most recently called 
		functions first.

		\return Non-zero if successful
	
		\see 
	*/
	static BOOL CallStack( CReg *pReg, CStackTrace *pSt );

	//==============================================================
	// History()
	//==============================================================
	/// Generate a stack history report
	/**
		\param [out] pReg	-	Receives the stack trace
		\param [in] pSt		-	CStackTrace object pointer
		
		Upon return, the CReg object will contain a key entry for 
		each thread, and the current stack trace for each thread will 
		be in the key value fields in order of most recently called 
		functions first.

		The stack trace is just the names of the last n number of 
		functions called.

		\return Non-zero if successful
	
		\see 
	*/
	static BOOL History( CReg *pReg, CStackTrace *pSt );

	//==============================================================
	// Profile()
	//==============================================================
	/// Call to generate a stack profile report
	/**
		\param [out] pReg	-	Receives the stack trace
		\param [in] pSt		-	CStackTrace object pointer
		
		Upon return, the CReg object will contain a key entry for 
		each thread, and the current stack profile for each thread 
		will be in the key value fields in order of most recently
		called functions first.

		The stack profile contains the name of each function as
		the key entry name, and the total amount of time spent in 
		that function as the key entry value.
		
		\return Non-zero if successful
	
		\see 
	*/
	static BOOL Profile( CReg *pReg, CStackTrace *pSt );


	//==============================================================
	// ErrorLog()
	//==============================================================
	/// Call to generate a stack profile report
	/**
		\param [out] pReg	-	Receives the stack trace
		\param [in] pErrLog	-	CErrLog object pointer
		
		Upon return, the CReg object will contain a key entry for 
		each error and log each errors message and time.
		
		\return Non-zero if successful
	
		\see 
	*/
	static BOOL ErrorLog(CReg *pReg, CErrLog* pErrLog ); // wjr 11/10/06


};

#endif // !defined(AFX_STACKREPORT_H__57E699D4_12C6_4216_BA1D_0245D27BFAD5__INCLUDED_)
