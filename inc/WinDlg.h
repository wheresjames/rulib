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
// WinDlg.h: interface for the CWinDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDLG_H__97067A65_B217_11D2_88EE_00104B2C9CFA__INCLUDED_)
#define AFX_WINDLG_H__97067A65_B217_11D2_88EE_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//==================================================================
// CWinDlg
//
/// Base class for Windows dialog
/**
	This class encapsulates a windows dialog and provides basic
	functionality.  	
*/
//==================================================================
class CWinDlg  
{

// Virtual functions
public:

	//==============================================================
	// OnWMMessage()
	//==============================================================
	/// Called for all WM_XXX messages
	/**
		\param [in] uMessage	-	Message id
		\param [in] wParam		-	WPARAM
		\param [in] lParam		-	LPARAM
		
		\return 
	
		\see 
	*/
	virtual BOOL OnWMMessage( UINT uMessage, WPARAM wParam, LPARAM lParam );

	//==============================================================
	// OnOk()
	//==============================================================
	/// Called when user clicks the Ok button
	virtual void OnOk();

	//==============================================================
	// OnCancel()
	//==============================================================
	/// Called when user clicks the Cancel button
	virtual void OnCancel();

	// Called when WM_INITDIALOG
	//==============================================================
	// OnInitDlg()
	//==============================================================
	/// Called in response to WM_INITDIALOG
	/**
		\param [in] hwndFocus	-	Window with the focus
	*/
	virtual BOOL OnInitDlg( HWND hwndFocus );

	// Called when WM_COMMAND
	//==============================================================
	// OnCommand()
	//==============================================================
	/// Called in response to WM_COMMAND
	/**
		\param [in] wParam	-	WPARAM
								WORD wNotifyCode = HIWORD( wParam );
								WORD wId = LOWORD( wParam );
		\param [in] lParam	-	Window handle to the control
		
		\return Zero if processed
	
		\see 
	*/
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

	//==============================================================
	// EndDlg()
	//==============================================================
	/// Call this function to end the dialog box
	/**
		\param [in] ret		-	Return coded
	*/
	virtual void EndDlg( int ret );

	//==============================================================
	// OnIdle()
	//==============================================================
	/// Called when there are no windows messages
	/**		
		If this function returns zero.  It will not be called again
		until a Windows message is received in the queue.  Return
		non-zero and this function will be called again regardless
		after checking the message queue.

		\return	Return non-zero to receive more calls after checking
				the Windows message queue.
	
		\see 
	*/
	virtual BOOL OnIdle();

	//==============================================================
	// DestroyWindow()
	//==============================================================
	/// Called to destroy the window
	virtual void DestroyWindow();

	//==============================================================
	// OnKeyDown()
	//==============================================================
	/// Called in response to a WM_KEYDOWN message
	/**
		\param [in] nVirtKey	-	Virtual key code
		\param [in] dwKeyData	-	Contains the repeat count, scan code,
									extended-key flag, context code,
									previous key state flag, and 
									transition state flag.

		Value		Description 
		0–15		Specifies the repeat count for the current message. 
					The value is the number of times the keystroke is 
					auto-repeated as a result of the user holding down 
					the key. If the keystroke is held long enough, multiple 
					messages are sent. However, the repeat count is not 
					cumulative. 

		16–23		Specifies the scan code. The value depends on the 
					original equipment manufacturer (OEM). 

		24			Specifies whether the key is an extended key, such as 
					the right-hand alt and ctrl keys that appear on an enhanced 
					101- or 102-key keyboard. The value is 1 if it is an extended 
					key; otherwise, it is 0. 

		25–28		Reserved; do not use. 

		29			Specifies the context code. The value is always 0 for a 
					WM_KEYDOWN message. 

		30			Specifies the previous key state. The value is 1 if the key is 
					down before the message is sent, or it is 0 if the key is up. 

		31			Specifies the transition state. The value is always 0 for a 
					WM_KEYDOWN message. 

		
		\return Zero if message is processed
	
		\see 
	*/
	virtual BOOL OnKeyDown( int nVirtKey, DWORD dwKeyData );

	//==============================================================
	// OnEndSession()
	//==============================================================
	/// Called in response to a WM_ENDSESSION message
	/**
		\param [in] bEndSession		-	Non-zero if session is being ended.
		\param [in] fLogOff			-	Indicates whether user is logging
										off, or the system is shutting down.
		
		\return Zero if processed
	
		\see 
	*/
	virtual BOOL OnEndSession( BOOL bEndSession, DWORD fLogOff );

public:

