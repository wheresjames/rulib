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
// Pipe.h: interface for the CPipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIPE_H__CF2F04F4_22EC_4E14_A6DF_09953CD5FB1D__INCLUDED_)
#define AFX_PIPE_H__CF2F04F4_22EC_4E14_A6DF_09953CD5FB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Semaphore.h"

/// Data transfer information structure
/**
	\see CPipe
*/
typedef struct tagDATAXFER
{
	/// Data buffer
	LPVOID		buf;

	/// Size of buffer in buf
	DWORD		size;

	/// Number of bytes transferred
	DWORD		xfered;

	/// Pointer to variable receiving the number of bytes transferred
	LPDWORD		pxfered;

} DATAXFER, *LPDATAXFER; // end typedef struct

//==================================================================
// CPipe
//
/// This class is used to simplify the serialization of data
/**
	This class is used to simplify the serialization of data.	
*/
//==================================================================
class CPipe : public CThread
{

public:

	//==============================================================
	// Decanonicalize()
	//==============================================================
	/// Decanonicalizes ( URL type decoding ) the internal buffer
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Decanonicalize();

	//==============================================================
	// Canonicalize()
	//==============================================================
	/// Canonicalizes ( URL type decoding ) the internal buffer
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Canonicalize();

	//==============================================================
	// Allocate()
	//==============================================================
	/// Allocates the specified size array
	/**
		\param [in] dwSize	-	Buffer size
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Allocate( DWORD dwSize );

	//==============================================================
	// Flush()
	//==============================================================
	/// Flushes the internal buffers
	void Flush();
	
	//==============================================================
	// PumpAll()
	//==============================================================
	/// Forces rx data processing
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL PumpAll();

	//==============================================================
	// Pump()
	//==============================================================
	/// Forces rx data processing of the specified number of bytes
	/**
		\param [in] bytes	-	Number of bytes to process
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Pump( DWORD bytes );
	
	//==============================================================
	// GetWritePtr()
	//==============================================================
	/// Returns the current write pointer location
	virtual DWORD GetWritePtr();

	//==============================================================
	// GetReadPtr()
	//==============================================================
	/// Returns the current read pointer location
	virtual DWORD GetReadPtr();

	//==============================================================
	// SetWritePtr()
	//==============================================================
	/// Sets the current write pointer location
	virtual void SetWritePtr( DWORD p );

	//==============================================================
	// SetReadPtr()
	//==============================================================
	/// Sets the current read pointer location
	virtual void SetReadPtr( DWORD p );

	//==============================================================
	// GetMaxWrite()
	//==============================================================
	/// Returns the maximum size buffer that can be written to the pipe
	virtual DWORD GetMaxWrite();

	//==============================================================
	// GetMaxRead()
	//==============================================================
	/// Returns the maximum number of bytes that can be read from the pipe
	virtual DWORD GetMaxRead();

	//==============================================================
	// EnableBuffer()
	//==============================================================
	/// Enable / disable data buffering
	/**
		\param [in] enable	-	Non-zero to enable data buffering
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EnableBuffer( BOOL enable );

	//==============================================================
	// EnableBlocking()
	//==============================================================
	/// Enable / disable blocking operations
	/**
		\param [in] enable	-	Non-zero to enable blocking data transfer
		
		If blocking is enabled, read and write operations do not return
		until the data transfer is complete.

		\return Non-zero if success
	
		\see 
	*/
	BOOL EnableBlocking( BOOL enable );


	//==============================================================
	// WaitRead()
	//==============================================================
	/// Waits for a pending read operation to complete
	/**
		\param [in] timeout	-	Time in milli-seconds to wait for 
								a pending read operation to complete

		\return Non-zero if operation completed before the timeout
	*/
	BOOL WaitRead( DWORD timeout = 3000 )
	{	return m_evRead.Wait( timeout ); }

	//==============================================================
	// WaitWrite()
	//==============================================================
	/// Waits for a pending write operation to complete
	/**
		\param [in] timeout	-	Time in milli-seconds to wait for 
								a pending write operation to complete

		\return Non-zero if operation completed before the timeout
	*/
	BOOL WaitWrite( DWORD timeout = 3000 )
	{	return m_evWrite.Wait( timeout ); }

