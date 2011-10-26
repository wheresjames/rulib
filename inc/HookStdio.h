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
// HookStdio.h: interface for the CHookStdio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOOKSTDIO_H__ED80AE58_E23C_400A_AC53_2A1E7ECB35A7__INCLUDED_)
#define AFX_HOOKSTDIO_H__ED80AE58_E23C_400A_AC53_2A1E7ECB35A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CHookStdio
//
/// Hooks the specified standard stream
/**
	Use this to hook the standard i/o streams

	Example:

	\code

	CHookStdio hs( STD_OUTPUT_HANDLE );

	printf( "Hello World!" );

	TCHAR buf[ 256 ];
	buf[ hs.Read( buf, sizeof( buf ) - 1 ) ] = 0;

	TRACE( buf );

	\endcode
	
*/
//==================================================================
class CHookStdio  
{
public:
	

	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to the stream
	/**
		\param [in] pBuf		-	Buffer containing data to write
		\param [in] dwSize		-	Number of bytes to write
		
		\return Number of bytes actuall written or zero if error.
	
		\see 
	*/
	DWORD Write( LPVOID pBuf, DWORD dwSize );

	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from the stream
	/**
		\param [in] pBuf		-	Buffer that receives the data
		\param [in] dwSize		-	Size of buffer in pBuf
		
		\return Number of bytes actually read
	
		\see 
	*/
	DWORD Read( LPVOID pBuf, DWORD dwSize );

	//==============================================================
	// Stop()
	//==============================================================
	/// Unhooks any stream
	void Stop();

	//==============================================================
	// Start()
	//==============================================================
	///	Hooks the specified stream
	/**
		\param [in] dwStreamId		-	Standard stream ID
											- STD_INPUT_HANDLE		=	Standard input handle 
											- STD_OUTPUT_HANDLE		=	Standard output handle 
											- STD_ERROR_HANDLE		=	Standard error handle 
		\param [in] dwBufferSize	-	The size buffer to use.
		
		\return Non-zero if success
	
		\see CHookStdio(), Stop()
	*/
	BOOL Start( DWORD dwStreamId, DWORD dwBufferSize = 64 * 1024 );

	//==============================================================
	// CHookStdio()
	//==============================================================
	/// Constructor
	/**
		\param [in] dwStreamId	-	Standard stream ID
										- STD_INPUT_HANDLE		=	Standard input handle 
										- STD_OUTPUT_HANDLE		=	Standard output handle 
										- STD_ERROR_HANDLE		=	Standard error handle 
		\param [in] dwBufferSize	-	The size buffer to use.

		The default argument of zero, will not hook any stream

		\see Start(), Stop
	*/
	CHookStdio( DWORD dwStreamId = 0, DWORD dwBufferSize = 0 );

	/// Destructor
	virtual ~CHookStdio();

	//==============================================================
	// GetReadHandle()
	//==============================================================
	/// Returns the read handle
	HANDLE GetReadHandle() { return m_hRead; }

	//==============================================================
	// GetReadHandle()
	//==============================================================
	/// Returns the read handle
	HANDLE GetWriteHandle() { return m_hRead; }

private:

	/// The stream that is hooked
	DWORD					m_dwStreamId;

	/// Write handle
	HANDLE					m_hWrite;

	/// Read handle
	HANDLE					m_hRead;

	/// Original handle
	HANDLE					m_hOriginal;

};

#endif // !defined(AFX_HOOKSTDIO_H__ED80AE58_E23C_400A_AC53_2A1E7ECB35A7__INCLUDED_)
