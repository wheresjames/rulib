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
// Pipe.cpp: implementation of the CPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipe::CPipe()
{_STTEX();
	Init();
}

CPipe::CPipe(LPVOID buf, DWORD size)
{_STTEX();
	Init();

	// Zero length means NULL terminated buffer
	if ( buf != NULL && size == 0 ) size = strlen( (char*)buf );

	// Set the buffer
	SetBuffer( buf, size );
}

void CPipe::Init()
{_STTEX();
	m_ubuf = NULL;
	m_usize = 0;

	m_readptr = 0;
	m_writeptr = 0;

	m_readcount = 0;
	m_writecount = 0;

	m_bBlocking = TRUE;
	m_bBuffered = FALSE;

	ZeroMemory( &m_xfRead, sizeof( m_xfRead ) );
	ZeroMemory( &m_xfWrite, sizeof( m_xfWrite ) );

	m_nextpipe = NULL;
	m_prevpipe = NULL;
}

CPipe::~CPipe()
{_STTEX();
	// Delete any attached pipe
	if ( m_nextpipe != NULL ) 
	{	m_nextpipe->m_prevpipe = NULL;
		delete m_nextpipe;
		m_nextpipe = NULL;
	} // end if

}

BOOL CPipe::Read(LPVOID buf, DWORD size, LPDWORD read)
{_STTEX();
	// Is a read in progress?
	if ( m_evRead.IsSet() ) return FALSE;

	// Buffer if needed
	if ( m_bBuffered || buf == NULL )
	{	if ( !m_bufRead.allocate( size ) ) return FALSE;	
		buf = m_bufRead.ptr();
	} // end if
	
	// Do the work now if blocking
	if ( m_bBlocking ) return OnRead( buf, size, read );

	// Won't know until later
	if ( read != NULL ) *read = 0;

	// Save the read data
	m_xfRead.buf = buf;
	m_xfRead.size = size;
	m_xfRead.xfered = 0;
		
	// Trigger the read
	return m_evRead.Set();
}

BOOL CPipe::OnRead(LPVOID buf, DWORD size, LPDWORD read)
{_STTEX();
	// Call next pipe if needed
	if ( m_nextpipe != NULL ) 
		return m_nextpipe->Read( buf, size, read );

	// Notify that read is complete
	OnReadComplete( buf, size, read );

	return TRUE;
}

BOOL CPipe::OnReadComplete(LPVOID buf, DWORD size, LPDWORD read)
{_STTEX();
	// Process the read
	ProcessRead( buf, size, read );

	// Count bytes read
	m_readcount += size;

	// Send message back upstream
	if ( m_prevpipe != NULL )
		return m_prevpipe->OnReadComplete( buf, size, read );

	return TRUE;
}

BOOL CPipe::ProcessRead(LPVOID buf, DWORD size, LPDWORD read)
{_STTEX();
	LPVOID b = m_ubuf;
	DWORD s = m_usize;

	// Did the user supply a buffer?
	if ( b == NULL ) { b = m_buf.ptr(); s = m_buf.size(); }

	// How many bytes are available?
	DWORD max = ( m_writeptr > m_readptr ) ? m_writeptr - m_readptr : 0;
	if ( size > max ) size = max;

	// How many bytes will we read
	if ( read != NULL ) *read = size;

	// Any bytes?
	if ( size == 0 ) return TRUE;
	
	// Copy the data
	if ( buf != NULL ) memcpy( buf, &( (LPBYTE)b )[ m_readptr ], size );	

	// Add to read pointer
	m_readptr += size;

	return TRUE;
}


