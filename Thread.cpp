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
// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#ifndef TRACE
#define TRACE
#endif

#ifndef ASSERT
#define ASSERT
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThread::CThread() :
	// Create a semephore
	m_hStop ( CreateEvent( NULL, TRUE, FALSE, NULL ) ),
	// Create a semephore
	m_hStopped ( CreateEvent( NULL, TRUE, FALSE, NULL ) ),
	// Create a semephore
	m_hPause ( CreateEvent( NULL, TRUE, FALSE, NULL ) ),
	// Create a semephore
	m_hPaused ( CreateEvent( NULL, TRUE, FALSE, NULL ) ),
	// Create a semephore
	m_hUnpause ( CreateEvent( NULL, TRUE, FALSE, NULL ) ),
	// Create a semephore
	m_hInitialized ( CreateEvent( NULL, TRUE, FALSE, NULL ) )
{
	m_hThread = NULL;
	m_pvoidData = NULL;
	m_dwPriority = 15;
	m_bException = FALSE;

	m_bMfc = FALSE;
	m_bMessagePump = FALSE;
}

CThread::~CThread()
{_STT();
	// Kill any thread that may be running
	StopThread();

	// Release the stop events
	if ( m_hStop != NULL )
	{
		CloseHandle( m_hStop );
		m_hStop = NULL;
	} // end if

	if ( m_hStopped != NULL )
	{
		CloseHandle( m_hStopped );
		m_hStopped = NULL;
	} // end if

	if ( m_hPause != NULL )
	{
		CloseHandle( m_hPause );
		m_hPause = NULL;
	} // end if

	if ( m_hPaused != NULL )
	{
		CloseHandle( m_hPaused );
		m_hPaused = NULL;
	} // end if

	if ( m_hUnpause != NULL )
	{
		CloseHandle( m_hUnpause );
		m_hUnpause = NULL;
	} // end if

	if ( m_hInitialized != NULL )
	{
		CloseHandle( m_hInitialized );
		m_hInitialized = NULL;
	} // end if
}

// The number of threads running
DWORD CThread::m_dwThreadCount = 0;
DWORD CThread::m_dwRunningThreadCount = 0;

//////////////////////////////////////////////////////////////
// CommThread () /////////////////////////////////////////////
//
// This static function is the actual body of the thread
// overide DoThread() for custom work
// 
//////////////////////////////////////////////////////////////
DWORD WINAPI CThread::Thread ( LPVOID pData )
{_STT();
	// Get a pointer to our data
	CThread		*pThread = (CThread*)pData;

	// Sanity Check
	if ( pThread == NULL ) return (DWORD)-1;

	CoInitialize( NULL );

	BOOL		bMfc = pThread->m_bMfc;
	DWORD		dwRet;
	HANDLE		hStop, hStopped;
	LPVOID		pUser = pThread->m_pvoidData;

#ifdef MFCSAFETHREADS	
	bMfc = ( pThread->m_pWinThread != NULL );
#endif

	// Get stop event
	hStop = pThread->m_hStop;
	hStopped = pThread->m_hStopped;

	if ( hStop == NULL || hStopped == NULL ) 
	{
		// Tell Everyone we are done
		return ( pThread->m_dwThreadReturn = (DWORD)-1 );
	} // end if

	RULIB_TRY
	{
		// Attempt to initialize the thread
		if ( pThread->InitThread( pUser ) )
		{
			// We did initialize
			SetEvent( pThread->m_hInitialized );

			// Count one running thread
			IncRunningThreadCount();

			// Run the thread
			do
			{
				// Do the message pump for this thread
				if ( pThread->m_bMessagePump ) MessagePump( pThread );

				// Pause if needed
				if ( pThread->IsPausing() )
				{
					// Signal that we are paused
					SetEvent( pThread->m_hPaused );

					// Wait for unpause
					WaitForSingleObject( pThread->m_hUnpause, INFINITE );

					// Reset pause event
					ResetEvent( pThread->m_hPause );

					// Reset unpause event
					ResetEvent( pThread->m_hUnpause );

					// Clear paused status
					ResetEvent( pThread->m_hPaused );

				} // end while

			} while (	// Check for stop flag
						!pThread->IsStopping()

						&&

						// Don't hog the processor
						pThread->ThreadSleep()

						&&

						// Let's do the important stuff
						pThread->DoThread( pUser ) );
			
			// Uncount one running thread
			DecRunningThreadCount();

		} // end if

		// Let the user clean up
		pThread->EndThread( pUser );

	} // end try

	RULIB_CATCH_ALL
	{
		ASSERT( 0 );

		// There was an exception
		pThread->m_bException = TRUE;
	} // end catch

	// Save the users return value
	dwRet = pThread->m_dwThreadReturn;

	CoUninitialize();

	// Tell Everyone we are done
	SetEvent( hStopped );

	// We can't access any data not on our own function stack now
	// This includes class member variables

	// Allow some ghosting
//	return pThread->GhostThread( pUser, dwRet );

#ifdef MFCSAFETHREADS	
	// Let MFC in on this
	if ( bMfc ) AfxEndThread( dwRet );
#endif

	return dwRet;

} // end CommThread()

