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
// DlgMover.h: interface for the CDlgMover class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGMOVER_H__EC4DF16A_BF6B_11D2_88F8_00104B2C9CFA__INCLUDED_)
#define AFX_DLGMOVER_H__EC4DF16A_BF6B_11D2_88F8_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define DM_DOCKLEFT		0x00000001
#define DM_DOCKTOP		0x00000002
#define DM_DOCKRIGHT	0x00000004
#define DM_DOCKBOTTOM	0x00000008
#define DM_DOCKALL		0x0000000f

#define DM_SIZELEFT		0x00000001
#define DM_SIZETOP		0x00000002
#define DM_SIZERIGHT	0x00000004
#define DM_SIZEBOTTOM	0x00000008
#define DM_SIZEALL		0x0000000f

//==================================================================
// CDlgMover
//
/// Encapsulates functionality needed to handle docking window
/**
	This encapsulates the functionality needed to create a dockable
	window.	
*/
//==================================================================
class CDlgMover  
{
public:

	//==============================================================
	// SetSizeCursor()
	//==============================================================
	/// Sets the appropriate window sizing cursor
	/**
		\param [in] pt		-	Current cursor position
		\param [in] bMove	-	Set to non-zero to update window size.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetSizeCursor( POINT pt, BOOL bMove );

	//==============================================================
	// SetAppBarPos()
	//==============================================================
	/// Allocates desktop space for docking
	/**
		\param [in] pRect	-	Docking area
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetAppBarPos( LPRECT pRect );
	
	//==============================================================
	// OnSize()
	//==============================================================
	/// Call when window is resized
	/**
		\return Non-zero if success
	*/
	BOOL OnSize();

	//==============================================================
	// OnMove()
	//==============================================================
	/// Call when window is moved
	/**
		\param [in] x	-	New horizontal offset
		\param [in] y	-	New vertical offset
		
		\return Non-zero on success
	
		\see 
	*/
	BOOL OnMove( int x, int y );

	//==============================================================
	// GetAppBarPos()
	//==============================================================
	/// Returns the desired position of the window
	/**
		\param [out] pRect	-	Receives position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetAppBarPos( LPRECT pRect );

	//==============================================================
	// OnAppBarMsg()
	//==============================================================
	/// Call in response to app bar message
	/**
		\param [in] wParam	-	Pass on from Operating system
		\param [in] lParam	-	Pass on from Operating system
		
		Call this function when your window receives an appbar message.

		\return Windows return value.
	
		\see 
	*/
	LRESULT OnAppBarMsg( WPARAM wParam, LPARAM lParam );

	//==============================================================
	// DestroyAppBar()
	//==============================================================
	/// Destroys the app bar
	void DestroyAppBar();

	//==============================================================
	// CreateAppBar()
	//==============================================================
	/// Reserves desktop space for app bar
	/**
		\param [in] pRect	-	Position of app bar

		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateAppBar( LPRECT pRect = NULL );

	//==============================================================
	// KillAppBar()
	//==============================================================
	/// Releases desktop space for app bar
	void KillAppBar();

	//==============================================================
	// InitAppBar()
	//==============================================================
	/// Initializes app bar handling
	/**
		\param [in] hWnd	-	App bar window handle
		\param [in] uMsg	-	App bar message callback value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL InitAppBar( HWND hWnd, UINT uMsg );

	//==============================================================
	// Dock()
	//==============================================================
	/// Call to dock the app bar
	/**
		\param [in] hWnd	-	Handle to app bar window
		\param [in] pt		-	Point to dock window
		\param [in] cursor	-	Cursor position
		
		\return Non-zero if docked
	
		\see 
	*/
	BOOL Dock( HWND hWnd, LPPOINT pt, LPPOINT cursor );

	//==============================================================
	// GetPos()
	//==============================================================
	/// Returns window position
	/**
		\param [in] hWnd	-	Handle to window
		\param [out] pPoint	-	Receives window position
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetPos( HWND hWnd, POINT *pPoint );

	//==============================================================
	// SetPos()
	//==============================================================
	/// Sets window position
	/**
		\param [in] hWnd	-	Handle of window
		\param [in] point	-	Contains new position of window
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetPos( HWND hWnd, POINT point );

	//==============================================================
	// OnMouseMove()
	//==============================================================
	/// Call in response to WM_MOUSEMOVE
	/**
		\param [in] nFlags	-	Mouse move flags
		\param [in] point	-	Cursor position
		
		\return Non-zero if window should be redrawn.
	
		\see 
	*/
	BOOL OnMouseMove( UINT nFlags, POINT point );

	//==============================================================
	// OnLButtonUp()
	//==============================================================
	/// Call in response to WM_LBUTTONUP
	/**
		
		\return Non-zero if window should be redrawn
	
		\see 
	*/
	BOOL OnLButtonUp();

	//==============================================================
	// OnLButtonDown()
	//==============================================================
	/// Call in response to WM_LBUTTONDOWN
	/**
		\param [in] hWnd		-	Handle to window
		\param [in] point		-	Cursor position
		\param [in] hCapture	-	Handle to window receiving input capture.
		
		\return 
	
		\see 
	*/
	void OnLButtonDown( HWND hWnd, POINT point, HWND hCapture = NULL );

