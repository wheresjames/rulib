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
// ClientDde.h: interface for the CClientDde class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTDDE_H__0FA0A607_B9E9_4FF3_83DD_8BA3CE93E877__INCLUDED_)
#define AFX_CLIENTDDE_H__0FA0A607_B9E9_4FF3_83DD_8BA3CE93E877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ddeml.h"

//==================================================================
// CClientDde
//
/// Simple wrapper for the Windows DDE API
/**
	This class provides basic Client DDE functionality.
*/
//==================================================================
class CClientDde  
{
public:
	
	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the Windows DDE API
	/**		
		\return Non-zero if success.
	
		\see Uninit()
	*/
	static BOOL Init();

	//==============================================================
	// Uninit()
	//==============================================================
	/// Uninitializes the Windows DDE API
	/**		
		\return 
	
		\see Init()
	*/
	static void Uninit();	

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources for this class
	void Destroy();

	//==============================================================
	// GetData()
	//==============================================================
	///	Returns a pointer to any received data 
	/**		
		\return Pointer to character string.
	
		\see 
	*/
	LPCTSTR GetData();

	//==============================================================
	// PokeString()
	//==============================================================
	/// Sends a string to the connected DDE server
	/**
		\param [in] pName		-	Name of value
		\param [in] pString		-	String data to send
		\param [in] dwTimeout	-	Number of milliseconds to wait for response.
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	BOOL PokeString( LPCTSTR pName, LPCTSTR pString, DWORD dwTimeout = 3000 )
	{	return PokeData( pName, (LPBYTE)pString, strlen( pString ), dwTimeout ); }

	//==============================================================
	// PokeData()
	//==============================================================
	/// Sends binary data to the connected DDE server
	/**
		\param [in] pName		-	Name of value
		\param [in] pData		-	Pointer to buffer containing data
		\param [in] dwSize		-	Size of buffer in pData
		\param [in] dwTimeout	-	Number of milliseconds to wait for response.
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	BOOL PokeData(LPCTSTR pName, LPBYTE pData, DWORD dwSize, DWORD dwTimeout = 3000);

	//==============================================================
	// RequestData()
	//==============================================================
	/// Requests data from DDE server
	/**
		\param [in] pName		-	Name of data to request
		\param [in] dwTimeout	-	Number of milliseconds to wait for response.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RequestData( LPCTSTR pName, DWORD dwTimeout = 3000 );

	//==============================================================
	// Disconnect()
	//==============================================================
	/// Disconnects from remote DDE server
	void Disconnect();

	//==============================================================
	// Connect()
	//==============================================================
	/// Connects to remote DDE server
	/**
		\param [in] pService	-	Name of DDE service
		\param [in] pTopic		-	Name of DDE topic
		
		\return Non-zero if success.
	*/
	BOOL Connect( LPCTSTR pService, LPCTSTR pTopic );

	/// Constructor
	CClientDde();

	/// Destructor
	virtual ~CClientDde();

	//==============================================================
	// DdeCallback()
	//==============================================================
	/// This function is called when DDE server responds with data
	/**
		\param [in] uType	-	Type of callback
		\param [in] uFmt	-	Format of data
		\param [in] hconv	-	Handle to conversation.
		\param [in] hsz1	-	Handle to string
		\param [in] hsz2	-	Handle to string
		\param [in] hdata	-	Handle to data
		\param [in] dwData1	-	DWORD data	
		\param [in] dwData2 -	DWORD data
		
		\return Handle to data
	
		\see 
	*/
#if defined( WIN64 ) || defined( _WIN64 )
	static HDDEDATA CALLBACK DdeCallback(	UINT uType, UINT uFmt, HCONV hconv,
											HSZ hsz1, HSZ hsz2, HDDEDATA hdata,
											ULONG_PTR dwData1, ULONG_PTR dwData2 );
#else
	static HDDEDATA CALLBACK DdeCallback(	UINT uType, UINT uFmt, HCONV hconv,
											HSZ hsz1, HSZ hsz2, HDDEDATA hdata,
											DWORD dwData1, DWORD dwData2 );
#endif

	//==============================================================
	// OnDdeCallback()
	//==============================================================
	/// Resolved function is called when DDE server responds with data
	/**
		\param [in] uType	-	Type of callback
		\param [in] uFmt	-	Format of data
		\param [in] hconv	-	Handle to conversation.
		\param [in] hsz1	-	Handle to string
		\param [in] hsz2	-	Handle to string
		\param [in] hdata	-	Handle to data
		\param [in] dwData1	-	DWORD data	
		\param [in] dwData2 -	DWORD data
		
		\return Handle to data
	
		\see 
	*/
	HDDEDATA OnDdeCallback(	UINT uType, UINT uFmt, HCONV hconv,
							HSZ hsz1, HSZ hsz2, HDDEDATA hdata,
							DWORD dwData1, DWORD dwData2 );

private:

	/// Used to create instance handle
	static DWORD	m_dwInstServer;

	/// Handle to current conversation
	HCONV			m_hConv;

	/// Received data from DDE server
	CStr			m_sData;

};

#endif // !defined(AFX_CLIENTDDE_H__0FA0A607_B9E9_4FF3_83DD_8BA3CE93E877__INCLUDED_)