BOOL CPipe::Write(const void * buf, DWORD size, LPDWORD written)
{_STTEX();
	// Is a write in progress?
	if ( m_evWrite.IsSet() ) return FALSE;

	// Zero size means NULL terminated
	if ( size == 0 ) size = strlen( (char*)buf );

	// Buffer data if needed
	if ( m_bBuffered )
	{
		// Buffer the data
		if ( !m_bufWrite.put( buf, size ) ) return FALSE;

		// Set buffer pointer
		buf = m_bufWrite.ptr();

	} // end if

	// Do the work now if blocking
	if ( m_bBlocking ) return OnWrite( buf, size, written );

	// Won't know until later
	if ( written != NULL ) *written = 0;

	// Save the write data
	m_xfWrite.buf = (LPVOID)buf;
	m_xfWrite.size = size;
	m_xfWrite.xfered = 0;
		
	// Trigger the write
	return m_evWrite.Set();
}

BOOL CPipe::OnWrite(const void * buf, DWORD size, LPDWORD written)
{_STTEX();
	// Process the write
	ProcessWrite( buf, size, written );

	// Call next pipe if needed
	if ( m_nextpipe != NULL ) 
		return m_nextpipe->Write( buf, size, written );

	// Notify that the write is complete
	OnWriteComplete( buf, size, written );

	return TRUE;
}

BOOL CPipe::ProcessWrite(const void * buf, DWORD size, LPDWORD written)
{_STTEX();
	LPVOID b = m_ubuf;
	DWORD s = m_usize;

	// Is the read buffer empty?
	if ( m_readptr >= m_writeptr ) Flush();

	// Did the user supply a buffer?
	if ( b == NULL ) 
	{ 	if ( m_buf.size() == 0 )
		{	if ( !m_buf.allocate( 1 + 1 ) ) return FALSE; }
		b = m_buf.ptr(); s = m_buf.size(); 
	} // end get buffer

	// Ensure buffer space
	if ( ( size + m_writeptr ) > s )
	{
		if ( m_ubuf == NULL )
		{
			if( !m_buf.grow( size + m_writeptr ) ) return FALSE;
		} // end if
		else
		{
			// Any space left in buffer?
			if ( m_writeptr >= s ) return FALSE;

			// Write what we can
			size = s - m_writeptr;

		} // end else

	} // end if

	// NULL write?
	if ( size == 0 ) return TRUE;

	// How many bytes written?
	if ( written != NULL ) *written = size;

	// Copy the data
	memcpy( m_buf.ptr( m_writeptr ), buf, size );
	
	// Adjust write pointer
	m_writeptr += size;

	return TRUE;
}

BOOL CPipe::OnWriteComplete(const void * buf, DWORD size, LPDWORD written)
{_STTEX();
	// Count bytes written
	m_writecount += size;

	// Pass back up the chain
	if ( m_prevpipe != NULL )
		m_prevpipe->OnWriteComplete( buf, size, written );

	return TRUE;
}

BOOL CPipe::EnableBlocking(BOOL enable)
{_STTEX();
	m_bBlocking = enable;

	// Start/Stop thread
	if ( enable ) return StopThread();

	// Start the thread
	return StopThread();
}

BOOL CPipe::EnableBuffer(BOOL enable)
{_STTEX();
	m_bBuffered = enable;

	return TRUE;
}

BOOL CPipe::InitThread( LPVOID pData )
{_STTEX();
	// Create read/write events
	m_evRead.Create();
	m_evWrite.Create();

	return TRUE;
}

#define WAITEVENTS		3
BOOL CPipe::DoThread( LPVOID pData )
{_STTEX();
	HANDLE	ha[ WAITEVENTS ];

	ha[ 0 ] = m_hStop;
	ha[ 1 ] = m_evRead;
	ha[ 2 ] = m_evWrite;

	// Wait for an event
	DWORD ret = WaitForMultipleObjects( WAITEVENTS, ha, FALSE, INFINITE );

	// Verify range
	if ( ret > WAIT_OBJECT_0 + ( WAITEVENTS - 1 ) )
		return TRUE;

	// Get event index
	ret -= WAIT_OBJECT_0;

	// Check for stop command
	if ( ret == 0 ) return TRUE;

	// Check for read command
	else if ( ret == 1 ) 
		OnRead( m_xfRead.buf, m_xfRead.size, &m_xfRead.xfered );

	// Check for write command
	else if ( ret == 2 ) 
		OnWrite( m_xfWrite.buf, m_xfWrite.size, &m_xfWrite.xfered );

	// Reset event
	ResetEvent( ha[ ret ] );

	return TRUE;
}

