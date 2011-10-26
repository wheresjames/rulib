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
// FifoSync.h: interface for the CFifoSync class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIFOSYNC_H__B6145F65_413B_459A_A36D_366E21AB03B0__INCLUDED_)
#define AFX_FIFOSYNC_H__B6145F65_413B_459A_A36D_366E21AB03B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CFifoSync
//
/// Implements a first in first out synchronized buffer
/**
	Use this to add variable length blocks into a synchronized
	first-in-first-out buffer.
*/
//==================================================================
class CFifoSync : public CCircBuf
{
public:

	//==============================================================
	// SkipBlock()
	//==============================================================
	/// Skips the next data block in the buffer
	/**
		\return Non-zero if success
	*/
	BOOL SkipBlock();

	//==============================================================
	// EndFifoWrite()
	//==============================================================
	/// Advances head fifo head pointer, i.e. commits write
	/**		
		\return Non-zero if success
	*/
	BOOL EndFifoWrite();

	//==============================================================
	// InitFifoWrite()
	//==============================================================
	/// Ensures space for a fifo write
	/**		
		\return Non-zero if success
	*/
	BOOL InitFifoWrite();

	//==============================================================
	// AddFifo()
	//==============================================================
	/// Adds a data block to the fifo
	/**
		\param [in] pBuf		-	Pointer to the data to add
		\param [in] dwSize		-	Size of the data in pBuf
		\param [in] dwEncode	-	Optional encoding
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddFifo( void const * pBuf, DWORD dwSize, DWORD dwEncode = 0 );

	//==============================================================
	// AllocateBuffers()
	//==============================================================
	/// Allocates buffers for fifo
	/**							  		
		\return Non-zero if success
	*/
	BOOL AllocateBuffers();

	/// Constructor
	CFifoSync();

	/// Destructor
	virtual ~CFifoSync();

	//==============================================================
	// Write()
	//==============================================================
	/// Writes a block to the fifo
	/**
		\param [in] pBuf		-	Pointer to data block
		\param [in] dwSize		-	Size of data in pBuf
		\param [in] dwEncode	-	Optional encoding
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL Write( void const * pBuf, DWORD dwSize, DWORD dwEncode = 0 );

	//==============================================================
	// Read()
	//==============================================================
	/// Reads a block of data from the fifo
	/**
		\param [out] pBuf		-	Pointer to buffer that receives the data
		\param [in] dwSize		-	Size of buffer in pBuf
		\param [in] pdwRead		-	Number of bytes read into pBuf
		\param [in] dwEncode	-	Optional decoding
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL Read( LPVOID pBuf, DWORD dwSize, LPDWORD pdwRead, DWORD dwEncode = 0 );

	//==============================================================
	// Peek()
	//==============================================================
	/// Peeks at the data in the fifo buffer
	/**
		\param [in] pBuf		-	Pointer to buffer that receives data
		\param [in] dwSize		-	Size of buffer in pBuf
		\param [in] pdwRead		-	Number of bytes read into pBuf
		\param [in] lOffset		-	Offset from first byte to start read
		\param [in] dwEncode	-	Optional decoding
		
		\return 
	
		\see 
	*/
	virtual BOOL Peek( LPVOID pBuf, DWORD dwSize, LPDWORD pdwRead, long lOffset = 0, DWORD dwEncode = 0 );

	//==============================================================
	// GetMaxBuffers()
	//==============================================================
	/// Returns the number of total buffer slots in the fifo
	DWORD GetMaxBuffers() { return m_dwMaxBuffers; }

	//==============================================================
	// SetMaxBuffers()
	//==============================================================
	/// Sets the total number of buffers slots in the fifo
	/**
		\param [in] dw	-	Number of buffers
	*/
	void SetMaxBuffers( DWORD dw ) { m_dwMaxBuffers = dw; }

	//==============================================================
	// GetUsedBuffers()
	//==============================================================
	/// Returns the number of buffer slots in the fifo that are in use
	DWORD GetUsedBuffers()
	{	return GetMaxRead( m_dwTailPtr, m_dwHeadPtr, m_dwMaxBuffers ); }

private:

	/// Maximum fifo buffer size
	DWORD				m_dwMaxBuffers;

	/// Fifo head pointer
	DWORD				m_dwHeadPtr;

	/// Fifo tail pointer
	DWORD				m_dwTailPtr;

	/// Fifo block size values
	TMem< DWORD >		m_rdwSize;
};

#endif // !defined(AFX_FIFOSYNC_H__B6145F65_413B_459A_A36D_366E21AB03B0__INCLUDED_)
