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
// StackReport.cpp: implementation of the CStackReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStackReport::CStackReport()
{_STT();
}

CStackReport::~CStackReport()
{_STT();
}

// wjr 11/10/06...
BOOL CStackReport::ErrorLog(CReg *pReg, CErrLog* pErrLog )
{_STT();
	if( !pReg || !pErrLog )
		return E_INVALIDARG;

	CTlLocalLock ll( *pErrLog );
	if ( !ll.IsLocked() ) return FALSE;

	// Reset object
	pReg->Destroy();

	int	iId = 1;
	char szMsg[ 256 ] = "";
	LPERRORITEMINFO pEii = NULL;
	while ( ( pEii = (LPERRORITEMINFO)pErrLog->GetNext( pEii ) ) != NULL )
	{
		wsprintf( szMsg, "Error %d", iId++ );

		CRKey *pRk = pReg->GetKey( szMsg );
		if ( pRk )
		{
			CWinTime wt;
			wt.SetUnixTime( pEii->dwTime );
			wt.GetString( szMsg, "%w %b %D %Y - %g:%m:%s" );

			// Save the time.
			pRk->Set( "time", szMsg );
			
			// Save function info if present.
			if( pEii->sFile && pEii->sFunction )
			{
				if( pEii->sFile->ptr() && pEii->sFunction->ptr() )
				{
					try
					{
						char* pStr = NULL;
						wsprintf( szMsg, "%s::%s", pEii->sFile->ptr(), pEii->sFunction->ptr() );
						// Parse out the path
						int ch = '\\';
						pStr = strrchr( szMsg, ch );

						if( pStr )
						{
							pStr++; // dump the "\".
							pRk->Set( "function", pStr );
						}

						// Save the file info
						// wjr 1/9/07...
						wsprintf( szMsg, "%s", pEii->sFile->ptr() );
						pRk->Set( "file", szMsg );
						// ...wjr 1/9/07
			
					}
					catch(...)
					{
						ASSERT(0);
					}

				}
			}
			
			// Save the message
			pRk->Set( "message", pEii->cpkey );

			// Save the error code
			CWin32::GetSystemErrorMsg( pEii->dwCode, szMsg );
			pRk->Set( "code", szMsg );

			// wjr 1/9/07...
			// Save the line
			pRk->Set( "line", pEii->dwLine );

			// Save the severity
			pRk->Set( "severity", pEii->dwSeverity );

			// Save the raw time data
			pRk->Set( "timedata", pEii->dwTime );
			// ...wjr 1/9/07

		} // end if

	} // end while

	return TRUE;
}
// ...wjr 11/10/06

BOOL CStackReport::CallStack(CReg *pReg, CStackTrace *pSt )
{_STT();
	CTlLocalLock ll( *pSt );
	if ( !ll.IsLocked() ) return FALSE;

	// Reset object
	pReg->Destroy();

	DWORD dwThreadId = GetCurrentThreadId();

	char szMsg[ 256 ] = "";
	CStackTrace::iterator it = NULL;
	CStackTrace::list *pList = pSt->GetList();
	while ( NULL != ( it = pList->next( it ) ) )
	{
		wsprintf( szMsg, "Thread %lu ( 0x%lx )", *it->key(), *it->key() );

		CRKey *pRk = pReg->GetKey( szMsg );
		if ( pRk )
		{
			// Flag if this is the current thread
			if ( dwThreadId == *it->key() ) pRk->Set( "current_thread", dwThreadId );
			
			// Save the thread id
			pRk->Set( "thread_id", (DWORD)*it->key() );

			// Get stack information
			UINT uStack = (*it)->GetStackPtr();
			LPCTSTR *pStack = (*it)->GetStack();

			// Save
			if ( uStack ) for ( UINT i = 0, p = uStack - 1; i < uStack; i++, p-- )
			{
				wsprintf( szMsg, "f_%lu", i );

				// If we have a valid string
				if ( pStack[ i ] ) pRk->Set( szMsg, pStack[ p ] );

			} // end for

		} // end if

	} // end while

	return TRUE;
}

