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
// Capture.h: interface for the CCapture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPTURE_H__6E394A44_DD33_11D2_891F_00104B2C9CFA__INCLUDED_)
#define AFX_CAPTURE_H__6E394A44_DD33_11D2_891F_00104B2C9CFA__INCLUDED_

#include <VFW.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define	CCAPTURE_INVALIDDRIVER		0xffff

#define CCAPTURE_MAXDRIVERS			10

#define CCAPTURE_DLG_NONE			0
#define CCAPTURE_DLG_VIDSOURCE		1
#define CCAPTURE_DLG_VIDFORMAT		2
#define CCAPTURE_DLG_VIDDISPLAY		3
#define CCAPTURE_DLG_VIDCOMPRESSION	4

typedef struct tagCAPTUREDRIVERINFO
{
	DWORD	index;
	TCHAR	szName[ MAX_PATH ];
	TCHAR	szVer[ MAX_PATH ];

} CAPTUREDRIVERINFO; // end typedef struct
typedef CAPTUREDRIVERINFO* LPCAPTUREDRIVERINFO;

class CCapture;
typedef struct tagFRAMECALLBACKINFO
{
	LPVIDEOHDR		pVHdr;
	CCapture		*pCap;

} FRAMECALLBACKINFO, *LPFRAMECALLBACKINFO; // end typedef struct

//==================================================================
// CCapture
//
/// Encapsulates the windows VFW capture API
/**
	This class manages a single VFW capture device.
*/
//==================================================================
class CCapture  
{
public:

	//==============================================================
	// ShowDialogBox()
	//==============================================================
	/// Displays the specified dialog box
	/**
		\param [in] db	-	The dialog box to display.
							- CCAPTURE_DLG_VIDSOURCE		-	Video source
							- CCAPTURE_DLG_VIDFORMAT		-	Video Format
							- CCAPTURE_DLG_VIDDISPLAY		-	Video display
							- CCAPTURE_DLG_VIDCOMPRESSION	-	Video compression

		\return Returns non-zero if success.
	*/
	BOOL ShowDialogBox( DWORD db );

	//==============================================================
	// LoadDriverInfo()
	//==============================================================
	/// Loads information about drivers into m_cdi
	/**
		\return Returns non-zero if success.
	*/
	BOOL LoadDriverInfo();
	
	//==============================================================
	// Connect()
	//==============================================================
	/// Connects to the specified device driver
	/**
		\param [in] i	-	The index of the driver to connect to.

		\return Returns non-zero if success.
	*/
	BOOL Connect( DWORD i );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes any open driver and releases all resources
	void Destroy();

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a capture window
	/**
		\param [in] pTitle		-	Title for the capture window.
		\param [in] hwndParent	-	Handle of the parent window.
		\param [in] dwStyle		-	Window styles for the capture window.
		\param [in] x			-	Horizontal offset for the capture window.
		\param [in] y			-	Vertical offset for the capture window.
		\param [in] width		-	Width of the capture window.
		\param [in] height		-	Height of the capture window.
		\param [in] nID			-	Contorl id for the capture window.

		\return Returns non-zero if success.
	*/
	BOOL Create(	LPCTSTR pTitle = NULL, HWND hwndParent = NULL, 
					DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW, 
					long x = 10, long y = 10, 
					long width = 320, long height = 240, 
					int nID = 0 );

	/// Constructor
	CCapture();

	/// Destructor
	virtual ~CCapture();

	//==============================================================
	// GetSafeHwnd()
	//==============================================================
	/// Returns the capture window handle.
	HWND GetSafeHwnd() { return m_hWnd; }

	//==============================================================
	// IsWnd()
	//==============================================================
	/// Returns non-zero if there is a valid capture window
	BOOL IsWnd() { return ( m_hWnd != NULL && IsWindow( m_hWnd ) ); }

	//==============================================================
	// IsConnected()
	//==============================================================
	/// Returns non-zero if there is an open capture device.
	BOOL IsConnected() { return m_bConnected; }

