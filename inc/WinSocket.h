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
// WinSocket.h: interface for the CWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSOCKET_H__B2667E68_60B2_4F4A_AAE3_535DCE3A61B2__INCLUDED_)
#define AFX_WINSOCKET_H__B2667E68_60B2_4F4A_AAE3_535DCE3A61B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

//==================================================================
// CWinSocket
//
/// Windows Socket API wrapper class
/**
	Provides synchronous socket support

	For most advanced applications, you will probably want the 
	asynchronous capability of TWinAsyncSocket.

	Example:
	\code

	CWinSocket ws;
	char szError[ 1024 ] = "";

	// Connect to web site
	if ( !ws.Connect( "google.com", 80 ) || !ws.WaitEvent( FD_CONNECT ) )

		printf( ws.GetLastErrorStr( szError ) );

	else
	{
		// Send the page request
		ws.Send( "GET / HTTP/1.0\r\n\r\n" );

		// Wait for data
		if ( m_ws.WaitEvent( FD_READ ) )
		{
			char szBuf[ 8 * 1024 ] = "";

			// Read in page data
			UINT uRead = ws.Recv( szBuf, sizeof( szBuf ) );

			printf( szBuf );

		} // end if

	} // end if

	\endcode

	\see CWinAsyncSocket, CWinSocketProtocol, CWinSocket
*/
//==================================================================
class CWinSocket
{

	/// Minimum socket library version
	enum { eMinSocketVersion = MAKEWORD( 2, 2 ) };
		
	/// This structure holds URL component information.
	/**
		\see CWinSocket
	*/
	struct SCrackUrlInfo
	{
		/// CrackUrl() structure
		URL_COMPONENTS	urlc;

		/// Protocol scheme, http, ftp, https, etc...
		char			szScheme[ MAX_PATH ];

		/// Scheme id
		DWORD			dwScheme;

		/// Network host name
		char			szHostName[ MAX_PATH ];

		/// TCP port
		DWORD			dwPort;

		/// Username
		char			szUserName[ MAX_PATH ];

		/// Password
		char			szPassword[ MAX_PATH ];

		/// URL file path
		char			szUrlPath[ MAX_PATH * 4 ];

		/// GET data
		char			szExtraInfo[ MAX_PATH * 4 ];

	}; // end struct

public:

	/// Default Constructor
	CWinSocket();

	/// Destructor
	virtual ~CWinSocket();

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes the socket and releases related resources
	void Destroy();

	//==============================================================
	// InitSockets()
	//==============================================================
	/// Initializes the Windows Socket API
	/**
		\warning	You must call this function before any other 
					socket functions!  Best if called from your
					application startup code.  Call UninitSockets()
					before your application shuts down and after
					all instances of this class have been closed.

		\return Returns non-zero if success.
	*/
	static BOOL InitSockets( WORD ver = eMinSocketVersion );

	//==============================================================
	// UninitSockets()
	//==============================================================
	/// Uninitializes the Windows Socket API
	static void UninitSockets();	

	//==============================================================
	// IsInitialized()
	//==============================================================
	/// Returns non-zero if the Windows Socket API was successfully initialized.
	static BOOL IsInitialized() { return ( m_nInit == 0 ); }

	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches to existing socket handle
	/**
		\param [in] hSocket		-	Existing socket handle

		\return Returns non-zero if success		
	*/
	BOOL Attach( SOCKET hSocket ) 
	{	Destroy(); 
		m_hSocket = hSocket; 
		return IsSocket(); 
	}

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from existing socket handle without releasing it.
	void Detach() { m_hSocket = INVALID_SOCKET; }
	
	//==============================================================
	// IsSocket()
	//==============================================================
	/// Returns non-zero if the class contains a valid socket handle
	BOOL IsSocket() { return ( m_hSocket != INVALID_SOCKET ); }
					 
	//==============================================================
	// GetSocketHandle()
	//==============================================================
	/// Returns a handle to the socket
	SOCKET GetSocketHandle() { return m_hSocket; }

