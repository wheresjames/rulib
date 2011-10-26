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
// AsyncWinSock.h: interface for the CAsyncWinSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASYNCWINSOCK_H__4E2142EB_B57D_11D4_8223_0050DA205C15__INCLUDED_)
#define AFX_ASYNCWINSOCK_H__4E2142EB_B57D_11D4_8223_0050DA205C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSock2.h"
#include "WinInet.h"

// The Minimum Socket Library version we need (2.2)
#define CWINSOCK_VERSION		MAKEWORD( 2, 2 )

#define CWINSOCK_STATUS_NOSOCKET		0
#define CWINSOCK_STATUS_IDLE			1
#define CWINSOCK_STATUS_CONNECTING		2
#define CWINSOCK_STATUS_CONNECTED		3
#define CWINSOCK_STATUS_LISTENING		4
#define CWINSOCK_STATUS_DISCONNECTING	5
#define CWINSOCK_STATUS_ERROR			6

#define CWINSOCK_STATUS_USER			0x80000000

#define	LINKTYPE_NONE					0
#define	LINKTYPE_HTTP					1
#define	LINKTYPE_FTP					2
#define	LINKTYPE_EMAIL					3
#define	LINKTYPE_NEWS					4
#define	LINKTYPE_MMS					5

#define LINKCAT_NONE					0
#define LINKCAT_TXT						0x00000001
#define LINKCAT_WEB						0x00000002
#define LINKCAT_IMG						0x00000004
#define LINKCAT_BIN						0x00000008

/// This structure holds URL component information.
/**
	\see CAsyncWinSock
*/
typedef struct tagCRACKURLINFO
{
	URL_COMPONENTS	urlc;
	char			szScheme[ CWF_STRSIZE ];
	DWORD			dwScheme;
	char			szHostName[ CWF_STRSIZE ];
	DWORD			dwPort;
	char			szUserName[ CWF_STRSIZE ];
	char			szPassword[ CWF_STRSIZE ];
	char			szUrlPath[ CWF_STRSIZE * 4 ];
	char			szExtraInfo[ CWF_STRSIZE * 4 ];

} CRACKURLINFO; // end typedef struct
typedef CRACKURLINFO* LPCRACKURLINFO;

class CAsyncWinSock;
typedef BOOL (*AsyncWinSockCallback)(	CAsyncWinSock* pSocket, DWORD dwUser, 
										WPARAM wParam, LPARAM lParam );										

//==================================================================
// CAsyncWinSock
//
///	Windows socket API Wrapper
/**
	This class provides a wrapper for the Windows Socket API.  
	Callbacks are handled in a thread so there is no need for
	a Window.  This class is reasonably compatible with the
	MFC CAsyncSocket class.

	\warning Obsolete, use CWinAsyncSocket instead.
*/
//==================================================================
class CAsyncWinSock : public CThread
{
public:

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
	// GetStatus()
	//==============================================================
	/// Returns the connection status
	DWORD GetStatus() { return m_dwStatus; }

	//==============================================================
	// IsConnected()
	//==============================================================
	/// Returns non-zero if the socket is connected
	BOOL IsConnected() { return m_dwStatus == CWINSOCK_STATUS_CONNECTED; }

	//==============================================================
	// IsConnecting()
	//==============================================================
	/// Returns non-zero if the socket is attempting a connection
	BOOL IsConnecting() { return m_dwStatus == CWINSOCK_STATUS_CONNECTING; }

	//==============================================================
	// IsSocket()
	//==============================================================
	/// Returns non-zero if the class contains a valid socket handle
	BOOL IsSocket() { return ( m_hSocket != INVALID_SOCKET ); }

	//==============================================================
	// IsInitialized()
	//==============================================================
	/// Returns non-zero if the Windows Socket API was successfully initialized.
	BOOL IsInitialized() { return ( m_nInit == 0 ); }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the socket and all other resources.
	void Destroy();

	/// Constructor
	CAsyncWinSock();

