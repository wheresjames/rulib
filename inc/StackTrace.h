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
// StackTrace.h: interface for the CStackTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKTRACE_H__444879CF_2267_458B_9685_5237B375EC2C__INCLUDED_)
#define AFX_STACKTRACE_H__444879CF_2267_458B_9685_5237B375EC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Normal stack tracing
#if !defined( _STT ) 
#	if defined( ENABLE_STACK_TRACING ) && ( defined( __INTEL_COMPILER ) || ( _MSC_VER > 1200 ) )
#		define _STT() CLocalStackTrace _l_lst( __FUNCTION__ );
#	else
#		define _STT()
#	endif
#endif

// More extensive stack tracing
#if !defined( _STTEX ) 
#	if defined( ENABLE_STACK_TRACING_EX ) && ( defined( __INTEL_COMPILER ) || ( _MSC_VER > 1200 ) )
#		define _STTEX() CLocalStackTrace _l_lst( __FUNCTION__ );
#	else
#		define _STTEX()
#	endif
#endif

/// Call before memory dump else you will 'think' you have memory leaks.
#define _STT_RELEASE() CStackTrace::Release()

// wjr 1/19/07...
#if defined( ENABLE_STACK_HISTORY ) && defined( ENABLE_STACK_HISTORY_TIME ) && ( defined( __INTEL_COMPILER ) || ( _MSC_VER > 1200 ) )
#	define _STT_TIME(dw) CStackTrace::SetHistoryTime( dw )
#else
#	define _STT_TIME(dw)
#endif
// ...wjr 1/19/07


//#define ENABLE_STACK_HISTORY
//#define ENABLE_STACK_PROFILE
//#define ENABLE_STACK_HISTORY_TIME
//#define ENABLE_STACK_HISTORY_TIME_AUTO

//==================================================================
// CStackTrace
//
/// Contains stack traces for multiple threads
/**
	This class handles recording stack traceing.  Thread safe.	
*/
//==================================================================
class CStackTrace  
{
public:

	enum { eMaxStack = 128 };
	
	/// Encapsulates stack tracing functionality for a single thread
	class CStack
	{
	public:

#ifdef ENABLE_STACK_PROFILE

		/// Stack profile item
		struct SProfileItem
		{
			SProfileItem() { llTotalTime = 0; }

			/// Timer is used to resolve the amount of time spent in a function
			CHqTimer		hqTimer;

			/// Accumulates the total amount of time spent on a function
			LONGLONG		llTotalTime;
		};

#endif

	public:

		/// Constructor
		CStack() 
		{
			m_uStackPtr = 0; 
			m_memStack.allocate( eMaxStack ); 
			m_memStack.Zero();
			m_pStack = m_memStack.ptr();

#ifdef ENABLE_STACK_HISTORY

			m_uMaxHistory = 0;
			m_uStackHistoryPtr = 0;

			m_memStackHistory.allocate( eMaxStack ); 
			m_memStackHistory.Zero();
			m_pStackHistory = m_memStackHistory.ptr();

		// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY_TIME

			llGlobalTime = GetTickCount();
			m_memStackHistoryTime.allocate( eMaxStack ); 
			m_memStackHistoryTime.Zero();
			m_pStackHistoryTime = m_memStackHistoryTime.ptr();
#endif
		// ...wjr 1/19/07

#endif

#ifdef ENABLE_STACK_PROFILE

			m_memCallTime.allocate( eMaxStack ); 
			m_memCallTime.Zero();
			m_pCallTime = m_memCallTime.ptr();

#endif
		}

		//==============================================================
		// Push()
		//==============================================================
		/// Push pointer onto the stack
		/**
			\param [in] pFunction	-	Name of function to push on the stack
			
			\return Pointer to profile item if profiling is enabled, otherwise void
		
			\see Pop()
		*/
#ifdef ENABLE_STACK_PROFILE
		SProfileItem* Push( LPCTSTR pFunction )
#else
		void Push( LPCTSTR pFunction )
#endif
		{
			// Normal stack
			if ( eMaxStack > m_uStackPtr )
				m_pStack[ m_uStackPtr ] = pFunction, m_uStackPtr++;

#ifdef ENABLE_STACK_PROFILE

			// This takes a little time...
			LONGLONG llTime = CHqTimer::GetTimer();

			// Save call time
			m_pCallTime[ m_uStackHistoryPtr ] = llTime;

#endif

#ifdef ENABLE_STACK_HISTORY

			// Save stack history
			m_pStackHistory[ m_uStackHistoryPtr ] = pFunction;

		// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY_TIME
#ifdef ENABLE_STACK_HISTORY_TIME_AUTO

			// Save stack history
			m_pStackHistoryTime[ m_uStackHistoryPtr ] = GetTickCount();
#else
			// Save stack history
			m_pStackHistoryTime[ m_uStackHistoryPtr ] = llGlobalTime;

#endif
#endif
		// ...wjr 1/19/07

			// Increment stack pointer
			m_uStackHistoryPtr++;

			// Track total items in history
			if ( m_uMaxHistory < m_uStackHistoryPtr ) m_uMaxHistory = m_uStackHistoryPtr;

			// Update the history pointer
			if ( m_uStackHistoryPtr >= eMaxStack ) m_uStackHistoryPtr = 0;

#endif

#ifdef ENABLE_STACK_PROFILE

			CTlLocalLock ll( *CStackTrace::St() );
			if ( !ll.IsLocked() ) return NULL;

			// Get existing profile object
			SProfileItem *pPi = m_lstProfile[ pFunction ];

			// Create a new one if needed
			if ( pPi == NULL ) pPi = m_lstProfile.push_back( pFunction );

			// Start the timer
			if ( pPi ) pPi->hqTimer.Start( llTime );

			return pPi;
#endif
		}

