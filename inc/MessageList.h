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
	CMessageList() { m_bClose = FALSE; }

	/// Destructor
	virtual ~CMessageList() { m_bClose = TRUE; ClearAllMessageTargets(); };

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
	GUID* CreateKey( GUID* pKey, DWORD dwUser1, DWORD dwUser2, const GUID *pguid )
	{	MD5_CTX ctx;
		CMd5Rsa::MD5Init( &ctx );
		CMd5Rsa::MD5Update( &ctx, (LPBYTE)&dwUser1, sizeof( dwUser1 ) );
		CMd5Rsa::MD5Update( &ctx, (LPBYTE)&dwUser2, sizeof( dwUser2 ) );
		if ( pguid && !IsEqualGUID( *pguid, CLSID_ZERO ) ) 
            CMd5Rsa::MD5Update( &ctx, (LPBYTE)pguid, sizeof( GUID ) );
		CMd5Rsa::MD5Final( (LPBYTE)pKey, &ctx );
		return pKey;
	}

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
	void SetMessageTarget( HWND hWnd, UINT uWMMessageID, BOOL bPost, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vMsgTargetQueue );
		if ( !ll.IsLocked() ) return;
		
		if ( hWnd != NULL && uWMMessageID && ::IsWindow( hWnd ) )			
		{	GUID guid; CreateKey( &guid, (DWORD)hWnd, (DWORD)uWMMessageID, pguid );
			m_vMsgTargetQueue.erase( guid );
			m_vMsgTargetQueue.push_back( guid, _PTR_NEW CMsgTarget( hWnd, uWMMessageID, bPost, 1, pguid ) );
		} // end if

	}

	//==============================================================
	// RemoveMessageTarget()
	//==============================================================
	/// Removes the specified Windows message target
	/**
		\param [in] hWnd			-	Window handle
		\param [in] uWMMessageID	-	Message ID
		\param [in] pguid			-	Pointer to a filter GUID
	*/
	void RemoveMessageTarget( HWND hWnd, UINT uWMMessageID, const GUID *pguid = NULL )
	{	
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vMsgTargetQueue );
		if ( !ll.IsLocked() ) return;

        if ( hWnd != NULL && uWMMessageID && ::IsWindow( hWnd ) )			
		{	GUID guid; CreateKey( &guid, (DWORD)hWnd, (DWORD)uWMMessageID, pguid );
			m_vMsgTargetQueue.erase( guid );
			m_vMsgTargetQueue.push_back( guid, _PTR_NEW CMsgTarget( hWnd, uWMMessageID, 0, 2, pguid ) );
		} // end if

