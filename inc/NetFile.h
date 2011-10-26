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
// NetFile.h: interface for the CNetFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETFILE_H__EAD79981_E9F6_11D3_813E_0050DA205C15__INCLUDED_)
#define AFX_NETFILE_H__EAD79981_E9F6_11D3_813E_0050DA205C15__INCLUDED_

#include <WinInet.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DOWNLOADTEMP				( (LPSTR)1 )

#define NETFILE_DS_NONE				0
#define NETFILE_DS_INITIALIZING		0x00010000
#define NETFILE_DS_CONNECTING		0x00020000
#define NETFILE_DS_DOWNLOADING		0x00040000
#define NETFILE_DS_UPLOADING		0x00080000
#define NETFILE_DS_DONE				0x00100000
#define NETFILE_DS_ERROR			0x00200000

typedef BOOL (*NetFileCallback)( DWORD dwUser, WPARAM wParam, LPARAM lParam );

//==================================================================
// CNetFile
//
/// Implements HTTP GET and POST. Wraps the windows Inet API
/**
	Wraps the Inet API to provide HTTP file downloading and uploading.
	This class supports FTP through the Inet API as well.
*/
//==================================================================
class CNetFile : public CThread
{
public:

	//==============================================================
	// Download()
	//==============================================================
	/// Downloads data from the specified URL
	/**
		\param [in] pUrl					-	URL
		\param [in] pLocal					-	File in which to save
												data.  If this is NULL,
												the data is saved to 
												a memory buffer.
		\param [in] bCloseFileAfterDownload	-	Non-zero to close file
												in pLocal after downloading.
												If zero, the file will
												remain open.
		\param [in] hWndParent				-	Parent window.
		
		\return Non-zero if transfer is successfully started.
	
		\see 
	*/
	BOOL Download( LPCTSTR pUrl, LPCTSTR pLocal = NULL, BOOL bCloseFileAfterDownload = TRUE, HWND hWndParent = NULL );

    /// Squirrel version
    BOOL sqDownload( LPCTSTR pUrl, LPCTSTR pLocal )
    {   return Download( pUrl, pLocal ); }

	//==============================================================
	// Open()
	//==============================================================
	/// Opens the specified URL for data transfer.
	/**
		\param [in] pUrl		-	URL
		\param [in] pLocal		-	Local file that will receive any
									transfer data.  If NULL, a memory
									buffer will receive the data instead.
		
		\return 
	
		\see 
	*/
	BOOL Open( LPCTSTR pUrl, LPCTSTR pLocal = NULL );

public:

	//==============================================================
	// HttpRequest()
	//==============================================================
	/// Builds an HTTP request
	/**
		\param [in] pUrl		-	URL
		\param [in] pHeaders	-	HTTP headers
		\param [in] pData		-	GET or POST data
		\param [in] pMethod		-	GET or POST
		\param [in] pLocal		-	Local file that receives any data
									transferred.  Set to NULL to have
									the data copied to a memory buffer
									instead.

		\param [in] bCloseFileAfterDownload		-	Non-zero to close 
													the file after download
													is complete.
		\param [in] hWndParent		-	Handle of parent window.
		\param [in] dwUrlEncoding	-	URL encoding.  See CRKey::EncodeUrl()
		
		\return Non-zero if transfer is successfully started.
	
		\see 
	*/
	BOOL HttpRequest( LPCTSTR pUrl, CRKey *pHeaders, CRKey *pData, LPCTSTR pMethod = "POST", LPCTSTR pLocal = NULL, BOOL bCloseFileAfterDownload = TRUE, HWND hWndParent = NULL, DWORD dwUrlEncoding = 1 );

	//==============================================================
	// Release()
	//==============================================================
	/// Releases all resources including downloaded data.
	void Release();

	//==============================================================
	// Upload()
	//==============================================================
	/// Uploads data to the specified URL
	/**
		\param [in] pUrl		-	URL 
		\param [in] buf			-	Data to upload
		\param [in] size		-	Number of bytes in buf
		\param [in] bPassive	-	Non-zero to use Passive FTP
		
		This function uploads data to the specified URL using FTP.
		Normal or passive FTP can be used.

		\return Non-zero if upload is successfully started.
	
		\see 
	*/
	BOOL Upload( LPCTSTR pUrl, LPBYTE buf, DWORD size, BOOL bPassive );