BOOL CPipe::EndThread( LPVOID pData )
{_STTEX();
	// Destroy read/write events
	m_evRead.Destroy();
	m_evWrite.Destroy();

	return TRUE;
}

DWORD CPipe::GetMaxRead()
{_STTEX();
	// Ask the next guy if any
	if ( m_nextpipe != NULL ) return m_nextpipe->GetMaxRead();

	DWORD s = m_usize;
	if ( m_ubuf == NULL ) s = m_buf.size();

	// Any data to be read?
	if ( m_writeptr <= m_readptr ) return 0;
	return ( m_writeptr - m_readptr );
}

DWORD CPipe::GetMaxWrite()
{_STTEX();
	// Ask the next guy if any
	if ( m_nextpipe != NULL ) return m_nextpipe->GetMaxWrite();

	// Using auto buffer?
	if( m_ubuf == NULL ) return MAXDWORD;

	// Any room for data?
	if ( m_usize <= m_writeptr ) return 0;
	return ( m_usize - m_writeptr );
}

void CPipe::SetReadPtr(DWORD p)
{_STTEX();
	// Pass along if needed
	if ( m_nextpipe != NULL ) { m_nextpipe->SetReadPtr( p ); return; }

	m_readptr = p;
}

void CPipe::SetWritePtr(DWORD p)
{_STTEX();
	// Pass along if needed
	if ( m_nextpipe != NULL ) { m_nextpipe->SetWritePtr( p ); return; }

	m_writeptr = p;
}

DWORD CPipe::GetReadPtr()
{_STTEX();
	// Ask the next guy if any
	if ( m_nextpipe != NULL ) return m_nextpipe->GetReadPtr();

	return m_readptr;
}

DWORD CPipe::GetWritePtr()
{_STTEX();
	// Ask the next guy if any
	if ( m_nextpipe != NULL ) return m_nextpipe->GetWritePtr();

	return m_writeptr;
}


BOOL CPipe::Pump(DWORD bytes)
{_STTEX();
	return Read( NULL, bytes );
}

BOOL CPipe::PumpAll()
{_STTEX();
	return Read( NULL, GetMaxRead() );	
}

void CPipe::Flush()
{_STTEX();
	// Reset buffer pointers
	m_readptr = 0; m_writeptr = 0;

	// Release memory if needed
	if ( m_ubuf == NULL ) m_buf.destroy();
}

BOOL CPipe::Allocate(DWORD dwSize)
{_STTEX();
	return m_buf.allocate( dwSize );
}

BOOL CPipe::Canonicalize()
{_STTEX();
	if ( !m_buf.size() ) return FALSE;

	TMem< BYTE >	temp;
	DWORD			dwSize = CCfgFile::GetMinCanonicalizeBufferSize( m_buf.size() );

	if ( !temp.allocate( dwSize ) ) 
		return FALSE;

	if ( !CCfgFile::CanonicalizeBuffer( (char*)temp.ptr(), m_buf, m_buf.size() ) )
		return FALSE;

	temp.realloc( strlen( (char*)temp.ptr() ) );

	m_buf.attach( temp );

	return TRUE;
}

BOOL CPipe::Decanonicalize()
{_STTEX();
	if ( !m_buf.size() ) return FALSE;

	TMem< BYTE >	temp;
	DWORD			dwSize = CCfgFile::GetMinDeCanonicalizeBufferSize( m_buf.size() );

	if ( !temp.allocate( dwSize ) ) 
		return FALSE;

	DWORD dwActualSize = 0;
	if ( !CCfgFile::DeCanonicalizeBuffer( (LPCTSTR)m_buf.ptr(), temp, m_buf.size(), &dwActualSize ) )
		return FALSE;

	temp.realloc( dwActualSize );

	m_buf.attach( temp );

	return TRUE;
}