	//==============================================================
	// OnWriteComplete()
	//==============================================================
	/// Called when a write operation completes
	/**
		\param [in] buf			-	Pointer to the write buffer
		\param [in] size		-	Number of bytes in buf
		\param [in,out] written	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnWriteComplete( const void * buf, DWORD size, LPDWORD written );

	//==============================================================
	// OnReadComplete()
	//==============================================================
	/// Called when a read operation completes
	/**
		\param [in] buf			-	Pointer to the read buffer
		\param [in] size		-	Size of buffer in buf
		\param [in,out] read	-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnReadComplete( LPVOID buf, DWORD size, LPDWORD read );

	//==============================================================
	// ProcessWrite()
	//==============================================================
	/// Called to process a write operation
	/**
		\param [in] buf			-	Buffer containing write data
		\param [in] size		-	Number of bytes in buf
		\param [out] written	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL ProcessWrite( const void * buf, DWORD size, LPDWORD written );

	//==============================================================
	// ProcessRead()
	//==============================================================
	/// Called to process a read operation
	/**
		\param [in] buf			-	Buffer containing read data
		\param [in] size		-	Size of buffer in buf
		\param [out] read		-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL ProcessRead( LPVOID buf, DWORD size, LPDWORD read );

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called before a write operation is started
	/**
		\param [in] buf			-	Buffer containing write data
		\param [in] size		-	Number of bytes in buf
		\param [out] written	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnWrite( const void * buf, DWORD size, LPDWORD written );

	//==============================================================
	// OnRead()
	//==============================================================
	/// Called before a read operation is started
	/**
		\param [in] buf			-	Buffer containing read data
		\param [in] size		-	Size of buffer in buf
		\param [out] read		-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnRead( LPVOID buf, DWORD size, LPDWORD read );

	//==============================================================
	// Write()
	//==============================================================
	/// Called to initiate a write operation
	/**
		\param [in] buf			-	Buffer containing write data
		\param [in] size		-	Number of bytes in buf
		\param [out] written	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL Write( const void * buf, DWORD size = 0, LPDWORD written = NULL );

	//==============================================================
	// Read()
	//==============================================================
	/// Called to initiate a read operation
	/**
		\param [in] buf			-	Buffer containing read data
		\param [in] size		-	Size of buffer in buf
		\param [out] read		-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL Read( LPVOID buf, DWORD size, LPDWORD read = NULL );

	//==============================================================
	// InitThread()
	//==============================================================
	/// Thread initialization
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero to continue thread
	
		\see 
	*/
	virtual BOOL InitThread( LPVOID pData );

	//==============================================================
	// DoThread()
	//==============================================================
	/// Thread function that processes non-blocking data transferes
	/**
		\param [in] pData	-	Reserved
		
		\return Non-zero to continue thread
	
		\see 
	*/
	virtual BOOL DoThread( LPVOID pData );

	//==============================================================
	// EndThread()
	//==============================================================
	/// Thread cleanup
	/**
		\param [in] pData	-	Reserved
		
		\return Ignored
	
		\see 
	*/
	virtual BOOL EndThread( LPVOID pData );

	//==============================================================
	// Write()
	//==============================================================
	/// Writes a string to the buffer
	/**
		\param [in] str			-	Buffer containing NULL terminated string
		\param [out] written	-	Receives the number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( LPCTSTR str, LPDWORD written = NULL )
	{	return Write( (LPVOID)str, strlen( str ), written ); }

	/// Default constructor
	CPipe();

	//==============================================================
	// CPipe()
	//==============================================================
	/// Constructs a pipe by wrapping the specified buffer
	/**
		\param [in] buf		-	Buffer
		\param [in] size	-	Size of buffer in buf
	*/
	CPipe( LPVOID buf, DWORD size = 0 );

	/// Destructor
	virtual ~CPipe();

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the pie for use
	void Init();

	//==============================================================
	// IsBlocking()
	//==============================================================
	/// Returns non-zero if blocking read / write mode is enabled
	BOOL IsBlocking() { return m_bBlocking; }