	//==============================================================
	// GetCurrentDriverInfo()
	//==============================================================
	/// Retrieves information about the current capture device
	/**
		\param [out] pcdi	-	Structure that receives information about the
								currently open capture driver.
		\return Returns non-zero if success.
	*/
	BOOL GetCurrentDriverInfo( LPCAPTUREDRIVERINFO pcdi )
	{	if ( !IsConnected() ) return FALSE;
		return GetDriverInfo( m_dwCurrentDriver, pcdi );
	}

	//==============================================================
	// GetDriverInfo()
	//==============================================================
	/// Retrieves information about the specified capture device
	/**
		\param [in] i		-	Index of the capture driver to query.
		\param [out] pcdi	-	Structure that receives information about the
								currently open capture driver.

		\return Returns non-zero if success.
	*/
	BOOL GetDriverInfo( DWORD i, LPCAPTUREDRIVERINFO pcdi )
	{	if ( !m_dwNumDrivers || !LoadDriverInfo() ) return FALSE;
		if ( pcdi == NULL || i >= m_dwNumDrivers ) return FALSE;
		memcpy( pcdi, &m_cdi[ i ], sizeof( CAPTUREDRIVERINFO ) );
		return TRUE;
	}

	//==============================================================
	// GetNumDrivers()
	//==============================================================
	/// Retuns the number of installed capture devices.
	DWORD GetNumDrivers()
	{ if ( !m_dwNumDrivers ) LoadDriverInfo(); return m_dwNumDrivers; }

	//==============================================================
	// GetStatus()
	//==============================================================
	/// Returns non-zero if a capture device is connected
	BOOL GetStatus()
	{ return capGetStatus( m_hWnd, &m_cs, sizeof( m_cs ) ); }

	//==============================================================
	// ShowVideoSourceDlg()
	//==============================================================
	/// Displays the video source dialog box
	/**
		\return Returns non-zero if success.
	*/
	BOOL ShowVideoSourceDlg() { return ShowDialogBox( CCAPTURE_DLG_VIDSOURCE ); }

	//==============================================================
	// ShowVideoFormatDlg()
	//==============================================================
	/// Displays the video format dialog box
	/**
		\return Returns non-zero if success.
	*/
	BOOL ShowVideoFormatDlg() { return ShowDialogBox( CCAPTURE_DLG_VIDFORMAT ); }

	//==============================================================
	// ShowVideoDisplayDlg()
	//==============================================================
	/// Displays the video display dialog box
	/**
		\return Returns non-zero if success.
	*/
	BOOL ShowVideoDisplayDlg() { return ShowDialogBox( CCAPTURE_DLG_VIDDISPLAY ); }

	//==============================================================
	// ShowVideoCompressionDlg()
	//==============================================================
	/// Displays the video compression dialog box
	/**
		\return Returns non-zero if success.
	*/
	BOOL ShowVideoCompressionDlg() { return ShowDialogBox( CCAPTURE_DLG_VIDCOMPRESSION ); }

	//==============================================================
	// Preview()
	//==============================================================
	/// Enables / disables preview mode
	/**
		\param [in] en		-	Set to non-zero to enable preview mode.  Zero to disable.

		\return Returns non-zero if success.
	*/
	BOOL Preview( BOOL en ) { return capPreview( m_hWnd, en ); };

	//==============================================================
	// SetPreviewRate()
	//==============================================================
	/// Sets the frame rate for preview mode
	/**
		\param [in] rate	-	Frame rate of video preview

		\return Returns non-zero if success.
	*/
	BOOL SetPreviewRate( DWORD rate = 30 ) { return capPreviewRate( m_hWnd, rate ); }
	
	//==============================================================
	// PreviewStretching()
	//==============================================================
	/// Enables / disables stretching of the preview video
	/**
		\param [in] en		-	Set to non-zero to enable stretching.  Zero to disable.

		\return Returns non-zero if success.
	*/
	BOOL PreviewStretching( BOOL en ) { return capPreviewScale( m_hWnd, en ); }

	//==============================================================
	// EnableOverlay()
	//==============================================================
	/// Enables / disables video overlay mode
	/**
		\param [in] en		-	Set to non-zero to enable overlay.  Zero to disable.

		\return Returns non-zero if success.
	*/
	BOOL EnableOverlay( BOOL en )
	{	if ( !m_cdc.fHasOverlay ) return FALSE;
		return capOverlay( m_hWnd, en );
	}

