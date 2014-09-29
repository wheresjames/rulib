/*******************************************************************
// Copyright (c) 2002, Robert Umbehant ( www.wheresjames.com )
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
// Comp2.h: interface for the CComp2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMP2_H__626F5398_95D9_11D3_BF4B_00104B2C9CFA__INCLUDED_)
#define AFX_COMP2_H__626F5398_95D9_11D3_BF4B_00104B2C9CFA__INCLUDED_

#include "WinFile.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CCOMP2_COMPRESS
//#undef CCOMP2_COMPRESS
#define CCOMP2_EXPAND
//#undef CCOMP2_EXPAND

// Don't use this
//#define CCOMP2_MULTITHREAD
#undef CCOMP2_MULTITHREAD

#ifdef CCOMP2_MULTITHREAD
typedef struct tagCOMPTHREADINFO
{
	HANDLE		hThread;
	DWORD		dwOrder;

	BOOL		bCompress;
	z_stream	*zs;
	int			err;

} COMPTHREADINFO;
typedef COMPTHREADINFO* LPCOMPTHREADINFO;
#endif // CCOMP_MULTITHREAD

//==================================================================
// CComp2
//
/// Simple wrapper for ZLIB compression routines
/**
	Provides a simple wrapper for zlib compression routines.

	Also provides functionality to do compression / decompression in
	another thread to prevent long operations from holding up the
	main thread.

	\warning Must link to the ZLIB library.	
*/
//==================================================================
class CComp2  
{
public:

#ifdef CCOMP2_EXPAND

	//==============================================================
	// ExpandInit()
	//==============================================================
	/// Initializes the ZLIB expand routines
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ExpandInit();

	//==============================================================
	// ExpandContinue()
	//==============================================================
	/// Continues expansion.
	/**
		\param [in] pDone	-	Set to non-zero when complete.

		Initialize pDone to zero, keep calling ExpandContinue until 
		pDone is set to non-zero.  If ExpandContinue() returns zero,
		you should abort.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL ExpandContinue( BOOL *pDone );

	//==============================================================
	// ReadHeader()
	//==============================================================
	/// Reads in the compression header
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadHeader();

#endif

#ifdef CCOMP2_COMPRESS

	//==============================================================
	// CompressInit()
	//==============================================================
	/// Initializes the ZLIB compressor
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CompressInit();

	//==============================================================
	// CompressContinue()
	//==============================================================
	/// Continues compression.
	/**
		\param [in] pDone	-	Set to non-zero when complete.

		Initialize pDone to zero, keep calling CompressContinue() until 
		pDone is set to non-zero.  If CompressContinue() returns zero,
		you should abort.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL CompressContinue( BOOL *pDone );

	//==============================================================
	// WriteHeader()
	//==============================================================
	/// Writes out compression header
	/**
		Writes a custom header that is used to setup the decompressor.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL WriteHeader();

#endif

	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from compressed stream
	/**
		\param [in] buf		-	Buffer that receives data.
		\param [in] size	-	Size of buffer in buf.
		\param [in] read	-	Number of bytes read into buf.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL Read( LPVOID buf, DWORD size, LPDWORD read = NULL );
	
	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to a compressed stream
	/**
		\param [in] buf		-	Pointer to buffer containing data.
		\param [in] size	-	Number of bytes in buf.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( LPVOID buf, DWORD size );

	//==============================================================
	// SetDest()
	//==============================================================
	/// Sets the destination buffer for encoding or decoding
	/**
		\param [in] buf		-	Buffer pointer.
		\param [in] size	-	Size of buffer in buf.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetDest( LPBYTE buf, DWORD size );
	
	//==============================================================
	// SetSource()
	//==============================================================
	/// Sets the source buffer
	/**
		\param [in] buf		-	Pointer to buffer containing source data.
		\param [in] size	-	Size of buffer in buf
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetSource( LPBYTE buf, DWORD size );

	//==============================================================
	// Destroy()
	//==============================================================
	///	Releases all resources for this class
	void Destroy();
	
	//==============================================================
	// SetDest()
	//==============================================================
	/// Sets the destination file for compression / decompression
	/**
		\param [in] pFile	-	Filename
		\param [in] hFile	-	Optional handle to file, used if pFile is NULL.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetDest( LPCTSTR pFile, HANDLE hFile = NULL );

	//==============================================================
	// SetSource()
	//==============================================================
	/// Sets file to use as data source for compression / decompression
	/**
		\param [in] pFile			-	Filename
		\param [in] hFile			-	Optional file handle, used if pFile is NULL
		\param [in] bSaveFileName	-	If set to non-zero, filename is saved to
										compressed stream for later use.
		\param [in] max				-	Maximum number of bytes to read.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetSource( LPCTSTR pFile, HANDLE hFile = NULL, BOOL bSaveFileName = FALSE, DWORD max = 0 );
	
	/// Constructor
	CComp2();

	/// Destructor
	~CComp2();
	
	//==============================================================
	// IsSource()
	//==============================================================
	/// Returns non-zero if a valid source is set
	BOOL IsSource() { return ( m_fSource.IsOpen() || m_pSource != NULL ); }

	//==============================================================
	// IsDest()
	//==============================================================
	/// Returns non-zero if a valid destination is set.
	BOOL IsDest() { return ( m_fDest.IsOpen() || m_pDest != NULL ); }

	//==============================================================
	// GetBytesRead()
	//==============================================================
	/// Returns the number of bytes read from source
	DWORD GetBytesRead() { return m_dwSourcePos; }

	//==============================================================
	// GetBytesWritten()
	//==============================================================
	/// Returns the number of bytes written to destination
	DWORD GetBytesWritten() { return m_dwDestPos; }
	
	//==============================================================
	// GetExpandFileSize()
	//==============================================================
	/// Returns the size of the expanded data
	DWORD GetExpandFileSize() { return m_dwExpandFileSize; }

	//==============================================================
	// GetSourceFileSize()
	//==============================================================
	/// Returns the size of the source file
	DWORD GetSourceFileSize() { return m_dwSource; }

	//==============================================================
	// GetExpandFileName()
	//==============================================================
	/// Returns the expanded file name from compressed stream if it was saved.
	LPCTSTR GetExpandFileName() { return m_szExpandFileName; }

	//==============================================================
	// GetFileID()
	//==============================================================
	/// Returns the file id
	LPCTSTR GetFileID() { return m_szFileID; }

	//==============================================================
	// SetFileID()
	//==============================================================
	/// Sets the file id
	/**
		\param [in] pID		-	File id string

		The file id is used to identify the compressed file	
	*/
	void SetFileID( LPCTSTR pID )
	{ if ( pID == NULL ) *m_szFileID = 0; else strcpy( m_szFileID, pID ); }

private:

