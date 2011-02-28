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
// TrayIcon.cpp: implementation of the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon()
{_STT();
}

CTrayIcon::~CTrayIcon()
{_STT();
}

BOOL CTrayIcon::Add(HWND hWnd, UINT uMsg, HICON hIcon, LPCTSTR tip, DWORD flags)
{_STT();
	NOTIFYICONDATA	nid;

	nid.cbSize				= sizeof( NOTIFYICONDATA );
	nid.hWnd				= hWnd;
	nid.uID					= 0;
	nid.uFlags				= 0;

	// Callback message?
	if ( uMsg != NULL )
		nid.uCallbackMessage = uMsg, nid.uFlags |= NIF_MESSAGE;

	// Icon?
	if ( hIcon != NULL )
		nid.hIcon = hIcon, nid.uFlags |= NIF_ICON;

	// Tooltip?
	if ( tip != NULL )
	{	nid.uFlags |= NIF_TIP;
		if ( tip != NULL ) strcpy( nid.szTip, tip );
		else *nid.szTip = 0x0;
	} // end if

	// Set the icon
	BOOL ret = Shell_NotifyIcon( flags, &nid );

	return ret;
}

BOOL CTrayIcon::Remove(HWND hWnd, UINT uMsg)
{_STT();
	return Add( hWnd, uMsg, NULL, NULL, NIM_DELETE );
}

BOOL CTrayIcon::ShowPopupMenu(HWND hParent, LPCTSTR pMenu, long x, long y, long nSub, HINSTANCE hInstance)
{_STT();
	if ( x == -1 && y == -1 )
	{	POINT	pt = { 0, 0 };
		::GetCursorPos( &pt );
		x = pt.x; y = pt.y;
	} // end if

	HMENU hMenu, hSub;
//	BOOL bOnTop = FALSE;

	// Load the template
	hMenu = LoadMenu( hInstance, pMenu );
	if ( hMenu == NULL ) return FALSE;

	if ( nSub == -1 ) hSub = hMenu;

	else 
	{	hSub = GetSubMenu( hMenu, 0 );
		if ( hSub == NULL ) return FALSE;
	} // end else

	// We must be the foreground window for this to work correctly
	::SetForegroundWindow( hParent );

	// Show the popup menu
	::TrackPopupMenu( hSub, TPM_RIGHTBUTTON, x, y, 0, hParent, NULL );

	// Unload the menu
	DestroyMenu( hMenu );

	return TRUE;
}
