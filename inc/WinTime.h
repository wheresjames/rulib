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
//////////////////////////////////////////////////////////////////////
// WinTime.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTIME_H__2118350D_8C12_4FFB_8344_2A215E3D14B6__INCLUDED_)
#define AFX_WINTIME_H__2118350D_8C12_4FFB_8344_2A215E3D14B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CWinTime
//
/// Provides time manipulation functions
/**
	Provides time manipulation functions.  I'm not especially proud
	of the time zone handling, though it works properly.  However,
	the functions to convert to and from time strings have been
	priceless.  Not to mention conversion between UNIX, DOS, Net, 
	and File times.

	Thanks to Joe Woodbury for his article
	http://www.codeproject.com/datetime/timezones.asp

	I am taking a few things from his class to add time zone support
	to this one.  I would prefer to add his class in it's entirety, 
	but I'm not using MFC in this library.

	Joe Woodbury's contributions are marked with 'jw'.
*/
//==================================================================
class CWinTime  
{

public:

	//==============================================================
	// AddBias()
	//==============================================================
	/// Adds the time zone bias
	/**
		\param [in] time	-	Time structure
		\param [in] bias	-	Time zone bias

		\note <b>jw</b>
	*/
	void AddBias( SYSTEMTIME& time, long bias );

	//==============================================================
	// GetNthDayOfWeek()
	//==============================================================
	/// Get day to nth "day of week" of month (i.e first Sunday in October)
	/**
		\param [in] year			-	Year
		\param [in] month			-	Month
		\param [in] day				-	Day
		\param [in] dayOfWeek		-	Day of the week
		\param [in] nthWeek			-	Week of the month
		\param [in] nthDayOfWeek	-	Day of the week
		
		\note <b>jw</b>

		\return What day of the week this falls on
	
		\see 
	*/
	WORD GetNthDayOfWeek( WORD year, WORD month, WORD day, WORD dayOfWeek, WORD nthWeek, WORD nthDayOfWeek );

	//==============================================================
	// GetNextDayOfWeek()
	//==============================================================
	/// Get day for nth "day of week" on or after a day (i.e first Sunday after October 15)
	/**
		\param [in] year			-	Year
		\param [in] month			-	Month
		\param [in] day				-	Day
		\param [in] dayOfWeek		-	Day of the week
		\param [in] dayOnOrAfter	-	Day On or after
		\param [in] nthWeek			-	Week of the month
		\param [in] nthDayOfWeek	-	Day of the week
		
		\note <b>jw</b>

		\return Returns the next day of the week
	
		\see 
	*/
	WORD GetNextDayOfWeek( WORD year, WORD month, WORD day, WORD dayOfWeek, WORD dayOnOrAfter, WORD nthWeek, WORD nthDayOfWeek );

	//==============================================================
	// IsDaylightTime()
	//==============================================================
	/// Returns non-zero if the specified time falls in daylight savings time
	/**
		\param [in] time	-	Time structure
		\param [in] tz		-	Time zone information
		
		\note <b>jw</b>

		\return Non-zero if success
	
		\see 
	*/
	bool IsDaylightTime(const SYSTEMTIME &time, const TIME_ZONE_INFORMATION &tz );

	//==============================================================
	// GetDaysInMonth()
	//==============================================================
	/// Returns the number of days in the specified month
	/**
		\param [in] month	-	Month to test
		
		\return Days in specified month
	
		\see 
	*/
	static BYTE GetDaysInMonth( WORD month )			
	{	if ( month == 2 ) return 28; return ( month == 4 || month == 6 || month == 9 || month == 11 ) ? 30 : 31; }

	//==============================================================
	// GetDaysInMonth()
	//==============================================================
	/// Gets the number of days in month correcting for leap year
	/**
		\param [in] month	-	Month to test
		\param [in] year	-	Year to test
		
		\return Non-zero
	
		\see 
	*/
	static BYTE GetDaysInMonth( WORD month, WORD year )	
	{	if ( month == 2 ) return !( year & 3 ) ? 29 : 28; return GetDaysInMonth( month ); }

public:

