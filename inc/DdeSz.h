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
// DdeSz.h: interface for the CDdeSz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DDESZ_H__D2003099_A300_4D4E_9A58_F36D926DE091__INCLUDED_)
#define AFX_DDESZ_H__D2003099_A300_4D4E_9A58_F36D926DE091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ddeml.h"

//==================================================================
// CDdeSz
//
/// DDE string wrapper
/**
	This class provides a simple wrapper for a DDE string value.	
*/
//==================================================================
class CDdeSz  
{
public:

	/// Default Constructor
	CDdeSz( DWORD dwInst, LPCTSTR pStr )
	{	m_dwInst = NULL; m_hSz = NULL; CreateSz( dwInst, pStr ); }

	/// Constructor
	CDdeSz(){ m_dwInst = NULL; m_hSz = NULL; }

	/// Destructor
	virtual ~CDdeSz(){ FreeSz(); }

	//==============================================================
	// CreateSz()
	//==============================================================
	/// Allocates a DDE string
	/**
		\param [in] dwInst	-	DDE instance	
		\param [in] pStr	-	Initial string value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateSz( DWORD dwInst, LPCTSTR pStr )
	{	FreeSz(); m_dwInst = dwInst;
		m_hSz = DdeCreateStringHandle( dwInst, pStr, CP_WINANSI );
		if ( m_hSz == NULL ) { m_dwInst = NULL; return FALSE; }
		return TRUE;
	}

	//==============================================================
	// FreeSz()
	//==============================================================
	/// Frees the previously allocated DDE string
	void FreeSz()
	{	if ( m_dwInst != NULL && m_hSz != NULL )
			DdeFreeStringHandle( m_dwInst, m_hSz );
		m_dwInst = NULL; m_hSz = NULL;
	}

	//==============================================================
	// HSZ()
	//==============================================================
	/// Returns pointer to DDE string handle
	operator HSZ() { return m_hSz; }

	/// DDE instance
	DWORD		m_dwInst;

	/// DDE string handle
	HSZ			m_hSz;

};

#endif // !defined(AFX_DDESZ_H__D2003099_A300_4D4E_9A58_F36D926DE091__INCLUDED_)
