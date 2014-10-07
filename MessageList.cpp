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
// MessageList.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessageList::CMessageList()
{
	m_bClose = FALSE;
}

CMessageList::~CMessageList()
{
	m_bClose = TRUE;
	ClearAllMessageTargets();
}

GUID* CMessageList::CreateKey( GUID* pKey, void* dwUser1, void* dwUser2, const GUID *pguid )
{
	MD5_CTX ctx;
	CMd5Rsa::MD5Init( &ctx );
	CMd5Rsa::MD5Update( &ctx, (LPBYTE)&dwUser1, sizeof( dwUser1 ) );
	CMd5Rsa::MD5Update( &ctx, (LPBYTE)&dwUser2, sizeof( dwUser2 ) );

	if ( pguid && !IsEqualGUID( *pguid, CLSID_ZERO ) )
		CMd5Rsa::MD5Update( &ctx, (LPBYTE)pguid, sizeof( GUID ) );

	CMd5Rsa::MD5Final( (LPBYTE)pKey, &ctx );

	return pKey;
}

void CMessageList::SetMessageTarget( HWND hWnd, UINT uWMMessageID, BOOL bPost, const GUID *pguid )
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vMsgTargetQueue );
	if ( !ll.IsLocked() )
		return;

	if ( hWnd != NULL && uWMMessageID && ::IsWindow( hWnd ) )
	{
		GUID guid;
		CreateKey( &guid, hWnd, (void*)uWMMessageID, pguid );

		m_vMsgTargetQueue.erase( guid );
		m_vMsgTargetQueue.push_back( guid, _PTR_NEW CMsgTarget( hWnd, uWMMessageID, bPost, 1, pguid ) );

	} // end if

}

void CMessageList::RemoveMessageTarget( HWND hWnd, UINT uWMMessageID, const GUID *pguid )
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vMsgTargetQueue );
	if ( !ll.IsLocked() )
		return;

	if ( hWnd != NULL && uWMMessageID && ::IsWindow( hWnd ) )
	{
		GUID guid;
		CreateKey( &guid, hWnd, (void*)uWMMessageID, pguid );

		m_vMsgTargetQueue.erase( guid );
		m_vMsgTargetQueue.push_back( guid, _PTR_NEW CMsgTarget( hWnd, uWMMessageID, 0, 2, pguid ) );

	} // end if

//        GUID guid; m_vMsgTargetQueue.erase( *CreateKey( &guid, (DWORD)hWnd, (DWORD)uWMMessageID, pguid ) );
}

DWORD CMessageList::MessageTargets()
{
	if ( m_bClose )
		return 0;

	return ( m_vMsgTarget.Size() + m_vMsgTargetQueue.Size() );
}

DWORD CMessageList::CallbackFunctions()
{
	if ( m_bClose )
		return 0;

	return ( m_vCallbackTarget.Size() + m_vCallbackTargetQueue.Size() );
}

void CMessageList::ClearAllMessageTargets()
{
	{
		CTlLocalLock ll( m_vMsgTargetQueue );
		if ( ll.IsLocked() )
			m_vMsgTargetQueue.clear();
	}

	{
		CTlLocalLock ll( m_vMsgTarget );
		if ( ll.IsLocked() )
			m_vMsgTarget.clear();
	}

	{
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( ll.IsLocked() )
			m_vCallbackTargetQueue.clear();
	}

	{
		CTlLocalLock ll( m_vCallbackTarget );
		if ( ll.IsLocked() )
			m_vCallbackTarget.clear();
	}
}

void CMessageList::UpdateMessageQueue()
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vMsgTargetQueue );
	if ( !ll.IsLocked() )
		return;

	THList< GUID, CMsgTarget >::iterator it = NULL;
	while ( ( it = m_vMsgTargetQueue.next( it ) ) != m_vMsgTargetQueue.end() )
	{
		// Remove any existing
		m_vMsgTarget.erase( *it->key() );

		// Add
		if ( 1 == (*it)->uAction )
			m_vMsgTarget.push_back( *it->key(), _PTR_NEW CMsgTarget( (*it)->hWnd, (*it)->uMsg, (*it)->bPost, 0, &(*it)->guid ) );

		it = m_vMsgTargetQueue.erase( it );

	} // end while
}

