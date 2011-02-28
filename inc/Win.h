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
// Win.h: interface for the CWin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN_H__D00C3004_D71B_11D2_8919_00104B2C9CFA__INCLUDED_)
#define AFX_WIN_H__D00C3004_D71B_11D2_8919_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//==================================================================
// CWin
//
/// Wraps a HWND handle
/**
	Basic window encapsulation.
*/
//==================================================================
class CWin  
{
public:

	//==============================================================
	// OnCreate()
	//==============================================================
	/// Called in response to WM_CREATE
	virtual LRESULT OnCreate();

	//==============================================================
	// OnCommand()
	//==============================================================
	/// Called in response to WM_COMMAND
	/**
		\param [in] wParam 
		\param [in] lParam 
	*/
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

public:
	
	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches to existing HWND
	/**
		\param [in] hWnd	-	Window handle
	*/
	void Attach( HWND hWnd ) { DestroyWindow(); m_hWnd = hWnd; }

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from HWND without destroying the window
	void Detach() { m_hWnd = NULL; }

	//==============================================================
	// OnPaint()
	//==============================================================
	/// Called in response to WM_PAINT message
	/**
		\param [in] hDC		-	Handle to Window Device Context
	*/
	virtual BOOL OnPaint( HDC hDC );

	//==============================================================
	// OnEraseBkGnd()
	//==============================================================
	/// Called in response to WM_ERASEBKGND message
	/**
		\param [in] hDC		-	Handle to window Device Context
		
		\return Non-zero unless you want default erase
	
		\see 
	*/
	virtual BOOL OnEraseBkGnd( HDC hDC );

	//==============================================================
	// CenterWindow()
	//==============================================================
	/// Centers window
	/**
		\param [in] hWnd		-	Window handle to center
		\param [in] hParent		-	Centers within specified window
		\param [in] pRect		-	Centers within specified rect, ignored
									if hParent is not NULL

		Call with either hParent, or pRect
		
		\return 
	
		\see 
	*/
	static void CenterWindow( HWND hWnd, HWND hParent = NULL, LPRECT pRect = NULL );

	//==============================================================
	// AutoCenter()
	//==============================================================
	/// If auto center is enabled, window will center in screen when created.
	/**
		\param [in] b	-	Non-zero to enable auto center.
	*/
	void AutoCenter( BOOL b ) { m_bAutoCenter = b; }

	//==============================================================
	// CenterOnParent()
	//==============================================================
	/// Enable to center window on parent when created
	/**
		\param [in] b	-	Non-zero to center on parent
	*/
	void CenterOnParent( BOOL b ) { m_bCenterOnParent = b; }

	//==============================================================
	// CenterWindow()
	//==============================================================
	/// Call to center window properly
	void CenterWindow()
	{	CenterWindow( GetSafeHwnd(), m_bCenterOnParent ? GetParent() : NULL, NULL ); }

	//==============================================================
	// DestroyWindow()
	//==============================================================
	/// Called in response to WM_DESTROY
	virtual void DestroyWindow();

	//==============================================================
	// DefaultCREATESTRUCT()
	//==============================================================
	/// Call to initialize CREATESTRUCT with default values
	void DefaultCREATESTRUCT();

	//==============================================================
	// GetUniqueString()
	//==============================================================
	/// Generates a unique string
	/**
		\param [out] buf		-	Receives unique string
		\param [in] max			-	Size of buffer in buf
		\param [in] prefix		-	Prefix for unique string
		
		You may want to use CWin32::GuidToString() instead.

		\return Non-zero if success
	
		\see 
	*/
	BOOL GetUniqueString( LPSTR buf, DWORD max, LPCTSTR prefix = NULL );

	//==============================================================
	// DefaultWNDCLASS()
	//==============================================================
	/// Initializes WNDCLASS structure to default values
	void DefaultWNDCLASS();

	//==============================================================
	// RegisterClass()
	//==============================================================
	/// Registers the window class
	/**
		\return Non-zero if success
	*/
	BOOL RegisterClass();

	//==============================================================
	// Create()
	//==============================================================
	/// Creates the window
	/**
		\param [in] pTitle	-	Window title
		
		\return Non-zero if window was created successfully
	*/
	BOOL Create( LPCTSTR pTitle );

	/// Constructor
	CWin();

	/// Destructor
	~CWin();

	//==============================================================
	// GetInstance()
	//==============================================================
	/// Returns instance handle
	HINSTANCE GetInstance() { return m_hInst; }

	//==============================================================
	// GetSafeHwnd()
	//==============================================================
	/// Returns window handle
	HWND GetSafeHwnd() { return m_hWnd; }

	//==============================================================
	// GetParent()
	//==============================================================
	/// Returns parent window handle
	HWND GetParent() { return m_cs.hwndParent; }