	//==============================================================
	// SetDataPtr()
	//==============================================================
	/// Sets the buffer that is used for data transfers
	/**
		\param [in] buf		-	Pointer to buffer.
		\param [in] size	-	Size of the buffer in buf.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetDataPtr( LPBYTE buf, DWORD size );

    /// Sets the data to be sent
    BOOL sqSetData( const std::string &sData )
    {   return SetDataPtr( (LPBYTE)sData.c_str(), sData.length() ); }

	//==============================================================
	// GetErrorMsg()
	//==============================================================
	/// Returns a string describing the error
	/**
		\param [out] msg	-	Receives the error string
		\param [in] err		-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetErrorMsg( LPSTR msg, DWORD err );

	//==============================================================
	// SetOption()
	//==============================================================
	/// Sets internet transfer options
	/**
		\param [in] option	-	Option flags
		\param [in] buf		-	Option data buffer
		\param [in] size	-	Number of bytes in buf
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetOption( DWORD option, LPVOID buf, DWORD size );

	//==============================================================
	// Upload()
	//==============================================================
	/// Uploads data to the specified URL
	/**
		\param [in] pUrl		-	URL 
		\param [in] pLocal		-	Disk filename to upload
		\param [in] bPassive	-	Non-zero to use Passive FTP
		
		This function uploads data to the specified URL using FTP.
		Normal or passive FTP can be used.

		\return Non-zero if upload is successfully started.
	
		\see 
	*/
	BOOL Upload( LPCTSTR pUrl, LPCTSTR pLocal, BOOL bPassive );

	BOOL sqUpload( LPCTSTR pUrl, LPCTSTR pLocal, BOOL bPassive )
    {   return Upload( pUrl, pLocal, bPassive ); }

	//==============================================================
	// Close()
	//==============================================================
	/// Closes network connection and cancels any ongoing transfer.
	void Close();

	//==============================================================
	// EndThread()
	//==============================================================
	/// Transfer thread cleanup
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL EndThread( LPVOID pData );

	//==============================================================
	// InitThread()
	//==============================================================
	/// Transfer thread initialization
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL InitThread( LPVOID pData );

	//==============================================================
	// DoThread()
	//==============================================================
	/// Transfer thread main body.  Handles the actual data transfer.
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero if transfer is still in progress
	
		\see 
	*/
	virtual BOOL DoThread( LPVOID pData );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes network connection and cancels any ongoing transfer.
	void Destroy();


	//==============================================================
	// CNetFile()
	//==============================================================
	/// Default constructor
	/**
		\param [in] pUserAgent	-	User agent string to pass on to server.
	*/
	CNetFile( LPCTSTR pUserAgent = "WheresJames Software (www_wheresjames_com)" );

	/// Destructor
	~CNetFile();

	//==============================================================
	// GetFile()
	//==============================================================
	/// Returns the local transfer file
	/**
		\return Pointer to CWinFile object
	*/
	CWinFile* GetFile() { return &m_local; }

	//==============================================================
	// operator CWinFile()
	//==============================================================
	/// Returns the local transfer file
	/**
		\return Pointer to CWinFile object
	*/
	operator CWinFile() { return m_local; }

	//==============================================================
	// GetMem()
	//==============================================================
	/// Returns a pointer to the transfer data.
	/**
		This function only returns a valid pointer if a local disk
		file was not specified as the target for data transfer.

		\return Pointer to transfered data or NULL if none.
	
		\see 
	*/
	LPBYTE GetMem() { return m_pMem; }

    /// Returns the data read as a string
    std::string sqGetData() 
    {   if ( !m_pMem ) return "";
        return std::string( (LPCTSTR)GetMem(), GetDataRead() );
    }

	//==============================================================
	// GetTransferStatus()
	//==============================================================
	/// Returns the current status of the file transfer
	/**
			-	NETFILE_DS_NONE			-	Connection is idle
			-	NETFILE_DS_INITIALIZING	-	Network is initializing
			-	NETFILE_DS_CONNECTING	-	Connecting to remote host
			-	NETFILE_DS_DOWNLOADING	-	Downloading data
			-	NETFILE_DS_UPLOADING	-	Uploading data
			-	NETFILE_DS_DONE			-	Transfer completed successfully
			-	NETFILE_DS_ERROR		-	Transfer aborted with error(s)
		
		\return Transfer status code
	
		\see 
	*/
	DWORD GetTransferStatus() { return m_dwTransferStatus; }