	//==============================================================
	// ToLocal()
	//==============================================================
	/// Converts time to local time
	/**
		\return Non-zero if converted
	*/
	BOOL ToLocal();

	//==============================================================
	// ToGmt()
	//==============================================================
	/// Converts time to GMT
	/**		
		\return Non-zero if converted
	*/
	BOOL ToGmt();

	//==============================================================
	// SetNetTime()
	//==============================================================
	/// Sets time using Internet time value
	/**
		\param [in] dwTime	-	Internet time value
		\param [in] lBias	-	Timezone bias

		Seconds since 1900
		
		\return Non-zero if success
	
		\see GetNetTime()
	*/
	BOOL SetNetTime( DWORD dwTime, long lBias = -1 );

	//==============================================================
	// GetNetTime()
	//==============================================================
	/// Returns the Internet time value
	/**
		Seconds since 1900

		\see SetNetTime()
	*/
	DWORD GetNetTime();

	//==============================================================
	// SetDosTime()
	//==============================================================
	/// Sets DOS time value
	/**
		\param [in] dwTime	-	DOS time value
		\param [in] lBias	-	time bias
		
		Seconds since 1980

		\return GetDosTime()
	
		\see 
	*/
	BOOL SetDosTime( DWORD dwTime, long lBias = -1 );

	//==============================================================
	// GetDosTime()
	//==============================================================
	/// Returns DOS time value
	/**
		Seconds since 1980

		\see SetDosTime()
	*/
	DWORD GetDosTime();

	//==============================================================
	// SetUnixTime()
	//==============================================================
	/// Sets UNIX time value
	/**
		\param [in] dwTime	-	UNIX time value
		\param [in] lBias	-	Timezone bias

		Seconds since 1970
		
		\return Non-zero if success
	
		\see GetUnixTime()
	*/
	BOOL SetUnixTime( DWORD dwTime, long lBias = -1 );

	//==============================================================
	// GetUnixTime()
	//==============================================================
	/// Returns UNIX time value
	/**
		Seconds since 1970
		
		\return UNIX time value
	
		\see SetUnixTime()
	*/
	DWORD GetUnixTime();

	//==============================================================
	// SetFileTime()
	//==============================================================
	/// Sets file time value
	/**
		\param [in] pFt		-	File time value
		
		Seconds since 1601

		\return Non-zero if success
	
		\see 
	*/
	BOOL SetFileTime( LPFILETIME pFt );

	//==============================================================
	// GetFileTime()
	//==============================================================
	/// Returns file time value 
	/**
		\param [out] pFt		-	Receives file time
		
		Seconds since 1601

		\return Non-zero if success
	
		\see 
	*/
	BOOL GetFileTime( LPFILETIME pFt );

	//==============================================================
	// strtonum()
	//==============================================================
	/// Converts a number string to a number
	/**
		\param [in] str		-	Number string
		\param [out] pNum	-	Receives converted number
		\param [in] len		-	Number of bytes in str
		\param [in] max		-	Maximum digits to convert
		
		\return Offset of end of number
	
		\see 
	*/
	static DWORD strtonum( LPCTSTR str, LPDWORD pNum, DWORD len, DWORD max = 0 );

	//==============================================================
	// strtosnum()
	//==============================================================
	/// Converts a signed number string to a number
	/**
		\param [in] str		-	Number string
		\param [out] pNum	-	Receives converted number
		\param [in] len		-	Number of bytes in str
		\param [in] max		-	Maximum digits to convert
		
		\return Offset of end of number
	
		\see 
	*/
	static DWORD strtosnum( LPCTSTR str, LPLONG pNum, DWORD len, DWORD max = 0 );