	//==============================================================
	// operator SOCKET()
	//==============================================================
	/// Returns a handle to the socket
	operator SOCKET() { return m_hSocket; }

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a new socket handle.
	/**
		\param [in] af			-	Address family specification.
		\param [in] type		-	The type specification.
		\param [in] protocol	-	The protocol to be used with the socket.

		\return Returns non-zero if success.

		\see Bind(), Listen(), Connect()
	*/
	virtual BOOL Create( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 );	

	//==============================================================
	// GetLastError()
	//==============================================================
	/// Returns the most recent error code
	UINT GetLastError() { return m_uLastError; }

	//==============================================================
	// GetLastErrorStr()
	//==============================================================
	/// Returns a string describing the last error
	/**
		\param [in] pStr		-	Receives the error string
		\param [in] pTemplate	-	Optional string template

		\return Pointer in pStr
	
		\see CWin32::GetSystemErrorMsg()
	*/
	LPCTSTR GetLastErrorStr( LPSTR pStr, LPCTSTR pTemplate = NULL )
	{	if ( pStr == NULL ) return NULL;
		if ( !CWin32::GetSystemErrorMsg( GetLastError(), pStr, pTemplate ) )
			strcpy( pStr, "Unknown Error" );
		return pStr;
	}

public:

	//==============================================================
	// Bind()
	//==============================================================
	/// Binds the open socket to the specified Port
	/**
		\param [in] uPort	-	Port to bind to.

		\return Returns non-zero if success.

		\see Create(), Listen(), Connect()
	*/
	BOOL Bind( UINT uPort );

	//==============================================================
	// Listen()
	//==============================================================
	/// Creates a socket listening on the bound port.
	/**
		\param [in] uMaxConnections	-	The maximum number of connections allowed.

		\return Returns non-zero if success, otherwise zero.

		\see Create(), Bind(), Connect()
	*/
	BOOL Listen( UINT uMaxConnections = 32 );

	//==============================================================
	// Connect()
	//==============================================================
	/// Connects to the specified address
	/**
		\param [in] pSa		-	The network address of the target computer.
		\param [in] uSize	-	Size of the structure in psai.

		If psai is NULL, the member variable m_sai is used as the target address.

		\return Returns non-zero if success, otherwise zero.
	*/
	virtual BOOL Connect( sockaddr_in *pSa, UINT uSize = sizeof( SOCKADDR ) );

	//==============================================================
	// Connect()
	//==============================================================
	/// Address of remote peer.
	/**
		\param [in] pAddress	-	URL formed address of remote peer.
		\param [in] uPort		-	Port of remote peer.

		\return Returns non-zero if success.
	*/
	BOOL Connect( LPCTSTR pAddress, UINT uPort );

	//==============================================================
	// GetHostByName()
	//==============================================================
	/// Gets host address information from DNS server
	/**
		\param [in] pHost	-	Address to lookup.
		\param [out] pHe	-	Receives the address information.
		\param [out] puPort	-	Receives the TCP port specified in pHost

		\return Returns non-zero if success.
	*/
	BOOL GetHostByName( LPCTSTR pHost, LPHOSTENT *pHe, UINT *puPort = NULL );

public:

	//==============================================================
	// EventSelect()
	//==============================================================
	/// Selects which events will generate callbacks
	/**
		\param [in] hEvent	-	The event handle
		\param [in] lEvents	-	The events to hook.

		\return Returns non-zero if success.
	*/
	BOOL EventSelect( WSAEVENT hEvent, long lEvents = FD_READ | FD_WRITE | FD_ACCEPT | FD_CONNECT | FD_CLOSE )
	{	if ( !IsSocket() ) return FALSE;
		return ( WSAEventSelect( m_hSocket, hEvent, lEvents ) == 0 ); }

	//==============================================================
	// EventSelect()
	//==============================================================
	/// Selects which events will generate callbacks
	/**
		\param [in] lEvents	-	The events to hook.

		\return Returns non-zero if success.
	*/
	BOOL EventSelect( long lEvents = FD_READ | FD_WRITE | FD_ACCEPT | FD_CONNECT | FD_CLOSE )
	{	if ( !IsSocket() ) return FALSE;
		return ( WSAEventSelect( m_hSocket, m_hEvent, lEvents ) == 0 ); }

