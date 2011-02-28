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
// TrayIcon.h: interface for the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICON_H__0CD4963D_5C06_4D27_8371_566F8EC456E2__INCLUDED_)
#define AFX_TRAYICON_H__0CD4963D_5C06_4D27_8371_566F8EC456E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CTrayIcon
//
/// Provides simple functions for adding tray icon support
/**
	Simple functions for adding tray icons to the windows taskbar 
	tray.	
*/
//==================================================================
class CTrayIcon  
{
public:

	//==============================================================
	// ShowPopupMenu()
	//==============================================================
	/// Call to display a popup window on the tray
	/**
		\param [in] hParent		-	Parent window handle
		\param [in] pMenu		-	Menu resource id
		\param [in] x			-	Horzontal position to display menu
		\param [in] y			-	Vertical position to display menu
		\param [in] nSub		-	Sub menu index
		\param [in] hInstance	-	Module containing the menu in pMenu
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ShowPopupMenu( HWND hParent, LPCTSTR pMenu, long x = -1, long y = -1, long nSub = 0, HINSTANCE hInstance = NULL );

	//==============================================================
	// Remove()
	//==============================================================
	/// Removes the tray icon
	/**
		\param [in] hWnd	-	Parent window
		\param [in] uMsg	-	Tray icon message id
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Remove( HWND hWnd, UINT uMsg );	

	//==============================================================
	// Add()
	//==============================================================
	/// Adds icon to tray
	/**
		\param [in] hWnd	-	Parent window handle
		\param [in] uMsg	-	Tray icon message id
		\param [in] hIcon	-	Handle to icon displayed in tray
		\param [in] tip		-	Tooltip for tray icon
		\param [in] flags	-	Tray icon flags
		
		\return 
	
		\see 
	*/
	static BOOL Add( HWND hWnd, UINT uMsg, HICON hIcon, LPCTSTR tip, DWORD flags = 0 );

	/// Constructor
	CTrayIcon();

	/// Destructor
	virtual ~CTrayIcon();

};

#endif // !defined(AFX_TRAYICON_H__0CD4963D_5C06_4D27_8371_566F8EC456E2__INCLUDED_)
