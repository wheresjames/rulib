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
// HqTimer.h: interface for the CHqTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HQTIMER_H__1DBB0229_BCE1_429E_874C_A2BBE79BF827__INCLUDED_)
#define AFX_HQTIMER_H__1DBB0229_BCE1_429E_874C_A2BBE79BF827__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CHqTimer
//
/// Timer class using high accuracy hardware timer if available
/**
	This class attempts to use the high accuracy hardware timer
	via the Windows API QueryPerformanceCounter() function if
	available.  If this counter is not available, it falls back
	on the less reliable GetTickCount() value.	
*/
//==================================================================
class CHqTimer  
{
public:
	
	/// Default Constructor
	CHqTimer()
	{
		// Get timer frequency
		if ( m_llFreq == 0 )
			if ( !QueryPerformanceFrequency( (LARGE_INTEGER*)&m_llFreq ) )
				m_llFreq = 0;

		// Reset timer values
		m_llStart = m_llStop = 0;
	}
	
	/// Destructor
	virtual ~CHqTimer() {}

	//==============================================================
	// GetTimerFrequency()
	//==============================================================
	/// Returns the frequency of the active timer
	static LONGLONG GetTimerFrequency() 
	{	if ( m_llFreq ) return m_llFreq; return 1000; }

	//==============================================================
	// GetTimer()
	//==============================================================
	/// Return the current timer value
	static LONGLONG GetTimer() 
	{	if ( m_llFreq )
		{	LONGLONG ll;
			QueryPerformanceCounter( (LARGE_INTEGER*)&ll );	 
			return ll;
		} // end if
		return GetTickCount();
	}

	//==============================================================
	// IsReset()
	//==============================================================
	/// Returns non-zero if timer is reset
	BOOL IsReset() { return ( !m_llStart && !m_llStop ); }

	//==============================================================
	// IsRunning()
	//==============================================================
	/// Returns non-zero if timer is running
	BOOL IsRunning() { return ( m_llStart && !m_llStop ); }

	//==============================================================
	// IsStopped()
	//==============================================================
	/// Returns non-zero if timer is stopped
	BOOL IsStopped() { return ( m_llStart && m_llStop ); }

	//==============================================================
	// Reset()
	//==============================================================
	/// Resets internal values
	void Reset() { m_llStart = m_llStop = 0; }

	//==============================================================
	// Start()
	//==============================================================
	/// Saves the current time as start time
	LONGLONG Start() { return m_llStart = GetTimer(); }

	//==============================================================
	// Start()
	//==============================================================
	/// Saves the specified time as start time
	/**
		\param [in] ll	-	Start time
	*/
	LONGLONG Start( LONGLONG ll ) { return m_llStart = ll; }

	//==============================================================
	// Stop()
	//==============================================================
	/// Saves the current time as stop time
	LONGLONG Stop() { return m_llStop = GetTimer(); }

	//==============================================================
	// Stop()
	//==============================================================
	/// Saves the specified time as stop time
	/**
		\param [in] ll	-	Stop time
	*/
	LONGLONG Stop( LONGLONG ll ) { return m_llStop = ll; }
	
	//==============================================================
	// Ellapsed()
	//==============================================================
	/// Returns amount of time captured or ellapsed since
	LONGLONG Ellapsed()	{ if ( m_llStop ) return m_llStop - m_llStart; return GetTimer() - m_llStart; }

	//==============================================================
	// EllapsedSeconds()
	//==============================================================
	/// Returns ellapsed time in seconds
	double EllapsedSeconds() { return ( (double)Ellapsed() / (double)GetTimerFrequency() ); }

private:

	/// Clock frequency
	static LONGLONG		m_llFreq;

	/// Start time marker
	LONGLONG			m_llStart;

	/// Stop time marker
	LONGLONG			m_llStop;

};

#endif // !defined(AFX_HQTIMER_H__1DBB0229_BCE1_429E_874C_A2BBE79BF827__INCLUDED_)