	//==============================================================
	// GetEventHandle()
	//==============================================================
	/// Retuns the current event handle
	HANDLE GetEventHandle() { return m_hEvent; }

	//==============================================================
	// IsEventHandle()
	//==============================================================
	/// Returns non-zero if there is a valid event handle
	BOOL IsEventHandle() { return ( WSA_INVALID_EVENT != GetEventHandle() ); }

	//==============================================================
	// CreateEventHandle()
	//==============================================================
	/// Creates a network event handle
	BOOL CreateEventHandle()
	{
		// Do we already have an event?
		if ( IsEventHandle() ) return TRUE;

		// Attempt to allocate an event
		return WSA_INVALID_EVENT != ( m_hEvent = WSACreateEvent() );
	}

	//==============================================================
	// CloseEventHandle()
	//==============================================================
	/// Closes the event handle
	void CloseEventHandle()
	{
		// Punt if no handle
		if ( !IsEventHandle() ) return;

		// Good practices and all...
		WSAEVENT hEvent = m_hEvent;
		m_hEvent = WSA_INVALID_EVENT;

		// Turn callbacks off
		if ( IsSocket() ) WSAEventSelect( m_hSocket, hEvent, 0 );

		// Close the event handle
		WSACloseEvent( hEvent );
	}

	//==============================================================
	// WaitEvent()
	//==============================================================
	/// Waits for a socket event to occur
	/**
		\param [in] hEvent		-	Handle to network event
		\param [in] lEventId	-	Mask identifying event(s) to wait
									for.
		\param [out] pWne		-	Information about event.
		\param [in] uTimeout	-	Maximum time to wait in milli-
									seconds.
		
		\return Non-zero if a socket event occurs before the timeout.
	
		\see 
	*/
	BOOL WaitEvent( WSAEVENT hEvent, long lEventId = 0xffffffff, LPWSANETWORKEVENTS pWne = NULL, UINT uTimeout = INFINITE );

	//==============================================================
	// WaitEvent()
	//==============================================================
	/// Waits for a socket event to occur
	/**
		\param [in] lEventId	-	Mask identifying event(s) to wait
									for.
		\param [out] pWne		-	Information about event.
		\param [in] uTimeout	-	Maximum time to wait in milli-
									seconds.
		
		\return Non-zero if a socket event occurs before the timeout.
	
		\see 
	*/
	BOOL WaitEvent( long lEventId = 0xffffffff, LPWSANETWORKEVENTS pWne = NULL, UINT uTimeout = INFINITE )
	{	if ( !IsEventHandle() ) return FALSE;
		return WaitEvent( m_hEvent, lEventId, pWne, uTimeout ); }

	//==============================================================
	// GetEventBit()
	//==============================================================
	/// Returns the bit offset for the specified event
	/**
		\param [in] lEventMask	-	Event mask
		
		\return Bit offset for specified event
	
		\see 
	*/
	UINT GetEventBit( long lEventMask );

public:

	//==============================================================
	// Recv()
	//==============================================================
	/// Reads data from the socket
	/**
		\param [in] pData		-	Receives the socket data
		\param [in] uSize		-	Size of buffer in pData
		\param [in] puRead		-	Receives the number of bytes read
		\param [in] uFlags		-	Socket receive flags
		
		\return Number of bytes read or SOCKET_ERROR if failure.
	
		\see 
	*/
	UINT Recv( LPVOID pData, UINT uSize, UINT *puRead = NULL, UINT uFlags = 0 );

	//==============================================================
	// Send()
	//==============================================================
	/// Writes data to the socket
	/**
		\param [in] pData		-	Buffer containing write data
		\param [in] uSize		-	Size of the buffer in pData
		\param [in] puSent		-	Receives the number of bytes written
		\param [in] uFlags		-	Socket write flags
		
		\return Number of bytes sent or SOCKET_ERROR if failure.
	
		\see 
	*/
	UINT Send( const LPVOID pData, UINT uSize, UINT *puSent = NULL, UINT uFlags = 0 );