	/// Source file object
	CWinFile	m_fSource;

	/// Destination file object
	CWinFile	m_fDest;

	/// Pointer to next source byte to be read
	DWORD		m_dwSourcePos;

	/// Size of buffer in m_pSource
	DWORD		m_dwSource;

	/// Source data buffer
	LPBYTE		m_pSource;	

	/// Position of next byte to be written
	DWORD		m_dwDestPos;

	/// Size of buffer in m_pDest
	DWORD		m_dwDest;

	/// Destination data buffer
	LPBYTE		m_pDest;

	/// CRC of encoded data
	DWORD		m_dwCRC;

	/// Size of the expanded data
	DWORD		m_dwExpandFileSize;

	/// Name of the expanded file
	TCHAR		m_szExpandFileName[ MAX_PATH ];

	/// Compressed file ID
	TCHAR		m_szFileID[ MAX_PATH ];

	/// ZLib data structure
	void		*m_zs;

	/// Set to non-zero if you do not want the source file closed
	BOOL		m_bNoCloseSrc;

	/// Set to non-zero if you do not want the destaination file closed
	BOOL		m_bNoCloseDst;

#ifdef CCOMP2_MULTITHREAD

public:

	//==============================================================
	// Thread()
	//==============================================================
	/// Compression / Decompression Thread
	DWORD FAR PASCAL Thread ( LPVOID pData );

	//==============================================================
	// MTExpandContinue()
	//==============================================================
	/// Handles expansion in a separate thread
	/**
		\param [in] pDone	-	Set to non-zero when complete.

		Initialize pDone to zero, keep calling MTExpandContinue until 
		pDone is set to non-zero.  If MTExpandContinue() returns zero,
		you should abort.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL MTExpandContinue( BOOL *pDone );

	//==============================================================
	// MTCompressContinue()
	//==============================================================
	/// Handles compression in another thread
	/**
		\param [in] pDone	-	Set to non-zero when complete.

		Initialize pDone to zero, keep calling MTCompressContinue() until 
		pDone is set to non-zero.  If MTCompressContinue() returns zero,
		you should abort.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL MTCompressContinue( BOOL *pDone );

private:

	LPCOMPTHREADINFO	m_cti;
#endif // CCOMP_MULTITHREAD

public:
private:
};

#endif // !defined(AFX_COMP2_H__626F5398_95D9_11D3_BF4B_00104B2C9CFA__INCLUDED_)
