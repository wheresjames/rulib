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
// CircBuf.h: interface for the CCircBuf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCBUF_H__822B2357_0330_4D5A_BC0C_19B42B37C145__INCLUDED_)
#define AFX_CIRCBUF_H__822B2357_0330_4D5A_BC0C_19B42B37C145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Default maximum of 32 megabytes
#define CCIRCBUF_DEFAULT_MAXBUFFER		( 32 * 1048576 )

//==================================================================
// CCircBuf
//
/// Implements a circular buffer
/**
	This class encapsulates the functionality of a cicular buffer.  It
	is specifically designed for buffering serial and network communications.
	It has hooks for supporting packetizing, compression, and encryption.
	Efficiency is maintained by allowing inplace operations on the data
	through the use of views.
*/
//==================================================================
class CCircBuf  
{
public:

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the circular buffer
	/**		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL Init();

	//==============================================================
	// GetWriteView()
	//==============================================================
	/// Waits for data to be written to the circular buffer
	/**
		\param [in] dwTimeout	-	Maximum number of milliseconds to wait for data. 
		
		\return Non-zero if data was received before timeout expired.
	
		\see 
	*/
	BOOL WaitData( DWORD dwTimeout = INFINITE );

	//==============================================================
	// GetDataReadyHandle()
	//==============================================================
	/// Returns a pointer to the data ready event handle
	HANDLE GetDataReadyHandle() { return m_hDataReady; }

	//==============================================================
	// GetWriteView()
	//==============================================================
	/// Returns a view of the buffer with respect to the write pointer
	/**
		\param [in] dwView		-	Index of the view to return.
		\param [in] dwOffset	-	Offset from the write pointer of data segment.
		\param [in] dwSize		-	Total size of data segment
		\param [out] pBuf		-	Pointer receives pointer to start of view
		\param [out] pdwSize	-	Receives the size of this view.
		
		To view all of data segment, call with dwView set to 0, increment
		on each call until GetWriteView() returns zero.

		\return Non-zero if success.
	
		\see GetReadView
	*/
	BOOL GetWriteView( DWORD dwView, DWORD dwOffset, DWORD dwSize, LPBYTE *pBuf, LPDWORD pdwSize )
	{	return GetView( dwView, NormalizePtr( m_dwWritePtr + dwOffset, m_dwSize ), dwSize, m_buf, m_dwSize, pBuf, pdwSize ); }

	//==============================================================
	// GetWriteView()
	//==============================================================
	/// Returns a view of the buffer with respect to the read pointer
	/**
		\param [in] dwView		-	Index of the view to return.
		\param [in] dwOffset	-	Offset from the read pointer of data segment.
		\param [in] dwSize		-	Total size of data segment
		\param [out] pBuf		-	Pointer receives pointer to start of view
		\param [out] pdwSize	-	Receives the size of this view.
		
		To view all of data segment, call with dwView set to 0, increment
		on each call until GetReadView() returns zero.

		\return Non-zero if success.
	
		\see GetWriteView
	*/
	BOOL GetReadView( DWORD dwView, DWORD dwOffset, DWORD dwSize, LPBYTE *pBuf, LPDWORD pdwSize )
	{	return GetView( dwView, NormalizePtr( m_dwReadPtr + dwOffset, m_dwSize ), dwSize, m_buf, m_dwSize, pBuf, pdwSize ); }

	//==============================================================
	// NormalizePtr()
	//==============================================================
	/// Wraps the specified pointer
	/**
		\param [in] dwPtr	-	Pointer position
		\param [in] dwMax	-	Maximum buffer position
		
		The pointer may point to the byte after the buffer, this wraps it

		\return Returns dwPtr wrapped to dwMax
	
		\see 
	*/
	DWORD NormalizePtr( DWORD dwPtr, DWORD dwMax );

