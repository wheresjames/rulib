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
// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_)
#define AFX_MESSAGE_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Message Callback Function
typedef long ( *CMESSAGE_CALLBACK_FUNCTION ) ( LPVOID pData, WPARAM wParam, LPARAM lParam );

// Makes callbacks a little less painfull
#define CMESSAGE_SET_CALLBACK( ct, c, fn ) SetCallbackFunction( (CMESSAGE_CALLBACK_FUNCTION)(##ct::_##fn), (LPVOID)c );
#define CMESSAGE_DECLARE_CALLBACK( f )	static HRESULT _##f( LPVOID pData, WPARAM wParam, LPARAM lParam );
// Microsoft Bug
#define CMESSAGE_DEFINE_CALLBACK( c, f )												\
HRESULT c::_##f( LPVOID pData, WPARAM wParam, LPARAM lParam )	\
{																						\
	if ( pData == NULL ) return FALSE;													\
	c	*ptr = ( c * )pData;															\
	return ptr->f( wParam, lParam );													\
}

//==================================================================
// CMessage
//
/// Provides callback mechanisms
/**
	This class makes it easy to provide callbacks through either
	Windows messaging or callback functions.  The callback mechanism
	is transparent to the user.
*/
//==================================================================
class CMessage  
{

public:

	/// Default constructor
	CMessage();

	/// Destructor
	virtual ~CMessage();

//--------------------------------------------------------------------
// For Message Handeling
//--------------------------------------------------------------------

public:

	//==============================================================
	// SetMessageTarget()
	//==============================================================
	/// Set the Message ID and window handle that will receive callback messages
	/**
		\param [in] hWnd			-	The window that will receive messages
		\param [in] uWMMessageID	-	The MessageID that will be sent to the
										window when characters are ready.
		
		\return 
	
		\see 
	*/
	void SetMessageTarget( HWND hWnd, UINT uWMMessageID );

	//==============================================================
	// GetWMMessageHWND()
	//==============================================================
	/// Returns the message target window handle
	HWND GetWMMessageHWND() { return m_hWnd; }

	//==============================================================
	// GetWMMessageID()
	//==============================================================
	/// Returns the message target Message ID
	UINT GetWMMessageID() { return m_uWMMessageID; }

	//==============================================================
	// IsMessageTarget()
	//==============================================================
	/// Returns non-zero if there is a valid message target
	BOOL IsMessageTarget() 
	{	return ( ( m_hWnd != NULL ) && ( m_uWMMessageID != 0 ) ); }

	//==============================================================
	// ClearMessageTarget()
	//==============================================================
	/// Clears the Windows messaging target
	void ClearMessageTarget() { m_hWnd = NULL; m_uWMMessageID = 0; }

	//==============================================================
	// SendWMMessage()
	//==============================================================
	/// Sends a Windows message to the target
	/**
		\param [in] wParam	-	WPARAM passed on to target
		\param [in] lParam	-	LPARAM passed on to target
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SendWMMessage(WPARAM wParam, LPARAM lParam);

	//==============================================================
	// PostWMMessage()
	//==============================================================
	/// Posts a Windows message to the target
	/**
		\param [in] wParam	-	WPARAM passed on to target
		\param [in] lParam	-	LPARAM passed on to target
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL PostWMMessage(WPARAM wParam, LPARAM lParam);

private:

	/// Window handle that will receive callback messages
	HWND	m_hWnd;

	/// Callback message ID
	UINT	m_uWMMessageID;

///////////////////////////////////////////////
// For Handling Callback Targets
///////////////////////////////////////////////
public:

	//==============================================================
	// SetCallbackFunction()
	//==============================================================
	/// Sets the address of the function that will receive callback messages
	/**
		\param [in] pFunction	-	Callback function address
		\param [in] pData		-	User data, usually class pointer
		
		\return Non-zeo if success
	
		\see 
	*/
	virtual BOOL SetCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData );

	//==============================================================
	// IsCallbackFunction()
	//==============================================================
	/// Returns non-zero if there is a callback function
	BOOL IsCallbackFunction() { return ( m_pCallbackFunction != NULL ); }

	//==============================================================
	// ClearCallbackFunction()
	//==============================================================
	/// Clears the registered callback function address
	void ClearCallbackFunction() { m_pCallbackFunction = NULL; }

	//==============================================================
	// GetCallbackFunction()
	//==============================================================
	/// Returns a pointer to the registered callback function
	CMESSAGE_CALLBACK_FUNCTION GetCallbackFunction() { return m_pCallbackFunction; }

	//==============================================================
	// DoCallback()
	//==============================================================
	/// Performs the function callback
	/**
		\param [in] wParam	-	WPARAM passed on to target
		\param [in] lParam	-	LPARAM passed on to target
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DoCallback( WPARAM wParam, LPARAM lParam );

private:

	/// Callback message function
	CMESSAGE_CALLBACK_FUNCTION	( m_pCallbackFunction );

	/// Callback function data
	LPVOID						m_pData;

public:

	//==============================================================
	// MSendMessage()
	//==============================================================
	/// Send a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] bFunction 	-	Non-zero to use callback function
		\param [in] bWMessage	-	Non-zero to use Windows messaging
		
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL MSendMessage(	WPARAM wParam, LPARAM lParam,
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE );

	//==============================================================
	// MPostMessage()
	//==============================================================
	/// Post a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] bFunction 	-	Non-zero to use callback function
		\param [in] bWMessage	-	Non-zero to use Windows messaging
		
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL MPostMessage(	WPARAM wParam, LPARAM lParam,
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE );
						
};

#endif // !defined(AFX_MESSAGE_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_)



