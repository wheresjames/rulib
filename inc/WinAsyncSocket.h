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
// WinAsyncSocket.h: interface for the CWinAsyncSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINASYNCSOCKET_H__D37E3AA9_BAFC_464A_AFCD_1CA920633931__INCLUDED_)
#define AFX_WINASYNCSOCKET_H__D37E3AA9_BAFC_464A_AFCD_1CA920633931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

//==================================================================
// CWinAsyncSocket
//
/// Provides asynchronous socket communications.
/**
	This class uses CWinSocket and provides a thread to handle
	socket data.	

	You can derive from this class or more preferably, derive from
	CWinSocketProtocol and provide custom protocol handling.  This
	scheme works well for both TCP and UDP based protocols.

	\see TWinAsyncSocket, CWinSocketProtocol, CWinSocket
*/
//==================================================================
class CWinAsyncSocket : 
	private CThread, 
	public CMessageList,
	public CTlLock
{
public:

	enum
	{
		/// Invalid command ID
		eCmdNone = 0,

		/// Open connection
		eCmdOpen,

		/// Close connection
		eCmdClose,

	};

	enum
	{
		/// Socket is not connected
		eStatusDisconnected = 0,

		/// Socket is connecting
		eStatusConnecting,

		/// Socket is connected
		eStatusConnected
	};

	/// This structure holds information about a thread command
	/**
		\see CWinAsyncSocket
	*/
	struct SCmdData
	{
		/// Command ID
		UINT			uCmd;

		/// Command UINT data
		UINT			uParam1;

		/// Length of data following this command
		UINT			uDataLen;

	}; // end stuct

public:

	/// Default constructor
	CWinAsyncSocket();

	/// Destructor
	virtual ~CWinAsyncSocket();

	//==============================================================
	// InitThread()
	//==============================================================
	/// Initializes the thread
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero to continue the thread
	
		\see 
	*/
	virtual BOOL InitThread( LPVOID pData );

	//==============================================================
	// DoThread()
	//==============================================================
	/// Processes socket information
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero to continue the thread
	
		\see 
	*/
	virtual BOOL DoThread( LPVOID pData );

	//==============================================================
	// EndThread()
	//==============================================================
	/// Provides thread cleanup
	/**
		\param [in] pData	-	Reserved
		
		\return Ignored
	
		\see 
	*/
	virtual BOOL EndThread( LPVOID pData );

protected:

	//==============================================================
	// OnCommand()
	//==============================================================
	/// Processes a command from the command queue
	/**
		\return Non-zero if success
	
		\see 
	*/
	BOOL OnCommand();

	//==============================================================
	// OnSocketEvent()
	//==============================================================
	/// Processes socket events
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL OnSocketEvent();

public:

	//==============================================================
	// Connect()
	//==============================================================
	/// Connects the socket to the specified address
	/**
		\param [in] pIpAddress	-	Remote address
		\param [in] uPort		-	Connection port
		
		\return Non-zero if command was queued
	
		\see Close()
	*/
	BOOL Connect( LPCTSTR pIpAddress, UINT uPort );
	
	//==============================================================
	// Close()
	//==============================================================
	/// Closes the socket
	/**		
		\return Non-zero if command was queued
	
		\see Connect()
	*/
	BOOL Close();

	//==============================================================
	// GetSocket()
	//==============================================================
	/// Returns a pointer to the encapsulated CWinSocket class
	CWinSocket* GetSocket() { return &m_ws; }

public:

	//==============================================================
	// OnRead()
	//==============================================================
	/// Called when data has been received from connected socket
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnRead( int nErr ) { return FALSE; }

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when previously full transmit buffer is now empty.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnWrite( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnOOB()
	//==============================================================
	/// Called when Out-Of-Band data has been received from connected socket
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnOOB( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnAccept()
	//==============================================================
	/// Called when an incomming connection request is recieved.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		You should call Accept in response to this callback.

		\return Return non-zero if handled
	*/
	virtual BOOL OnAccept( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnConnect()
	//==============================================================
	/// Called when the socket has connection has completed, or failed.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnConnect( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnClose()
	//==============================================================
	/// Called when socket connection has been closed or aborted.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnClose( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnQOS()
	//==============================================================
	/// Called when Quality-Of-Service parameters has changed
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnQOS( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnGroupQOS()
	//==============================================================
	/// Called when group Quality-Of-Service parameters has changed
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnGroupQOS( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnRoutingInterfaceChange()
	//==============================================================
	/// Called when the routing interface parameters have changed.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnRoutingInterfaceChange( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnAddressListChange()
	//==============================================================
	/// Called when the multi-cast address list has changed
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnAddressListChange( int nErr ) { return FALSE; }
	
	//==============================================================
	// OnError()
	//==============================================================
	/// Called on socket error.
	/**
		\param [in] wEvent	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnError( WORD wEvent ) { return FALSE; }

	//==============================================================
	// GetDataReadyHandle()
	//==============================================================
	/// Return a valid handle if you want to receive data tx messages
	/**
		
		\return Waitable event handle
	
		\see 
	*/
	virtual HANDLE GetDataReadyHandle() { return NULL; }

	//==============================================================
	// OnDataReady()
	//==============================================================
	/// Called when the event handle returned by GetDataReadyHandle() is set
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnDataReady() { return FALSE; }

	//==============================================================
	// GetStatus()
	//==============================================================
	/// Gets the socket connection status
	UINT GetStatus() { return m_uStatus; }

	//==============================================================
	// IsConnected()
	//==============================================================
	/// Returns non-zero if the socket is connected
	BOOL IsConnected() { return eStatusConnected == m_uStatus; }

	//==============================================================
	// IsConnecting()
	//==============================================================
	/// Returns non-zero if the socket is connecting
	BOOL IsConnecting() { return eStatusConnecting == m_uStatus; }


private:

	/// Windows socket class
	CWinSocket					m_ws;

	/// Thread sync command buffer
	CFifoSync					m_fifo;

	/// Connection status
	UINT						m_uStatus;
};


//==================================================================
// CWinSocketProtocol
//
/// Base class for CWinAsyncSocket protocol handlers.
/**
	Derive from this class to implement a protocol handler for 
	TWinAsyncSocket.

	Supports both TCP and UDP protocols.

	\see TWinAsyncSocket, CWinAsyncSocket, CWinSocket
*/
//==================================================================
class CWinSocketProtocol
{
public:

	/// Default constructor
	CWinSocketProtocol() { m_pAsyncSocket = NULL; m_pSocket = NULL; }

	/// Destructor
	virtual ~CWinSocketProtocol() { m_pAsyncSocket = NULL; m_pSocket = NULL; }

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when there is data to be transmitted
	/**
		\param [in] nErr	-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnWrite( int nErr ) = 0;	

	//==============================================================
	// OnRead()
	//==============================================================
	/// Called when new data arrives
	/**
		\param [in] nErr	-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnRead( int nErr ) = 0;

	//==============================================================
	// GetDataReadyHandle()
	//==============================================================
	/// Return a valid handle if you want to receive data tx messages
	/**
		\return Waitable event handle
	
		\see 
	*/
	virtual HANDLE GetDataReadyHandle() = 0;

	//==============================================================
	// OnDataReady()
	//==============================================================
	/// Called when the event handle returned by GetDataReadyHandle() is set
	/**
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnDataReady() = 0;


	//==============================================================
	// GetSocket()
	//==============================================================
	/// Returns a pointer the attached socket
	CWinSocket* GetSocket() { return m_pSocket; }

	//==============================================================
	// SetSocket()
	//==============================================================
	/// Sets the attached socket
	/**
		\param [in] pSocket		-	Pointer to CWinSocket object
	*/
	void SetSocket( CWinSocket *pSocket ) { m_pSocket = pSocket; }

	//==============================================================
	// GetSocket()
	//==============================================================
	/// Returns a pointer the attached socket
	CWinAsyncSocket* GetAsyncSocket() { return m_pAsyncSocket; }

	//==============================================================
	// SetSocket()
	//==============================================================
	/// Sets the attached socket
	/**
		\param [in] pSocket		-	Pointer to CWinAsyncSocket object
	*/
	void SetAsyncSocket( CWinAsyncSocket *pSocket ) { m_pAsyncSocket = pSocket; }

private:

	/// Pointer to the attached socket
	CWinSocket			*m_pSocket;

	/// Pointer to the attached socket
	CWinAsyncSocket		*m_pAsyncSocket;

};


//==================================================================
// TWinAsyncSocket
//
/// Provides asynchronous socket communications.
/**
	This class derives from CWinAsyncSocket and provides an interface
	to CWinSocketProtocol derived classes.  See CWinSocket for
	synchronous socket.

	Example:

  \code

	// Create stream socket
	TWinAsyncSocket< CWspStream > wasStream;

	wasStream.Connect( "google.com", 80 );

	Sleep( 1000 );

	wasStream.GetProtocol()->Write( "GET / HTTP/1.0\r\n\r\n" );

	Sleep( 1000 );

	UINT uRead = 0;
	char szBuf[ 8 * 1024 ];
	wasStream.GetProtocol()->Read( szBuf, sizeof( szBuf ), &uRead );

	printf( szBuf );

	\see CWinAsyncSocket, CWinSocketProtocol, CWinSocket

  \endcode

*/
//==================================================================
template < class T > class TWinAsyncSocket : public CWinAsyncSocket
{
public:

	/// Default constructor
	TWinAsyncSocket() 
	{	m_pWsp = new T; 
		m_pWsp->SetSocket( GetSocket() ); 
		m_pWsp->SetAsyncSocket( this ); 
	}

	/// Destructor
	~TWinAsyncSocket() { if ( m_pWsp ) { delete m_pWsp; m_pWsp = NULL; } }

	//==============================================================
	// Protocol()
	//==============================================================
	/// Returns a pointer to the protocol interface
	T* Protocol() { return m_pWsp; }

	//==============================================================
	// GetDataReadyHandle()
	//==============================================================
	/// Return a valid handle if you want to receive data tx messages
	/**
		
		\return Waitable event handle
	
		\see 
	*/
	virtual HANDLE GetDataReadyHandle() 
	{	if ( !m_pWsp ) return NULL;
		return m_pWsp->GetDataReadyHandle();
	}

	//==============================================================
	// OnDataReady()
	//==============================================================
	/// Called when the event handle returned by GetDataReadyHandle() is set
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnDataReady() 
	{	if ( !m_pWsp ) return FALSE;
		return m_pWsp->OnDataReady();
	}

	//==============================================================
	// OnRead()
	//==============================================================
	/// Called when data has been received from connected socket
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnRead( int nErr )
	{	if ( !m_pWsp ) return FALSE;
		return m_pWsp->OnRead( nErr );
	}

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when previously full transmit buffer is now empty.
	/**
		\param [in] nErr	-	Zero if no error, otherwise socket error value.

		\return Return non-zero if handled
	*/
	virtual BOOL OnWrite( int nErr )
	{	if ( !m_pWsp ) return FALSE;
		return m_pWsp->OnWrite( nErr );
	}

private:

	/// Pointer to attached protocol object
	T				*m_pWsp;
};



#endif // !defined(AFX_WINASYNCSOCKET_H__D37E3AA9_BAFC_464A_AFCD_1CA920633931__INCLUDED_)
