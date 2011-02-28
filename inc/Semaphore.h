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
// Semaphore.h: interface for the CLoSemaphore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMAPHORE_H__B79B9025_1693_4A3B_8493_97A62901D8B6__INCLUDED_)
#define AFX_SEMAPHORE_H__B79B9025_1693_4A3B_8493_97A62901D8B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoSemaphore  
{
public:

	CLoSemaphore() { m_event = NULL; }
	CLoSemaphore( BOOL bCreate, BOOL bSet )
	{	m_event = NULL; if ( bCreate ) Create( bSet ); }
	CLoSemaphore( HANDLE event )
	{	m_event = event; }
	
	virtual ~CLoSemaphore() { Destroy(); }

	void Attach( HANDLE event )
	{	Destroy(); m_event = event; }
	void Detach() { m_event = NULL; }

	BOOL Create( BOOL bSet = FALSE )
	{	m_event = CreateEvent( NULL, TRUE, bSet, NULL ); 
		return ( m_event != NULL ); }

	void Destroy()
	{	if ( m_event != NULL ) CloseHandle( m_event ); }

	BOOL Set() 
	{	if ( m_event != NULL ) return SetEvent( m_event ); 
	return FALSE; }

	BOOL Reset() 
	{	if ( m_event != NULL ) return ResetEvent( m_event ); 
	return FALSE; }

	BOOL Wait( DWORD timeout )
	{	if ( m_event != NULL ) 
			return ( WaitForSingleObject( m_event, timeout ) != WAIT_TIMEOUT );
		return FALSE; }

	BOOL IsSet() { return Wait( 0 ); }

	operator HANDLE() { return m_event; }


private:

	HANDLE		m_event;

};

#endif // !defined(AFX_SEMAPHORE_H__B79B9025_1693_4A3B_8493_97A62901D8B6__INCLUDED_)