	//==============================================================
	// SetCaptureFileName()
	//==============================================================
	/// Sets the capture filename
	/**
		\param [in] pName	-	The filename for capture

		\return Returns non-zero if success.
	*/
	BOOL SetCaptureFileName( LPCTSTR pName )
	{	if ( pName == NULL ) return FALSE; 
		if ( !capFileSetCaptureFile( m_hWnd, pName ) ) return FALSE;
		strcpy( m_szCaptureFileName, pName );
		return TRUE;
	}

	//==============================================================
	// GetCaptureFileName()
	//==============================================================
	/// Returns the currently set capture filename
	LPCTSTR GetCaptureFileName() 
	{	GetCaptureFileName( m_szCaptureFileName );
		return m_szCaptureFileName; 
	}

	//==============================================================
	// GetCaptureFileName()
	//==============================================================
	/// Copies the currently set capture filename to the specified buffer
	/**
		\param [in] buf		-	Character buffer that receives the filename.

		\return Returns non-zero if success.
	*/
	BOOL GetCaptureFileName( LPSTR buf )
	{	return capFileGetCaptureFile( m_hWnd, buf, MAX_PATH - 1 ); }

	//==============================================================
	// FileAlloc()
	//==============================================================
	/// Pre allocates disk space for video capture.
	/**
		\param [in] size	-	The amount of video space to allocate.

		\return Returns non-zero if success.
	*/
	BOOL FileAlloc( DWORD size ) 
	{	return capFileAlloc( m_hWnd, size ); }

	//==============================================================
	// CaptureToFile()
	//==============================================================
	/// Starts capture of data to file
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureToFile()
	{	return capCaptureSequence( m_hWnd ); }

	//==============================================================
	// SaveAs()
	//==============================================================
	/// Copies the contents of the capture file to another.
	/**
		\param [in] pName	-	New filename.

		If a larger amount of data was allocated with FileAlloc() than
		was actually used.   This function copies only the used data.

		\return Returns non-zero if success.
	*/
	BOOL SaveAs( LPCTSTR pName )
	{	return capFileSaveAs( m_hWnd, pName ); }

	//==============================================================
	// IsAudioFormatData()
	//==============================================================
	/// Returns non-zero if there is audio format data
	BOOL IsAudioFormatData() { return ( m_pAudioFormat != NULL ); }

	//==============================================================
	// SetAudioFormat()
	//==============================================================
	/// Sets the audio format of the capture device.
	/**
		\return Returns non-zero if success.
	*/
	BOOL SetAudioFormat() 
	{	if ( !IsAudioFormatData() ) return FALSE;
		return capSetAudioFormat( m_hWnd, m_pAudioFormat, sizeof( m_dwAudioDataSize ) ); 
	}

	//==============================================================
	// GetCaptureSetup()
	//==============================================================
	/// Retrieves capture setup data to m_cp
	/**
		\return Returns non-zero if success.
	*/
	BOOL GetCaptureSetup() 
	{	return capCaptureGetSetup( m_hWnd, &m_cp, sizeof( m_cp ) ); }

	//==============================================================
	// SetCaptureSetup()
	//==============================================================
	/// Sets capture setup data in m_cp to the device
	/**
		\return Returns non-zero if success.
	*/
	BOOL SetCaptureSetup() 
	{	return capCaptureSetSetup( m_hWnd, &m_cp, sizeof( m_cp ) ); }

	//==============================================================
	// SetCaptureAudio()
	//==============================================================
	/// Set audio capture flag in capture setup structure m_cp
	void SetCaptureAudio( BOOL ca )
	{ GetCaptureSetup(); m_cp.fCaptureAudio = ca; SetCaptureSetup(); }

	//==============================================================
	// SetYield()
	//==============================================================
	/// Sets yield bit in capture setup structure m_cp
	void SetYield( BOOL yield )
	{ GetCaptureSetup(); m_cp.fYield = yield; SetCaptureSetup(); }