	//==============================================================
	// Send()
	//==============================================================
	/// Writes a NULL terminated string to the socket
	/**
		\param [in] pStr		-	Pointer to NULL terminated string
		\param [in] puSent		-	Number of bytes sent
		\param [in] uFlags		-	Socket write flags
		
		\return Number of bytes sent or SOCKET_ERROR if failure.
	
		\see 
	*/
	UINT Send( LPCTSTR pStr, UINT *puSent = NULL, UINT uFlags = 0 )
	{	return Send( (LPVOID)pStr, strlen( pStr ), puSent, uFlags ); }

public:

	//==============================================================
	// GetPeerName()
	//==============================================================
	/// Gets the remote socket information
	/**
		\param [out] pName		-	Receives the remote address of the connected socket.
		\param [out] pdwPort	-	Receives the remote TCP port of the connected socket.

		\return Returns non-zero if success.
	*/
	BOOL GetPeerName( LPSTR pName, LPDWORD pdwPort = NULL );

	//==============================================================
	// GetSocketName()
	//==============================================================
	/// Gets the local socket information
	/**
		\param [out] pName		-	Receives the local address of the connected socket.
		\param [out] pdwPort	-	Receives the local TCP port of the connected socket.

		\return Returns non-zero if success.
	*/
	BOOL GetSocketName( LPSTR pName, LPDWORD pdwPort = NULL );
	
	//==============================================================
	// IoCtlSocket()
	//==============================================================
	/// Controls the i/o mode of the socket
	/**
		\param [in]	cmd		-	The command to perform on the socket.
		\param [in,out] pdw	-	Input and/or return value for cmd.

		See the Windows Socket API function ioctlsocket() for more details.

		\return Returns non-zero on success, otherwise zero.
	*/
	BOOL IoCtlSocket( long cmd, LPDWORD pdw )
	{	if ( !IsSocket() ) return SOCKET_ERROR;
		return ( ioctlsocket( m_hSocket, cmd, pdw ) == 0 ); 
	}

	//==============================================================
	// IoCtl()
	//==============================================================
	/// Controls Socket2 mode features of the socket
	/**
		\param [in] code			-	The code of the operation to be performed.
		\param [in] pInbuf			-	Pointer to the input buffer.
		\param [in] dwInbuf			-	Size of the buffer in pInBuf.
		\param [in] pOutbuf			-	Pointer to the output buffer.
		\param [in] dwOutbuf		-	Size of the buffer in pOutbuf.
		\param [in] pBytesReturned	-	Number of bytes returned in pOutbuf.

		See the Windows Socket API function WSAIoctl() for more details.

		\return Returns non-zero if success.
	*/
	BOOL IoCtl( DWORD code, LPVOID pInbuf = NULL, DWORD dwInbuf = 0, LPVOID pOutbuf = NULL, DWORD dwOutbuf = 0, LPDWORD pBytesReturned = NULL )
	{	if ( !IsSocket() ) return FALSE;
		return ( WSAIoctl(	m_hSocket, code, pInbuf, dwInbuf, 
							pOutbuf, dwOutbuf, pBytesReturned, NULL, NULL ) == 0 );
	}

	//==============================================================
	// EnableCircularQueueing()
	//==============================================================
	/// Enables circular queueing.
	/**
		\return Returns non-zero if success.
	*/
	BOOL EnableCircularQueueing()
	{	return IoCtl( SIO_ENABLE_CIRCULAR_QUEUEING ); }

	//==============================================================
	// FindRoute()
	//==============================================================
	/// Finds the route to the specified address
	/**
		\param [in] psa		-	Socket address whose route to find.

		\return Returns non-zero if success.
	*/
	BOOL FindRoute( LPSOCKADDR psa )
	{	return IoCtl( SIO_FIND_ROUTE, psa, sizeof( SOCKADDR ) ); }

	//==============================================================
	// Flush()
	//==============================================================
	/// Flushes the socket data queue
	/**
		\return Returns non-zero if success.
	*/
	BOOL Flush()
	{	return IoCtl( SIO_FLUSH ); }


	//==============================================================
	// GetBroadcastAddress()
	//==============================================================
	/// Receives the current broadcast address
	/**
		\param [out] psa	-	Receives the current broadcast address

		\return Returns non-zero if success.
	*/
	BOOL GetBroadcastAddress( LPSOCKADDR psa )
	{	return IoCtl( SIO_GET_BROADCAST_ADDRESS, NULL, 0, psa, sizeof( SOCKADDR ) ); }

