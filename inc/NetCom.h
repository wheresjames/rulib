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
// NetCom.h: interface for the CNetCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETCOM_H__EDA81920_9C42_4052_B04D_1CF55AED1FA7__INCLUDED_)
#define AFX_NETCOM_H__EDA81920_9C42_4052_B04D_1CF55AED1FA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CNetCom
//
/// Basic network communication class.
/**
	Binds the receive and transmit portions of the network layer.	
*/
//==================================================================
class CNetCom : public CNetCmd
{
public:

	//==============================================================
	// GetParamsHash()
	//==============================================================
	/// Gets the hash value of last passed parameter list
	/**
		\param [out] pGuid	-	Receives the hash
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetParamsHash( GUID *pGuid );

	//==============================================================
	// VerifyParams()
	//==============================================================
	/// Verifies the hash of the last sent parameter list
	/**
		\param [out] pGuid	-	Receives the hash
		
		\return Non-zero if parameters are valid
	
		\see 
	*/
	BOOL VerifyParams( const GUID *pGuid );

	//==============================================================
	// GetParams()
	//==============================================================
	/// Copies the parameter list from the last received command
	/**
		\param [in] pParams		-	Receives the parameter list
		\param [in] pGuid		-	Receives the parameter list hash
									Can be NULL
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetParams( CReg *pParams, GUID *pGuid = NULL );
	
	//==============================================================
	// NetMsg()
	//==============================================================
	/// Callback function that handles network notifications
	/**
		\param [in] pData	-	CNetCom class pointer
		\param [in] wParam	-	Message WPARAM value
		\param [in] lParam	-	Message LPARAM value
		
		\return Message return value
	
		\see 
	*/
	static long NetMsg( LPVOID pData, WPARAM wParam, LPARAM lParam );
	
	//==============================================================
	// OnNetMsg()
	//==============================================================
	/// Resolved callback function that handles network notifications
	/**
		\param [in] wParam	-	Message WPARAM value
		\param [in] lParam	-	Message LPARAM value
		
		Called by NetMsg()

		\return Message return value
	
		\see NetMsg()
	*/
	long OnNetMsg( WPARAM wParam, LPARAM lParam );
	
	//==============================================================
	// OnRx()
	//==============================================================
	/// Called when new data is received from the network
	/**		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnRx();

	//==============================================================
	// OnTx()
	//==============================================================
	/// Called when the transmit buffer is empty and is able to receive more data
	/**
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnTx() { return FALSE; }

	//==============================================================
	// IsValid()
	//==============================================================
	/// Returns non-zero if this object is valid
	/**
		\warning	Returning zero from this function may cause this
					object to be promptly destroyed by the network
					manager.
	*/
	virtual BOOL IsValid() { return TRUE; }

	//==============================================================
	// CNetCom()
	//==============================================================
	/// Constructs a network communication object
	/**
		\param [in] pRx		-	Receiver messenger object
								NULL for default
		\param [in] pTx		-	Transmiter messenger object
								NULL for default
		
		\return 
	
		\see 
	*/
	CNetCom( CNetMsg* pRx = NULL, CNetMsg* pTx = NULL );

	/// Destructor
	virtual ~CNetCom();

	//==============================================================
	// GetDst()
	//==============================================================
	/// Returns the address of the destination session
	CNetMsg::SAddress& GetDst() { return m_conn.addrNext; }

	//==============================================================
	// GetSrc()
	//==============================================================
	/// Returns the address of the source session
	CNetMsg::SAddress& GetSrc() { return m_conn.addrThis; }

	//==============================================================
	// Rx()
	//==============================================================
	/// Returns a pointer to the receiver messenger
	CNetMsg* Rx() { return m_prx; }

	//==============================================================
	// Tx()
	//==============================================================
	/// Returns a pointer to the transmitter messenger
	CNetMsg* Tx() { return m_ptx; }

	//==============================================================
	// Conn()
	//==============================================================
	/// Returns a reference connection information structure
	CNetMsg::SConnection& Conn() { return m_conn; }

	//==============================================================
	// SMsg()
	//==============================================================
	/// Returns the message information data structure
	CNetMsg::SMsg& SMsg() { return m_msg; }

	//==============================================================
	// GetNode()
	//==============================================================
	/// Return a valid CNetCom*, in this case, the this pointer.
	/**
		\param [in] pGuid	-	Session ID

		Return a pointer to a matching CNetCom object.
		
		\return Pointer to a CNetCom object
	
		\see 
	*/
	virtual CNetCom* GetNode( const GUID *pGuid, DWORD dwTo = TL_LOCK_TIMEOUT ) { return this; }

	//==============================================================
	// FindByRxName()
	//==============================================================
	/// Return a valid CNetCom*, in this case, the this pointer.
	/**
		\param [in] pName	-	Session name

		Return a pointer to a matching CNetCom object.
		
		\return Pointer to a CNetCom object
	
		\see 
	*/
	virtual CNetCom* FindByRxName( LPCTSTR pName ) { return this; }

	//==============================================================
	// FindByTxName()
	//==============================================================
	/// Return a valid CNetCom*, in this case, the this pointer.
	/**
		\param [in] pName	-	Session name

		Return a pointer to a matching CNetCom object.
		
		\return Pointer to a CNetCom object
	
		\see 
	*/
	virtual CNetCom* FindByTxName( LPCTSTR pName ) { return this; }

	//==============================================================
	// OnInitSession()
	//==============================================================
	/// Over-ride to provide custom initialization
	/**
		
		You should always call the base class implementation unless
		you have specific reasons for not doing it.

		\return Non-zero if successful initialization
	
		\see 
	*/
	virtual BOOL OnInitSession();

	//==============================================================
	// Disconnect()
	//==============================================================
	/// Over-ride and implement a disconnect
	/**
		\return	Non-zero if success
	
		\see 
	*/
	virtual BOOL Disconnect() { return FALSE; }

protected:

	/// Delete in destructor if custom type then set to NULL
	CNetMsg					*m_prx;

	/// Delete in destructor if custom type then set to NULL
	CNetMsg					*m_ptx;

private:

	/// Connection information structure
	CNetMsg::SConnection	m_conn;

	/// Message information structure
	CNetMsg::SMsg			m_msg;

};

#endif // !defined(AFX_NETCOM_H__EDA81920_9C42_4052_B04D_1CF55AED1FA7__INCLUDED_)
