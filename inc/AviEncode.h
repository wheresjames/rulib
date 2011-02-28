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
// AviEncode.h: interface for the CAviEncode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIENCODE_H__F6F75054_624E_498E_8632_40C17CE75D13__INCLUDED_)
#define AFX_AVIENCODE_H__F6F75054_624E_498E_8632_40C17CE75D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CAviEncode
//
/// Encapsulates Windows AVI functionality.
/**
	This class contains its own thread to handle AVI encoding.
	When AddFrame() is called, the image data is queued in an 
	asynchronous buffer until the thread can write it into
	the AVI file.  This takes away blocking effect from the
	calling thread.
*/
//==================================================================
class CAviEncode : public CThread  
{
public:

	//==============================================================
	// StartRecording()
	//==============================================================
	/// Opens the avi and prepares for writing frames
	/**
		\param [in] pFile		-	The filename for the AVI.
		\param [in] pRect		-	The size of a single video frame.
		\param [in] dwFrames	-	The number of frames in dwSeconds of video.
		\param [in] dwSeconds	-	The number of seconds of dwFrames of video.
		\param [in] dwFourCC	-	The four character code of the compressor to use.
		\param [in] dwQuality	-	Video quality value to send to codec

		Examples for how to set dwFrames and dwSeconds:
			30 fps	-	dwFrames = 30,	dwSeconds = 1
			5  fps	-	dwFrames = 5,	dwSeconds = 1
			2  fpm	-	dwFrames = 2,	dwSeconds = 60 or dwFrames = 1, dwSeconds = 30
			
		\return Returns non-zero if success.
	*/
	BOOL StartRecording( LPCTSTR pFile, LPRECT pRect, DWORD dwFrames, DWORD dwSeconds, DWORD dwFourCC, DWORD dwQuality );

	//==============================================================
	// StopRecording()
	//==============================================================
	/// Closes the avi and releases all resources.
	/**
		\param [in] pRename		-	Optional filename.  If not NULL, the file will be renamed.

		\return Returns non-zero if success.
	*/
	BOOL StopRecording( LPCTSTR pRename = NULL );

	//==============================================================
	// AddFrame()
	//==============================================================
	/// Adds a frame of video to the avi
	/**
		\param [in] pDc		- CWinDc object holding single video frame to add to the AVI file.
		\param [in] dwTime	- UNIX timestamp, 0 for current time

		\return Returns non-zero if success.
	*/
	BOOL AddFrame( CWinDc *pDc, DWORD dwTime = 0 );

	//==============================================================
	// AddFrame()
	//==============================================================
	/// Adds a frame of video to the avi
	/**
		\param [in] pBuf	- Buffer containing image data
		\param [in] dwSize	- Size of data in pBuf
		\param [in] lWidth	- Image width
		\param [in] lHeight	- Image height
		\param [in] lBpp	- Bits per pixel
		\param [in] dwTime	- UNIX timestamp, 0 for current time

		\return Returns non-zero if success.
	*/
	BOOL AddFrame( LPBYTE pBuf, DWORD dwSize, LONG lWidth, LONG lHeight, LONG lBpp, DWORD dwTime = 0 );

	//==============================================================
	// ReleaseBuffers()
	//==============================================================
	/// Releases async buffers.
	void ReleaseBuffers();

	//==============================================================
	// AllocateBuffers()
	//==============================================================
	/// Allocates async buffers
	/**
		\return Returns non-zero if success.
	*/
	BOOL AllocateBuffers();

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes the AVI file and releases all resources
	/**
		It is ok to call this function instead of StopRecording(), however,
		this function also releases all async buffers.
	*/
	void Destroy();

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the class by allocating async buffers
	/**
		\param [in] dwBufferSize	-	Number of async buffers to allocate.

		\return Returns non-zero if success.
	*/
	BOOL Init( DWORD dwBufferSize );

	//==============================================================
	// EndThread()
	//==============================================================
	/// Thread clean-up function
	/**
		\see CThread::EndThread()

		\return Return zero to end thread
	*/
	virtual BOOL EndThread( LPVOID pData );

	//==============================================================
	// DoThread()
	//==============================================================
	/// Main thread body
	/**
		\see CThread::DoThread()

		\return Return zero to end thread
	*/
	virtual BOOL DoThread( LPVOID pData );