	//==============================================================
	// IntegrateSeconds()
	//==============================================================
	/// Combines days, hours, minutes, and seconds into seconds
	/**
		\param [in] days	-	Days
		\param [in] hours	-	Hours
		\param [in] mins	-	Minutes
		\param [in] secs	-	Seconds
		
		\return Total number of seconds
	
		\see 
	*/
	static DWORD IntegrateSeconds( DWORD days, DWORD hours, DWORD mins, DWORD secs );

	//==============================================================
	// DivideSeconds()
	//==============================================================
	/// Breaks seconds value into days, hours, minutes, and seconds
	/**
		\param [in] totalseconds	-	Total number of seconds
		\param [out] days			-	Receives number of days			
		\param [out] hours			-	Receives number of hours
		\param [out] mins			-	Receives number of minutes
		\param [out] secs			-	Receives number of seconds
		
		If any parameters are NULL, they are not considered in the
		calculation.

		For instance <b>86670</b> seconds is <b>1 day</b>, <b>4 hours</b>, 
		and <b>30 minutes</b>.

		Buf if you call this function with <b>hours</b> set to <b>NULL</b>.  

		The function will return <b>1 day</b>, <b>270 minutes</b>

		\return Value in totalseconds
	
		\see 
	*/
	static DWORD DivideSeconds( DWORD totalseconds, LPDWORD days, LPDWORD hours, LPDWORD mins, LPDWORD secs );

	//==============================================================
	// DrawCalender()
	//==============================================================
	/// Draws a calender icon with the set time highligted
	/**
		\param [in] hDC		-	Device context handle
		\param [in] pRect	-	Position in hDC for drawing calendar
		\param [in] pst		-	Pointer to time structure
		\param [in] rgb		-	Calendar foreground color
		\param [in] bck		-	Calendar background color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawCalender( HDC hDC, LPRECT pRect, LPSYSTEMTIME pst, COLORREF rgb, COLORREF bck );

	//==============================================================
	// DrawHand()
	//==============================================================
	/// Draws hand for clock
	/**
		\param [in] hDC		-	Handle to device context
		\param [in] col		-	Clock hand color
		\param [in] ol		-	Clock hand outline color
		\param [in] fc		-	Foreground color
		\param [in] bc		-	Background color
		\param [in] width	-	Hand width
		\param [in] mwidth	-	Maximum width
		\param [in] scx		-	Hand horizontal offset
		\param [in] scy		-	Hand vertical offset
		\param [in] ox		-	Clock horizontal offset
		\param [in] oy		-	Clock vertical offset
		\param [in] angle	-	Clock hand angle
		\param [in] len		-	Hand length
		\param [in] gap		-	Center pin gap
	*/
	static void DrawHand(	HDC hDC, COLORREF col, COLORREF ol, 
							COLORREF fc, COLORREF bc, long width, long mwidth, 
							long scx, long scy, long ox, long oy, double angle, 
							double len, DWORD gap );

	//==============================================================
	// DrawHands()
	//==============================================================
	/// Draws clock hands
	/**
		\param [in] hDC		-	Device context handle
		\param [in] pRect	-	Drawing position within the hDC
		\param [in] pst		-	Time
		\param [in] rgb		-	Hand colors
		\param [in] shadow	-	Hand shadow color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawHands( HDC hDC, LPRECT pRect, LPSYSTEMTIME pst, COLORREF rgb, long shadow );

	//==============================================================
	// GetClockMetrics()
	//==============================================================
	/// Calculates clock drawing metrics
	/**
		\param [in] pRect	-	Target rectangle
		\param [in] cx		-	Horizontal center offset
		\param [in] cy		-	Vertical center offset
		\param [in] rx		-	Horizontal radius
		\param [in] ry		-	Vertical radius
		\param [in] bSquare	-	Non-zero for square clock
	*/
	static void GetClockMetrics( LPRECT pRect, long *cx, long *cy, long *rx, long *ry, BOOL bSquare );