BOOL CMessageList::SendWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid)
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vMsgTarget );
	if ( !ll.IsLocked() )
		return FALSE;

	UpdateMessageQueue();

	THList< GUID, CMsgTarget >::iterator it = NULL;
	while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
	{
		if ( ::IsWindow( (*it)->hWnd ) )
		{
			if ( pguid == NULL
				 || IsEqualGUID( (*it)->guid, CLSID_ZERO )
				 || IsEqualGUID( (*it)->guid, *pguid ) )
				::SendMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );

		} // end if

		else
			it = m_vMsgTarget.erase( it );
	} // end while

	return TRUE;
}

BOOL CMessageList::PostWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid)
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vMsgTarget );
	if ( !ll.IsLocked() )
		return FALSE;

	UpdateMessageQueue();

	THList< GUID, CMsgTarget >::iterator it = NULL;
	while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
	{
		if ( ::IsWindow( (*it)->hWnd ) )
		{
			if ( pguid == NULL
				 || IsEqualGUID( (*it)->guid, CLSID_ZERO )
				 || IsEqualGUID( (*it)->guid, *pguid ) )
				::PostMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
		} // end if

		else
			it = m_vMsgTarget.erase( it );

	} // end while

	return TRUE;
}

BOOL CMessageList::MWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid)
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vMsgTarget );
	if ( !ll.IsLocked() )
		return FALSE;

	UpdateMessageQueue();

	THList< GUID, CMsgTarget >::iterator it = NULL;
	while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
	{
		if ( ::IsWindow( (*it)->hWnd ) )
		{
			if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
			{
				if ( (*it)->bPost )
					::PostMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
				else
					::SendMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );

			} // end if

		} // end if

		else
			it = m_vMsgTarget.erase( it );

	} // end while

	return TRUE;
}

void CMessageList::UpdateCallbackQueue()
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vCallbackTargetQueue );
	if ( !ll.IsLocked() )
		return;

	THList< GUID, CCallbackTarget >::iterator it = NULL;
	while ( ( it = m_vCallbackTargetQueue.next( it ) ) != m_vCallbackTargetQueue.end() )
	{
		// Remove any existing
		m_vCallbackTarget.erase( *it->key() );

		// Add
		if ( 1 == (*it)->uAction )
			m_vCallbackTarget.push_back( *it->key(), _PTR_NEW CCallbackTarget( (*it)->pCallbackFunction, (*it)->pCallbackFunction2, (*it)->pData, 1, &(*it)->guid ) );

		it = m_vCallbackTargetQueue.erase( it );

	} // end while
}

BOOL CMessageList::SetCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid )
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vCallbackTargetQueue );
	if ( !ll.IsLocked() )
		return FALSE;

	if ( pFunction != NULL )
	{
		GUID guid;
		CreateKey( &guid, (void*)pFunction, pData, pguid );

		m_vCallbackTargetQueue.erase( guid );
		m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( pFunction, NULL, pData, 1, pguid ) );

	} // end if

	return TRUE;
}

BOOL CMessageList::SetCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid )
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vCallbackTargetQueue );
	if ( !ll.IsLocked() )
		return FALSE;

	if ( pFunction != NULL )
	{
		GUID guid; CreateKey( &guid, (void*)pFunction, pData, pguid );
		m_vCallbackTargetQueue.erase( guid );
		m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( NULL, pFunction, pData, 1, pguid ) );

	} // end if

	return TRUE;
}