	/// Destructor
	virtual ~CAsyncWinSock();

private:

	//==============================================================
	// DoThread()
	//==============================================================
	/// The main thread loop
	virtual BOOL DoThread( LPVOID pData );

	//==============================================================
	// UninitSockets()
	//==============================================================
	/// Uninitializes the Windows Socket API
	void UninitSockets();	

	//==============================================================
	// InitSockets()
	//==============================================================
	/// Initializes the Windows Socket API
	/**
		\return Returns non-zero if success.
	*/
	BOOL InitSockets( WORD ver = CWINSOCK_VERSION );

public:

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
	BOOL IoCtlSocket( long cmd, LPDWORD pdw );
	
	//==============================================================
	// Send()
	//==============================================================
	/// Sends the specified buffer over the socket
	/**
		\param [in] pData		-	Pointer to the buffer to send.
		\param [in] len			-	Length of the buffer in pData in bytes.
		\param [out] sent		-	Receives the number of bytes sent. Can be NULL.
		\param [in] flags		-	Send flags.

		\return Returns the number of bytes sent or zero if error.
	*/
	virtual DWORD Send( LPVOID pData, DWORD len = 0, LPDWORD sent = NULL, DWORD flags = 0 );

	//==============================================================
	// Recv()
	//==============================================================
	/// Reads data from socket input queue
	/**
		\param [out] pData		-	Pointer to the buffer that receives the data
		\param [in] len			-	Length of the buffer in pData in bytes.
		\param [out] read		-	Number of bytes actually read.  Can be NULL.
		\param [in] flags		-	Read flags

		\return Returns the number of bytes read or zero if error.
	*/
	virtual DWORD Recv( LPVOID pData, DWORD len, LPDWORD read = NULL, DWORD flags = 0 );

	//==============================================================
	// Listen()
	//==============================================================
	/// Creates a socket listening on the bound port.
	/**
		\param [in] dwMaxConnections		-	The maximum number of connections allowed.

		\return Returns non-zero if success, otherwise zero.

		\see Bind()
	*/
	BOOL Listen( DWORD dwMaxConnections = 8 );

	//==============================================================
	// Connect()
	//==============================================================
	/// Connects to the specified address
	/**
		\param [in] psai		-	The network address of the target computer.
		\param [in] size		-	Size of the structure in psai.

		If psai is NULL, the member variable m_sai is used as the target address.

		\return Returns non-zero if success, otherwise zero.
	*/
	virtual BOOL Connect( PSOCKADDR psai = NULL, DWORD size = 0 );

	//==============================================================
	// CloseSocket()
	//==============================================================
	/// Closes any open socket.
	/**
		\param [in] bBlocking		-	Set to non-zero if the function should wait until
										the socket is released.
		\param [in] dwTimeout		-	If bBlocking is non-zero, specifies the maximum 
										amount of time, in milli-seconds, to wait for 
										the socket to be released

		\return Returns non-zero if success.
	*/
	virtual BOOL CloseSocket( BOOL bBlocking = FALSE, DWORD dwTimeout = 3000 );

	//==============================================================
	// OpenSocket()
	//==============================================================
	/// Creates a new socket handle.
	/**
		\param [in] af			-	Address family specification.
		\param [in] type		-	The type specification.
		\param [in] protocol	-	The protocol to be used with the socket.

		\return Returns non-zero if success.
		
	*/
	virtual BOOL OpenSocket( int af = AF_INET, int type = SOCK_STREAM, int protocol = 0 );	

	//==============================================================
	// Accept()
	//==============================================================
	/// Call this in response to an OnAccept() message to accept the incomming connection.
	/**
		\param [in] pSocket		-	Pointer to the CAsyncWinSock class that should accept
									an incomming connection.
		\param [out] pSa		-	Receives the address of the connecting computer.  Can
									be NULL if not required.
		\param [out] nAddr		-	Recieves the address of the connecting computer.  Can
									be NULL if not required.

		\return Returns non-zero if success.
	*/
	virtual BOOL Accept( CAsyncWinSock *pSocket, SOCKADDR *pSa = NULL, int *nAddr = NULL );