	//==============================================================
	// DrawTicks()
	//==============================================================
	/// Draws clock tick marks
	/**
		\param [in] hDC		-	Device context handle
		\param [in] pRect	-	Clock position
		\param [in] rgb		-	Tick mark color
		\param [in] shadow	-	Tick shadow color
		\param [in] bHour	-	Hour ticks
		\param [in] bMinute	-	Minute ticks
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawTicks( HDC hDC, LPRECT pRect, COLORREF rgb, long shadow, BOOL bHour, BOOL bMinute );

	//==============================================================
	// DrawClock()
	//==============================================================
	/// Draws the clock
	/**
		\param [in] hDC		-	Target device context
		\param [in] pRect	-	Clock position
		\param [in] pst		-	Time
		\param [in] shadow	-	Shadow offset
		\param [in] rgb		-	Foreground color
		\param [in] bck		-	Background color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawClock( HDC hDC, LPRECT pRect, LPSYSTEMTIME pst, long shadow, COLORREF rgb, COLORREF bck );

	//==============================================================
	// CalcTzBias()
	//==============================================================
	/// Returns the timezone bias of the current machine
	static long CalcTzBias();


	//==============================================================
	// GetString()
	//==============================================================
	/// Returns a formated time string
	/**
		\param [out] str	-	Receives the formated time string
		\param [in] tmpl	-	Template string

		Formats a time string based on the specified template.

		-	\%h = hour 12 hour fixed 2 digits
		-	\%H = hour 12 hour
		-	\%g = hour 24 hour fixed 2 digits
		-	\%G = hour 24 hour
		-	\%m = minute fixed 2 digits
		-	\%M = minute
		-	\%s = second fixed 2 digits
		-	\%S = second
		-	\%l = milli seconds fixed 3 digits
		-	\%L = milli seconds
		-	\%a = am/pm
		-	\%A = AM/PM
		-	\%c = Month [01-12] fixed 2 digits
		-	\%C = Month [1-12]
		-	\%d = Day [01-31] fixed 2 digits
		-	\%D = Day [1-31]
		-	\%i = Day of the week [0-6]
		-	\%I = Day of the week [1-7]
		-	\%y = 2 digit year
		-	\%Y = 4 digit year
		-	\%a = am/pm
		-	\%A = AM/PM
		-	\%w = Abbreviated day of week [Sun,Mon,Tue,Wed,Thu,Fri,Sat]
		-	\%W = Day of week [Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday]
		-	\%b = Abbreviated Month [Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec]
		-	\%B = Month [January,February,March,April,May,June,July,August,September,October,November,December]
		-	\%Za = Lower case time zone abbreviation [est,gmt,pst...]
		-	\%ZA = Upper case time zone abbreviation [EST,GMT,PST...]
		-	\%Zs = Time zone sign +/-
		-	\%Zh = Hours of offset in time zone fixed two digits
		-	\%ZH = Hours of offset in time zone
		-	\%Zh = Minutes of offset in time zone fixed two digits
		-	\%ZH = Minutes of offset in time zone
		-	\%Zz = Time zone offset in minutes with leading +/-
		-	\%ZZ = Time zone offset in seconds with leading +/-
		 
		\return Non-zero if success
	
		\see ParseString()
	*/
	BOOL GetString( LPSTR str, LPCTSTR tmpl );
    
    /// Sq
    std::string sGetString( const std::string &sTmpl )
    {   char szTemp[ CWF_STRSIZE ] = "";
        if ( !GetString( szTemp, sTmpl.c_str() ) ) return "";
        return szTemp;
    }

