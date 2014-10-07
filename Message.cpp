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
// Message.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{
	ClearMessageTarget();
	ClearCallbackFunction();
}

CMessage::~CMessage()
{
}

void CMessage::SetMessageTarget( HWND hWnd, UINT uWMMessageID )
{
	m_hWnd = hWnd;
	m_uWMMessageID = uWMMessageID;
}

BOOL CMessage::SendWMMessage(WPARAM wParam, LPARAM lParam)
{
	if ( !IsMessageTarget() )
		return FALSE;

	::SendMessage( m_hWnd, m_uWMMessageID, wParam, lParam );

	return TRUE;
}

BOOL CMessage::PostWMMessage(WPARAM wParam, LPARAM lParam)
{
	if (!IsMessageTarget())
		return FALSE;

	::PostMessage( m_hWnd, m_uWMMessageID, wParam, lParam );

	return TRUE;
}

BOOL CMessage::SetCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData )
{
	if ( pFunction == NULL )
		return FALSE;

	m_pCallbackFunction = pFunction;
	m_pData = pData;

	return TRUE;
}

BOOL CMessage::DoCallback( WPARAM wParam, LPARAM lParam )
{
	if ( !IsCallbackFunction() )
		return FALSE;

	return ( GetCallbackFunction() )( m_pData, wParam, lParam );
}

BOOL CMessage::MSendMessage( WPARAM wParam, LPARAM lParam, BOOL bFunction, BOOL bWMessage )
{
	BOOL bRet = FALSE;

	if ( bFunction )
		bRet |= DoCallback( wParam, lParam );

	if ( IsMessageTarget() )
		bRet |= SendWMMessage( wParam, lParam );

	return bRet;
}

BOOL CMessage::MPostMessage( WPARAM wParam, LPARAM lParam, BOOL bFunction, BOOL bWMessage )
{
	BOOL bRet = FALSE;

	if ( bFunction )
		bRet |= DoCallback( wParam, lParam );

	if ( IsMessageTarget() )
		bRet |= PostWMMessage( wParam, lParam );

	return bRet;
}