	// IOCtl Socket 1.1

	//==============================================================
	// SetBlockingMode()
	//==============================================================
	/// Sets the blocking mode for the socket
	/**
		\param [in] nonblocking		-	Non-zero for non-blocking socket.  Zero for blocking
										socket.  This should normally be non-zero.

		\return Returns non-zero if success.
	*/
	BOOL SetBlockingMode( BOOL nonblocking )
	{	return IoCtlSocket( FIONBIO, (LPDWORD)nonblocking ); }

	//==============================================================
	// GetRecvBytesWaiting()
	//==============================================================
	/// Returns the number of bytes waiting to be read.
	/**
		\param [out] pdwBytes	-	Receives the number of bytes that can be read with
									a call to Recv().  Must not be NULL.

		\return Returns non-zero if success.
	*/
	BOOL GetRecvBytesWaiting( LPDWORD pdwBytes )
	{	
//		return IoCtl( FIONREAD, NULL, NULL, pdwBytes, sizeof( DWORD ), NULL );
		return IoCtlSocket( FIONREAD, pdwBytes ); 
	}

	//==============================================================
	// GetOOBBytesWaiting()
	//==============================================================
	/// Returns the number of Out-Of-Band bytes waiting to be read.
	/**
		\param [out] pdwBytes	-	Receives the number of bytes that can be read with
									a call to Recv().  Must not be NULL.

		\return Returns non-zero if success.
	*/
	BOOL GetOOBBytesWaiting( LPDWORD pdwBytes )
	{	return IoCtlSocket( SIOCATMARK, pdwBytes ); }

	// IOCtl Socket 2

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
	BOOL IoCtl( DWORD code, LPVOID pInbuf = NULL, DWORD dwInbuf = 0, LPVOID pOutbuf = NULL, DWORD dwOutbuf = 0, LPDWORD pBytesReturned = NULL );

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
	// EventSelect()
	//==============================================================
	/// Selects which events will generate callbacks
	/**
		\param [in] hEvent	-	The event handle
		\param [in] lEvents	-	The events to hook.

		\return Returns non-zero if success.
	*/
	BOOL EventSelect( WSAEVENT hEvent, long lEvents = FD_READ | FD_WRITE | FD_ACCEPT | FD_CONNECT | FD_CLOSE )
	{	return ( WSAEventSelect( m_hSocket, hEvent, lEvents ) == 0 ); }

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

	//==============================================================
	// GetEnabledCallbacks()
	//==============================================================
	/// Returns a mask indicating the currently active callback events.
	/**

		\see EventSelect()

		\return Returns non-zero if success.
	*/
	long GetEnabledCallbacks() { return m_lEnableCallbacks; }

	//==============================================================
	// EnableCallbacks()
	//==============================================================
	/// Specifies which callback events will be enabled.
	/**
		\param [in] lEvents	-	The events to hook.

		\see EventSelect()

		\return Returns non-zero if success.
	*/
	void EnableCallbacks( long lEvents = FD_READ | FD_WRITE | FD_ACCEPT | FD_CONNECT | FD_CLOSE )
	{	m_lEnableCallbacks = lEvents; }

	//==============================================================
	// SendMessage()
	//==============================================================
	/// Sends a callback message to hooked window or function if any
	void SendMessage( WPARAM wParam, LPARAM lParam )
	{	if ( IsStopping() ) return;
		if ( m_hWnd != NULL && ::IsWindow( m_hWnd ) )
			::SendMessage( m_hWnd, m_uMsg, wParam, lParam );
		if ( m_pCallback != NULL )
			m_pCallback( this, m_dwCallback, wParam, lParam );
	}