void CThread::MessagePump( CThread *pThread )
{_STT();
	MSG msg;

	// If there is a message
	while (	PeekMessage( &msg, NULL, NULL, NULL, PM_REMOVE ) )
	{	
		// Translate accelerators
//		if ( !TranslateAccelerator( msg.hwnd, hAccel, &msg ) )
		{
			// Translate key strokes
			TranslateMessage( &msg );	

			// Dispatch the message
			DispatchMessage( &msg );

		} // end if

    } // end if
/*
#ifdef MFCSAFETHREADS	

	if ( pThread != NULL )
	{
		CWinThread *pWinThread = pThread->m_pWinThread;
		if ( pWinThread != NULL )
		{
			BOOL bIdle = TRUE;
			LONG lIdleCount = 0;

			while ( bIdle &&
				!::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE ) )
			{
				// call OnIdle while in bIdle state
				if ( !pWinThread->OnIdle( lIdleCount++ ) )
					bIdle = FALSE; // assume "no idle" state
			} // end while

		} // end if

	} // end if

#endif
*/
}


BOOL CThread::StartThread( LPVOID pData, BOOL bRestart, BOOL bMessagePump, BOOL bMfc )
{_STT();
	// Do we want to restart?
	if ( bRestart ) StopThread();

	// Quit if we are already running
	else if ( IsRunning() != NULL ) return FALSE;

	// Reset initialized event
	ResetEvent( m_hInitialized );

	// Save message pump params
	m_bMessagePump = bMessagePump;
	m_bMfc = bMfc;

	// No exception yet
	m_bException = FALSE;
	
	// Save the users data
	m_pvoidData = pData;

	// Assume thread success
	m_dwThreadReturn = 0;

	// Reset the semephores
	ResetEvent( m_hStop );
	ResetEvent( m_hStopped );
	ResetEvent( m_hPause );


#ifdef MFCSAFETHREADS	

	if ( m_bMfc )
	{
		// MFC crap	( create suspended so we can read information )
		m_pWinThread = 
			AfxBeginThread( CThread::Thread, 
							(LPVOID)this, THREAD_PRIORITY_NORMAL,
							0, CREATE_SUSPENDED );
		if ( m_pWinThread == NULL ) return FALSE;

		// Save thread id's
		m_hThread = m_pWinThread->m_hThread;
		m_dwThreadId = m_pWinThread->m_nThreadID;

		// Resume the thread
		if ( m_pWinThread->ResumeThread() == MAXDWORD )
		{	StopThread();
			return FALSE;		
		} // end if

	} // end if

	else
	{

#endif
	
		// Create a thread
		m_hThread = CreateThread(	(LPSECURITY_ATTRIBUTES)NULL,
									0,
									CThread::Thread,
									(LPVOID)this,	
									0, 
									&m_dwThreadId );

#ifdef MFCSAFETHREADS	

	} // end else

#endif

	// Did we get the thread?
	if ( m_hThread == NULL ) return FALSE;

	// Count one thread
	IncThreadCount();

	// Apparently, all is well
	return TRUE;
}

BOOL CThread::StopThread( BOOL bKill, DWORD dwWait )
{_STT();
	// Quit if no thread
	if ( m_hThread == NULL ) return FALSE;

	// Tell Thread To Stop
	SignalStop();

	if ( bKill )
	{
		HANDLE	phEvents[ 2 ];

		phEvents[ 0 ] = m_hThread;
		phEvents[ 1 ] = m_hStopped;

		// Record the time
		DWORD dwCount = GetTickCount() + dwWait;

		// Wait to see if the thread will quit on its own
		while(	( WaitForMultipleObjects( 2, phEvents, FALSE, 30 ) == WAIT_TIMEOUT )
				&& ( dwCount > GetTickCount() ) ) 
		{	/*+++ MessagePump( this ); */ Sleep( 0 ); }

		// Kill the thread off if it is still running
		if ( ( WaitForMultipleObjects( 2, phEvents, FALSE, 30 ) == WAIT_TIMEOUT ) ) 
		{
			TRACE( _T( "!!! TerminateThread() being called !!!\n" ) );
			TerminateThread( m_hThread, FALSE );
#ifdef MFCSAFETHREADS	
			if ( m_pWinThread != NULL ) CloseHandle( m_hThread );
#endif
		} // end if

	} // end if

#ifdef MFCSAFETHREADS	

	if ( m_pWinThread != NULL ) m_pWinThread = NULL;

	// Close The Handle To The Thread
	else CloseHandle( m_hThread );

#else

	// Close The Handle To The Thread
	CloseHandle( m_hThread );

#endif

	m_hThread = NULL;

	// Reset the semephores
	ResetEvent( m_hStop );
	ResetEvent( m_hStopped );
	ResetEvent( m_hPause );

	// Uncount one thread
	DecThreadCount();

	return TRUE;
} 


void CThread::SignalStop()
{_STT();
	// Quit if no thread
	if ( m_hThread == NULL ) return;

	// Tell Thread To Stop
	SetEvent( m_hStop );

	// Post quit message to thread
	PostThreadMessage( GetThreadId(), WM_QUIT, 0, 0L );

}

BOOL CThread::WaitThreadInit( DWORD dwTimeout )
{_STT();
	HANDLE	phEvents[ 4 ];

	// Trigger on any of these
	phEvents[ 0 ] = m_hStop;
	phEvents[ 1 ] = m_hStopped;
	phEvents[ 2 ] = m_hPause;
	phEvents[ 3 ] = m_hInitialized;

	// Wait on an event
	return ( WaitForMultipleObjects( 4, phEvents, FALSE, dwTimeout ) != WAIT_TIMEOUT );
}
