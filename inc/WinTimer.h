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
// WinTimer.h: interface for the CWinTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTIMER_H__E4640A0C_C662_4963_B1E5_351F69716984__INCLUDED_)
#define AFX_WINTIMER_H__E4640A0C_C662_4963_B1E5_351F69716984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//==================================================================
// CWinTimer
//
/// Provides a wrapper for the Windows timer API
/**
	Wraps the Windows timer API making setting timers very slightly
	less painful.  Ok, so using this class may be overkill...	
*/
//==================================================================
class CWinTimer  
{
public:

	//==============================================================
	// SetTimer()
	//==============================================================
	///	Requests Windows call the timer function at specified interval
	/**
		\param [in] uValue		-	Timer interval in milli-seconds
		\param [in] pTimerProc	-	Callback function.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetTimer( UINT uValue, TIMERPROC pTimerProc );

	//==============================================================
	// KillTimer()
	//==============================================================
	/// Stops the timer
	void KillTimer();

	//==============================================================
	// SetTimer()
	//==============================================================
	/// Requests Windows send the timer event at the specified interval
	/**
		\param [in] hWnd	-	Window handle receiving WM_TIMER message
		\param [in] uEvent	-	Unique event ID
		\param [in] uValue	-	Timer interval in milli-seconds
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetTimer( HWND hWnd, UINT uEvent, UINT uValue );

	/// Default constructor
	CWinTimer();

	/// Destructor
	virtual ~CWinTimer();

	//==============================================================
	// GetId()
	//==============================================================
	/// Returns the timer ID
	UINT GetId() { return m_uId; }

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the timer callback interval
	UINT GetValue() { return m_uValue; }

	//==============================================================
	// GetEvent()
	//==============================================================
	/// Returns the timer event ID
	UINT GetEvent() { return m_uEvent; }

	//==============================================================
	// GetTimerProc()
	//==============================================================
	/// Returns a pointer to the timer callback function
	TIMERPROC GetTimerProc() { return m_pTimerProc; }

private:

	/// Handle of window receiving WM_TIMER messages
	HWND			m_hWnd;

	/// Timer ID
	UINT			m_uId;

	/// Timer interval value
	UINT			m_uValue;

	/// Timer event ID
	UINT			m_uEvent;

	/// Pointer to timer callback function
	TIMERPROC		m_pTimerProc;
};

#endif // !defined(AFX_WINTIMER_H__E4640A0C_C662_4963_B1E5_351F69716984__INCLUDED_)