	//==============================================================
	// AdvanceReadPtr()
	//==============================================================
	/// Advances the read pointer
	/**
		\param [in] lStep	-	Number of bytes to advance read pointer.
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	BOOL AdvanceReadPtr( long lStep )
	{	CTlLocalLock ll( &m_lock );
		if ( !ll.IsLocked() ) return FALSE;		
		if ( !GetMaxRead() ) return FALSE;
		m_dwReadPtr = NormalizePtr( AdvancePtr( m_dwReadPtr, lStep, m_dwSize ), m_dwSize ); 
		Defrag();
		return TRUE;
	}

	//==============================================================
	// AdvanceWritePtr()
	//==============================================================
	/// Advances the write pointer
	/**
		\param [in] lStep	-	Number of bytes to advance write pointer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AdvanceWritePtr( long lStep )
	{	CTlLocalLock ll( &m_lock );
		if ( !ll.IsLocked() ) return FALSE;		
		if ( !GetMaxWrite() ) return FALSE;
		m_dwWritePtr = AdvancePtr( m_dwWritePtr, lStep, m_dwSize );
		return TRUE;
	}
	
	//==============================================================
	// AdvancePtr()
	//==============================================================
	/// Advances a pointer
	/**
		\param [in] dwPtr	-	Current pointer value
		\param [in] lStep	-	Amount to add to pointer
		\param [in] dwMax	-	Maximum size of buffer
		
		\return New pointer value
	
		\see 
	*/
	static DWORD AdvancePtr( DWORD dwPtr, long lStep, DWORD dwMax );

	//==============================================================
	// GetView()
	//==============================================================
	/// Returns the desired view of a specified data segment
	/**
		\param [in] dwView		-	Index of the view to return.
		\param [in] dwPtr		-	Offset of the start of the segment.
		\param [in] dwSize		-	Total size of the segment.
		\param [in] pRing		-	Pointer to the start of the buffer.
		\param [in] dwMax		-	Size of the buffer
		\param [out] pBuf		-	Pointer to start of view
		\param [out] pdwSize	-	Size of this view segment.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetView( DWORD dwView, DWORD dwPtr, DWORD dwSize, LPBYTE pRing, DWORD dwMax, LPBYTE *pBuf, LPDWORD pdwSize );

	//==============================================================
	// Defrag()
	//==============================================================
	/// Defragments the circular buffer
	/**
		Resets read and write pointers to zero if buffer is empty.
		This attempts to keep the buffer from wrapping
	*/
	void Defrag();

	//==============================================================
	// EnsureWriteSpace()
	//==============================================================
	/// Ensures space is available for write by growing buffer if needed.
	/**
		\param [in] dwSize		-	Number of bytes required. 
		\param [in] dwReadPtr	-	Position of read pointer.
		\param [in] dwWritePtr	-	Position of write pointer.
		\param [in] dwMax		-	Maximum size of buffer.
		
		Checks to ensure the specified amount of space is available for writing.
		If it is not, the function creates it without losing data that is in the buffer.

		\return Space available
	
		\see 
	*/
	DWORD EnsureWriteSpace( DWORD dwSize, DWORD dwReadPtr, DWORD dwWritePtr, DWORD dwMax );

	//==============================================================
	// Resize()
	//==============================================================
	/// Resizes the buffer without losing the data within
	/**
		\param [in] dwNewSize	-	New size for circular buffer
		
		\return	New size of buffer
	
		\see 
	*/
	DWORD Resize( DWORD dwNewSize );

	//==============================================================
	// Read()
	//==============================================================
	/// Read a string from the buffer
	/**
		\param [in] pStr	-	Buffer to hold string
		\param [in] dwMax	-	Size of buffer in pStr.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Read( LPSTR pStr, DWORD dwMax );

	//==============================================================
	// Write()
	//==============================================================
	/// Write a string to the buffer
	/**
		\param [in] pStr	-	String value to write to buffer
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL Write( LPCTSTR pStr );

	//==============================================================
	// GetMaxWrite()
	//==============================================================
	/// Returns the maximum amount of data that can be written to the buffer
	/**
		\param [in] dwReadPtr	-	Position of read pointer.
		\param [in] dwWritePtr	-	Position of write pointer.
		\param [in] dwMax		-	Total size of circular buffer
		
		\return Number of bytes that can be written
	
		\see 
	*/
	static DWORD GetMaxWrite( DWORD dwReadPtr, DWORD dwWritePtr, DWORD dwMax );

	//==============================================================
	// GetMaxWrite()
	//==============================================================
	/// Returns the maximum amount of data that can be written to the buffer
	/**
		\return Number of bytes that can be written
	*/
	DWORD GetMaxWrite() { return GetMaxWrite( m_dwReadPtr, m_dwWritePtr, m_dwSize ); }