BOOL CStackReport::History(CReg *pReg, CStackTrace *pSt )
{_STT();
#ifndef ENABLE_STACK_HISTORY

	return FALSE;

#else

	CTlLocalLock ll( *pSt );
	if ( !ll.IsLocked() ) return FALSE;

	// Reset object
	pReg->Destroy();

	DWORD dwThreadId = GetCurrentThreadId();

	char szMsg[ 256 ] = "";
	CStackTrace::iterator it = NULL;
	CStackTrace::list *pList = pSt->GetList();
	while ( NULL != ( it = pList->next( it ) ) )
	{
		wsprintf( szMsg, "Thread %lu ( 0x%lx )", *it->key(), *it->key() );

		CRKey *pRk = pReg->GetKey( szMsg );
		if ( pRk )
		{
			// Flag if this is the current thread
			if ( dwThreadId == *it->key() ) pRk->Set( "current_thread", dwThreadId );
			
			// Save the thread id
			pRk->Set( "thread_id", (DWORD)*it->key() );

			// Get stack information
			UINT uSize = (*it)->GetMaxHistory();
			UINT uPtr = (*it)->GetHistoryPtr();
			LPCTSTR *pHistory = (*it)->GetHistory();
#ifdef ENABLE_STACK_HISTORY_TIME
			LONGLONG *pHistoryTime = (*it)->GetHistoryTime(); // wjr 1/19/07
#endif

			// Is there anything in the history?
			if ( uSize )
			{
				// Save
				for ( UINT i = 0; i < uSize; i++ )
				{
					// Adjust pointer
					if ( uPtr ) uPtr--; else uPtr = uSize - 1;

					wsprintf( szMsg, "f_%04lu", i );// wjr 1/19/07

					if ( pHistory[ uPtr ] )
					{
						// wjr 1/19/07...
#ifdef ENABLE_STACK_HISTORY_TIME
						char szMsg2[ 256 ] = "";
						if( pHistoryTime[ uPtr ] )
							wsprintf( szMsg2, "%ld - %s", (unsigned long)pHistoryTime[ uPtr ], pHistory[ uPtr ] );
						else
							wsprintf( szMsg2, "%s", pHistory[ uPtr ] );// wjr 1/22/07

						pRk->Set( szMsg, szMsg2 );
#else
						pRk->Set( szMsg, pHistory[ uPtr ] );
#endif
						// ...wjr 1/19/07
					}

				} // end for

			} // end if

		} // end if

	} // end while

	return TRUE;

#endif

}

BOOL CStackReport::Profile(CReg *pReg, CStackTrace *pSt )
{_STT();
#ifndef ENABLE_STACK_PROFILE

	return FALSE;

#else

	CTlLocalLock ll( *pSt );
	if ( !ll.IsLocked() ) return FALSE;

	// Reset object
	pReg->Destroy();

	DWORD dwThreadId = GetCurrentThreadId();

	char szMsg[ 256 ] = "";
	CStackTrace::iterator it = NULL;
	CStackTrace::list *pList = pSt->GetList();
	while ( NULL != ( it = pList->next( it ) ) )
	{
		wsprintf( szMsg, "Thread %lu ( 0x%lx )", *it->key(), *it->key() );

		CRKey *pRk = pReg->GetKey( szMsg );
		if ( pRk )
		{
			// Flag if this is the current thread
			if ( dwThreadId == *it->key() ) pRk->Set( "current_thread", dwThreadId );
			
			// Save the thread id
			pRk->Set( "thread_id", *it->key() );

			TStrList< CStackTrace::CStack::SProfileItem > *pProfile = (*it)->GetProfile();
			if ( pProfile )
			{
				DWORD i = 0;
				TStrList< CStackTrace::CStack::SProfileItem >::iterator itPi = NULL;
				while ( NULL != ( itPi = pProfile->next( itPi ) ) )
				{
					// Save function name
					wsprintf( szMsg, "f_%lu", i );
					pRk->Set( szMsg, itPi->key() );

					// Save total time
					wsprintf( szMsg, "t_%lu", i );
					pRk->Set( szMsg, (double)( (double)(*itPi)->llTotalTime / (double)CHqTimer::GetTimerFrequency() ) );

					// Next index
					i++;

				} // end while

			} // end if

		} // end if

	} // end while

	return TRUE;
	
#endif
}