	//==============================================================
	// CaptureSingleFrameOpen()
	//==============================================================
	/// Opens the capture file for single frame capture
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureSingleFrameOpen()
	{	return capCaptureSingleFrameOpen( m_hWnd ); }

	//==============================================================
	// CaptureSingleFrameClose()
	//==============================================================
	/// Closes the capture file for single frame capture
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureSingleFrameClose()
	{	return capCaptureSingleFrameClose( m_hWnd ); }

	//==============================================================
	// CaptureSingleFrame()
	//==============================================================
	/// Appends a single frame of video to the capture file
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureSingleFrame()
	{	return capCaptureSingleFrame( m_hWnd ); }

	//==============================================================
	// CaptureSequence()
	//==============================================================
	/// Starts streaming video capture to a file.
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureSequence()
	{	return capCaptureSequence( m_hWnd ); }

	//==============================================================
	// CaptureSequenceNoFile()
	//==============================================================
	/// Starts streaming video capture without writing to file.
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureSequenceNoFile()
	{	return capCaptureSequenceNoFile( m_hWnd ); }

	//==============================================================
	// CaptureStop()
	//==============================================================
	/// Stops current video capture
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureStop()
	{	return capCaptureStop( m_hWnd ); }

	//==============================================================
	// CaptureAbort()
	//==============================================================
	/// Aborts the current capture
	/**
		\return Returns non-zero if success.
	*/
	BOOL CaptureAbort()
	{	return capCaptureAbort( m_hWnd ); }

	//==============================================================
	// CopyToClipboard()
	//==============================================================
	/// Copies the current video frame and palette to the clipboard
	/**
		\return Returns non-zero if success.
	*/
	BOOL CopyToClipboard()
	{	return capEditCopy( m_hWnd ); }	  

	//==============================================================
	// SaveAsDIB()
	//==============================================================
	/// Saves the current frame of video as a DIB file
	/**
		\param [in] pFile	-	File name for DIB.

		\return Returns non-zero if success.
	*/
	BOOL SaveAsDIB( LPCTSTR pFile )
	{	if ( pFile == NULL ) return FALSE;
		return capFileSaveDIB( m_hWnd, pFile ); 
	}
	
	//==============================================================
	// SetMCIDeviceName()
	//==============================================================
	/// Specifies the name of the MCI video device to use for capture.
	/**
		\param [in] pName	-	Name of the MCI device.

		\return Returns non-zero if success.
	*/
	BOOL SetMCIDeviceName( LPCTSTR pName )
	{	return capSetMCIDeviceName( m_hWnd, pName ); }

	//==============================================================
	// GetMCIDeviceName()
	//==============================================================
	/// Returns the name of the current MCI device being used for capture.
	/**
		\param [in] pName	-	Address of a buffer that receives the MCI device name.

		\return Returns non-zero if success.
	*/
	BOOL GetMCIDeviceName( LPSTR pName )
	{	return capGetMCIDeviceName( m_hWnd, pName, MAX_PATH - 1 ); }

	//==============================================================
	// SetUserData()
	//==============================================================
	/// Associates a four byte value with the current capture window.
	/**
		\param [in] data	-	Data to associate.

		\return Returns non-zero if success.
	*/
	BOOL SetUserData( long data )
	{	return capSetUserData( m_hWnd, data ); }

	//==============================================================
	// GetUserData()
	//==============================================================
	/// Returns the associated four byte value with the current capture window.
	/**
		\return Returns four byte user value.
	*/
	long GetUserData()
	{	return (long)capGetUserData( m_hWnd ); }

	//==============================================================
	// GrabFrame()
	//==============================================================
	/// Grabs a single frame of video from the current video device.
	/**
		\return Returns non-zero if success.
	*/
	BOOL GrabFrame();

	//==============================================================
	// GrabFrameNoStop()
	//==============================================================
	/// Grabs a single frame of video from the current video device without stopping the capture.
	/**
		\return Returns non-zero if success.
	*/
	BOOL GrabFrameNoStop()
	{	return capGrabFrameNoStop( m_hWnd ); }

