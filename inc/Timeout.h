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
// Timeout.h: interface for the CTimeout class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEOUT_H__30DFF472_456B_4ED9_B41E_DD9AC0C21EBE__INCLUDED_)
#define AFX_TIMEOUT_H__30DFF472_456B_4ED9_B41E_DD9AC0C21EBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CTimeout
//
/// This class provies simple timing functionality
/**
	Use this class to determine the passage of time within one 
	milli-second.  For better accuracy, use CHqTimer
*/
//==================================================================
class CTimeout  
{
public:

	// Constructor
	CTimeout( DWORD dwTimeout = 0, DWORD dwTick = 0 )
	{	m_dwTimeout = dwTimeout; m_dwValue = 0; }

	/// Destructor
	virtual ~CTimeout() {}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the value of the timeout
	/**
		\param [in] dwTime	-	Time value in milliseconds
	*/
	void SetValue( DWORD dwTime ) { m_dwTimeout = dwTime; }

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the current timeout value 
	DWORD GetValue() { return m_dwTimeout; }
	
	//==============================================================
	// Reset()
	//==============================================================
	/// Starts the timer
	/**
		\param [in] dwTime	-	Current timer value
	*/
	void Reset( DWORD dwTime = 0 )
	{	if ( dwTime == 0 ) dwTime = GetTickCount(); 
		m_dwValue = dwTime + m_dwTimeout;
	}

	//==============================================================
	// Reset()
	//==============================================================
	/// Starts the timer at the specified time
	/**
		\param [in] dwTimeout	-	Timeout value in milliseconds
		\param [in] dwTime		-	Current timer value
	*/
	void ResetTo( DWORD dwTimeout, DWORD dwTime = 0 )
	{	if ( dwTime == 0 ) dwTime = GetTickCount(); 
		m_dwTimeout = dwTimeout;
		m_dwValue = dwTime + dwTimeout;
	}

	//==============================================================
	// IsValid()
	//==============================================================
	/// Returns non-zero if the timer is timming and has not timed out
	/**
		\param [in] dwTime	-	Current timer value
	*/
	BOOL IsValid( DWORD dwTime = 0 ) 
	{	if ( dwTime == 0 ) dwTime = GetTickCount();
		return ( m_dwValue > dwTime && m_dwValue <= ( dwTime + m_dwTimeout ) );
	}

	//==============================================================
	// IsExpired()
	//==============================================================
	/// Returns non-zero if the timer is timming and has timed out
	/**
		\param [in] dwTime	-	Current timer value
	*/
	BOOL IsExpired( DWORD dwTime = 0 ) 
	{	return ( m_dwValue != 0 && !IsValid() ); }

	//==============================================================
	// Expire()
	//==============================================================
	/// Expires the timer
	void Expire() { m_dwValue = MAXDWORD; }

	//==============================================================
	// Expire()
	//==============================================================
	/// Sets the timer to invalid state
	void Zero() { m_dwValue = 0; }

private:

	/// Timer value
	DWORD			m_dwTimeout;

	/// Current timeout value
	DWORD			m_dwValue;
};

#endif // !defined(AFX_TIMEOUT_H__30DFF472_456B_4ED9_B41E_DD9AC0C21EBE__INCLUDED_)
