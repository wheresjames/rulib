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

#if !defined(AFX_MESSAGELIST_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_)
#define AFX_MESSAGELIST_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <objbase.h>

// ct = Class type, c = class instance, fn = function name, g = guid
#define CMESSAGE_SET_CALLBACK_GUID( ct, c, fn, g ) SetCallbackFunction( (CMESSAGE_CALLBACK_FUNCTION)(##ct::_##fn), (LPVOID)c, g );

// Message Callback Function
typedef long ( *CMESSAGELIST_CALLBACK_FUNCTION ) ( LPVOID pData, WPARAM wParam, LPARAM lParam, const GUID *pGuid );

#define CMESSAGELIST_ENABLE_LOCKING

//==================================================================
// CMessageList
//
/// Provides callback mechanisms
/**
	This class makes it easy to provide callbacks through either
	Windows messaging or callback functions.  The callback mechanism
	is transparent to the user.

	This class supports an unlimited number of Windows messageing or
	function callback hooks.  This class also allows the use of 
	GUIDs as callback filters.
*/
//==================================================================
class CMessageList  
{
public:

	/// Contains information about a single Windows messaging callback target
	/**
		See CMessageList
	*/
	struct CMsgTarget
	{
		/// Default constructor
		CMsgTarget() { uAction = 0; hWnd = NULL; uMsg = 0; ZeroMemory( &guid, sizeof( guid ) ); }

		//==============================================================
		// CMsgTarget()
		//==============================================================
		/// 
		/**
			\param [in] h		-	Handle to window
			\param [in] u		-	Callback message id
			\param [in] p		-	Non-zero to post messages
            \param [in] a       -   Action to take
			\param [in] pguid	-	Filter GUID
		*/
		CMsgTarget( HWND h, UINT u, BOOL p, UINT a, const GUID *pguid = NULL ) 
		{	hWnd = h; uMsg = u; bPost = p; uAction = a;
			if ( pguid != NULL ) memcpy( &guid, pguid, sizeof( guid ) ); 
			else ZeroMemory( &guid, sizeof( guid ) );
		}

		/// Window handle
		HWND	hWnd;

		/// Callback message ID
		UINT	uMsg;

		/// Non-zero to post messages
		BOOL	bPost;

		/// Filter GUID
		GUID	guid;

        /// Action to take?
        UINT    uAction;
	};

private:

	/// Non-zero when class is closing
	BOOL				m_bClose;

public:

	/// Default constructor
	CMessageList();

	/// Destructor
	virtual ~CMessageList();

	//==============================================================
	// CreateKey()
	//==============================================================
	/// Creates a unique key using the input parameters
	/**
		\param [out] pKey		-	Receives key
		\param [in] dwUser1		-	Param 1
		\param [in] dwUser2		-	Param 2
		\param [in] pguid		-	GUID param
		
		\return Pointer to the buffer in pKey
	
		\see 
	*/
	GUID* CreateKey( GUID* pKey, void* dwUser1, void* dwUser2, const GUID *pguid );

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
		\param [in] uWMMessageID	-	The Message ID that will be sent to the
										window when characters are ready.
		\param [in] bPost			-	Non-zero to post message to window zero
										to send message.
		\param [in] pguid			-	Pointer to a filter GUID
	*/
	void SetMessageTarget( HWND hWnd, UINT uWMMessageID, BOOL bPost, const GUID *pguid = NULL );

	//==============================================================
	// RemoveMessageTarget()
	//==============================================================
	/// Removes the specified Windows message target
	/**
		\param [in] hWnd			-	Window handle
		\param [in] uWMMessageID	-	Message ID
		\param [in] pguid			-	Pointer to a filter GUID
	*/
	void RemoveMessageTarget( HWND hWnd, UINT uWMMessageID, const GUID *pguid = NULL );

	//==============================================================
	// MessageTargets()
	//==============================================================
	/// Returns the number of registered Windows message targets
	DWORD MessageTargets();
	
	//==============================================================
	// CallbackFunctions()
	//==============================================================
	/// Returns the number of registered callback functions
	DWORD CallbackFunctions();

	//==============================================================
	// ClearAllMessageTargets()
	//==============================================================
	/// Clears all the message targets
	void ClearAllMessageTargets();

	void UpdateMessageQueue();

	//==============================================================
	// SendWMMessage()
	//==============================================================
	/// Send a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID

		Message is only sent to registered targets with matching
		filter GUIDs.
	  
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL SendWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL);

	//==============================================================
	// PostWMMessage()
	//==============================================================
	/// Posts a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID

		Message is only sent to registered targets with matching
		filter GUIDs.
	  
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL PostWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL);

	//==============================================================
	// PostWMMessage()
	//==============================================================
	/// Sends or posts a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID

		Message is only sent to registered targets with matching
		filter GUIDs.
	  
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL MWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL);

private:

    /// List of Windows messaging targets
	THList< GUID, CMsgTarget >			m_vMsgTarget;

	/// List of Windows messaging targets
	THList< GUID, CMsgTarget >			m_vMsgTargetQueue;