		//==============================================================
		// Pop()
		//==============================================================
		/// Pop a pointer off the stack
		void Pop() 
		{
			if ( m_uStackPtr ) m_uStackPtr--; 
		}

		/// Returns the current stack pointer
		UINT GetStackPtr() { return m_uStackPtr; }

		/// Retuns the actual stack
		LPCTSTR* GetStack() { return m_memStack; }

#ifdef ENABLE_STACK_HISTORY

		/// Returns the number of items in the history
		UINT GetMaxHistory() { return m_uMaxHistory; }

		/// Returns the history pointer
		UINT GetHistoryPtr() { return m_uStackHistoryPtr; }

		/// Returns the history stack
		LPCTSTR* GetHistory() { return m_memStackHistory; }

		// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY_TIME

		/// Returns the history stack time pointer
		LONGLONG* GetHistoryTime() { return m_memStackHistoryTime; }

#endif
		// ...wjr 1/19/07

#endif

#ifdef ENABLE_STACK_PROFILE

		/// Returns a pointer to the profile list
		TStrList< SProfileItem >* GetProfile() { return &m_lstProfile; }

#endif

	private:

		/// pointer to next empty slot on the stack
		UINT						m_uStackPtr;

		/// Stack memory
		TMem< LPCTSTR >				m_memStack;

		/// Stack memory pointer
		LPCTSTR						*m_pStack;

#ifdef ENABLE_STACK_HISTORY

		/// Holds the number of items in the history
		UINT						m_uMaxHistory;

		/// Pointer to next empty slot in stack history
		UINT						m_uStackHistoryPtr;

		/// Stack history
		TMem< LPCTSTR >				m_memStackHistory;

		/// Stack history pointer
		LPCTSTR						*m_pStackHistory;

		// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY_TIME

		/// Stack history call times
		TMem< LONGLONG >			m_memStackHistoryTime;

		/// Call time pointer
		LONGLONG					*m_pStackHistoryTime;

#endif
		// ...wjr 1/19/07

#endif

#ifdef ENABLE_STACK_PROFILE

		/// Stack history call times
		TMem< LONGLONG >			m_memCallTime;

		/// Call time pointer
		LONGLONG					*m_pCallTime;

		/// Stack profile
		TStrList< SProfileItem >	m_lstProfile;		

#endif

	};

	/// List type
	typedef THList< DWORD, CStack > list;

	/// Iterator type
	typedef THList< DWORD, CStack >::iterator iterator;

public:

	//==============================================================
	// GetStack()
	//==============================================================
	/// Returns stack object for current thread
	/**
		\return Pointer to the stack object for the calling thread
	
		\see 
	*/
	CStack* GetStack()
	{	DWORD dwThreadId = ::GetCurrentThreadId();
		CStack *pStack = m_lstStack[ dwThreadId ];
		if ( pStack != NULL ) return pStack;
		return m_lstStack.push_back( dwThreadId );
	}

	//==============================================================
	// InitPush()
	//==============================================================
	/// Initializes stack for calling thread if needed and returns pointer
	/**
		\return Pointer to the stack object for the calling thread
	
		\see 
	*/
	CStack* InitPush() 
	{

#if defined( CStackTrace_USING_TLSAPI )

		// Return thread local storage index
		CStack *pStack;
		if ( MAXDWORD != m_tls_dwIndex )
		{	pStack = (CStack*)TlsGetValue( m_tls_dwIndex );
			if ( pStack != NULL ) return pStack;
		} // end if

		// Initialize for this thread
		CTlLocalLock ll( m_lock );
		if ( !ll.IsLocked() ) return NULL;

		// Allocate TLS 
		if ( MAXDWORD == m_tls_dwIndex )
			m_tls_dwIndex = TlsAlloc();
		
		// Set stack value
		pStack = GetStack();
		if ( MAXDWORD != m_tls_dwIndex ) 
			TlsSetValue( m_tls_dwIndex, pStack );
			
		return pStack;

#elif defined( CStackTrace_USING_TLS )

		// Are we already initialized for this thread?
		if ( m_tls_pStack ) return m_tls_pStack;

		// Initialize for this thread
		CTlLocalLock ll( m_lock );
		if ( !ll.IsLocked() ) return NULL;
		return ( m_tls_pStack = GetStack() );

#else

		// Lock first
		CTlLocalLock ll( m_lock );
		if ( !ll.IsLocked() ) return NULL;
		return GetStack(); 

#endif	

	}