BOOL CMessageList::DoCallback( WPARAM wParam, LPARAM lParam, const GUID *pguid )
{
	if ( m_bClose )
		return FALSE;

	CTlLocalLock ll( m_vCallbackTarget );
	if ( !ll.IsLocked() )
		return FALSE;

	UpdateCallbackQueue();

	THList< GUID, CCallbackTarget >::iterator it = NULL;
	while ( ( it = m_vCallbackTarget.next( it ) ) != m_vCallbackTarget.end() )
	{
		RULIB_TRY
		{
			if ( (*it)->pCallbackFunction != NULL || (*it)->pCallbackFunction2 != NULL )
			{
				if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
				{
					if ( (*it)->pCallbackFunction != NULL )
						(*it)->pCallbackFunction( (*it)->pData, wParam, lParam );
					else if ( (*it)->pCallbackFunction2 != NULL )
						(*it)->pCallbackFunction2( (*it)->pData, wParam, lParam, pguid );

				} // end if

			} // end if

			else
				it = m_vCallbackTarget.erase( it );

		} RULIB_CATCH_ALL

		// Next
		{ RULIB_TRY { it = m_vCallbackTarget.erase( it ); } RULIB_CATCH_ALL { return TRUE; } }

	} // end while

	return TRUE;
}

void CMessageList::RemoveCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid )
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vCallbackTargetQueue );
	if ( !ll.IsLocked() )
		return;

	if ( pFunction != NULL )
	{
		GUID guid;
		CreateKey( &guid, (void*)pFunction, pData, pguid );

		m_vCallbackTargetQueue.erase( guid );
		m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( pFunction, NULL, pData, 2, pguid ) );

	} // end if

//  GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
//	m_vCallbackTarget.erase( guid );
}

void CMessageList::RemoveCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid )
{
	if ( m_bClose )
		return;

	CTlLocalLock ll( m_vCallbackTargetQueue );
	if ( !ll.IsLocked() )
		return;

	if ( pFunction != NULL )
	{
		GUID guid;
		CreateKey( &guid, (void*)pFunction, pData, pguid );

		m_vCallbackTargetQueue.erase( guid );
		m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( NULL, pFunction, pData, 2, pguid ) );

	} // end if

//  GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
//	m_vCallbackTarget.erase( guid );
}

BOOL CMessageList::MMessage( WPARAM wParam, LPARAM lParam, const GUID *pguid, BOOL bFunction, BOOL bWMessage )
{
	if ( m_bClose )
		return FALSE;

	BOOL bRet = FALSE;

	if ( bFunction )
		bRet |= DoCallback( wParam, lParam, pguid );

	if ( MessageTargets() )
		bRet |= MWMMessage( wParam, lParam, pguid );

	return bRet;
}

BOOL CMessageList::_MSendMessage( WPARAM wParam, LPARAM lParam, const GUID *pguid, BOOL bFunction, BOOL bWMessage )
{
	if ( m_bClose )
		return FALSE;

	BOOL bRet = FALSE;

	if ( bFunction )
		bRet |= DoCallback( wParam, lParam, pguid );

	if ( MessageTargets() )
		bRet |= SendWMMessage( wParam, lParam, pguid );

	return bRet;
}

BOOL CMessageList::_MPostMessage( WPARAM wParam, LPARAM lParam, const GUID *pguid, BOOL bFunction, BOOL bWMessage )
{
	if ( m_bClose )
		return FALSE;

	BOOL bRet = FALSE;

	if ( bFunction )
		bRet |= DoCallback( wParam, lParam, pguid );

	if ( MessageTargets() )
		bRet |= PostWMMessage( wParam, lParam, pguid );

	return bRet;
}

BOOL CThreadTimer::DoThread( LPVOID pData )
{
	DWORD dwWait = m_dwDelay;
	m_dwPos = GetTickCount() + m_dwDelay;
	
	while ( 0 != ::WaitForSingleObject( GetStopEvent(), dwWait ) )
	{
		// Do the callback
		DoCallback( (WPARAM)dwWait, (LPARAM)this );

		// Where we want to be
		m_dwPos += m_dwDelay;
					
		// Where we are
		DWORD dwTick = GetTickCount();
		if ( dwTick < m_dwPos )
			dwWait = m_dwPos - dwTick;
		else
			dwWait = 0;

	} // end while

	return FALSE;
}