	//==============================================================
	// ParseString()
	//==============================================================
	/// 
	/**
		\param [out] str	-	Formated time string
		\param [in] tmpl	-	Template string

		This function decomposes an existing time string based 
		on the template string.

		-	\%h = hour 12 hour fixed 2 digits
		-	\%H = hour 12 hour
		-	\%g = hour 24 hour fixed 2 digits
		-	\%G = hour 24 hour
		-	\%m = minute fixed 2 digits
		-	\%M = minute
		-	\%s = second fixed 2 digits
		-	\%S = second
		-	\%l = milli seconds fixed 3 digits
		-	\%L = milli seconds
		-	\%a = am/pm
		-	\%A = AM/PM
		-	\%c = Month [01-12] fixed 2 digits
		-	\%C = Month [1-12]
		-	\%d = Day [01-31] fixed 2 digits
		-	\%D = Day [1-31]
		-	\%i = Day of the week [0-6]
		-	\%I = Day of the week [1-7]
		-	\%y = 2 digit year
		-	\%Y = 4 digit year
		-	\%a = am/pm
		-	\%A = AM/PM
		-	\%w = Abbreviated day of week [Sun,Mon,Tue,Wed,Thu,Fri,Sat]
		-	\%W = Day of week [Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday]
		-	\%b = Abbreviated Month [Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec]
		-	\%B = Month [January,February,March,April,May,June,July,August,September,October,November,December]
		-	\%Za = Lower case time zone abbreviation [est,gmt,pst...]
		-	\%ZA = Upper case time zone abbreviation [EST,GMT,PST...]
		-	\%Zs = Time zone sign +/-
		-	\%Zh = Hours of offset in time zone fixed two digits
		-	\%ZH = Hours of offset in time zone
		-	\%Zh = Minutes of offset in time zone fixed two digits
		-	\%ZH = Minutes of offset in time zone
		-	\%Zz = Time zone offset in minutes with leading +/-
		-	\%ZZ = Time zone offset in seconds with leading +/-
		 
		For timezones, leading +/- is optional. + is the default.
		
		\return This function returns Zero if the format does not match the template
	
		\see GetString()
	*/
	BOOL ParseString( LPCTSTR str, LPCTSTR tmpl );

    /// Sq
    BOOL sParseString( const std::string &sStr, const std::string &sTmpl )
    {   return ParseString( sStr.c_str(), sTmpl.c_str() ); }


	//==============================================================
	// GetDayName()
	//==============================================================
	/// Returns a pointer to the specified day of the week name
	/**
		\param [in] d	-	Day of the week ( 1 - 7 )
		
		\return String describing the day of the week
	
		\see 
	*/
	LPCTSTR GetDayName( long d );

	//==============================================================
	// GetAbrDayName()
	//==============================================================
	/// Returns a pointer to the abriviated day of the week name
	/**
		\param [in] d	-	Day of the week( 1 - 7 )
		
		\return String containing the abriviated day of the week
	
		\see 
	*/
	LPCTSTR GetAbrDayName( long d );

	//==============================================================
	// GetMonthName()
	//==============================================================
	/// Returns a pointer to the month name
	/**
		\param [in] m	-	Month ( 1 - 12 )
		
		\return String containing the month name
	
		\see 
	*/
	LPCTSTR GetMonthName( long m );

	//==============================================================
	// GetAbrMonthName()
	//==============================================================
	/// Returns a pointer to the abbriviated month name
	/**
		\param [in] m	-	Month ( 1 - 12 )
		
		\return String containing the abbriviated month name
	
		\see 
	*/
	LPCTSTR GetAbrMonthName( long m );

	//==============================================================
	// IsPM()
	//==============================================================
	/// Returns non-zero if the time is 'Post Meridiem' or 'after noon'.
	/**
		
		This function incorrectly returns non-zero for the time 12:00:00. 		
		Technically it is not PM until 12:00:01.  12:00:00 is not AM or 
		PM.  Same for 00:00:00.  But this is the common use.

		\return Non-zero if the time is 'Post Meridiem' or 'after noon'.
	
		\see 
	*/
	BOOL IsPM() { return ( m_st.wHour >= 12 ); } 