	//==============================================================
	// WaitGrab()
	//==============================================================
	/// Waits for a grab operation to complete
	/**
		\param [in] timeout	-	Maximum amount of time, in milli-seconds, to wait for
								a grab operation to complete.

		\return Returns non-zero if success.
	*/
	BOOL WaitGrab( DWORD timeout = 3000 )
	{	return ( WaitForSingleObject( m_hGrab, timeout ) != WAIT_TIMEOUT ); }

	//==============================================================
	// PaletteAuto()
	//==============================================================
	/// Requests the capture driver automatically create new palettes for video frames.
	/**
		\param [in] frames		-	Number of frames to sample.
		\param [in] colors		-	Number of colors in palette, cannot be more than 256.

		\return Returns non-zero if success.
	*/
	BOOL PaletteAuto( DWORD frames, DWORD colors )
	{	return capPaletteAuto( m_hWnd, frames, colors ); }

	//==============================================================
	// PaletteManual()
	//==============================================================
	/// Requests the capture driver manually sample video frames.
	/**
		\param [in] grab		-	Set to true for each frame included in palette.
		\param [in] colors		-	Number of colors in palette, cannot be more than 256.
									This value is only used on the first frame of the sequence.

		Call this function for each frame you want to include in the palette.

		\return Returns non-zero if success.
	*/
	BOOL PaletteManual( BOOL grab, DWORD colors )
	{	return capPaletteManual( m_hWnd, grab, colors ); }

	//==============================================================
	// LoadPalette()
	//==============================================================
	/// Loads a color palette from a file.
	/**
		\param [in] pFile	-	File name of the palette.

		\return Returns non-zero if success.
	*/
	BOOL LoadPalette( LPCTSTR pFile )
	{	return capPaletteOpen( m_hWnd, pFile ); }

	//==============================================================
	// GetPaletteFromClipboard()
	//==============================================================
	/// Loads a color palette from the clipboard
	/**
		\return Returns non-zero if success.
	*/
	BOOL GetPaletteFromClipboard()
	{	return capPalettePaste( m_hWnd ); }

	//==============================================================
	// SavePalette()
	//==============================================================
	/// Saves the color palette to a file
	/**
		\param [in] pFile	-	File name of the palette.

		\return Returns non-zero if success.
	*/
	BOOL SavePalette( LPCTSTR pFile )
	{	return capPaletteSave( m_hWnd, pFile ); }

	//==============================================================
	// SetScrollPos()
	//==============================================================
	/// Sets the offset of the upper left corner of the video capture window.
	/**
		\param [in] x	-	Horizontal offset of video capture window.
		\param [in] y	-	Vertical offset of video capture window.

		\return Returns non-zero if success.
	*/
	BOOL SetScrollPos( long x, long y )
	{	POINT	pt = { x, y };
		return capSetScrollPos( m_hWnd, &pt );
	}

	//==============================================================
	// GetNumStatusCallbacks()
	//==============================================================
	/// Returns the nnumber of status callbacks since start of capture.
	DWORD GetNumStatusCallbacks() { return m_dwOnStatus; }

	//==============================================================
	// GetNumErrorCallbacks()
	//==============================================================
	/// Returns the number of error callbacks since start of capture.
	DWORD GetNumErrorCallbacks() { return m_dwOnError; }

	//==============================================================
	// GetNumFrameCallbacks()
	//==============================================================
	/// Returns the number of frame callbacks since the start of capture.
	DWORD GetNumFrameCallbacks() { return m_dwOnFrame; }

	//==============================================================
	// CallbacksEnabled()
	//==============================================================
	/// Returns non-zero if callbacks are enabled.
	BOOL CallbacksEnabled() { return m_bCallbacksEnabled; }

	//==============================================================
	// SetPos()
	//==============================================================
	/// Sets the offset of the capture window with respect to it's parent window
	/**
		\param [in] x	-	Horizontal offset of video capture window.
		\param [in] y	-	Vertical offset of video capture window.

		\return Returns non-zero if success.
	*/
	BOOL SetPos( long x, long y )
	{	return SetWindowPos( m_hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE ); }

