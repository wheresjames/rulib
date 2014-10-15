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
// NetCmd.h: interface for the CNetCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCMD_H__FFF599DF_4A5D_4800_87AA_AE2723D58B8C__INCLUDED_)
#define AFX_NETCMD_H__FFF599DF_4A5D_4800_87AA_AE2723D58B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetCom;

namespace NetCmd
{
	// {7DCC3474-6A7E-4059-8D71-4CAC01734A7F}
	static const GUID IID = 
	{ 0x7dcc3474, 0x6a7e, 0x4059, { 0x8d, 0x71, 0x4c, 0xac, 0x1, 0x73, 0x4a, 0x7f } };

	enum
	{
		/// Session connected
		efConnect = 1,		

		/// Session disconnected
		efDisconnect,

		/// New session has been added
		efNodeAdded,

		/// Session has been removed
		efNodeRemoved,

		/// Request for session connection information
		efGetConnectionInfo,

		/// Tell CNetMgr to cleanup unused connections
		efDoCleanup
	};

}

//==================================================================
// CNetCmd
//
/// Provides a network command interface.
/**
	Formats commands for sending over the network layer.	
*/
//==================================================================
class CNetCmd : public CMessageList
{

public:

	//==============================================================
	// LocalMsg()
	//==============================================================
	/// Sends a message to local network objects
	/**
		\param [in] pguidClass	-	Class ID
		\param [in] dwFunction	-	Function ID
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL LocalMsg( const GUID *pguidClass, DWORD dwFunction );

	//==============================================================
	// NotifyDisconnect()
	//==============================================================
	/// Call to notify the network when this node dissconnects
	/**
		\param [in] lError	-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL NotifyDisconnect( long lError );

	//==============================================================
	// NotifyConnect()
	//==============================================================
	/// Call this function to notify the network when this node connects, or fails to connect
	/// 
	/**
		\param [in] lError	-	Error code, zero if no error
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL NotifyConnect( long lError );

	/// Default constructor
	CNetCmd();

	/// Desturctor
	virtual ~CNetCmd();

	//==============================================================
	// GetNode()
	//==============================================================
	/// Over-ride this function and return a valid CNetCom object
	/**
		\param [in] pGuid	-	Network session ID
		
		\return Pointer to valid CNetCom object or NULL if not found
	
		\see 
	*/
	virtual CNetCom* GetNode( const GUID *pGuid, DWORD dwTo = TL_LOCK_TIMEOUT ) = 0;

	//==============================================================
	// FindByRxName()
	//==============================================================
	/// Over-ride this function and return a valid CNetCom object
	/**
		\param [in] pName	-	Network session receiver (remote) name
		
		\return Pointer to valid CNetCom object or NULL if not found
	
		\see 
	*/
	virtual CNetCom* FindByRxName( LPCTSTR pName ) = 0;

	//==============================================================
	// FindByTxName()
	//==============================================================
	/// Over-ride this function and return a valid CNetCom object
	/**
		\param [in] pName	-	Network session transmitter (local) name
		
		\return Pointer to valid CNetCom object or NULL if not found
	
		\see 
	*/
	virtual CNetCom* FindByTxName( LPCTSTR pName ) = 0;

	//==============================================================
	// vMsg()
	//==============================================================
	/// For sending messages to the connected node
	/**
		\param [in] pNode		-	Session ID
		\param [in] pClass		-	Class ID
		\param [in] dwFunction	-	Function ID
		\param [in] pParams		-	Parameter list
		\param [in] dwBuffers	-	Number of buffer arguments
		\param [in] pArgs 		-	Pointer to buffer arguments
		
		pArgs = ( DWORD dwType, LPBYTE pPtr, DWORD dwSize, ... )

			- <b>dwType</b>	=	Data buffer type
			- <b>pPtr</b>	=	Pointer to data buffer
			- <b>dwSize</b>	=	Number of bytes in pPtr

		\return Non-zero if success
	
		\see 
	*/
	BOOL vMsg( const GUID *pNode, const GUID *pClass, DWORD dwFunction = 0, CReg *pParams = NULL, DWORD dwBuffers = 0, ruVaList pArgs = 0 );

	//==============================================================
	// Msg()
	//==============================================================
	/// For sending messages to the connected node
	/**
		\param [in] pNode		-	Session ID
		\param [in] pClass		-	Class ID
		\param [in] dwFunction	-	Function ID
		\param [in] pParams		-	Parameter list
		\param [in] dwBuffers	-	Number of buffer arguments
		\param [in] ...
		
		... = ( DWORD dwType, LPBYTE pPtr, DWORD dwSize, ... )

			- <b>dwType</b>	=	Data buffer type
			- <b>pPtr</b>	=	Pointer to data buffer
			- <b>dwSize</b>	=	Number of bytes in pPtr

		\return Non-zero if success
	
		\see 
	*/
	BOOL Msg( const GUID *pNode, const GUID *pClass, DWORD dwFunction = 0, CReg *pParams = NULL, DWORD dwBuffers = 0, ... );

	//==============================================================
	// OnRxAuthenticate()
	//==============================================================
	/// Over-ride to supply custom authentication
	/**
		\param [in] pguidNode		-	Session ID
		\param [in] pguidClass		-	Class ID
		\param [in] dwFunction		-	Function
		
		\return Non-zero to allow connection
	
		\see 
	*/
	virtual BOOL OnRxAuthenticate( const GUID *pguidNode, const GUID *pguidClass, DWORD dwFunction ) 
	{	return TRUE; }

	//==============================================================
	// OnTxAuthenticate()
	//==============================================================
	/// Over-ride to supply custom authentication
	/**
		\param [in] pguidNode		-	Session ID
		\param [in] pguidClass		-	Class ID
		\param [in] dwFunction		-	Function
		
		\return Non-zero to allow connection
	
		\see 
	*/
	virtual BOOL OnTxAuthenticate( const GUID *pguidNode, const GUID *pguidClass, DWORD dwFunction ) 
	{	return TRUE; }

	//==============================================================
	// NotifyNetwork()
	//==============================================================
	/// Call this function to notify the network of a change
	/**
		\param [in] pguidClass		-	Class ID that has changed
		\param [in] dwCmd			-	Command ID that has changed
		
		This informs any class that requested change notifications that
		a change to the specified value has taken place.  It is then
		up to that class to query the new value or not.

		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL NotifyNetwork( const GUID *pguidClass, DWORD dwCmd ) { return TRUE; }

	//==============================================================
	// GetNetError()
	//==============================================================
	/// Returns the last network error value
	long GetNetError() { return m_lError; }

	//==============================================================
	// SetNetError()
	//==============================================================
	/// Sets the last network error value
	void SetNetError( long lError ) { m_lError = lError; }

	//==============================================================
	// ClearNetError()
	//==============================================================
	/// Clears the last network error value
	void ClearNetError() { SetNetError( 0 ); }

protected:

	/// The last network error value
	long					m_lError;

private:

};

#endif // !defined(AFX_NETCMD_H__FFF599DF_4A5D_4800_87AA_AE2723D58B8C__INCLUDED_)