///////////////////////////////////////////////
// For Handling Callback Targets
///////////////////////////////////////////////
public:

	void UpdateCallbackQueue();

	//==============================================================
	// SetCallbackFunction()
	//==============================================================
	/// Sets the address of the function that will receive callback messages
	/**
		\param [in] pFunction	-	Callback function address
		\param [in] pData		-	User data, usually class pointer
		\param [in] pguid		-	Filter GUID
		
		\return Non-zeo if success
	
		\see 
	*/
	virtual BOOL SetCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData = NULL, const GUID *pguid = NULL );

	//==============================================================
	// SetCallbackFunction()
	//==============================================================
	/// Sets the address of the function that will receive callback messages
	/**
		\param [in] pFunction	-	Callback function address
		\param [in] pData		-	User data, usually class pointer
		\param [in] pguid		-	Filter GUID
		
		\return Non-zeo if success
	
		\see 
	*/
	virtual BOOL SetCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData = NULL, const GUID *pguid = NULL );

	//==============================================================
	// DoCallback()
	//==============================================================
	/// Performs the function callback
	/**
		\param [in] wParam	-	WPARAM passed on to target
		\param [in] lParam	-	LPARAM passed on to target
		\param [in] pguid	-	Filter GUID

		Callbacks are only made to targets with matching filter GUIDs.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DoCallback( WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL );

	//==============================================================
	// RemoveCallbackFunction()
	//==============================================================
	/// Removes the specified callback target
	/**
		\param [in] pFunction	-	Callback function address
		\param [in] pData		-	User data, usually class pointer
		\param [in] pguid		-	Filter GUID
		
		\return Non-zeo if success
	
		\see 
	*/
	void RemoveCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid = NULL );

	//==============================================================
	// RemoveCallbackFunction()
	//==============================================================
	/// Removes the specified callback target
	/**
		\param [in] pFunction	-	Callback function address
		\param [in] pData		-	User data, usually class pointer
		\param [in] pguid		-	Filter GUID
		
		\return Non-zeo if success
	
		\see 
	*/
	void RemoveCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid = NULL );

private:

	/// Structure contains information about a function callback target
	struct CCallbackTarget
	{
		/// Default contstructor
		CCallbackTarget() { pCallbackFunction = NULL; pData = 0; uAction = 0; }


		//==============================================================
		// CCallbackTarget()
		//==============================================================
		/// Constructor
		/**
			\param [in] f		-	Function pointer
			\param [in] f2		-	Function pointer
			\param [in] d		-	User data, usually class pointer
			\param [in] pguid	-	Filter GUID
		*/
		CCallbackTarget( CMESSAGE_CALLBACK_FUNCTION f, CMESSAGELIST_CALLBACK_FUNCTION f2, LPVOID d, UINT a, const GUID *pguid = NULL ) 
		{	pCallbackFunction = f; pCallbackFunction2 = f2; pData = d; uAction = a;
			if ( pguid ) memcpy( &guid, pguid, sizeof( guid ) ); 
			else ZeroMemory( &guid, sizeof( guid ) );
		}

		/// Callback function pointer
		CMESSAGE_CALLBACK_FUNCTION		( pCallbackFunction );

		/// Callback function pointer
		CMESSAGELIST_CALLBACK_FUNCTION	( pCallbackFunction2 );

		/// User data, usually a class pointer
		LPVOID							pData;

		/// Filter GUID
		GUID							guid;

        /// Action
        UINT                            uAction;

	};

	/// List of callback function targets
	THList< GUID, CCallbackTarget >		m_vCallbackTarget;

	/// List of Windows messaging targets
	THList< GUID, CCallbackTarget >     m_vCallbackTargetQueue;

public:

	//==============================================================
	// MMessage()
	//==============================================================
	/// Sends or posts a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID
		\param [in] bFunction 	-	Non-zero to use callback function
		\param [in] bWMessage	-	Non-zero to use Windows messaging
		
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL MMessage(	WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL,
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE );

	//==============================================================
	// _MSendMessage()
	//==============================================================
	/// Sends a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID
		\param [in] bFunction 	-	Non-zero to use callback function
		\param [in] bWMessage	-	Non-zero to use Windows messaging
		
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL _MSendMessage(	WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL,
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE );

	//==============================================================
	// _MPostMessage()
	//==============================================================
	/// Posts a Windows message and/or do the callback
	/**
		\param [in] wParam		-	WPARAM passed on to target
		\param [in] lParam		-	LPARAM passed on to target
		\param [in] pguid		-	Filter GUID
		\param [in] bFunction 	-	Non-zero to use callback function
		\param [in] bWMessage	-	Non-zero to use Windows messaging
		
		\return Non-zero if a callback is made
	
		\see 
	*/
	BOOL _MPostMessage(	WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL,
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE );

};

class CThreadTimer : public CThread, public CMessageList
{        
public:

	/// Constructor
	CThreadTimer() { m_dwDelay = 1000; }

	/// Destructor
	~CThreadTimer() {}

    /// Thread initialization
    virtual BOOL DoThread( LPVOID pData );

	/// Start the timer
	BOOL StartTimer( DWORD dwDelay )
	{	m_dwDelay = dwDelay; 
		return StartThread(); 
	}   

	/// Stop the timer
	BOOL StopTimer()
	{	return StopThread( TRUE ); }

private:

	/// Timer delay
	DWORD	m_dwDelay;

	/// Track timer pos
	DWORD	m_dwPos;

};


#endif // !defined(AFX_MESSAGELIST_H__90009DC9_A2DD_11D1_A427_00104B2C9CFA__INCLUDED_)