	//==============================================================
	// GetMaxRead()
	//==============================================================
	/// Returns the maximum amount of data that can be read from the buffer	
	/**
		\param [in] dwReadPtr	-	Position of read pointer.
		\param [in] dwWritePtr	-	Position of write pointer.
		\param [in] dwMax		-	Total size of circular buffer
		
		\return Number of bytes that can be read
	
		\see 
	*/
	static DWORD GetMaxRead( DWORD dwReadPtr, DWORD dwWritePtr, DWORD dwMax );

	//==============================================================
	// GetMaxRead()
	//==============================================================
	/// Returns the maximum amount of data that can be read from the buffer
	/**
		\return Number of bytes that can be read
	*/
	DWORD GetMaxRead() { return GetMaxRead( m_dwReadPtr, m_dwWritePtr, m_dwSize ); }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources associated with the circular buffer.
	void Destroy();
	
	//==============================================================
	// Write()
	//==============================================================
	/// Writes specified data to the circular buffer
	/**
		\param [in] pBuf		-	Pointer to buffer containing data to be written.
		\param [in] dwSize		-	Size of the buffer in pBuf.
		\param [in] pdwPtr		-	Write pointer position
		\param [in] dwEncode	-	Optional encoding
				
		\return Non-zero on success.
	
		\see 
	*/
	BOOL Write( void const * pBuf, DWORD dwSize, LPDWORD pdwPtr, DWORD dwEncode = 0 );

	//==============================================================
	// Write()
	//==============================================================
	/// Writes specified data to the circular buffer
	/**
		\param [in] pBuf		-	Pointer to buffer containing data to be written.
		\param [in] dwSize		-	Size of the buffer in pBuf.
		\param [in] dwEncode	-	Optional encoding
				
		\return Non-zero on success
	
		\see 
	*/
	virtual BOOL Write( void const * pBuf, DWORD dwSize, DWORD dwEncode = 0 )
	{	if ( m_bAutoGrow ) EnsureWriteSpace( dwSize, m_dwReadPtr, m_dwWritePtr, m_dwSize );
		if ( !Write( pBuf, dwSize, &m_dwWritePtr, dwEncode ) ) return FALSE;
		return OnWrite();
	}

	//==============================================================
	// InitPoke()
	//==============================================================
	/// Used for revokable writing to buffer
	/**
		Used for putting data into the buffer without committing it.
		Usefull if you may fail while writing multi-part data.

		\see CancelPoke(), Poke(), EndPoke(), GetPokeSize()
	*/
	void InitPoke() { m_dwPokePtr = m_dwWritePtr; }

	//==============================================================
	// CancelPoke()
	//==============================================================
	/// Cancels a poke operation
	/**
		\see InitPoke(), Poke(), EndPoke(), GetPokeSize()
	*/
	void CancelPoke() { m_dwPokePtr = m_dwWritePtr; }

	//==============================================================
	// Poke()
	//==============================================================
	/// Pokes data into the buffer
	/**
		\param [in] pBuf		-	Data to poke into write buffer
		\param [in] dwSize		-	Size of pBuf
		\param [in] dwEncode	-	Optional data encoding
		
		\return Non-zero if success
	
		\see InitPoke(), CancelPoke(), EndPoke(), GetPokeSize()
	*/
	BOOL Poke( void const * pBuf, DWORD dwSize, DWORD dwEncode = 0 )
	{	if ( m_bAutoGrow ) EnsureWriteSpace( dwSize, m_dwReadPtr, m_dwPokePtr, m_dwSize );
		return Write( pBuf, dwSize, &m_dwPokePtr, dwEncode );
	}

	//==============================================================
	// EndPoke()
	//==============================================================
	/// Committs the poked data to the buffer
	/**		
		\return Non-zero if success
	
		\see InitPoke(), CancelPoke(), Poke(), GetPokeSize()
	*/
	BOOL EndPoke() 
	{	if ( m_dwWritePtr != m_dwPokePtr )
		{	m_dwWritePtr = m_dwPokePtr;
			return OnWrite();
		} // end if
		return FALSE;		
	}

	//==============================================================
	// GetPokeSize()
	//==============================================================
	/// Returns the size of currently poked data
	/**		
		\return Number of bytes currently poked
	
		\see InitPoke(), CancelPoke(), Poke(), EndPoke()
	*/
	DWORD GetPokeSize() 
	{ return GetMaxRead( m_dwReadPtr, m_dwPokePtr, m_dwSize ); }

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when data is written to the circular buffer
	/**
		\return Non-zero if success
		
		Over-ride this function to provide write data handling.

		\see 
	*/
	virtual BOOL OnWrite() { DataReady(); return TRUE; }