	//==============================================================
	// SetCallback()
	//==============================================================
	/// Sets user callback function
	/**
		\param f	-	Address of the callback function.  Set to NULL to disable.
		\param user	-	User data passed to callback function.  Hint: set to <b>this</b> pointer.
				
	*/
	void SetCallback( AsyncWinSockCallback f, DWORD user )
	{	m_pCallback = f; m_dwCallback = user; }

	//==============================================================
	// GetSocket()
	//==============================================================
	/// Returns the current socket handle or INVALID_HANDLE_VALUE if none.
	SOCKET GetSocket() { return m_hSocket; }

	//==============================================================
	// GetEventHandle()
	//==============================================================
	/// Retuns the current event handle
	HANDLE GetEventHandle() { return m_hEvent; }

	//==============================================================
	// GetEvents()
	//==============================================================
	/// Retunrs a structure describing current network events
	LPWSANETWORKEVENTS GetEvents() { return &m_ne; }

	//==============================================================
	// GetShutdownHandle()
	//==============================================================
	/// Returns the handle for the socket shutdown event
	HANDLE GetShutdownHandle() { return m_hShutDown; }

private:

	/// Windows Socket API initialization return code
	int						m_nInit;

	/// Socket data information
	WSADATA					m_wd;

	/// Current socket handle
	SOCKET					m_hSocket;

	/// Last error code
	int						m_iErr;

	/// Last error string
	char					m_szErrorMsg[ MAX_PATH ];

	/// Socket event
	WSAEVENT				m_hEvent;

	/// Socket shutdown event
	HANDLE					m_hShutDown;

	/// Event callback information
	WSANETWORKEVENTS		m_ne;

	/// Socket connection status
	DWORD					m_dwStatus;

	/// Mask representing the callbacks that are currently enabled
	long					m_lEnableCallbacks;

	/// User callback data for callback function
	DWORD					m_dwCallback;

	/// User callback function address
	AsyncWinSockCallback	m_pCallback;

protected:

	/// User callback window handle
	HWND			m_hWnd;

	/// User callback window message value
	UINT			m_uMsg;

public:	

	// URL Information
	URL_COMPONENTS	m_urlc;
	char			m_szScheme[ MAX_PATH ];
	DWORD			m_dwScheme;
	char			m_szHostName[ MAX_PATH ];
	DWORD			m_dwPort;
	char			m_szUserName[ MAX_PATH ];
	char			m_szPassword[ MAX_PATH ];
	char			m_szUrlPath[ MAX_PATH ];
	char			m_szExtraInfo[ MAX_PATH * 4 ];

	
	LPSERVENT		m_ps;
	HOSTENT			m_he;
	LPIN_ADDR		m_ppaddr;
	IN_ADDR			m_paddr[ 1 ];
	LPHOSTENT		m_phe;
	sockaddr_in		m_sai;

	/// Callback internal error codes
	int m_internalerr[ FD_MAX_EVENTS + 1 ];


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

public:

	//==============================================================
	// GetLinkCat()
	//==============================================================
	/// Attempts to determine the link category from the file extension of the specified link.
	/**
		\param [in] pLink	-	The link to analzye.		

		\return Returns the link category.
			- LINKCAT_TXT	-	Text
			- LINKCAT_IMAGE	-	Image
			- LINKCAT_NONE	-	Could not be determined
	*/
	DWORD GetLinkCat( LPCTSTR pLink );

	//==============================================================
	// OnShutdownEvent()
	//==============================================================
	/// Called on shutdown event
	BOOL OnShutdownEvent();

	//==============================================================
	// OnNetworkEvent()
	//==============================================================
	/// Called on network event
	/**
		\param [in] hEvent	-	The network event handle.
		\param [in] pne		-	Network event information.

		\return Returns non-zero if event was handled.
	*/
	virtual BOOL OnNetworkEvent( HANDLE hEvent,  LPWSANETWORKEVENTS pne );

	//==============================================================
	// GetLocalAddress()
	//==============================================================
	/// Returns the local address of this computer
	static DWORD GetLocalAddress();