	//==============================================================
	// SetSize()
	//==============================================================
	/// Sets the size of the capture window
	/**
		\param [in] x	-	Horizontal size of video capture window.
		\param [in] y	-	Vertical size of video capture window.

		\return Returns non-zero if success.
	*/
	BOOL SetSize( long x, long y )
	{	return SetWindowPos( m_hWnd, NULL, 0, 0, x, y, SWP_NOZORDER | SWP_NOMOVE ); }

	//==============================================================
	// SetWindow()
	//==============================================================
	/// Sets the size of the capture window.
	/**
		\param [in] pRect	-	Rectangle defining new capture window position with
								respect to the parent window.

		\return Returns non-zero if success.
	*/
	BOOL SetWindow( LPRECT pRect )
	{ 	if ( pRect == NULL ) return FALSE;
		return SetWindowPos(	m_hWnd, NULL, 
								pRect->left, 
								pRect->top, 
								pRect->right - pRect->left,
								pRect->bottom - pRect->top,
								SWP_NOZORDER ); 
	}
	
	//==============================================================
	// ShowWindow()
	//==============================================================
	/// Sets the capture windows show state
	/**
		\param [in] uCmdShow	-	Flag specifying how the capture window is to be
									shown.

		See the Windows API function ::ShowWindow()

		\return Returns non-zero if success.
	*/
	BOOL ShowWindow( UINT uCmdShow = SW_SHOWNORMAL )
	{	return ::ShowWindow( m_hWnd, uCmdShow ); }

	//==============================================================
	// LockDC()
	//==============================================================
	/// Locks the DC.
	/**
		Use this function to coordinate multi thread access to video frames.
		If this function returns zero, do not attempt to access the video buffers.

		\return Returns non-zero if buffer was locked.
	*/
	BOOL LockDC() { return m_dclock.lock( 0 ); }

	//==============================================================
	// UnlockDC()
	//==============================================================
	/// Unlocks the DC
	/**
		Use this function to coordinate multi thread access to video frames
	*/
	void UnlockDC() { m_dclock.unlock(); }

	//==============================================================
	// IsInCallback()
	//==============================================================
	/// Returns non-zero if the capture thread is currently in a callback.
	BOOL IsInCallback() { return m_lockInCallback.IsLocked(); }

private:

	/// HDC that receives a copy of the video frame
	HDC			m_hDrawDc;

	/// Draw DIB handle used to decode video frames
	HDRAWDIB 	m_hDrawDib;

	/// Size of the decoded video
	RECT		m_rectDraw;

	/// Window that gets callback messages on video frame events
	HWND		m_hFrameWnd;

	/// Window message value used for video frame event callbacks
	UINT		m_uFrameMsg;

	/// Non-zero if video frame event callbacks are enabled
	BOOL		m_bCallbacksEnabled;

	/// Video status code
	DWORD		m_dwOnStatus;

	/// Video error code
	DWORD		m_dwOnError;

	/// Video frame code
	DWORD		m_dwOnFrame;

protected:

	//==============================================================
	// StatusCallbackProc()
	//==============================================================
	/// Static function proxies video status callbacks
	static LRESULT PASCAL StatusCallbackProc(HWND hWnd, int nID, LPSTR lpStatusText);

	//==============================================================
	// ErrorCallbackProc()
	//==============================================================
	/// Static function proxies video error callbacks
	static LRESULT PASCAL ErrorCallbackProc(HWND hWnd, int nErrID, LPSTR lpErrorText);