	//==============================================================
	// Read()
	//==============================================================
	/// Call to read and remove data from the circular buffer
	/**
		\param [out] pBuf		-	Buffer to receive read data.
		\param [in] dwMax		-	Size of buffer in pBuf
		\param [out] pdwRead	-	Receives number of bytes read
		\param [out] pdwPtr		-	Receives read pointer
		\param [in] dwEncode	-	Optional decoding
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	BOOL Read( LPVOID pBuf, DWORD dwMax, LPDWORD pdwRead, LPDWORD pdwPtr, DWORD dwEncode = 0 );

	//==============================================================
	// Read()
	//==============================================================
	/// Call to read and remove data from the circular buffer
	/**
		\param [out] pBuf		-	Buffer to receive read data.
		\param [in] dwMax		-	Size of buffer in pBuf
		\param [out] pdwRead	-	Receives number of bytes read
		\param [in] dwEncode	-	Optional decoding
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	virtual BOOL Read( LPVOID pBuf, DWORD dwMax, LPDWORD pdwRead, DWORD dwEncode = 0 );

	//==============================================================
	// Peek()
	//==============================================================
	/// Call to read data from the buffer without removing it
	/**
		\param [in] pBuf		-	Buffer that receives peeked data
		\param [in] dwMax		-	Size of the buffer in pBuf
		\param [in] pdwRead		-	Number of bytes read into pBuf
		\param [in] lOffset		-	Offset from read pointer to start
		\param [in] dwEncode	-	Optional data decoding
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL Peek( LPVOID pBuf, DWORD dwMax, LPDWORD pdwRead, long lOffset = 0, DWORD dwEncode = 0 );

	//==============================================================
	// Allocate()
	//==============================================================
	/// Pre allocates the specified amount of data for the circular buffer
	/**
		\param [in] dwSize	-	Number of bytes to allocate.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Allocate( DWORD dwSize );

	/// Constructor
	CCircBuf( BOOL bSync = TRUE, DWORD dwSize = 0, BOOL bAutoGrow = TRUE );

	/// Destructor
	virtual ~CCircBuf();

	//==============================================================
	// SetReadPtr()
	//==============================================================
	/// Sets the position of the read pointer
	/**
		\param [in] dwPtr	-	New value for read pointer.
	*/
	void SetReadPtr( DWORD dwPtr ) { m_dwReadPtr = dwPtr; }
	
	//==============================================================
	// GetReadPtr()
	//==============================================================
	/// Returns the position of the read pointer
	/**
		\return read pointer position
	*/
	DWORD GetReadPtr() { return m_dwReadPtr; }

	//==============================================================
	// SetWritePtr()
	//==============================================================
	/// Sets the position of the write pointer
	/**
		\param [in] dwPtr	-	New value for write pointer.
	*/
	void SetWritePtr( DWORD dwPtr ) { m_dwWritePtr = dwPtr; }

	//==============================================================
	// GetWritePtr()
	//==============================================================
	/// Returns the position of the write pointer
	/**
		\return write pointer position
	*/
	DWORD GetWritePtr() { return m_dwWritePtr; }

	/// Returns a pointer for the buffers thread locking class
	operator CTlLock*() { return &m_lock; }

	//==============================================================
	// SetMaxSize()
	//==============================================================
	/// Sets the maximum size the buffer can grow
	/**
		\param [in] dwMaxSize	-	Sets the maximum size for the circular buffer
		
		\warning This should really be set to something reasonable!

		\see SetAutoGrow(), SetAutoGrow(), SetMaxSize() 
	*/
	void SetMaxSize( DWORD dwMaxSize ) { m_dwMaxSize = dwMaxSize; }

	//==============================================================
	// GetMaxSize()
	//==============================================================
	/// Returns the maximum size the buffer can grow
	/**			
		\return Returns the maximum size for the circular buffer.
	
		\see GetAutoGrow(), SetAutoGrow(), SetMaxSize() 
	*/
	DWORD GetMaxSize() { return m_dwMaxSize; }

	//==============================================================
	// SetAutoGrow()
	//==============================================================
	/// Enables / disables the auto resizing feature of CCircBuf
	/**
		\param [in] b	-	Non-zero to enable resizing, zero to disable.
		
		Sets auto grow enable / disable. If auto grow is enabled, the buffer will
		automatically resized to accomidate any write less than m_dwMaxSize.

		\see GetAutoGrow(), SetMaxSize(), GetMaxSize()
	*/
	void SetAutoGrow( BOOL b ) { m_bAutoGrow = b; }

