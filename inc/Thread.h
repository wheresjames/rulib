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
// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__0808D743_1A58_11D2_835D_00104B2C9CFA__INCLUDED_)
#define AFX_THREAD_H__0808D743_1A58_11D2_835D_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#define MFCSAFETHREADS
#undef MFCSAFETHREADS

//==================================================================
// CThread
//
/// Provides base thread functionality
/**
	Derive a class from from CThread and over-ride InitThread(),
	DoThread(), and EndThread() to provide custom functionality.	
*/
//==================================================================
class CThread  
{
public:

	/// Default Constructor
	CThread();

	/// Destrucotr
	virtual ~CThread();
	
	//==============================================================
	// Start()
	//==============================================================
	/// Starts the thread
	virtual BOOL Start() { return StartThread(); }

	//==============================================================
	// Stop()
	//==============================================================
	/// Stops the thread
	virtual BOOL Stop() { return StopThread(); }

	//==============================================================
	// StartThread()
	//==============================================================
	/// Starts the thread
	/**
		\param [in] pData			-	Custom data passed on to thread
		\param [in] bRestart		-	Non-zero to restart thread if running
		\param [in] bMessagePump	-	Non-zero to have Windows messages pumped.
		\param [in] bMfc			-	Non-zero for MFC friendly thread.
		
		\return Non-zero if thread was started.
	
		\see 
	*/
	virtual BOOL StartThread( LPVOID pData = NULL, BOOL bRestart = TRUE, BOOL bMessagePump = FALSE, BOOL bMfc = FALSE );

	//==============================================================
	// StopThread()
	//==============================================================
	/// Stops any running thread
	/**
		\param [in] bKill	-	Set to non-zero to kill thread if it
								fails to stop gracefully.
		\param [in] dwWait	-	Time in milli-seconds to wait for 
								thread to stop before killing it.
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL StopThread( BOOL bKill = TRUE, DWORD dwWait = 3000 );

	//==============================================================
	// IsRunning()
	//==============================================================
	/// Returns non-zero if thread is currently running
	BOOL IsRunning() { 	if ( m_hThread == NULL ) return FALSE;						
						return ( WaitForSingleObject( m_hThread, 0 ) == WAIT_TIMEOUT &&
								 WaitForSingleObject( m_hStopped, 0 ) == WAIT_TIMEOUT ); }

	//==============================================================
	// PauseThread()
	//==============================================================
	/// Pauses any running thread
	/**
		If the thread is started paused then InitThread() will still be called.
		The thread will pause before the first call to DoThread().  
		If StopThread() is called while the thread is paused, DoThread() will
		*not* be called again. EndThread() and GhostThread() will still be called.
		In short, Pausing the thread prevents any calls to DoThread().

		\return 
	
		\see 
	*/
	void PauseThread() { SetEvent( m_hPause ); }

	//==============================================================
	// IsPausing()
	//==============================================================
	/// Returns non-zero if thread is paused
	BOOL IsPausing() { return ( WaitForSingleObject( m_hPause, 0 ) != WAIT_TIMEOUT ); }
	
	//==============================================================
	// WaitPause()
	//==============================================================
	/// Waits for thread to pause
	/**
		\param [in] timeout	-	Time in milli-seconds to wait for thread
								to pause.

		\return Non-zero if thread was paused before timeout
	
		\see 
	*/
	BOOL WaitPause( DWORD timeout = 3000 ) 
	{	if ( !IsRunning() ) return FALSE; 
		return ( WaitForSingleObject( m_hPaused, timeout ) != WAIT_TIMEOUT ); 
	}
	
	//==============================================================
	// MsgWaitPause()
	//==============================================================
	/// Pumps windows messages while waiting for thread to pause.
	/**
		\param [in] timeout	-	Time in milli-seconds to wait for thread
								to pause.

		\return Non-zero if thread was paused before timeout
	
		\see 
	*/
	BOOL MsgWaitPause( DWORD timeout = 3000 ) 
	{	if ( !IsRunning() ) return FALSE; 
		DWORD dwCount = GetTickCount() + timeout;
		while(	( WaitForSingleObject( m_hPaused, 30 ) != WAIT_TIMEOUT )
				&& ( dwCount > GetTickCount() ) ) 
		{	MessagePump( this ); Sleep( 0 ); }
		return IsPaused();
	}

	//==============================================================
	// UnpauseThread()
	//==============================================================
	/// Unpauses thread
	void UnpauseThread() { SetEvent( m_hUnpause ); }

	//==============================================================
	// IsPaused()
	//==============================================================
	// Returns non-zero if thread is currently paused
	BOOL IsPaused() { return ( WaitForSingleObject( m_hPaused, 0 ) != WAIT_TIMEOUT ); }

	//==============================================================
	// IsStopping()
	//==============================================================
	/// Returns non zero if thread is stopping
	BOOL IsStopping() { return ( WaitForSingleObject( m_hStop, 0 ) != WAIT_TIMEOUT ); }