//        GUID guid; m_vMsgTargetQueue.erase( *CreateKey( &guid, (DWORD)hWnd, (DWORD)uWMMessageID, pguid ) );
	}

	//==============================================================
	// MessageTargets()
	//==============================================================
	/// Returns the number of registered Windows message targets
	DWORD MessageTargets()
	{	
		if ( m_bClose ) return 0;
		return ( m_vMsgTarget.Size() + m_vMsgTargetQueue.Size() ); 
	}

	//==============================================================
	// CallbackFunctions()
	//==============================================================
	/// Returns the number of registered callback functions
	DWORD CallbackFunctions()
	{	
		if ( m_bClose ) return 0;
		return ( m_vCallbackTarget.Size() + m_vCallbackTargetQueue.Size() ); 
	}

	//==============================================================
	// ClearAllMessageTargets()
	//==============================================================
	/// Clears all the message targets
	void ClearAllMessageTargets() 
	{
		{	CTlLocalLock ll( m_vMsgTargetQueue );
			if ( ll.IsLocked() ) m_vMsgTargetQueue.clear(); 
		}
		{	CTlLocalLock ll( m_vMsgTarget );
			if ( ll.IsLocked() ) m_vMsgTarget.clear(); 
		}

		{	CTlLocalLock ll( m_vCallbackTargetQueue );
			if ( ll.IsLocked() ) m_vCallbackTargetQueue.clear(); 
		}
		{	CTlLocalLock ll( m_vCallbackTarget );
			if ( ll.IsLocked() ) m_vCallbackTarget.clear(); 
		}
	}

	void UpdateMessageQueue()
	{
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vMsgTargetQueue );
		if ( !ll.IsLocked() ) return;

        THList< GUID, CMsgTarget >::iterator it = NULL;
		while ( ( it = m_vMsgTargetQueue.next( it ) ) != m_vMsgTargetQueue.end() )
        {
            // Remove any existing
            m_vMsgTarget.erase( *it->key() );

            // Add
            if ( 1 == (*it)->uAction )
                m_vMsgTarget.push_back( *it->key(), _PTR_NEW CMsgTarget( (*it)->hWnd, (*it)->uMsg, (*it)->bPost, 0, &(*it)->guid ) );

            it = m_vMsgTargetQueue.erase( it );

        } // end while
    }

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
	BOOL SendWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL)
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vMsgTarget );
		if ( !ll.IsLocked() ) return FALSE;

        UpdateMessageQueue();

		THList< GUID, CMsgTarget >::iterator it = NULL;
		while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
		{	if ( ::IsWindow( (*it)->hWnd ) ) 
			{	if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
					::SendMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
			} // end if
			else it = m_vMsgTarget.erase( it );
		} // end while
		return TRUE;
	}

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
	BOOL PostWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL)
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vMsgTarget );
		if ( !ll.IsLocked() ) return FALSE;

        UpdateMessageQueue();

		THList< GUID, CMsgTarget >::iterator it = NULL;
		while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
		{	if ( ::IsWindow( (*it)->hWnd ) ) 
			{	if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
					::PostMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
			} // end if
			else it = m_vMsgTarget.erase( it );
		} // end while
		return TRUE;
	}

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
	BOOL MWMMessage(WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL)
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vMsgTarget );
		if ( !ll.IsLocked() ) return FALSE;

        UpdateMessageQueue();

		THList< GUID, CMsgTarget >::iterator it =NULL;
		while ( ( it = m_vMsgTarget.next( it ) ) != m_vMsgTarget.end() )
		{	if ( ::IsWindow( (*it)->hWnd ) ) 
			{	if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
				{	if ( (*it)->bPost ) ::PostMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
					else ::SendMessage( (*it)->hWnd, (*it)->uMsg, wParam, lParam );
				} // end if
			} // end if
			else it = m_vMsgTarget.erase( it );
		} // end while
		return TRUE;
	}

private:

    /// List of Windows messaging targets
	THList< GUID, CMsgTarget >			m_vMsgTarget;

	/// List of Windows messaging targets
	THList< GUID, CMsgTarget >			m_vMsgTargetQueue;

///////////////////////////////////////////////
// For Handling Callback Targets
///////////////////////////////////////////////
public:

	void UpdateCallbackQueue()
	{
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( !ll.IsLocked() ) return;

		THList< GUID, CCallbackTarget >::iterator it = NULL;
		while ( ( it = m_vCallbackTargetQueue.next( it ) ) != m_vCallbackTargetQueue.end() )
        {
            // Remove any existing
            m_vCallbackTarget.erase( *it->key() );

            // Add
            if ( 1 == (*it)->uAction )
			    m_vCallbackTarget.push_back( *it->key(), _PTR_NEW CCallbackTarget( (*it)->pCallbackFunction, (*it)->pCallbackFunction2, (*it)->pData, 1, &(*it)->guid ) );

            it = m_vCallbackTargetQueue.erase( it );

        } // end while
    }

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
	virtual BOOL SetCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData = NULL, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( !ll.IsLocked() ) return FALSE;
		
		if ( pFunction != NULL )			
		{	GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
			m_vCallbackTargetQueue.erase( guid );
			m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( pFunction, NULL, pData, 1, pguid ) ); 
		} // end if
		return TRUE; 	
	}

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
	virtual BOOL SetCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData = NULL, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( !ll.IsLocked() ) return FALSE;
		
		if ( pFunction != NULL )			
		{	GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
			m_vCallbackTargetQueue.erase( guid );
			m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( NULL, pFunction, pData, 1, pguid ) ); 
		} // end if
		return TRUE; 	

	}

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
	BOOL DoCallback( WPARAM wParam, LPARAM lParam, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return FALSE;
		CTlLocalLock ll( m_vCallbackTarget );
		if ( !ll.IsLocked() ) return FALSE;

        UpdateCallbackQueue();

		THList< GUID, CCallbackTarget >::iterator it = NULL;
		while ( ( it = m_vCallbackTarget.next( it ) ) != m_vCallbackTarget.end() )
		{	RULIB_TRY
			{	if ( (*it)->pCallbackFunction != NULL || (*it)->pCallbackFunction2 != NULL ) 
				{	if ( pguid == NULL || IsEqualGUID( (*it)->guid, CLSID_ZERO ) || IsEqualGUID( (*it)->guid, *pguid ) )
					{	if ( (*it)->pCallbackFunction != NULL )
							(*it)->pCallbackFunction( (*it)->pData, wParam, lParam );
						else if ( (*it)->pCallbackFunction2 != NULL )
							(*it)->pCallbackFunction2( (*it)->pData, wParam, lParam, pguid );
					} // end if
				} // end if
				else it = m_vCallbackTarget.erase( it );
			} RULIB_CATCH_ALL 
			{ RULIB_TRY { it = m_vCallbackTarget.erase( it ); } RULIB_CATCH_ALL { return TRUE; } }
		} // end while
		return TRUE;
	}

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
	void RemoveCallbackFunction( CMESSAGE_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( !ll.IsLocked() ) return;
		
		if ( pFunction != NULL )			
		{	GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
			m_vCallbackTargetQueue.erase( guid );
			m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( pFunction, NULL, pData, 2, pguid ) ); 
		} // end if
        
