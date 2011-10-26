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
// Tooltip.h: interface for the CTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLTIP_H__50F67050_017F_42DA_A823_47BB5092F620__INCLUDED_)
#define AFX_TOOLTIP_H__50F67050_017F_42DA_A823_47BB5092F620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CTooltip
//
/// Provides a wrapper for windows tooltips
/**
	Easily add tooltips to any window.	
*/
//==================================================================
class CTooltip  
{
public:

	//==============================================================
	// UpdateText()
	//==============================================================
	/// Call to update the tooltip text for an item 
	/**
		\param [in] uId		-	Item id
		\param [in] pText	-	New tooltip text
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL UpdateText( UINT uId, LPCTSTR pText );

	//==============================================================
	// RelayEvent()
	//==============================================================
	/// Called to relay tooltip event
	/**
		\param [in] hWnd	-	Window handle
		\param [in] uMsg	-	Message id
		\param [in] wParam	-	WPARAM value
		\param [in] lParam	-	LPARAM value
		\param [in] dwTime	-	Time
		\param [in] pt		-	Point
		
		\return 
	
		\see 
	*/
	BOOL RelayEvent( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, DWORD dwTime, POINT pt );

	//==============================================================
	// RelayEvent()
	//==============================================================
	/// Called to relay tooltip event
	/**
		\param [in] pMsg 
		
		\return 
	
		\see 
	*/
	BOOL RelayEvent( LPMSG pMsg );

	//==============================================================
	// Track()
	//==============================================================
	/// Call to move tracking tooltip
	/**
		\param [in] x	-	Horzontal offset of tooltip position
		\param [in] y	-	Vertical offset of tooltip position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Track( long x, long y );

	//==============================================================
	// TrackActivate()
	//==============================================================
	/// Call to activate a tracking tooltip
	/**
		\param [in] uId			-	ID of tooltip
		\param [in] bActivate	-	Non-zero to activate tooltip
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL TrackActivate( UINT uId, BOOL bActivate );
	
	//==============================================================
	// Add()
	//==============================================================
	/// Adds a tooltip to window control
	/**
		\param [in] uId			-	Control ID
		\param [in] pText		-	Tooltip text
		\param [in] dwFlags		-	Tooltip flags
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Add( UINT uId, LPCTSTR pText, DWORD dwFlags = TTF_SUBCLASS );

	//==============================================================
	// Add()
	//==============================================================
	/// Adds a tooltip region to window
	/**
		\param [in] pRect		-	Area associated with tooltip
		\param [in] pText		-	Tooltip text
		\param [in] dwFlags		-	Tooltip flags
		
		\return Non-zero on success
	
		\see 
	*/
	BOOL Add( LPRECT pRect, LPCTSTR pText, DWORD dwFlags = TTF_SUBCLASS );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Call to destroy all tooltips
	void Destroy();

	//==============================================================
	// Create()
	//==============================================================
	/// Initializes tooltip support for the specified window
	/**
		\param [in] hWnd		-	Window that will support tooltips
		\param [in] hInstance	-	Application instance handle
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( HWND hWnd, HINSTANCE hInstance );

	/// Constructor
	CTooltip();

	/// Destructor
	virtual ~CTooltip();

	//==============================================================
	// IsTip()
	//==============================================================
	/// Returns non-zero if tooltips are initialized
	BOOL IsTip() { return ( m_hWnd != NULL && IsWindow( m_hWnd ) ); }

	//==============================================================
	// GetSafeHwnd()
	//==============================================================
	/// Returns the tooltip window
	HWND GetSafeHwnd() { return m_hWnd; }

	//==============================================================
	// HWND()
	//==============================================================
	/// Returns the tooltip window
	operator HWND() { return m_hWnd; }

private:

	/// Tooltip window handle
	HWND			m_hWnd;

	/// Parent window handle
	HWND			m_hParent;

};

#endif // !defined(AFX_TOOLTIP_H__50F67050_017F_42DA_A823_47BB5092F620__INCLUDED_)