	//==============================================================
	// InitThread()
	//==============================================================
	/// 
	/**
		\param [in] pData - User defined data
		
		The pData value passed to these functions represent the original value
		in m_pvoidData when the thread started.  Changing m_pvoidData in these
		functions does not affect the passed value to the next function.
		This -hopefully- garentees that pData is always equal to the value
		passed to StartThread()

		Overide this function to provide custom thread Initialization
		If you don't need initialization you can just use DoThread()

		pData - whatever was passed to StartThread()

		return TRUE if you want to continue the thread ( calling DoThread() )
		return FALSE to end the thread now

		\return Non-zero to continue thread, zero to terminate thread execution.
	
		\see DoThread(), EndThread(), GhostThread()
	*/
	virtual BOOL InitThread( LPVOID pData ) { return TRUE; }

	//==============================================================
	// DoThread()
	//==============================================================
	/// 
	/**
		\param [in] pData	-	User defined value
		
		Overide this function to provide custom thread work

		This function is called over and over until it returns FALSE
		or StopThread() is called

		pData - whatever was passed to StartThread()
		return TRUE if you want more processing (i.e. DoThread() will be called again )
		return FALSE to end the thread

		\return Non-zero to continue thread, zero to terminate thread execution.
	
		\see InitThread(), EndThread(), GhostThread()
	*/
	virtual BOOL DoThread( LPVOID pData ) { return FALSE; }

	//==============================================================
	// EndThread()
	//==============================================================
	/// 
	/**
		\param [in] pData	-	User defined value
		
		Overide this function to provide custom cleanup

		pData - whatever was passed to StartThread()

		The return value is ignored
		if you want to return a thread value then set m_dwThreadReturn

		\return Return value is ignored
	
		\see InitThread(), DoThread(), GhostThread()
	*/
	virtual BOOL EndThread( LPVOID pData ) { return FALSE; }

	//==============================================================
	// GhostThread()
	//==============================================================
	/// 
	/**
		\param [in] pData				-	User defined data
		\param [in] dwThreadReturn		-	Thread return value
		
		This function is called between when the thread tells the
		program it is done and when it actually exits
 
		At this point the thread is unaccessable to the program 
		and no class data should be accessed since it may now belong
		to another thread.  Also this implementation is inline which 
		means that no function call is actually made.  The call is made
		from a static function so inline function code will not disapear 
		when the class is deleted.  If you overide this with a non-inline 
		function it will be subject to destruction along with the class 
		and you must make sure that the function code is still valid 
		even after the thread has reported it has terminated.


		The return value is the final value returned by the thread
		By default it is m_dwThreadReturn as it was after EndThread()

		Just to make sure it is inline * any code in a class definition is inline

		\return Value returned by thread
	
		\see InitThread(), DoThread(), EndThread()
	*/
	inline virtual DWORD GhostThread( LPVOID pData, DWORD dwThreadReturn ) { return dwThreadReturn; }


	//==============================================================
	// GetThreadCount()
	//==============================================================
	/// Returns the total number of threads started using this class
	/**
		This is retrieved from a static variable.  So it only indicates
		threads started from the calling process.

		\return Number of threads started using this class
	
		\see 
	*/
	static DWORD GetThreadCount() { return m_dwThreadCount; }

	//==============================================================
	// GetRunningThreadCount()
	//==============================================================
	/// Returns the total number of threads running using this class
	/**
		This is retrieved from a static variable.  So it only indicates
		threads running in the calling process.

		\return Number of threads running using this class
	
		\see 
	*/
	static DWORD GetRunningThreadCount() { return m_dwRunningThreadCount; }

	//==============================================================
	// SetThreadPriority()
	//==============================================================
	/// Sets the thread priority
	/**
		\param [in] dwPriority	-	Thread priority
		
		Get/Set Thread Prioritys
		All Running CThreads share the same priority
		the threads will pause for m_dwPriority while processing
		Zero is the highest priority and will cause the thread to use all
		Idle CPU cycles

	*/
	void SetThreadPriority( DWORD dwPriority ) { m_dwPriority = dwPriority; }

	//==============================================================
	// GetThreadPriority()
	//==============================================================
	/// Returns the thread priority
	DWORD GetThreadPriority() { return m_dwPriority; }

	//==============================================================
	// ThreadSleep()
	//==============================================================
	/// 
	/**
		\param [in] add		-	Time to add to default wait time
		\param [in] abs		-	Absolute wait time
		
		Sleeps for amount of time specified by Priority
		Returns FALSE if the thread should terminate
		TRUE if it is ok to keep running

		\return Non-zero if thread is not stopping
	
		\see 
	*/
	BOOL ThreadSleep( DWORD add = 0, DWORD abs = 0 ) 
	{	return ( WaitForSingleObject( m_hStop, ( abs > 0 ) ? abs : ( GetThreadPriority() + add ) ) == WAIT_TIMEOUT ); }