	//==============================================================
	// GetStatus()
	//==============================================================
	/// Returns a human readable string defining the transfer status code.
	/**
		\param [in] status	-	Transfer status code
								
		\return Human readable string describing transfer status code.
	
		\see 
	*/
	static LPCTSTR GetStatus( DWORD status );

	//==============================================================
	// GetStatus()
	//==============================================================
	/// Returns a human readable string defining the current transfer status code.
	/**
		\return Human readable string describing current transfer status code.
	
		\see 
	*/
	LPCTSTR GetStatus() { return GetStatus( m_dwTransferStatus ); }

	//==============================================================
	// GetTransferError()
	//==============================================================
	/// Returns the last transfer error code
	DWORD GetTransferError() { return m_dwTransferError; }

	//==============================================================
	// GetDataRead()
	//==============================================================
	/// Returns the number of bytes read from the remote host.
	DWORD GetDataRead() { return m_dwDataRead; }

	//==============================================================
	// GetDataWritten()
	//==============================================================
	/// Returns the number of bytes written to the remote host
	DWORD GetDataWritten() { return m_dwDataWritten; }

	//==============================================================
	// CloseFileAfterDownload()
	//==============================================================
	/// Sets whether the file should be closed after downloading.
	/**
		\param [in] b	-	Non-zero to close file after download
	*/
	void CloseFileAfterDownload( BOOL b )
	{	m_bCloseFileAfterDownload = b; }

	//==============================================================
	// SetBlockSize()
	//==============================================================
	/// Sets the transfer block size
	/**
		\param [in] bs	-	Transfer block size in bytes.
	*/
	void SetBlockSize( DWORD bs ) { m_dwBlockSize = bs; }

	//==============================================================
	// GetUrl()
	//==============================================================
	/// Returns the current target URL
	LPCTSTR GetUrl() { return m_szUrl; }

	//==============================================================
	// GetLocalFile()
	//==============================================================
	/// Returns the path of the current transfer disk file
	LPCTSTR GetLocalFile() { return m_szLocal; }

	//==============================================================
	// SetOpenFlags()
	//==============================================================
	/// Sets URL open flags
	/**
		\param [in] f	-	Open flags
	*/
	void SetOpenFlags( DWORD f ) { m_dwOpenFlags = f; }

	//==============================================================
	// SetInternetFlags()
	//==============================================================
	/// Sets internet open flags
	/**
		\param [in] f	-	Internet open flags
	*/
	void SetInternetFlags( DWORD f ) { m_dwInetFlags = f; }

	//==============================================================
	// SetUserAgent()
	//==============================================================
	/// Sets the user agent name
	/**
		\param [in] str		-	User-agent name sent to remote host.
	*/
	void SetUserAgent( LPCTSTR str ) { strcpy_sz( m_szUserAgent, str ); }

	//==============================================================
	// SetUsernamePassword()
	//==============================================================
	/// Sets the username and password for URL connections
	/**
		\param [in] un	-	Username
		\param [in] pw	-	Password
	*/
	void SetUsernamePassword( LPCTSTR un, LPCTSTR pw )
	{	strcpy_sz( m_szUsername, un ); strcpy_sz( m_szPassword, pw ); }

	//==============================================================
	// SetMessageTarget()
	//==============================================================
	/// Sets the window handle that will receive status messages
	/**
		\param [in] hWnd	-	Window handle receiving status messages
		\param [in] uMsg	-	Message id for callbacks
	*/
	void SetMessageTarget( HWND hWnd, UINT uMsg )
	{	m_hWndCallback = hWnd, m_uMsgCallback = uMsg; }

	//==============================================================
	// SetCallback()
	//==============================================================
	/// Sets the address of a callback function that receives status messages
	/**
		\param [in] f		-	Pointer to callback function
		\param [in] user	-	User data, usually a class pointer
	*/
	void SetCallback( NetFileCallback f, DWORD user )
	{	m_pCallback = f; m_dwCallback = user; }