	//==============================================================
	// InitThread()
	//==============================================================
	/// Thread initialization function
	/**
		\see CThread::InitThread()

		\return Return code ignored
	*/
	virtual BOOL InitThread( LPVOID pData );

	/// Constructor
	CAviEncode();

	/// Destructor
	virtual ~CAviEncode();

	//==============================================================
	// IsRecording()
	//==============================================================
	/// Returns non-zero if an AVI is currently available open for writing
	BOOL IsRecording() { return IsRunning() && m_avi.IsOpen(); }

	//==============================================================
	// GetFileName()
	//==============================================================
	/// Returns a pointer to the open filename.
	LPCTSTR GetFileName() { return m_szFile; }

	//==============================================================
	// GetAvi()
	//==============================================================
	/// Returns a pointer to the encapsulated AVI class
	CWinAvi* GetAvi() { return &m_avi; }

	//==============================================================
	// GetRect()
	//==============================================================
	/// Retuns the size of a single frame of video
	LPRECT GetRect() { return &m_rAvi; }

	//==============================================================
	// EnableTimestamp()
	//==============================================================
	/// Enables / disables the timestamp overlay
	/**
		\param [in] bEnable		- Non-zero to enable the timestamp
		\param [in] pString		- Timestamp format string

		\return Return code ignored
	*/
	void EnableTimestamp( BOOL bEnable, LPCTSTR pString )
	{
		m_bTimestamp = bEnable;
		if ( pString )
			strcpy_sz( m_szTimestamp, pString );
	}

	//==============================================================
	// AddTimestamp()
	//==============================================================
	/// Adds a timestamp to the image
	/**
		\param [in] pDc			- Non-zero to enable the timestamp
		\param [in] pString		- Timestamp format string
		\param [in] pTxt		- Text drawing object
		\param [in] dwTime		- UNIX timestamp, 0 for current time

		\return Return code ignored
	*/
	static BOOL AddTimestamp( CWinDc *pDc, LPCTSTR pString, CText *pTxt, DWORD dwTime = 0 );

	//==============================================================
	// SetTimestampFont()
	//==============================================================
	/// Sets the timestamp font
	/**
		\param [in] rgbColor	- Font color
		\param [in] dwFontSize	- Font size
		\param [in] pFont		- Font name

		\return Return code ignored
	*/
	void SetTimestampFont( COLORREF rgbColor, DWORD dwFontSize, LPCTSTR pFont )
	{	m_txt.SetColor( rgbColor );
		m_txt.SetFont( dwFontSize, pFont );
	}

	//==============================================================
	// GetTimestampText()
	//==============================================================
	/// Returns a reference to the timestamp text object
	CText& GetTimestampText() { return m_txt; }

private:

	/// Number of async buffers to create
	DWORD			m_dwCreateBuffers;

	/// CWinDc object queue
	CWinDc			**m_pDc;

	/// Number of buffers in m_pDc
	DWORD			m_dwBuffers;

	/// Event set when data is ready
	HANDLE			*m_phData;

	/// Event set when data needs saving
	HANDLE			m_hSave;

	/// Event set when thread is ready
	HANDLE			m_hReady;

	/// Event set to tell thread to open file
	HANDLE			m_hStartRec;

	/// Event set to tell thread to close file
	HANDLE			m_hStopRec;

	/// Encapsulates AVI functionality
	CWinAvi			m_avi;

	/// AVI filename
	TCHAR			m_szFile[ CWF_STRSIZE ];

	/// Name to give avi file after closing
	TCHAR			m_szRename[ CWF_STRSIZE ];

	/// Size of a single frame of video
	RECT			m_rAvi;

	/// Number of frame in m_dwSeconds seconds
	DWORD			m_dwFrames;

	/// Length of time over-which there are m_dwFrames frames
	DWORD			m_dwSeconds;

	/// Four character code for the compressor
	DWORD			m_dwFourCC;

	/// Archive quality value
	DWORD			m_dwQuality;

	/// Pointer to the next writable frame slot
	DWORD			m_dwHeadFrame;

	/// Pointer tot eh next readable frame slot
	DWORD			m_dwTailFrame;

	/// Non-zero to add timestamp to the image
	BOOL			m_bTimestamp;

	/// Timestamp template
	char			m_szTimestamp[ CWF_STRSIZE ];

	/// Timestamp object
	CText			m_txt;

};

#endif // !defined(AFX_AVIENCODE_H__F6F75054_624E_498E_8632_40C17CE75D13__INCLUDED_)