	/// Constructor
	CDlgMover();

	/// Destructor
	virtual ~CDlgMover();
	
	//==============================================================
	// IsMoving()
	//==============================================================
	/// Returns non-zero if window is being moved
	BOOL IsMoving() { return ( m_hWnd != NULL ); }

	//==============================================================
	// SetFrameWindow()
	//==============================================================
	/// Sets the frame window handle
	/**
		\param [in] hWnd	-	Window handle
	*/
	void SetFrameWindow( HWND hWnd ) { m_hFrame = hWnd; }

	//==============================================================
	// SetDock()
	//==============================================================
	/// Sets dock point
	/**
		\param [in] x	-	Horizontal offset of dock point
		\param [in] y	-	Vertical offset of dock point
	*/
	void SetDock( long x, long y ) { m_xDock = x; m_yDock = y; }

	//==============================================================
	// SizeDock()
	//==============================================================
	/// Allows window to be sized when docked
	/**
		\param [in] b	-	Non-zero to allow docked window sizing.
	*/
	void SizeDock( BOOL b ) { m_bSizeDock = b; }

	//==============================================================
	// Size()
	//==============================================================
	/// Sets initial undocked window size
	/**
		\param [in] dw		-	Window size
		\param [in] hWnd	-	Handle to window
	*/
	void Size( DWORD dw, HWND hWnd ) { m_dwSize = dw; m_hWnd = hWnd; }

	//==============================================================
	// SetDockingSize()
	//==============================================================
	/// Sets initial docked size
	/**
		\param [in] w	-	Docked width
		\param [in] h	-	Docked height
		
		\return 
	
		\see 
	*/
	void SetDockingSize( long w, long h )
	{	m_szDocking.cx = w; m_szDocking.cy = h; }

	//==============================================================
	// SetDragPoint()
	//==============================================================
	/// Sets the drag point
	/**
		\param [in] x	-	Horizontal offset of drag point
		\param [in] y	-	Vertical offset of drag point
	*/
	void SetDragPoint( long x, long y )
	{	m_ptDrag.x = x; m_ptDrag.y = y; }

	//==============================================================
	// SetMinTrackSize()
	//==============================================================
	/// Sets the minimum size for the window
	/**
		\param [in] cx	-	Minimum horizontal size
		\param [in] cy	-	Minimum vertical size
	*/
	void SetMinTrackSize( long cx, long cy )
	{	m_szMinTrackSize.cx = cx; m_szMinTrackSize.cy = cy; }
		
	//==============================================================
	// SetMaxTrackSize()
	//==============================================================
	/// Sets the maximum size for the window
	/**
		\param [in] cx	-	Maximum horizontal size
		\param [in] cy	-	Maximum vertical size
	*/
	void SetMaxTrackSize( long cx, long cy )
	{	m_szMaxTrackSize.cx = cx; m_szMaxTrackSize.cy = cy; }

	//==============================================================
	// AllowDocking()
	//==============================================================
	/// Sets allowed docking positions
	/**
		\param [in] f	-	Flags indicating allowed docking positions

							-	DM_DOCKLEFT		-	Left docking
							-	DM_DOCKTOP		-	Top docking
							-	DM_DOCKRIGHT	-	Right docking
							-	DM_DOCKBOTTOM	-	Bottom docking
							-	DM_DOCKALL		-	Any docking
		
	*/
	void AllowDocking( DWORD f ) { m_dwAllowDocking = f; }
	
	//==============================================================
	// SetEdgeSize()
	//==============================================================
	/// Sets the size of the resizing edge area
	/**
		\param [in] x	-	Horizontal size in pixels
		\param [in] y	-	Veritcal size in pixels
	*/
	void SetEdgeSize( long x, long y ) { m_lxEdge = x; m_lyEdge = y; }

private:

	POINT	m_point;
	HWND	m_hWnd;

	DWORD	m_dwSize;

	BOOL	m_bResized;
	DWORD	m_dwCursor;
	POINT	m_ptMove;
	RECT	m_rectRef;

	long	m_xDock;
	long	m_yDock;

	long	m_lyEdge;
	long	m_lxEdge;

	BOOL	m_bDocked;
	DWORD	m_dwDockPos;
	DWORD	m_dwAllowDocking;

	BOOL	m_bSizeDock;
	RECT	m_rSize;
	RECT	m_rDockedSize;
	RECT	m_rUndockedSize;

	HWND	m_hFrame;

	HWND	m_hAppBar;
	UINT	m_uAppBarMsg;
	BOOL	m_bAppBar;

	BOOL	m_bButtonDown;

	DWORD	m_dwDockPosOnClick;

	SIZE	m_szDocking;

	SIZE	m_szMinTrackSize;
	SIZE	m_szMaxTrackSize;

	POINT	m_ptDrag;

};

#endif // !defined(AFX_DLGMOVER_H__EC4DF16A_BF6B_11D2_88F8_00104B2C9CFA__INCLUDED_)