	//==============================================================
	// GetAutoGrow()
	//==============================================================
	/// Returns the auto grow status
	/**			
		\return Non-zero if auto-grow is enabled.
	
		\see SetAutoGrow(), SetMaxSize(), GetMaxSize()
	*/
	BOOL GetAutoGrow() { return m_bAutoGrow; }

	//==============================================================
	// IsEmpty()
	//==============================================================
	/// Provides a quick way to check for read data availability
	/**
		\return Non-zero if data can be read from the buffer.
	*/
	BOOL IsEmpty() { return m_bEmpty; }

	//==============================================================
	// DataReady()
	//==============================================================
	/// Call this function to signal that data is ready to be read.
	void DataReady() 
	{	m_bEmpty = FALSE;
		if ( m_hDataReady ) SetEvent( m_hDataReady ); 
	}

	//==============================================================
	// Empty()
	//==============================================================
	/// Empties the circular buffer
	/**
		\return Non-zero if success.
	*/
	BOOL Empty()
	{	CTlLocalLock ll( &m_lock );
		if ( !ll.IsLocked() ) return FALSE;		
		m_bEmpty = TRUE;
		m_dwWritePtr = m_dwReadPtr = 0;
		ResetEvent( m_hDataReady );
		return TRUE;
	}

	//==============================================================
	// OnInspectRead()
	//==============================================================
	/// Over-ride to implement read block inspection
	/**
		\param [in] dwBlock		-	Index of block
		\param [in] pBuf		-	Pointer to data
		\param [in] dwSize		-	Size of buffer in pBuf
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnInspectRead( DWORD dwBlock, LPBYTE pBuf, DWORD dwSize ) { return TRUE; }

	//==============================================================
	// OnInspectWrite()
	//==============================================================
	/// Over-ride to implement write block inspection
	/**
		\param [in] dwBlock		-	Index of block
		\param [in] pBuf		-	Pointer to data
		\param [in] dwSize		-	Size of buffer in pBuf
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnInspectWrite( DWORD dwBlock, LPBYTE pBuf, DWORD dwSize ) { return TRUE; }

	/// Over-ride this function to enable generic block encoding
	//==============================================================
	// OnEncode()
	//==============================================================
	/// 
	/**
		\param [in] dwType		-	Type of encoding to perform
		\param [in] dwBlock		-	Index of block
		\param [in] pBuf		-	Pointer to data
		\param [in] dwSize		-	Size of buffer in pBuf
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnEncode( DWORD dwType, DWORD dwBlock, LPBYTE pBuf, DWORD dwSize ) { return TRUE; }

	/// Over-ride this function to enable generic block decoding
	//==============================================================
	// OnDecode()
	//==============================================================
	/// 
	/**
		\param [in] dwType		-	Type of encoding to perform
		\param [in] dwBlock		-	Index of block
		\param [in] pBuf		-	Pointer to data
		\param [in] dwSize		-	Size of buffer in pBuf
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnDecode( DWORD dwType, DWORD dwBlock, LPBYTE pBuf, DWORD dwSize ) { return TRUE; }

	//==============================================================
	// GetBufferSize()
	//==============================================================
	/// Returns the size of the buffer
	/**		
		\return	Total size of the circular buffer
	*/
	DWORD GetBufferSize() { return m_dwSize; }

private:

	/// Thread lock
	CTlLock					m_lock;

	/// Data ready event
	HANDLE					m_hDataReady;

	/// Non-zero if buffer is empty
	BOOL					m_bEmpty;

	/// Set to non-zero to allow the buffer to grow during write operations
	BOOL					m_bAutoGrow;

	/// Sets the maximum size for the circular buffer
	DWORD					m_dwMaxSize;

	/// The current size of the circular buffer
	DWORD					m_dwSize;

	/// The position of the next character slot available for reading
	DWORD					m_dwReadPtr;

	/// The position of the next character slot available for writing
	DWORD					m_dwWritePtr;

	/// Position of data that is being poked into the buffer
	DWORD					m_dwPokePtr;

	/// Encapsulates the actual circular buffer memory
	TMem< BYTE >			m_buf;

};

#endif // !defined(AFX_CIRCBUF_H__822B2357_0330_4D5A_BC0C_19B42B37C145__INCLUDED_)