	//==============================================================
	// GetLinkType()
	//==============================================================
	/// Determines the type of link based on the protocal string
	/**
		\param [in] pLink	-	The link to analyze.
		\param [in] pAdd	-	Added to link before analylzing.

		\return Returns the type of link, or zero if unknown.

			-	LINKTYPE_HTTP	-	http://
			-	LINKTYPE_FTP	-	ftp://
			-	LINKTYPE_HTTP	-	www.
			-	LINKTYPE_FTP	-	ftp.
			-	LINKTYPE_EMAIL	-	mailto:
			-	LINKTYPE_NEWS	-	news:
			-	LINKTYPE_MMS	-	mms://
	*/
	static DWORD GetLinkType( LPCTSTR pLink, LPSTR pAdd = NULL );

	//==============================================================
	// GetDomainName()
	//==============================================================
	/// Extracts the domain name from the specified link
	/**
		\param [in] pLink		-	The link from which the domain name is extracted.
		\param [out] pDomain	-	Receives the extracted domain name.

		\return Returns non-zero if success.
	*/
	static BOOL GetDomainName( LPCTSTR pLink, LPSTR pDomain );

	//==============================================================
	// GetConnectionInfo()
	//==============================================================
	/// Gets information about the connected peer.
	/**
		\param [out] pdwAddr		-	The address of the connected peer.
		\param [out] pdwPort		-	The port of the connected peer.
		\param [out] pAddr			-	The address string of the connected peer.

		\return Returns non-zero if success.
	*/
	BOOL GetConnectionInfo( LPDWORD pdwAddr, LPDWORD pdwPort, LPSTR pAddr = NULL );

	//==============================================================
	// Connect()
	//==============================================================
	/// Attempts connection to the specified target address
	/**
		\param [in] ip		-	The ip address of the remote peer.
		\param [in] port	-	The TCP port of the remote peer.

		\return Returns non-zero if success.
	*/
	BOOL Connect( DWORD ip, DWORD port );

	//==============================================================
	// IsRecvData()
	//==============================================================
	/// Returns non-zero if there is data ready to be read.
	BOOL IsRecvData();

	//==============================================================
	// CrackUrl()
	//==============================================================
	/// Breaks down the specified URL into its components.
	/**
		\param [in] pUrl	-	The URL to crack.
		\param [out] pcui	-	Structure that recieves the URL component information.

		\return Returns non-zero if success
	*/
	static BOOL CrackUrl( LPCTSTR pUrl, LPCRACKURLINFO pcui );

	//==============================================================
	// CrackUrl()
	//==============================================================
	/// Breaks down the specified URL into its components.
	/**
		\param [in] pUrl	-	The URL to crack.

		Result is stored in	m_urlc.

		\return Returns non-zero if success
	*/
	BOOL CrackUrl( LPCTSTR pUrl );

	//==============================================================
	// ThreadCloseSocket()
	//==============================================================
	/// Use this function to close the socket from within the callback thread
	/**
		\return Returns non-zero if success.
	*/
	BOOL ThreadCloseSocket();

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
	// SetMessageTarget()
	//==============================================================
	/// Registers a window handle for socket event callbacks
	/**
		\param [in] hWnd	-	The window that will receive callback notifications.
		\param [in] uMsg	-	Parameter to pass to SendMessage.

		\return Returns non-zero if success.
	*/
	BOOL SetMessageTarget( HWND hWnd, UINT uMsg )
	{	m_hWnd = hWnd, m_uMsg = uMsg; return TRUE; }

	//==============================================================
	// StopCallbacks()
	//==============================================================
	/// Call this function to stop event callbacks
	/**
		\param [in] dwTimeout	-	Amount of time to wait for socket thread to respond
									that it has received the message.  Set to zero if
									you do not want to wait.

		\return Returns non-zero if success.
	*/
	BOOL StopCallbacks( DWORD dwTimeout = 3000 );