	//==============================================================
	// GetExceptionStatus()
	//==============================================================
	/// Returns the thread exception status
	/**
		This function returns non-zero if an exception has occured
		within the thread
	*/
	BOOL GetExceptionStatus() { return m_bException; }

	//==============================================================
	// GetSafeThreadHandle()
	//==============================================================
	/// Returns the thread handle
	HANDLE GetSafeThreadHandle() { return m_hThread; }

	//==============================================================
	// WaitThreadInit()
	//==============================================================
	/// Waits for the thread to initialize
	/**
		\param [in] dwTimeout	-	Maximum amount of time in milli-
									seconds to wait for thread to
									initialize.
		
		\return Non-zero if thread was initialized before timeout.
	
		\see 
	*/
	BOOL WaitThreadInit( DWORD dwTimeout );

	//==============================================================
	// GetStopEvent()
	//==============================================================
	/// Returns the handle to the stop event
	HANDLE GetStopEvent() { return m_hStop; }

	//==============================================================
	// GetThreadId()
	//==============================================================
	/// Returns the thread ID
	DWORD GetThreadId() { return m_dwThreadId; }

protected:

	/// Handle to thead
	HANDLE		m_hThread;

	/// Stop event
	HANDLE		m_hStop;

	/// Event triggered when thread has stopped
	HANDLE		m_hStopped;

	/// Event triggered when thread has initialized
	HANDLE		m_hInitialized;

#ifdef MFCSAFETHREADS

	/// For MFC safe threads
	CWinThread	*m_pWinThread;

#endif

private:

	/// Set to Non zero when thread needs initialization
	BOOL		m_bInit;

	/// Current thread ID
	DWORD		m_dwThreadId;

	/// User data
	LPVOID		m_pvoidData;

	/// Thread return value
	DWORD		m_dwThreadReturn;

	/// Thread pause event handle
	HANDLE		m_hPause;

	/// Thread paused event handle
	HANDLE		m_hPaused;

	/// Thread unpause event handle
	HANDLE		m_hUnpause;

	/// Set to non-zero if thread generates an exception
	BOOL		m_bException;

	/// Set to non-zero to run Windows message pump
	BOOL		m_bMessagePump;

	/// Set to non-zero for MFC friendly thread
	BOOL		m_bMfc;

private:

	//==============================================================
	// Thread()
	//==============================================================
	/// This is the main thread function
	/**
		\param [in] pData		-	User defined value.

		\return Thread return value
	*/
	static DWORD WINAPI Thread ( LPVOID pData );

	//==============================================================
	// IncThreadCount()
	//==============================================================
	/// Returns the thread ID
	static void IncThreadCount() { m_dwThreadCount++; }

	//==============================================================
	// DecThreadCount()
	//==============================================================
	/// Returns the thread ID
	static void DecThreadCount() { m_dwThreadCount--; }

	// Count of threads started
	static DWORD m_dwThreadCount;

	//==============================================================
	// IncRunningThreadCount()
	//==============================================================
	/// Returns the thread ID
	static void IncRunningThreadCount() { m_dwRunningThreadCount++; }

	//==============================================================
	// DecRunningThreadCount()
	//==============================================================
	/// Returns the thread ID
	static void DecRunningThreadCount() { m_dwRunningThreadCount--; }

	/// Count of running thread
	static DWORD m_dwRunningThreadCount;

	/// Current thread priority
	DWORD m_dwPriority;

public:

	//==============================================================
	// SignalStop()
	//==============================================================
	/// Called to signal to the thread that it should exit
	void SignalStop();

	//==============================================================
	// MessagePump()
	//==============================================================
	/// Windows message pump
	/**
		\param [in] pThread	-	Thread for which messages will be 
								pumped.  Set to NULL to pump messages
								for all threads.
	*/
	static void MessagePump( CThread *pThread = NULL );

	//==============================================================
	// WaitAllThreadInit()
	//==============================================================
	/// Use to wait on all threads to initialize.
	/**
		\param [in] dwTimeout	-	Maximum amount of time in milli-
									seconds to wait for threads to
									initialize
		\param [in] dwPriority	-	Time in milli-seconds between
									thread status checks.

		This function will wait on all threads derived from this
		class that have been started to initialize.  Cool trick 
		sometimes...
		
		\return Non-zero if all threads started before timeout
	
		\see 
	*/
	static BOOL WaitAllThreadInit( DWORD dwTimeout, DWORD dwPriority = 15 )
	{
		DWORD dwTickTimeout = GetTickCount() + dwTimeout;
		while ( dwTickTimeout > GetTickCount() )
		{
			if ( m_dwThreadCount != m_dwRunningThreadCount ) 
				Sleep( dwPriority );
			else return TRUE;
		} // end while
		return FALSE;
	}

};

#endif // !defined(AFX_THREAD_H__0808D743_1A58_11D2_835D_00104B2C9CFA__INCLUDED_)