	//==============================================================
	// IsWindow()
	//==============================================================
	/// Returns non-zero if valid window handle
	BOOL IsWindow() { return ( GetSafeHwnd() && ::IsWindow( GetSafeHwnd() ) ); }

	//==============================================================
	// IsWnd()
	//==============================================================
	/// Returns non-zero if valid window handle
	BOOL IsWnd() { return ( GetSafeHwnd() != NULL ); }

	//==============================================================
	// ShowWindow()
	//==============================================================
	/// Sets the windows show state
	/**
		\param nCmdShow	-	Show state

		\return Non-zero if success
	*/
	BOOL ShowWindow( int nCmdShow = SW_SHOWNORMAL )
	{	return ::ShowWindow( m_hWnd, nCmdShow ); }

	//==============================================================
	// UpdateWindow()
	//==============================================================
	/// Sends a WM_PAINT message to window
	/**		
		\return Non-zero if success
	*/
	BOOL UpdateWindow()
	{	return ::UpdateWindow( m_hWnd ); }

	//==============================================================
	// Invalidate()
	//==============================================================
	/// Marks the window client area as invalid to invoke a redraw
	/**
		\return Non-zero if success
	*/
	BOOL Invalidate() 
	{	return ::InvalidateRect( m_hWnd, NULL, TRUE ); }

	//==============================================================
	// RedrawWindow()
	//==============================================================
	/// Redraws the window
	/**
		\param [in] pRect	-	Area to redraw
		\param [in] hRgn	-	Region to redraw
		\param [in] uFlags	-	Redraw flags
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RedrawWindow(	const RECT *pRect = NULL,
						HRGN hRgn = NULL,
						UINT uFlags =	RDW_ERASE | RDW_INVALIDATE |
										RDW_ERASENOW | RDW_UPDATENOW |
										RDW_ALLCHILDREN )
	{	return ::RedrawWindow( m_hWnd, pRect, hRgn, uFlags ); }

	//==============================================================
	// SetPos()
	//==============================================================
	/// Sets the windows position in the Z-Order
	/**
		\param [in] hWnd - Window handle to insert after
		
		\return Non-zero if success
	*/
	BOOL SetPos( HWND hWnd )
	{	return ::SetWindowPos( m_hWnd, hWnd, 0, 0, 0, 0,
								SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE );
	}

	//==============================================================
	// SetPos()
	//==============================================================
	/// Sets the windows position
	/**
		\param [in] x	-	Horizontal offset of upper left corner
		\param [in] y	-	Vertical offset of upper left corner
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPos( int x, int y )
	{	return SetPos( m_hWnd, x, y ); }

	//==============================================================
	// SetPos()
	//==============================================================
	/// 
	/**
		\param [in] hWnd	-	Window handle
		\param [in] x		-	Horizontal offset of upper left corner
		\param [in] y		-	Vertical offset of upper left corner
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetPos( HWND hWnd, int x, int y )
	{	return ::SetWindowPos( hWnd, NULL, x, y, 0, 0,
								SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER );
	}

	//==============================================================
	// SetWindowPos()
	//==============================================================
	/// Sets the window position
	/**
		\param [in] hWndInsertAfter		-	Window to inser after in 
											Z-order.
		\param [in] x					-	Horizontal offset of upper
											left corner.
		\param [in] y					-	Vertical offset of upper
											left corner
		\param [in] cx					-	Horizontal size
		\param [in] cy					-	Vertical size
		\param [in] uFlags				-	SetWindowPos() flags
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetWindowPos( HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags )
	{ return ::SetWindowPos( m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags ); }

	//==============================================================
	// SetInstance()
	//==============================================================
	/// Sets the instance handle 
	/**
		\param [in] hInst	-	Module instance handle
	*/
	void SetInstance( HINSTANCE hInst ) 
	{ m_hInst = m_cs.hInstance = m_wc.hInstance = hInst; }

	//==============================================================
	// SetForegroundWindow()
	//==============================================================
	/// Sets this window as the foreground window
	/**			
		\return Non-zero if success
	*/
	BOOL SetForegroundWindow() { return ::SetForegroundWindow( GetSafeHwnd() ); }

	//==============================================================
	// BeginPaint()
	//==============================================================
	/// Call in response to a WM_PAINT to validate window region
	/**		
		\return	DC handle that needs redrawing
	*/
	virtual HDC BeginPaint();

	//==============================================================
	// GetRegisteredClassAtom()
	//==============================================================
	/// Gets atom registered for class
	ATOM GetRegisteredClassAtom() { return m_hAtom; }

protected:

	/// Window handle
	HWND			m_hWnd;

private:

	/// For hooking windows
	WNDPROC	m_pPrevWndProc;

