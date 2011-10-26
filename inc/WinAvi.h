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
// WinAvi.h: interface for the CWinAvi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINAVI_H__2771129C_4A84_485D_9D43_C467DDFEF132__INCLUDED_)
#define AFX_WINAVI_H__2771129C_4A84_485D_9D43_C467DDFEF132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CWinAvi
//
/// Simple wrapper for the windows AVI API
/**
	Use this class to read / write video frames from an AVI file.	
*/
//==================================================================
class CWinAvi  
{
public:

	//==============================================================
	// GetErrorStr()
	//==============================================================
	/// Returns an error string for AVI errors
	/**
		\param [in] dwError		-	AVI error code
		
		\return Pointer to error code string
	
		\see 
	*/
	static LPCTSTR GetErrorStr( DWORD dwError );

	//==============================================================
	// Close()
	//==============================================================
	/// Closes AVI file 
	void Close();

	//==============================================================
	// OpenExisting()
	//==============================================================
	/// Opens an existing AVI file
	/**
		\param [in] pFile	-	AVI filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL OpenExisting( LPCTSTR pFile );

	//==============================================================
	// AddFrame()
	//==============================================================
	/// Adds a frame of video to the AVI file
	/**
		\param [in] pDC		-	CWinDc object encapsulating the video frame
		
		\return Non-zero if added successfully
	
		\see 
	*/
	BOOL AddFrame(CWinDc *pDC);

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes AVI and releases all resources
	void Destroy();

	//==============================================================
	// OpenNew()
	//==============================================================
	/// Creates a new AVI file, over-writes any existing file
	/**
		\param [in] pFile				-	AVI filename
		\param [in] pRect				-	Video frame size
		\param [in] dwFrames			-	Number of frames in dwSeconds
											seconds.
		\param [in] dwSeconds			-	Number of seconds overwhich
											there are dwFrames frames.
		\param [in] fourCC				-	Four Character Code identifying
											the encoder to use.
		\param [in] pCompData			-	Compressor initialization data.
		\param [in] dwCompDataSize		-	Number of bytes in pCompData.
		\param [in] bCreateDIBSection	-	True to create internal DIB
											section for frame copy.
		\param [in] dwQuality			-	Video quality
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL OpenNew(	LPCTSTR pFile, LPRECT pRect, DWORD dwFrames, DWORD dwSeconds, 
					DWORD fourCC, LPVOID pCompData, DWORD dwCompDataSize, BOOL bCreateDIBSection,
					DWORD dwQuality = MAXDWORD );

	/// Constructor
	CWinAvi();

	/// Destructor
	virtual ~CWinAvi();

	//==============================================================
	// IsOpen()
	//==============================================================
	/// Returns non-zero if AVI file is open
	BOOL IsOpen() { return ( m_paviCompStream != NULL || m_paviStream != NULL ); }

	//==============================================================
	// GetRect()
	//==============================================================
	/// Returns the dimensions of a single video frame
	/**
		\param [in] pRect	-	Receives video frame dimensions.
	*/
	void GetRect( LPRECT pRect )
	{	if ( pRect != NULL ) CopyRect( pRect, &m_aviStreamInfo.rcFrame ); }

	//==============================================================
	// GetFrameRate()
	//==============================================================
	/// Returns the video frame rate
	DWORD GetFrameRate() { return m_aviStreamInfo.dwRate; }

	//==============================================================
	// GetScale()
	//==============================================================
	/// Returns the video frame rate scale
	DWORD GetScale() { return m_aviStreamInfo.dwScale; }

	//==============================================================
	// GetFccType()
	//==============================================================
	/// Returns the Four Character Code of the compressor
	DWORD GetFccType() { return m_aviStreamInfo.fccType; }

	//==============================================================
	// GetFileName()
	//==============================================================
	/// Returns a pointer to the currently open filename
	LPCTSTR GetFileName() { return m_szFile; }

	//==============================================================
	// GetCurFrame()
	//==============================================================
	/// Returns the current frame index
	long GetCurFrame() { return m_lSample; }

	//==============================================================
	// GetError()
	//==============================================================
	/// Returns the last error code
	DWORD GetError() { return m_dwError; }

	//==============================================================
	// GetErrorStr()
	//==============================================================
	/// Returns a pointer to a string describing the last error.
	LPCTSTR GetErrorStr() { return m_szError; }

private:

	/// AVI file information structure
	PAVIFILE			m_paviFile;
	
	/// AVI stream handle
	PAVISTREAM			m_paviStream;

	/// AVI stream information structure
	AVISTREAMINFO		m_aviStreamInfo;

	/// AVI compressed stream handle
	PAVISTREAM			m_paviCompStream;

	/// AVI compressed stream options
	AVICOMPRESSOPTIONS	m_aviCompOptions;

	/// Video format information
	BITMAPINFO			m_bmpInfo;

	/// Current sample index number
	long				m_lSample;

	/// Currently open filename
	char				m_szFile[ CWF_STRSIZE ];

	/// Internal frame buffer
	CWinDc				m_dc;

	/// Last error code
	DWORD				m_dwError;

	/// Last error code description string
	char				m_szError[ 256 ];

};


#endif // !defined(AFX_WINAVI_H__2771129C_4A84_485D_9D43_C467DDFEF132__INCLUDED_)