//      GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
//		m_vCallbackTarget.erase( guid ); 
    }

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
	void RemoveCallbackFunction( CMESSAGELIST_CALLBACK_FUNCTION pFunction, LPVOID pData, const GUID *pguid = NULL )
	{
		if ( m_bClose ) return;
		CTlLocalLock ll( m_vCallbackTargetQueue );
		if ( !ll.IsLocked() ) return;
		
		if ( pFunction != NULL )			
		{	GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
			m_vCallbackTargetQueue.erase( guid );
			m_vCallbackTargetQueue.push_back( guid, _PTR_NEW CCallbackTarget( NULL, pFunction, pData, 2, pguid ) ); 
		} // end if

//        GUID guid; CreateKey( &guid, (DWORD)pFunction, (DWORD)pData, pguid );
//		m_vCallbackTarget.erase( guid ); 
    }

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
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE )
	{
		if ( m_bClose ) return FALSE;
		BOOL bRet = FALSE;
		if ( bFunction ) bRet |= DoCallback( wParam, lParam, pguid );
		if ( MessageTargets() ) bRet |= MWMMessage( wParam, lParam, pguid );
		return bRet;
	}

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
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE )
	{
		if ( m_bClose ) return FALSE;
		BOOL bRet = FALSE;
		if ( bFunction ) bRet |= DoCallback( wParam, lParam, pguid );
		if ( MessageTargets() ) bRet |= SendWMMessage( wParam, lParam, pguid );
		return bRet;
	}

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
						BOOL bFunction = TRUE, BOOL bWMessage = TRUE )
	{
		if ( m_bClose ) return FALSE;
		BOOL bRet = FALSE;
		if ( bFunction ) bRet |= DoCallback( wParam, lParam, pguid );
		if ( MessageTargets() ) bRet |= PostWMMessage( wParam, lParam, pguid );
		return bRet;
	}
};

class CThreadTimer : public CThread, public CMessageList
{        
public:

	/// Constructor
	CThreadTimer() { m_dwDelay = 1000; }

	/// Destructor
	~CThreadTimer() {}

    /// Thread initialization
    virtual BOOL DoThread( LPVOID pData )
	{
		DWORD dwWait = m_dwDelay;
		m_dwPos = GetTickCount() + m_dwDelay;
		
		while ( 0 != ::WaitForSingleObject( GetStopEvent(), dwWait ) )
		{
			// Do the callback
			DoCallback( (WPARAM)dwWait, (LPARAM)this );

			// Where we want to be
			m_dwPos += m_dwDelay;
						
			// Where we are
			DWORD dwTick = GetTickCount();
			if ( dwTick < m_dwPos )
				dwWait = m_dwPos - dwTick;
			else
				dwWait = 0;

		} // end while

		return FALSE;
	}

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



