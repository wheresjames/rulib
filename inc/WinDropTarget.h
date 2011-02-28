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
// WinDropTarget.h: interface for the CWinDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDROPTARGET_H__229E1D52_132B_4915_BBAF_355BF38921C8__INCLUDED_)
#define AFX_WINDROPTARGET_H__229E1D52_132B_4915_BBAF_355BF38921C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// Drop target data
/**
	\see CWinDropTarget
*/
typedef struct tagWINDROPTARGETDATA
{
	/// Data object interface pointer
	IDataObject			*pdto;

	/// Key state at start of drop
	DWORD				grfKeyState;

	/// Point of start of drop
	POINTL				ptl;

	/// Drop effect
	DWORD				dwEffect;

} WINDROPTARGETDATA, *LPWINDROPTARGETDATA;

//==================================================================
// CWinDropTarget
//
/// Implements the IDropTarget interface
/**
	Use this class to add drag-and-drop support to any generic window.	
	Drop information is relayed to the specified window via a standard
	windows message.
*/
//==================================================================
class CWinDropTarget : public IDropTarget  
{
public:
	
	//==============================================================
	// GetData()
	//==============================================================
	/// Retrieves data from the IDataObject
	/**
		\param [in] pDo					-	Pointer to IDataObject
		\param [in] uClipboardFormat	-	Encoding format
		\param [in] buf					-	Receives the data
		
		\return Non-zero if data is retrieved
	
		\see 
	*/
	static BOOL GetData( IDataObject *pDo, UINT uClipboardFormat, TMem< BYTE > &buf );

	//==============================================================
	// Revoke()
	//==============================================================
	/// Cancels drag and drop support for encapsulated window
	/**
		\return Non-zero if success
	
		\see 
	*/
	BOOL Revoke();

	//==============================================================
	// Register()
	//==============================================================
	/// Registers the specified window for drag and drop support
	/**
		\param [in] hWnd	-	Window requiring drag and drop support
		\param [in] uMsg	-	Drag and drop message callback
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Register( HWND hWnd, UINT uMsg );

	//==============================================================
	// Drop()
	//==============================================================
	/// Implements the drop message
	/**
		\param [in] pdto		-	Pointer to IDataObject
		\param [in] grfKeyState	-	Key state at drop
		\param [in] ptl			-	Drop point
		\param [out] pdwEffect	-	Receives drop effect
		
		\return Zero if success
	
		\see 
	*/
	HRESULT STDMETHODCALLTYPE Drop( IDataObject *pdto, DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect );

	//==============================================================
	// DragOver()
	//==============================================================
	/// Handles the drag over message
	/**
		\param [in] grfKeyState -	Key state
		\param [in] ptl			-	Drag point
		\param [out] pdwEffect	-	Receives drag effect
		
		\return 
	
		\see 
	*/
	HRESULT STDMETHODCALLTYPE DragOver( DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect );

	//==============================================================
	// DragLeave()
	//==============================================================
	/// Called when drag leaves a window
	HRESULT STDMETHODCALLTYPE DragLeave();

	//==============================================================
	// DragEnter()
	//==============================================================
	/// Called when a drag enters a window
	/**
		\param [in] pdto			-	Pointer to drop information
		\param [in] grfKeyState		-	Key state
		\param [in] ptl				-	Drag point
		\param [out] pdwEffect		-	Receives drag effect
		
		\return 
	
		\see 
	*/
	HRESULT STDMETHODCALLTYPE DragEnter( IDataObject *pdto, DWORD grfKeyState, POINTL ptl, DWORD *pdwEffect );

	/// Constructor
	CWinDropTarget();

	/// Destructor
	virtual ~CWinDropTarget();

	// *** Com stuff

	//==============================================================
	// QueryInterface()
	//==============================================================
	/// Query interface implementation
	/**		
		\param [in] iid				-	Interface ID
		\param [out] ppvObject		-	Receives interface pointer.
		
		\return Zero if success
	
		\see 
	*/
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject)
	{	if ( ppvObject == NULL ) return E_POINTER;
		if ( iid == IID_IUnknown || iid == IID_IDropTarget ) 
		{	*ppvObject = (void*)this; return NOERROR; }
		return E_NOINTERFACE;
	}

	//==============================================================
	// AddRef()
	//==============================================================
	/// Adds object reference
	ULONG STDMETHODCALLTYPE AddRef() { return m_dwRefCount++; }

	//==============================================================
	// Release()
	//==============================================================
	/// Removes object reference
	ULONG STDMETHODCALLTYPE Release() 
	{	if ( m_dwRefCount ) m_dwRefCount--; return m_dwRefCount; }

	//==============================================================
	// PlainTextMode()
	//==============================================================
	/// Sets plain text mode
	/**
		\param [in] b	-	Desired plain text mode
		
		If non-zero, specifies that all drop messages should be 
		converted to plain text.  Otherwise, messages are passed on
		as is.
	*/
	void PlainTextMode( BOOL b ) { m_bPlainTextMode = b; }

private:

	/// Non-zero if plain text mode is enabled
	BOOL						m_bPlainTextMode;

	/// Current object reference count
	DWORD						m_dwRefCount;

	/// Handle to window receiving drop messages
	HWND						m_hWnd;

	/// Callback message id for notifying m_hWnd of drops
	UINT						m_uMsg;

	/// Pointer to drop data
    CComPtr<IDataObject>		m_spdtoDragging;

	/// Pointer to string converted data
	char						m_szData[ 4096 ];

	/// Url format information
	UINT						m_fmtUrl;

};

#endif // !defined(AFX_WINDROPTARGET_H__229E1D52_132B_4915_BBAF_355BF38921C8__INCLUDED_)