	//==============================================================
	// Get12Hour()
	//==============================================================
	/// Returns the 12-hour value for the hour.
	DWORD Get12Hour()
	{	DWORD hour = m_st.wHour;
		if ( hour > 12 ) hour -= 12;
		else if ( hour == 0 ) hour = 12;
		return hour; 
	}

	//==============================================================
	// NextDay()
	//==============================================================
	/// Advances the time to the same time the next day.
	void NextDay()
	{	WORD wDaysInMonth = GetDaysInMonth( m_st.wMonth, m_st.wYear );
		if ( m_st.wDay < wDaysInMonth ) { m_st.wDay++; return; }
		m_st.wDay = 1; if ( m_st.wMonth < 12 ) { m_st.wMonth++; return; }
		m_st.wMonth = 1; m_st.wYear++;
	}

	/// Default constructor
	CWinTime();

	//==============================================================
	// CWinTime()
	//==============================================================
	/// Constructs the object by copying another CWinTime object
	/**
		\param [in] wt	-	Pointer to source CWinTime object
	*/
	CWinTime( CWinTime &wt )
	{	m_lBias = wt.m_lBias;
		memcpy( &m_st, &wt.m_st, sizeof( m_st ) );
		memcpy( &m_tz, &wt.m_tz, sizeof( m_tz ) );
	}

	//==============================================================
	// CWinTime()
	//==============================================================
	/// Constructs the object with the specified time and timezone
	/**
		\param [in] pst		-	Time information structure
		\param [in] pTzi	-	Timezone information
		
		\return 
	
		\see 
	*/
	CWinTime( LPSYSTEMTIME pst, LPTIME_ZONE_INFORMATION pTzi );

	//==============================================================
	// CWinTime()
	//==============================================================
	/// Constructs the object with the specified time and timezone bias
	/**
		\param [in] pst		-	Time informatio structure
		\param [in] lBias	-	Timezone bias
		
		\return 
	
		\see 
	*/
	CWinTime( LPSYSTEMTIME pst, LONG lBias = MAXLONG );

	//==============================================================
	// CWinTime()
	//==============================================================
	/// Constructs the object with the specified time
	/**
		\param [in] dwYear			-	Four digit year
		\param [in] dwMonth			-	Month ( 1 - 12 )
		\param [in] dwDay			-	Day of the month ( 1 - 31 )
		\param [in] dwHour			-	Hour of the day ( 0 - 23 )
		\param [in] dwMinute		-	Minute of the hour ( 0 - 59 )
		\param [in] dwSecond		-	Second of the minute ( 0 - 59 )
		\param [in] dwMilliseconds	-	Millisecond value ( 0 - 999 )
		\param [in] dwDayOfWeek		-	Day of the week ( 1 - 7 )
		\param [in] lTzBias			-	Time zone bias, MAXLONG for 
										none.
	*/
	CWinTime(	DWORD dwYear, DWORD dwMonth, DWORD dwDay,
				DWORD dwHour, DWORD dwMinute, DWORD dwSecond,
				DWORD dwMilliseconds = MAXDWORD, DWORD dwDayOfWeek = MAXDWORD,
				LONG lTzBias = MAXLONG )
	{	m_lBias = 0; ZeroMemory( &m_st, sizeof( m_st ) ); ZeroMemory( &m_tz, sizeof( m_tz ) );
		SetTime( dwYear, dwMonth, dwDay, dwHour, dwMinute, dwSecond, dwMilliseconds, dwDayOfWeek, lTzBias ); }
	
	/// Destructor
	virtual ~CWinTime();

	//==============================================================
	// GetLocalTime()
	//==============================================================
	/// Retrieves the local time from the system
	void GetLocalTime() 
	{	m_lBias = MAXLONG; ::GetLocalTime( &m_st ); ::GetTimeZoneInformation( &m_tz ); }