	//==============================================================
	// ChooseColor()
	//==============================================================
	/// Launches the Windows color picker
	/**
		\param [in,out] pCol	-	Pointer to color variable
		\param [in] hOwner		-	Owner of the color dialog box
		\param [in] dwFlags		-	Color picker dialog box flags
		
		\return Non-zero if a new color is chosen
	
		\see 
	*/
	static BOOL ChooseColor( COLORREF *pCol, HWND hOwner = NULL, DWORD dwFlags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT );

	//==============================================================
	// DoMessagePump()
	//==============================================================
	/// Pumps the Windows message queue
	/**
		\param [in] hWnd	-	Window handle to pump message for.
								NULL for all windows.
		\param [in] hAccel	-	Accelerator table for message translation.
		
		\return Non-zero if WM_QUIT message was not processed.
	
		\see 
	*/
	static BOOL DoMessagePump( HWND hWnd = NULL, HACCEL hAccel = NULL );

	//==============================================================
	// CreateDlg()
	//==============================================================
	/// Creates a windows dialog box
	/**
		\param [in] hInst	-	The application instance handle
		\param [in] hParent	-	Parent window or NULL.
		
		\return Non-zero if dialog box is created.
	
		\see 
	*/
	BOOL CreateDlg( HINSTANCE hInst, HWND hParent = NULL );

	//==============================================================
	// DoModal()
	//==============================================================
	/// Runs a modal dialog box
	/**
		\param [in] hInst		-	Application module instance
		\param [in] hParent		-	Parent window or NULL
		\param [in] nCmdShow	-	Show mode
		
		\return Return code from dialog box
	
		\see 
	*/
	int DoModal( HINSTANCE hInst, HWND hParent = NULL, int nCmdShow = SW_SHOWNORMAL );

	//==============================================================
	// CenterWindow()
	//==============================================================
	/// Centers the specified window
	/**
		\param [in] hWnd		-	Window handle
		\param [in] hParent		-	Parent window for centering
		\param [in] pRect		-	Rectangle in which to center hWnd.
									This parameter is ignored if hParent
									is not NULL.
	*/
	static void CenterWindow( HWND hWnd, HWND hParent = NULL, LPRECT pRect = NULL );

	//==============================================================
	// AutoCenter()
	//==============================================================
	/// Specifies that the dialog box should auto center when created.
	/**
		\param [in] b	-	Non-zero to enable auto centering
	*/
	void AutoCenter( BOOL b ) { m_bAutoCenter = b; }

	//==============================================================
	// CenterOnParent()
	//==============================================================
	/// Specifies if the dialog box should center on the parent window when created.
	/**
		\param [in] b	-	Enable / disable parent centering.
	*/
	void CenterOnParent( BOOL b ) { m_bCenterOnParent = b; }

	//==============================================================
	// CenterWindow()
	//==============================================================
	/// Appropriately centers the dialog box.
	void CenterWindow()
	{	CenterWindow( GetSafeHwnd(), m_bCenterOnParent ? GetParent() : NULL, NULL ); }