	//==============================================================
	// DoCallback()
	//==============================================================
	/// Executes a callback
	/**
		\param [in] wParam	-	WPARAM value passed to callback
		\param [in] lParam	-	LPARAM value passed to callback
		
		\return Non-zero if callback was executed.
	
		\see 
	*/
	BOOL DoCallback( WPARAM wParam, LPARAM lParam )
	{
		// Post message to message que
		if ( m_hWndCallback != NULL && IsWindow( m_hWndCallback ) )
			PostMessage( m_hWndCallback, m_uMsgCallback, wParam, lParam );

		// Callback function
		if ( m_pCallback != NULL )
			return m_pCallback( m_dwCallback, wParam, lParam );

		return TRUE;
	}

	//==============================================================
	// EnableCache()
	//==============================================================
	/// Enables fetching data from the local internet cache.
	/**
		\param [in] b	-	Non-zero to enable use of the data cache.
	*/
	void EnableCache( BOOL b ) 
	{	if ( !b ) m_dwOpenFlags |= ( INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE );
		else m_dwOpenFlags &= ~( INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE ); }

	//==============================================================
	// IsWorking()
	//==============================================================
	/// Returns non-zero if a transfer is currently in progress.
	BOOL IsWorking()
	{	return ( 	m_dwTransferError == 0 &&
					( m_dwTransferStatus & (	NETFILE_DS_INITIALIZING |
											NETFILE_DS_CONNECTING |
											NETFILE_DS_DOWNLOADING |
											NETFILE_DS_UPLOADING ) ) != 0 );
	}

	//==============================================================
	// SetAccessFlags()
	//==============================================================
	/// Sets data access flags
	/**
		\param [in] f	-	Access flags
	*/
	void SetAccessFlags( DWORD f ) { m_dwAccessFlags = f; }

	//==============================================================
	// GetTotalSize()
	//==============================================================
	/// Gets the length, in bytes, of the data being transfered
	DWORD GetTotalSize() { return m_dwTotalLength; }

private:

	/// Non-zero to close local file after download is complete
	BOOL				m_bCloseFileAfterDownload;

	/// Encapsulates local data transfer file
	CWinFile			m_local;

	/// Memory buffer to hold downloaded data
	LPBYTE				m_pMem;

	/// Size of buffer in m_pMem
	DWORD				m_dwMemSize;

	/// Non-zero if memory buffer is being used instead of disk file
	BOOL				m_bMem;

	/// Information on target URL
	URLINFO				m_cui;

	/// Number of bytes read from remote host
	DWORD				m_dwDataRead;

	/// Number of bytes written to remote host
	DWORD				m_dwDataWritten;

	/// Handle to internet connection
	HINTERNET			m_hInternet;

	/// Handle to Host connection
	HINTERNET			m_hConnect;

	/// Handle to Host file connection
	HINTERNET			m_hFile;

	/// Target URL string
	TCHAR				m_szUrl[ CWF_STRSIZE ];

	/// Local data transfer filename
	TCHAR				m_szLocal[ CWF_STRSIZE ];

	/// Transfer status
	DWORD				m_dwTransferStatus;

	/// Last transfer error code
	DWORD				m_dwTransferError;

	/// Transfer block size
	DWORD				m_dwBlockSize;

	/// Connectio open flags
	DWORD				m_dwOpenFlags;

	/// Internet open flags
	DWORD				m_dwInetFlags;

	/// Data access level
	DWORD				m_dwAccessFlags;

	/// User-agent string
	TCHAR				m_szUserAgent[ 256 ];

	/// Handle of parent window
	HWND				m_hWndParent;

	/// Non-zero if we are uploading data to the URL
	BOOL				m_bUpload;

	/// Login username
	TCHAR				m_szUsername[ 256 ];

	/// Login password
	TCHAR				m_szPassword[ 256 ];

	/// Handle of window receiving status update messages
	HWND				m_hWndCallback;

	/// Message ID for status update messages
	UINT				m_uMsgCallback;

	/// Callback function data
	DWORD				m_dwCallback;

	/// Callback function address
	NetFileCallback		m_pCallback;

	/// Non-zero to use passive FTP
	BOOL				m_bPassive;

	/// Non-zero if we have received the total length of transfer data
	BOOL				m_bGetTotalLength;

	/// Total size of data being transfered
	DWORD				m_dwTotalLength;

	/// HTTP header data
	CStr				m_sHeaders;

	/// HTTP GET or POST data
	CStr				m_sData;

	/// GET / POST method
	CStr				m_sMethod;
};

#endif // !defined(AFX_NETFILE_H__EAD79981_E9F6_11D3_813E_0050DA205C15__INCLUDED_)
