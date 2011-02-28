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
// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETMSG_H__99948648_47AC_49B8_976D_D3295AC8A9CD__INCLUDED_)
#define AFX_NETMSG_H__99948648_47AC_49B8_976D_D3295AC8A9CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NETMSGDT_DSTADDRESS			0x10000000
#define NETMSGDT_SRCADDRESS			0x20000000
#define NETMSGDT_ECHOADDRESS		0x30000000
#define NETMSGDT_MSG				0x40000000
#define NETMSGDT_PARAMS				0x50000000
#define NETMSGDT_DATA				0x60000000

#define NETMSG_PACKETTYPE			0xfcb9b23f

// {43903EB4-ACE7-4cb5-A5F9-01D0934FB1C4}
static const GUID CLSID_CNETMSG_BROADCAST = 
{ 0x43903eb4, 0xace7, 0x4cb5, { 0xa5, 0xf9, 0x1, 0xd0, 0x93, 0x4f, 0xb1, 0xc4 } };

// {9E3A21B9-1628-4856-9BA1-F487DC0DEE99}
static const GUID CLSID_CNETMSG_LOCALBROADCAST = 
{ 0x9e3a21b9, 0x1628, 0x4856, { 0x9b, 0xa1, 0xf4, 0x87, 0xdc, 0xd, 0xee, 0x99 } };

//==================================================================
// CNetMsg
//
/// Encapsulates network messaging
/**
	This class wraps message data into packets for sending over the
	network layer.  It also decodeds and verifys incomming data and
	prepares the data for dispatching.	
*/
//==================================================================
class CNetMsg : 
		public CDataPacket, 
		public CMessage		
{
public:

	/// Want update notification
	enum { fWantUpdate =	0x80000000 };

	/// Want update notification
	enum { fCancelUpdate =	0x40000000 };

	/// If the item is a reply message
	enum { fReply =			0x08000000 };		

	/// Update notification
	enum { fUpdate =		0x04000000 };

	/// Mask off special flags in command
	enum { fCmdMask =		0x0fffffff };

	/// Mask off all flags
	enum { fMask =			0x00ffffff };
	
	/// This structure contains address and class information
	struct SAddress
	{
		/// The GUID identifying the connecting client node
		GUID				guidNode;

		/// The GUID identifying the connecting module on the client node
		GUID				guidClass;

	}; typedef SAddress* LPSAddress;

	/// This structure carries information that uniquely identifies a client connection
	struct SConnection
	{
		/// The target address
		SAddress			addrNext;

		/// The source address
		SAddress			addrThis;

		/// The address responsible for the connection if any
		SAddress			addrPrev;

	}; typedef SConnection* LPSConnection;

	/// This structure carries information about a particular message.
	/**
		This is mainly to structure the information for transmission.
	*/
	struct SMsg
	{
		/// Function information.  This can be anything.  Use it to speed
		/// message routing in your implementation.  It is quicker to access
		/// than the params.
		DWORD		dwFunction;

	}; typedef SMsg* LPSMsg;

	enum
	{
		eRx = 1,
		eTx
	};

	//==============================================================
	// Address()
	//==============================================================
	/// Returns a pointer to the current address
	GUID* Address() { return &m_guidAddress; }

	//==============================================================
	// operator GUID*()
	//==============================================================
	/// Returns a pointer to the current address
	operator GUID*() { return &m_guidAddress; }
	
public:

	//==============================================================
	// ReadPacket()
	//==============================================================
	/// Reads raw serial data from the incomming data port
	/**
		\param [in] pBuf	-	Raw serial data
		\param [in] dwSize	-	Number of bytes in pBuf
		
		When data is received, such as from a socket, call this function
		to process the data.  De-serializing, message dispatch, etc...
		is all handled by this function.

		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL ReadPacket( LPVOID pBuf, DWORD dwSize );

	//==============================================================
	// VerifyPacket()
	//==============================================================
	/// Returns Non-zero if there is currently a valid packet in the buffer
	virtual BOOL VerifyPacket();
	
	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when new raw data is ready to be written
	/**
		In response to this function, you should read data from the
		transmit buffer ( call Tx() ), and send it to the remote
		connection.  The read data is completely formatted and ready 
		to be processed by the corrisponding ReadPacket() function 
		on the remote connection.
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnWrite();

	/// Default constructor
	CNetMsg();

	/// Destructor
	virtual ~CNetMsg();

	//==============================================================
	// GetConnectionName()
	//==============================================================
	/// Retrieves the connection name (usually ip address or com port)
	LPCTSTR GetConnectionName() { return m_sName; }

	//==============================================================
	// SetConnectionName()
	//==============================================================
	/// Sets the connection name (usually ip address or com port)
	void SetConnectionName( LPCTSTR pName ) { m_sName = pName; }

	//==============================================================
	// GetConnectionId()
	//==============================================================
	/// Returns the connection ID
	const GUID* GetConnectionId() { return &m_guidAddress; }

	//==============================================================
	// GetConnectionId()
	//==============================================================
	/// Returns the connection ID
	/**
		\param [out] pGuid	-	Receives the connection ID
		
		\return 
	
		\see 
	*/
	void GetConnectionId( GUID *pGuid )
	{	if ( pGuid != NULL ) memcpy( pGuid, &m_guidAddress, sizeof( GUID ) ); }

	//==============================================================
	// SetConnectionId()
	//==============================================================
	/// Sets the connection ID
	/**
		\param [in] pGuid	-	Connection ID
	*/
	void SetConnectionId( const GUID *pGuid )
	{	if ( pGuid != NULL ) memcpy( &m_guidAddress, pGuid, sizeof( GUID ) );
		else memcpy( &m_guidAddress, &CLSID_ZERO, sizeof( GUID ) );
	}

	//==============================================================
	// SetConnectionDescription()
	//==============================================================
	/// Sets the connection description
	/**
		\param [in] pDescription	-	Description sent to remote host
	*/
	void SetConnectionDescription( LPCTSTR pDescription )
	{	m_sDescription = pDescription; }

	//==============================================================
	// GetConnectionDescription()
	//==============================================================
	/// Returns the connection description
	/**
		This can be the value set by SetConnectionDescription(), or
		may be the value received from the remote host.  Depends on
		this being a transmitter or receiver.
		
		\return Pointer to the connection description
	
		\see 
	*/
	LPCTSTR GetConnectionDescription() { return m_sDescription; }

private:

	/// Connection address, at higher levels, this can be the Session ID
	GUID					m_guidAddress;

	/// Connection name
	CStr					m_sName;

	/// Connection description
	CStr					m_sDescription;

};

#endif // !defined(AFX_NETMSG_H__99948648_47AC_49B8_976D_D3295AC8A9CD__INCLUDED_)