	//==============================================================
	// GetList()
	//==============================================================
	/// Returns a pointer to the stack list
	list* GetList() { return &m_lstStack; }

	/// Constructor
	CStackTrace();

	/// Destructor
	virtual ~CStackTrace();

private:	

	/// Current stack
	THList< DWORD, CStack >		m_lstStack;

	/// Lock
	CTlLock						m_lock;

public:

	//==============================================================
	// CTlLock*()
	//==============================================================
	/// Return the lock object
	operator CTlLock*() { return &m_lock; }

	//==============================================================
	// Save()
	//==============================================================
	/// Save stack to file
	/**
		\param [in] pFile	-	Disk filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Save( LPCTSTR pFile );

	/// Instance of stack trace
	static CStackTrace			*m_pst;

		// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY
#ifdef ENABLE_STACK_HISTORY_TIME

	/// Update the global time, if valid
	static void SetHistoryTime( DWORD dwTime )
	{ 
		if( dwTime > llGlobalTime )
		{
			llGlobalTime = dwTime;
		}
	}

	/// History time vars
	static LONGLONG				llGlobalTime;

#endif
#endif
		// ...wjr 1/19/07


	//==============================================================
	// St()
	//==============================================================
	/// Returns instance of stack trace object
	static CStackTrace* St() 
	{ 	if ( !m_pst ) m_pst = new CStackTrace(); return m_pst; }

	//==============================================================
	// Release()
	//==============================================================
	/// Releases stack trace object
	static void Release() 
	{	
		// No more stack tracing
		m_bShutdown = TRUE; 

		RULIB_TRY
		{
			// Delete object if any
			if ( m_pst )
			{
				// Lock for good
				m_pst->m_lock.Lock( 30000 );

				// Delete
				delete m_pst; 

			} // end if

		} RULIB_CATCH_ALL {}

		// All gone
		m_pst = NULL; 
	}

	/// Release all
	static BOOL			m_bShutdown;

#if defined( CStackTrace_USING_TLSAPI )

	static DWORD					  m_tls_dwIndex;

#elif defined( CStackTrace_USING_TLS )

	/// Thread specific stack pointer
	static _declspec( thread ) CStack *m_tls_pStack;

#endif

};

//==================================================================
// CLocalStackTrace
//
/// Use this class to wrap local stack pushes
/**
	Wraps the CStack Push() and ensures Pop() is called when the 
	function exits.
*/
//==================================================================
class CLocalStackTrace
{
public:

	/// Default Constructor
	CLocalStackTrace( LPCTSTR pFunction ) 
	{
		if ( !CStackTrace::m_bShutdown )
		{
			m_pStack = CStackTrace::St()->InitPush(); 

#ifdef ENABLE_STACK_PROFILE
			if ( m_pStack ) m_pPi = m_pStack->Push( pFunction );
			else m_pPi = NULL;
#else
			if ( m_pStack ) m_pStack->Push( pFunction );
#endif

		} // end if
		
#ifdef ENABLE_STACK_PROFILE
		else m_pStack = NULL, m_pPi = NULL; 
#else
		else m_pStack = NULL; 
#endif

	}

	/// Destructor
	~CLocalStackTrace() 
	{ 
		if ( CStackTrace::m_bShutdown ) return;

		if ( m_pStack ) m_pStack->Pop();

#ifdef ENABLE_STACK_PROFILE

		if ( m_pPi ) m_pPi->llTotalTime += m_pPi->hqTimer.Ellapsed();

#endif

	}

private:

	/// Pointer to stack item that will be released
	CStackTrace::CStack					*m_pStack;

#ifdef ENABLE_STACK_PROFILE

	/// This is just to make sure the timer frequency is initialized
	CHqTimer							m_hqDummy;

	/// Pointer to profile item that is to be updated on release
	CStackTrace::CStack::SProfileItem	*m_pPi;

#endif
};


#endif // !defined(AFX_STACKTRACE_H__444879CF_2267_458B_9685_5237B375EC2C__INCLUDED_)