	//==============================================================
	// FrameCallbackProc()
	//==============================================================
	/// Static function proxies video frame callbacks
	static LRESULT PASCAL FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);

	//==============================================================
	// OnStatus()
	//==============================================================
	/// Resolved video status callback
	virtual BOOL OnStatus( int nID, LPCTSTR pStatus );

	//==============================================================
	// OnError()
	//==============================================================
	/// Resolved video error callback
	virtual BOOL OnError( int nID, LPCTSTR pStatus );

	//==============================================================
	// OnFrame()
	//==============================================================
	/// Resolved video frame callback
	/**
		\param [in] pVHdr	-	Video header information.

		Video header structure
		
		\code
		
		typedef struct 
		{
			LPSTR lpData;            // address of video buffer
			DWORD dwBufferLength;    // size, in bytes, of the 
									 // data buffer
			DWORD dwBytesUsed;       // see below
			DWORD dwTimeCaptured;    // see below
			DWORD dwUser;            // user-specific data
			DWORD dwFlags;           // see below
			DWORD dwReserved[4];     // reserved; do not use
		} VIDEOHDR;

		\endcode
 
		- Members -

		<b>dwBytesUsed</b>:
		Number of bytes used in the data buffer. 

		<b>dwTimeCaptured</b>:
		Time, in milliseconds, when the frame was captured relative to 
		the first frame in the stream. 
		Synchronization of audio and video in the resulting AVI file 
		depends solely on this parameter, which should be derived 
		directly from a counter (VSYNC_Count) incremented by a vertical 
		sync interrupt. When using NTSC frame rates and frame-based 
		interrupts, the returned value would be: 

		<b>dwTimeCaptured</b> = VSYNC_Count * 1/29.97.
 
		The capture driver must maintain VSYNC_Count accurately by 
		allowing recursive entry into the interrupt service routine 
		(if only for the purpose of incrementing VSYNC_Count) and by not
		disabling interrupts for long periods of time. 

		<b>dwFlags</b>:
		Flags giving information about the data buffer. The following 
		flags are defined for this field: VHDR_DONE  Set by the device 
		driver to indicate it is finished with the data buffer and it is
		returning the buffer to the application.  

		VHDR_PREPARED	Set by the system to indicate the data buffer has 
						been prepared with videoStreamPrepareHeader.  
		VHDR_INQUEUE	Set by the system to indicate the data buffer is 
						queued for playback.  
		VHDR_KEYFRAME	Set by the device driver to indicate a key frame.  
	*/
	virtual BOOL OnFrame( LPVIDEOHDR pVHdr );

private:

	/// Window handle of capture window
	HWND				m_hWnd;

	/// Non-zero if capture device is connected
	BOOL				m_bConnected;

	/// Zero based index of current capture driver
	DWORD				m_dwCurrentDriver;

	/// Total number of cature drivers installed
	DWORD				m_dwNumDrivers;

	/// Information about installed capture drivers
	CAPTUREDRIVERINFO	m_cdi[ CCAPTURE_MAXDRIVERS ];

	/// Video format information
	LPBITMAPINFO		m_pVideoFormat;

	/// Pointer to custom video data
	BYTE				*m_pVideoData;

	/// Size of buffer in m_pVideoData
	DWORD				m_dwVideoDataSize;

	/// Currently set capture filename
	TCHAR				m_szCaptureFileName[ CWF_STRSIZE ];
	
	/// Audio format information
	LPWAVEFORMATEX		m_pAudioFormat;

	/// Pointer to custom audio format data
	BYTE				*m_pAudioData;

	/// Size of buffer in m_pAudioData
	DWORD				m_dwAudioDataSize;

	/// Handle of grab event
	HANDLE				m_hGrab;

	/// Frame callback information
	FRAMECALLBACKINFO	m_fci;

	/// Lock for callback sync.
	CTlLock		m_lockInCallback;