	//==============================================================
	// IsBuffered()
	//==============================================================
	/// Returns non-zero if buffered read / write mode is enabled
	BOOL IsBuffered() { return m_bBuffered; }

	//==============================================================
	// GetReadCount()
	//==============================================================
	/// Returns the number of bytes read from the pipe
	DWORD GetReadCount() { return m_readcount; }

	//==============================================================
	// GetWriteCount()
	//==============================================================
	/// Returns the number of bytes written to the pipe
	DWORD GetWriteCount() { return m_writecount; }

	//==============================================================
	// SetReadCount()
	//==============================================================
	/// Sets the number of bytes read from the pipe
	void SetReadCount( DWORD c ) { m_readcount = c; }

	//==============================================================
	// SetWriteCount()
	//==============================================================
	/// Sets the number of bytes written to the pipe
	void SetWriteCount( DWORD c ) { m_writecount = c; }

	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches this pipe to another
	/**
		\param [in] next	-	Pointer to next pipe
	*/
	void Attach( CPipe *next ) 
	{	m_nextpipe = next; 
		if ( next != NULL ) next->m_prevpipe = this; }

	//==============================================================
	// Detach()
	//==============================================================
	///	Detaches from attached pipe
	void Detach() 
	{	if ( m_nextpipe != NULL )
		{	m_nextpipe->m_prevpipe = NULL;
			m_nextpipe = NULL; 
		} // end if
	}

	//==============================================================
	// SetBuffer()
	//==============================================================
	/// Wraps the specified buffer
	/**
		\param [in] buf		-	Buffer pointer
		\param [in] size	-	Size of buffer in buf
	*/
	void SetBuffer( LPVOID buf, DWORD size )
	{	m_ubuf = buf, m_usize = size; }

	//==============================================================
	// GetReadBuffer()
	//==============================================================
	/// Returns a pointer to the read buffer
	LPVOID GetReadBuffer() { return m_bufRead.ptr(); }

	//==============================================================
	// GetReadBufferSize()
	//==============================================================
	/// Returns the size of the read buffer
	DWORD GetReadBufferSize() { return m_bufRead.size(); }

	//==============================================================
	// GetWriteBuffer()
	//==============================================================
	/// Returns a pointer to the write buffer
	LPVOID GetWriteBuffer() { return m_bufWrite.ptr(); }

	//==============================================================
	// GetWriteBufferSize()
	//==============================================================
	/// Returns a pointer to the write buffer
	DWORD GetWriteBufferSize() { return m_bufWrite.size(); }

	//==============================================================
	// GetBuffer()
	//==============================================================
	/// Returns a pointer to the buffered io buffer
	LPVOID GetBuffer() { return m_buf.ptr(); }

	//==============================================================
	// GetBufferSize()
	//==============================================================
	/// Returns the size of the buffered io buffer
	DWORD GetBufferSize() { return m_buf.size(); }

private:

	/// Buffered io
	TMem< BYTE >		m_buf;

	/// User buffer pointer
	LPVOID				m_ubuf;

	/// Size of buffer in m_ubuf
	DWORD				m_usize;

	/// Read pointer
	DWORD				m_readptr;

	/// Write pointer
	DWORD				m_writeptr;

	/// Total bytes read
	DWORD				m_readcount;

	/// Total bytes written
	DWORD				m_writecount;

	/// Read lock
	CLoSemaphore		m_evRead;

	/// Write lock
	CLoSemaphore		m_evWrite;

	/// Read data transfer buffer
	DATAXFER			m_xfRead;

	/// Write data transfer buffer
	DATAXFER			m_xfWrite;

	/// Read buffer
	TMem< BYTE >		m_bufRead;

	/// Write buffer
	TMem< BYTE >		m_bufWrite;

	/// Non-zero for blocking io transfers
	BOOL				m_bBlocking;

	/// Non-zero for buffered io transfers
	BOOL				m_bBuffered;

	/// Pointer to next linked CPipe object
	CPipe				*m_nextpipe;

	/// Pointer to previous linked CPipe object
	CPipe				*m_prevpipe;

};

#endif // !defined(AFX_PIPE_H__CF2F04F4_22EC_4E14_A6DF_09953CD5FB1D__INCLUDED_)