	/// Window info
	HINSTANCE		m_hInst;

	/// Window class name
	char			m_classname[ MAX_PATH ];

	//==============================================================
	// WindowProc()
	//==============================================================
	/// Window callback function
	/**
		\param [in] hwnd		-	Window handle
		\param [in] uMsg		-	Message ID
		\param [in] wParam		-	WPARAM
		\param [in] lParam		-	LPARAM
		
		\return Windows result
	
		\see 
	*/
	static LRESULT CALLBACK WindowProc(	HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CWin			*m_pWin;

	BOOL			m_bEndPaint;
	PAINTSTRUCT		m_ps;

	BOOL			m_bAutoCenter;
	BOOL			m_bCenterOnParent;

	ATOM			m_hAtom;

public:

	//==============================================================
	// OnWindowProc()
	//==============================================================
	/// Called in response to windows message
	/**
		\param [in] bSubclassing	-	Non-zero if subclassing
		\param [in] hWnd			-	Window handle
		\param [in] uMsg			-	Window message
		\param [in] wParam			-	WPARAM
		\param [in] lParam			-	LPARAM
		
		\return 
	
		\see 
	*/
	virtual LRESULT OnWindowProc( BOOL bSubclassing, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//==============================================================
	// UnregisterClass()
	//==============================================================
	/// Unregisters windows class
	void UnregisterClass();

	//==============================================================
	// Quit()
	//==============================================================
	/// Puts WM_QUIT message into Windows message queue
	void Quit( int nRet = 0 );

	//==============================================================
	// OnIdle()
	//==============================================================
	/// Called during idle periods
	/**
		\param [in] dwCalls		-	Number of times called since message
									was received.
		
		\return Non-zero to get called again after checking for messages.
	
		\see 
	*/
	virtual BOOL OnIdle( DWORD dwCalls );

	//==============================================================
	// PumpMessage()
	//==============================================================
	/// Pumps a single message from Windows message queue
	/**
		\param [in] hWnd	-	Window handle to pump messages.
								Set to NULL to pump all messages.
		\param [in] hAccel	-	Accelerator table for translating
								messages.
		
		\return Non-zero unless WM_QUIT was processed.
	
		\see 
	*/
	static BOOL PumpMessage( HWND hWnd = NULL, HACCEL hAccel = NULL );

	//==============================================================
	// PumpMessage()
	//==============================================================
	/// Pumps all messages from Windows message queue
	/**
		\param [in] hWnd	-	Window handle to pump messages.
								Set to NULL to pump all messages.
		\param [in] hAccel	-	Accelerator table for translating
								messages.
		
		\return Non-zero unless WM_QUIT was processed.
	
		\see 
	*/
	static BOOL PumpMessages( HWND hWnd = NULL, HACCEL hAccel = NULL );

	//==============================================================
	// DoMessagePump()
	//==============================================================
	/// Pumps messages from Windows message queue
	/**
		\param [in] hWnd	-	Window handle to pump messages.
								Set to NULL to pump all messages.
		\param [in] hAccel	-	Accelerator table for translating
								messages.
		
		\return Non-zero unless WM_QUIT was processed.
	
		\see 
	*/
	BOOL DoMessagePump( HWND hWnd = NULL, HACCEL hAccel = NULL );

	//==============================================================
	// CallWindowProc()
	//==============================================================
	/// Calls previous windows message handler function
	/**
		\param [in] uMsg	-	Message ID
		\param [in] wParam	-	WPARAM
		\param [in] lParam	-	LPARAM
		
		\return Windows return code
	
		\see 
	*/
	LRESULT CallWindowProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Destroys window and releases resources
	void Destroy();

	//==============================================================
	// UnhookHwnd()
	//==============================================================
	/// Unhooks any subclassed window
	/**		
		\return Non-zero if success
	*/
	BOOL UnhookHwnd();

	//==============================================================
	// HookHwnd()
	//==============================================================
	/// Subclasses specified window
	/**
		\param [in] hWnd	-	Handle to window to subclass
		
		\return Non-zero if success
	*/
	BOOL HookHwnd( HWND hWnd );

	//==============================================================
	// OnWMMessage()
	//==============================================================
	/// Called when WMMessage is received
	/**
		\param [in] uMsg		-	Message ID
		\param [in] wParam		-	WPARAM
		\param [in] lParam		-	LPARAM
		
		\return Non-zero if success
	*/
	virtual BOOL OnWMMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	/// Window class information structure
	WNDCLASS		m_wc;

	/// Window create structure information
	CREATESTRUCT	m_cs;
};

#endif // !defined(AFX_WIN_H__D00C3004_D71B_11D2_8919_00104B2C9CFA__INCLUDED_)