	//==============================================================
	// GetQOS()
	//==============================================================
	/// Returns current quality of service information	
	/**
		\param [out] pqos	-	Recieves quality of service information.

		\return Returns non-zero if success.
	*/
	BOOL GetQOS( LPQOS pqos )
	{	return IoCtl( SIO_GET_QOS, NULL, 0, pqos, sizeof( QOS ) ); }

	//==============================================================
	// GetGroupQOS()
	//==============================================================
	/// Returns current quality of service information	
	/**
		\param [out] pqos	-	Recieves quality of service information.

		\return Returns non-zero if success.
	*/
	BOOL GetGroupQOS( LPQOS pqos )
	{	return IoCtl( SIO_GET_GROUP_QOS, NULL, 0, pqos, sizeof( QOS ) ); }

	//==============================================================
	// SetMultipointLoopback()
	//==============================================================
	/// Enables / disables multi-point loopback
	/**
		\param [in] bLoopback	-	Set to non-zero to enable multi-point loopback.

		\return Returns non-zero if success.
	*/
	BOOL SetMultipointLoopback( BOOL bLoopback )
	{	return IoCtl( SIO_MULTIPOINT_LOOPBACK, (LPVOID)bLoopback ); }

	//==============================================================
	// SetMultipointScope()
	//==============================================================
	/// Sets multipoint scope
	/**
		\param [in] scope	-	Multipoint scope value

		\return Returns non-zero if success.
	*/
	BOOL SetMultipointScope( DWORD scope )
	{	return IoCtl( SIO_MULTICAST_SCOPE, (LPVOID)scope ); }

	//==============================================================
	// SetQOS()
	//==============================================================
	/// Sets quality of service information	
	/**
		\param [out] pqos	-	Quality of service information.

		\return Returns non-zero if success.
	*/
	BOOL SetQOS( LPQOS pqos )
	{	return IoCtl( SIO_SET_QOS, pqos, sizeof( QOS ) ); }

	//==============================================================
	// SetGroupQOS()
	//==============================================================
	/// Sets group quality of service information	
	/**
		\param [out] pqos	-	Quality of service information.

		\return Returns non-zero if success.
	*/
	BOOL SetGroupQOS( LPQOS pqos )
	{	return IoCtl( SIO_SET_GROUP_QOS, pqos, sizeof( QOS ) ); }

	//==============================================================
	// SetSendBufferSize()
	//==============================================================
	/// Sets the send buffer size on the socket
	/**
		\param [in] size	-	The new size of the send buffer

		\return Returns non-zero if success
	*/
	BOOL SetSendBufferSize( DWORD size )
	{	int sz = (int)size;
		return setsockopt( m_hSocket, SOL_SOCKET, SO_SNDBUF, (LPCTSTR)&sz, sizeof( sz ) ); 
	}

	//==============================================================
	// SetRecvBufferSize()
	//==============================================================
	/// Sets the receive buffer size on the socket
	/**
		\param [in] size	-	The new size of the receive buffer

		\return Returns non-zero if success
	*/
	BOOL SetRecvBufferSize( DWORD size )
	{	int sz = (int)size;
		return setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, (LPCTSTR)&sz, sizeof( sz ) ); 
	}

private:

	/// Windows Socket API initialization return code
	static int				m_nInit;

	/// Socket data information
	static WSADATA			m_wd;

	/// Supporting GetHostByName()
	HOSTENT					m_he;

	/// Supporting GetHostByName()
	LPIN_ADDR				m_ppaddr;

	/// Supporting GetHostByName()
	IN_ADDR					m_paddr[ 1 ];

	/// Windows socket handle
	SOCKET					m_hSocket;

	/// Last error code
	UINT					m_uLastError;

	/// Address
	std::string				m_sAddress;

	/// Dot address
	std::string				m_sDotAddress;

	/// Port
	UINT					m_uPort;

	/// Socket event
	WSAEVENT				m_hEvent;

};

#endif // !defined(AFX_WINSOCKET_H__B2667E68_60B2_4F4A_AAE3_535DCE3A61B2__INCLUDED_)