	//==============================================================
	// DlgProc()
	//==============================================================
	/// The main dialog box callback procedure
	/**
		\param [in] hWnd		-	Handle to dialog box
		\param [in] uMessage	-	Windows message
		\param [in] wParam		-	WPARAM
		\param [in] lParam		-	LPARAM
		
		\return Zero if message was processed
	
		\see 
	*/
	static BOOL CALLBACK DlgProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam );

	/// Constructor
	CWinDlg();

	/// Destructor
	virtual ~CWinDlg();

	//==============================================================
	// GetInstance()
	//==============================================================
	/// Returns the application module instance
	HINSTANCE GetInstance() { return m_hInst; }

	//==============================================================
	// GetSafeHwnd()
	//==============================================================
	/// Returns the dialog box window handle
	HWND GetSafeHwnd() { return m_hWnd; }

	//==============================================================
	// GetParent()
	//==============================================================
	/// Returns the window handle of the parent window
	HWND GetParent() { return m_hwndParent; }

	//==============================================================
	// IsWindow()
	//==============================================================
	/// Returns non-zero if there is a valid window handle
	BOOL IsWindow() { return ( GetSafeHwnd() != NULL && ::IsWindow( GetSafeHwnd() ) ); }

	//==============================================================
	// GetDlgItem()
	//==============================================================
	/// Returns the window handle to the specified control
	/**
		\param [in] id	-	Control id
	*/
	HWND GetDlgItem( int id )
	{	return ::GetDlgItem( GetSafeHwnd(), id ); }

	//==============================================================
	// ShowWindow()
	//==============================================================
	/// Sets the dialog box show mode
	/**
		\return Non-zero if success
	*/
	BOOL ShowWindow( int nCmdShow = SW_SHOWNORMAL )
	{	return ::ShowWindow( m_hWnd, nCmdShow ); }

	//==============================================================
	// UpdateWindow()
	//==============================================================
	/// Sends a WM_PAINT message to the dialog box
	/**
		\return Non-zero if success
	*/
	BOOL UpdateWindow()
	{	return ::UpdateWindow( m_hWnd ); }

	//==============================================================
	// Invalidate()
	//==============================================================
	/// Invalidates the client area of the window forcing a redraw
	/**
		\return Non-zero if success
	*/
	BOOL Invalidate() 
	{	return ::InvalidateRect( m_hWnd, NULL, TRUE ); }

	//==============================================================
	// RedrawWindow()
	//==============================================================
	/// Updates the specified area of the dialog box window
	/**
		\param [in] pRect	-	Area to redraw, NULL for all.
		\param [in] hRgn	-	Region to redraw, NULL for all.
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
	{	return ::SetWindowPos(	hWnd, NULL,
								x, y, 0, 0,
								SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER );
	}

	//==============================================================
	// SetForegroundWindow()
	//==============================================================
	/// Sets this window as the foreground window
	/**			
		\return Non-zero if success
	*/
	BOOL SetForegroundWindow() { return ::SetForegroundWindow( GetSafeHwnd() ); }

	//==============================================================
	// SetFocus()
	//==============================================================
	/// Gives this window the input focus
	/**			
		\return Handle to previous window with the input focus
	*/
	HWND SetFocus() { return ::SetFocus( GetSafeHwnd() ); }
	//==============================================================
	// LoadAcceleratorTable()
	//==============================================================
	/// Loads the specified accelerator table
	/**
		\param [in] pAccelRes	-	Resource ID of the accelerator.
		
		\return Non-zero if success
	
		\see 
	*/

	BOOL LoadAcceleratorTable( LPCTSTR pAccelRes )
	{	m_hAccel = LoadAccelerators( GetInstance(), pAccelRes );
		return ( m_hAccel != NULL );
	}

	//==============================================================
	// IsAccel()
	//==============================================================
	/// Returns non-zero if there is a valid accelerator table
	BOOL IsAccel() { return ( m_hAccel != NULL ); }

	//==============================================================
	// GetAccel()
	//==============================================================
	/// Returns a handle to the current accelerator table
	HACCEL GetAccel() { return m_hAccel; }

	//==============================================================
	// SetInstance()
	//==============================================================
	/// Sets the application module instance
	/**
		\param [in] hInst	-	The application module instance
	*/
	void SetInstance( HINSTANCE hInst ) { m_hInst = hInst; }

	//==============================================================
	// SetDlgTemplate()
	//==============================================================
	/// Sets the resource ID of the dialog box template to use.
	/**
		\param [in] pRes	-	Dialog box resource template ID.
	*/
	void SetDlgTemplate( LPCTSTR pRes ) { m_pTemplate = pRes; }

	//==============================================================
	// GetDlgTemplate()
	//==============================================================
	/// Returns the current dialog box resource ID
	LPCTSTR GetDlgTemplate() { return m_pTemplate; }

	//==============================================================
	// IsRunning()
	//==============================================================
	/// Returns non-zero if a valid dialog box is currently being displayed
	BOOL IsRunning() { return m_bRun; }

private:	

	/// Non-zero to continue running dialog box
	BOOL				m_bRun;

	/// Dialog box return code
	int					m_nRet;
	
	/// Dialog box window handle
	HWND				m_hWnd;

	/// Handle to parent window
	HWND				m_hwndParent;

	/// Application module instance
	HINSTANCE			m_hInst;

	/// Dialog box resource handle
	LPCTSTR				m_pTemplate;

	/// Non-zero if dialog box has been initialized
	BOOL				m_bInitialized;

	/// Non-zero if the dialog box should center upon creation
	BOOL				m_bAutoCenter;

	/// Non-zero if the dialog box should center on parent window
	BOOL				m_bCenterOnParent;

	/// Static array of custom colors, used by the color picker
	static	COLORREF	m_rgbCustomColors[ 16 ];

	/// Non-zero if the dialog box is running modeless
	BOOL				m_bModeless;

	/// Handle to accelerator table
	HACCEL				m_hAccel;
};

#endif // !defined(AFX_WINDLG_H__97067A65_B217_11D2_88EE_00104B2C9CFA__INCLUDED_)