	//==============================================================
	// GetSystemTime()
	//==============================================================
	/// Retrieves GMT time from the system
	void GetSystemTime() 
	{	m_lBias = 0; ::GetSystemTime( &m_st ); ZeroMemory( &m_tz, sizeof( m_tz ) ); }

	//==============================================================
	// SetTime()
	//==============================================================
	/// Sets the specified time and timezone
	/**
		\param [in] pst		-	Time information structure
		\param [in] pTz		-	Timezone information
	*/
	void SetTime( LPSYSTEMTIME pst, PTIME_ZONE_INFORMATION pTz )
	{	if ( pst == NULL ) ZeroMemory( &m_st, sizeof( &m_st ) );
		else memcpy( &m_st, pst, sizeof( m_st ) );
		if ( pTz != NULL ) memcpy( &m_tz, pTz, sizeof( m_tz ) );
		else ZeroMemory( &m_tz, sizeof( m_tz ) );
	} // end if

	//==============================================================
	// SetTime()
	//==============================================================
	/// Sets the specified time and timezone bias
	/**
		\param [in] pst		-	Time informatio structure
		\param [in] lTzBias	-	Timezone bias
	*/
	void SetTime( LPSYSTEMTIME pst, LONG lTzBias = 0 )
	{	if ( pst == NULL ) ZeroMemory( &m_st, sizeof( &m_st ) );
		else memcpy( &m_st, pst, sizeof( m_st ) );
		ZeroMemory( &m_tz, sizeof( m_tz ) );
		m_lBias = lTzBias;
	} // end if

	//==============================================================
	// SetTime()
	//==============================================================
	/// Sets the specified time
	/**
		\param [in] dwYear			-	Four digit year
		\param [in] dwMonth			-	Month ( 1 - 12 )
		\param [in] dwDay			-	Day of the month ( 1 - 31 )
		\param [in] dwHour			-	Hour of the day ( 0 - 23 )
		\param [in] dwMinute		-	Minute of the hour ( 0 - 59 )
		\param [in] dwSecond		-	Second of the minute ( 0 - 59 )
		\param [in] dwMilliseconds	-	Millisecond value ( 0 - 999 )
		\param [in] dwDayOfWeek		-	Day of the week ( 1 - 7 )
		\param [in] lTzBias			-	Time zone bias, MAXLONG for 
										none.
	*/
	void SetTime(	DWORD dwYear = MAXDWORD, DWORD dwMonth = MAXDWORD, DWORD dwDay = MAXDWORD,
					DWORD dwHour = MAXDWORD, DWORD dwMinute = MAXDWORD, DWORD dwSecond = MAXDWORD,
					DWORD dwMilliseconds = MAXDWORD, DWORD dwDayOfWeek = MAXDWORD,
					LONG lTzBias = MAXLONG )
	{	if ( dwYear != MAXDWORD ) m_st.wYear = (WORD)dwYear;
		if ( dwMonth != MAXDWORD ) m_st.wMonth = (WORD)dwMonth;
		if ( dwDay != MAXDWORD ) m_st.wDay = (WORD)dwDay;
		if ( dwHour != MAXDWORD ) m_st.wHour = (WORD)dwHour;
		if ( dwMinute != MAXDWORD ) m_st.wMinute = (WORD)dwMinute;
		if ( dwSecond != MAXDWORD ) m_st.wSecond = (WORD)dwSecond;
		if ( dwMilliseconds != MAXDWORD ) m_st.wMilliseconds = (WORD)dwMilliseconds;
		if ( dwDayOfWeek != MAXDWORD ) m_st.wDayOfWeek = (WORD)dwDayOfWeek;
		if ( lTzBias != MAXLONG )
		{	ZeroMemory( &m_tz, sizeof( m_tz ) ); m_lBias = lTzBias; }
	}

	//==============================================================
	// NoTimeZone()
	//==============================================================
	/// Clears the curent timezone information
	void NoTimeZone() { ZeroMemory( &m_tz, sizeof( m_tz ) ); }