	//==============================================================
	// StartCallbacks()
	//==============================================================
	/// Call this function to start event callbacks
	/**
		\param [in] lEvents	-	Socket events to hook.
		\param [in] bThread -	Set to non-zero for threaded callbacks

		\return Returns non-zero if success.
	*/
	BOOL StartCallbacks( long lEvents = FD_READ | FD_WRITE | FD_ACCEPT | FD_CONNECT | FD_CLOSE, BOOL bThread = TRUE );

	//==============================================================
	// Connect()
	//==============================================================
	/// Connects to specified URL
	/**
		\param [in] pUrl	-	The URL to connect to.

		\return Returns non-zero if success.
	*/
	BOOL Connect( LPCTSTR pUrl );

	//==============================================================
	// GetErrorMsg()
	//==============================================================
	/// Returns an information string about the specified error.
	/**
		\param [out] pMsg		-	Buffer to receive error message string.
		\param [in] err			-	Error code.
		\param [in] bLineBreaks	-	Set to non-zero if you want to allow line breaks
									in the output string.

		\return Returns non-zero if err is a valid error code.
	*/
	static BOOL GetErrorMsg( LPSTR pMsg, DWORD err = 0, BOOL bLineBreaks = TRUE );

	//==============================================================
	// GetServeByName()
	//==============================================================
	/// Gets information about the specified address
	/**
		\param [in] pName		-	The name of the server to lookup.
		\param [in] pProto		-	The name of the protocol to match.
		\param [out] pServent	-	Receives the information about the address.

		\return Returns non-zero if success.
	*/
	BOOL GetServeByName( LPCTSTR pName, LPCTSTR pProto, LPSERVENT *pServent = NULL );

	//==============================================================
	// GetDotAddress()
	//==============================================================
	/// Converts dotted address string
	/**
		\param [in] pHost		-	Address to convert
		\param [out] pdwIP		-	Receives the IP address.
		\param [out] pdwPort	-	Receives the TCP port if any.

	*/
	static BOOL GetDotAddress( LPCTSTR pHost, LPDWORD pdwIP, LPDWORD pdwPort = NULL );

	//==============================================================
	// GetHostByName()
	//==============================================================
	/// Gets host address information from DNS server
	/**
		\param [in] pHost	-	Address to lookup.
		\param [out] pPort	-	Receives the TCP port specified in pHost
		\param [out] pHE	-	Receives the address information.

		\return Returns non-zero if success.
	*/
	BOOL GetHostByName( LPCTSTR pHost, LPDWORD pPort, LPHOSTENT *pHE = NULL );

	//==============================================================
	// Bind()
	//==============================================================
	/// Binds the open socket to the specified port
	/**
		\param [in] dwPort	-	Port to bind to.

		\return Returns non-zero if success.
	*/
	BOOL Bind( DWORD dwPort );

	//==============================================================
	// Connect()
	//==============================================================
	/// Address of remote peer.
	/**
		\param [in] pAddress	-	URL formed address of remote peer.
		\param [in] dwPort		-	TCP port of remote peer.

		\return Returns non-zero if success.
	*/
	BOOL Connect( LPCTSTR pAddress, DWORD dwPort );

	//==============================================================
	// GetErrorMsg()
	//==============================================================
	/// Retuns an error string for the specified error code
	/**
		\param [in] err	-	Error code.

		\return Returns the address of a string describing the error code.
	*/
	LPCTSTR GetErrorMsg( DWORD err = 0 )
	{ GetErrorMsg( m_szErrorMsg, err ); return m_szErrorMsg; }
	
	//==============================================================
	// ThreadShutdown()
	//==============================================================
	/// Call this function from within a thread to shutdown the socket.
	/**
		\return Returns non-zero if success.
	*/
	BOOL ThreadShutdown() 
	{	if ( m_hShutDown == NULL ) return FALSE;
		return SetEvent( m_hShutDown ); }
};

#endif // !defined(AFX_ASYNCWINSOCK_H__4E2142EB_B57D_11D4_8223_0050DA205C15__INCLUDED_)