public:

	//==============================================================
	// Reconnect()
	//==============================================================
	/// Attempts reconnect of current capture device
	/**
		\return Returns non-zero if success.
	*/
	BOOL Reconnect();

	//==============================================================
	// GetDriverIndex()
	//==============================================================
	/// Returns the index of the current capture device
	DWORD GetDriverIndex() { return m_dwCurrentDriver; }

	//==============================================================
	// SetFrameMessageTarget()
	//==============================================================
	/// Sets the window handle and message id for video event callbacks
	/**
		\param [in] hDC		-	DC that will receive each video frame.
		\param [in] pRect	-	Size of the DC in hDC.
		\param [in] hWnd	-	Window that receives the callback.
		\param [in] uMsg	-	Message ID for callbacks.

		\return Returns non-zero if success.
	*/
	BOOL SetFrameMessageTarget( HDC hDC, LPRECT pRect, HWND hWnd, UINT uMsg );

	//==============================================================
	// SetCaptureWindowPos()
	//==============================================================
	/// Sets the Z-Order, position and size of the capture window.
	/**
		\param [in] hwndAfter	-	Puts the capture window after this window in the Z-Order.
		\param [in] x			-	Horizontal offset of upper left corner of capture window.
		\param [in] y			-	Veritical offset of upper left corner of capture window.
		\param [in] cx			-	Horizontal size of capture window.
		\param [in] cy			-	Vertical size of capture window.
		\param [in] flags		-	Flags to pass to SetWindowPos()

		\return Returns non-zero if success.
	*/
	BOOL SetCaptureWindowPos( HWND hwndAfter, int x, int y, int cx, int cy, UINT flags );

	//==============================================================
	// HBitmapFromFrame()
	//==============================================================
	/// Converts the current frame to HBITMAP
	/**
		\param [out] phbmp	-	Receives the handle of the newly created HBITMAP.
		\param [out] pvhdr	-	Video header for frame.

		\return Returns non-zero if success.
	*/
	BOOL HBitmapFromFrame( HBITMAP *phbmp, LPVIDEOHDR pvhdr );

	//==============================================================
	// ReleaseVideoFormatData()
	//==============================================================
	/// Releases video format data
	void ReleaseVideoFormatData();

	//==============================================================
	// ReleaseAudioFormatData()
	//==============================================================
	/// Releases audio format data
	void ReleaseAudioFormatData();

	//==============================================================
	// GetAudioFormat()
	//==============================================================
	/// Gets the audio format information
	/**
		\param [out] ppwfx	-	Receives the audio format data.

		\return Returns non-zero if success.
	*/
	BOOL GetAudioFormat( LPWAVEFORMATEX *ppwfx = NULL );

	//==============================================================
	// Disconnect()
	//==============================================================
	/// Disconnects current capture device
	void Disconnect();

	//==============================================================
	// DisableCallbacks()
	//==============================================================
	/// Disables frame callbacks
	void DisableCallbacks();

	//==============================================================
	// EnableCallbacks()
	//==============================================================
	/// Enables frame callbacks
	/**
		\return Returns non-zero if success.
	*/
	BOOL EnableCallbacks();

	//==============================================================
	// AddInfoChunk()
	//==============================================================
	/// Adds information chunk to AVI file at current position
	/**
		\param [in] type	-	Value specifying info chunk type.
		\param [in] buf		-	Buffer containing info chunk data.
		\param [in] size	-	size of the buffer in buf.
		
		Use this function to add arbitrary data into the current
		AVI video stream.

		\return Returns non-zero if success.
	*/
	BOOL AddInfoChunk( FOURCC type, LPBYTE buf, DWORD size );

	//==============================================================
	// GetVideoFormat()
	//==============================================================
	/// Returns information about the current video format
	/**
		\param [out] ppbmp	-	Structure to receive current video format information.
		\param [out] psize	-	Receives the size of the data copied to ppbmp.

		\return Returns non-zero if success.
	*/
	BOOL GetVideoFormat( LPBITMAPINFO *ppbmp = NULL, LPDWORD psize = NULL );

	//==============================================================
	// SetVideoFormat()
	//==============================================================
	/// Sets the current video format
	/**
		\param [in] pbmp	-	Structure containing video format information.
		\param [in] size	-	Receives the size of the data copied to pbmp.

		\return Returns non-zero if success.
	*/
	BOOL SetVideoFormat( LPBITMAPINFO pbmp, DWORD size )
	{	return capSetVideoFormat( m_hWnd, pbmp, size ); }

	/// Current driver capabilities
	CAPDRIVERCAPS		m_cdc;

	/// Current driver status ( call UpdateStatus() )
	CAPSTATUS			m_cs;

	/// Capture params
	CAPTUREPARMS		m_cp;

	/// Video capture access lock
	TThreadSafe< BOOL >	m_capturelock;

	/// Video decode DC access lock
	TThreadSafe< BOOL > m_dclock;

};

#endif // !defined(AFX_CAPTURE_H__6E394A44_DD33_11D2_891F_00104B2C9CFA__INCLUDED_)