	//==============================================================
	// operator LPSYSTEMTIME()
	//==============================================================
	/// Returns a pointer to a SYSTEMTIME structure
	operator LPSYSTEMTIME () { return &m_st; }

	//==============================================================
	// operator PTIME_ZONE_INFORMATION()
	//==============================================================
	/// Returns a pointer to a TIME_ZONE_INFORMATION structure
	operator PTIME_ZONE_INFORMATION () { return &m_tz; }

	//==============================================================
	// GetYear()
	//==============================================================
	/// Returns the four digit year
	DWORD GetYear() { return m_st.wYear; }

    void SetYear( DWORD dw ) { m_st.wYear = (WORD)dw; }

	//==============================================================
	// GetMonth()
	//==============================================================
	/// Returns the month ( 1 - 12 )
	DWORD GetMonth() { return m_st.wMonth; }

    void SetMonth( DWORD dw ) { m_st.wMonth = (WORD)dw; }


	//==============================================================
	// GetDay()
	//==============================================================
	/// Returns the day ( 1 - 31 )
	DWORD GetDay() { return m_st.wDay; }

    void SetDay( DWORD dw ) { m_st.wDay = (WORD)dw; }
	
	//==============================================================
	// GetDayOfWeek()
	//==============================================================
	/// Returns the day of the week ( 1 - 7 )
	DWORD GetDayOfWeek() { return m_st.wDayOfWeek; }

    void SetDayOfWeek( DWORD dw ) { m_st.wDayOfWeek = (WORD)dw; }
	
	//==============================================================
	// GetHour()
	//==============================================================
	/// Returns the hour ( 0 - 23 ).  For 12-hour value, see Get12Hour()
	DWORD GetHour() { return m_st.wHour; }
	
    void SetHour( DWORD dw ) { m_st.wHour = (WORD)dw; }

	//==============================================================
	// GetMinute()
	//==============================================================
	/// Returns the minute ( 0 - 59 )
	DWORD GetMinute() { return m_st.wMinute; }

    void SetMinute( DWORD dw ) { m_st.wMinute = (WORD)dw; }
	
	//==============================================================
	// GetSecond()
	//==============================================================
	/// Returns the second ( 0 - 59 )
	DWORD GetSecond() { return m_st.wSecond; }	
	
    void SetSecond( DWORD dw ) { m_st.wSecond = (WORD)dw; }

	//==============================================================
	// GetMilliSeconds()
	//==============================================================
	/// Returns the milli-second value ( 0 - 999 )
	DWORD GetMilliSeconds() { return m_st.wMilliseconds; }

    void SetMilliSeconds( DWORD dw ) { m_st.wMilliseconds = (WORD)dw; }

	//==============================================================
	// SetTzBias()
	//==============================================================
	/// Sets the current timezone bias.
	/**
		\param [in] lTzBias 

		\return The value in lTzBias
	*/
	long SetTzBias( long lTzBias ) 
	{	return ( m_lBias = lTzBias ); }

	//==============================================================
	// GetTzBias()
	//==============================================================
	/// Returns the current timezone bias
	long GetTzBias() 
	{	if ( m_lBias == MAXLONG ) return m_tz.Bias; return m_lBias; }

	//==============================================================
	// TimeZoneInfoPtr()
	//==============================================================
	/// Returns a pointer to a TIME_ZONE_INFORMATION structure
	PTIME_ZONE_INFORMATION TimeZoneInfoPtr() { return &m_tz; }

private:

	/// The current timezone bias
	LONG					m_lBias;

	/// Information about the current time zone
	TIME_ZONE_INFORMATION	m_tz;

	/// Time information
	SYSTEMTIME				m_st;
	
};

#endif // !defined(AFX_WINTIME_H__2118350D_8C12_4FFB_8344_2A215E3D14B6__INCLUDED_)
