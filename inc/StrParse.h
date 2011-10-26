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
#pragma once


//==================================================================
// CStrParse
//
/// Provides string parsing functions
/**
	Provides basic string parsing functions.	
*/
//==================================================================
class CStrParse
{

public:

	/// Constructor
	CStrParse(void);

	/// Destructor
	virtual ~CStrParse(void);

public:


	/// Searches a string for a number character
	static LONG FindNumber( LPCTSTR pBuf, LONG i, LONG lMax )
	{	while ( pBuf[ i ] && i < lMax 
					&& ( pBuf[ i ] < '0' || pBuf[ i ] > '9' )
					&& pBuf[ i ] != '.' && pBuf[ i ] != '-'
					&& pBuf[ i ] != '+' ) i++;
		return i;
	}

	/// Searches a string for a number character
	static LONG WFindNumber( LPCWSTR pBuf, LONG i, LONG lMax )
	{	while ( pBuf[ i ] && i < lMax 
					&& ( pBuf[ i ] < '0' || pBuf[ i ] > '9' )
					&& pBuf[ i ] != '.' && pBuf[ i ] != '-'
					&& pBuf[ i ] != '+' ) i++;
		return i;
	}

public:

	/// Returns non-zero if line break is found
	static UINT IsLineBreak( LPCTSTR pBuf, UINT *puSize = NULL )
	{	if ( pBuf[ 0 ] == '\r' && pBuf[ 1 ] == '\n' )
		{	if ( puSize ) *puSize = 2; return 2; }
		if ( pBuf[ 1 ] == '\n' )
		{	if ( puSize ) *puSize = 1; return 1; }
		return 0;
	}

	/// Returns non-zero if ch is a text character
	static BOOL IsTextChar( const char ch )
	{	return ch && ch >= ' ' && ch <= '~'; }

	/// Returns non-zero if ch is a white space / non-text character 
	static BOOL IsWhiteSpace( const char ch )
	{	return ch && ( ch <= ' ' || ch > '~' ); }

	/// Returns the index of the first character that is <b>not</b> white space.
	static DWORD SkipWhiteSpace( LPCTSTR pBuf, UINT i )
	{	RULIB_TRY { while ( IsWhiteSpace( pBuf[ i ] ) ) i++; } RULIB_CATCH_ALL {} return i; }

	/// Returns the index of the first character that <b>is</b> white space
	static UINT SkipNonWhiteSpace( LPCTSTR pBuf, UINT i )
	{	RULIB_TRY { while ( pBuf[ i ] && !IsWhiteSpace( pBuf[ i ] ) ) i++; return i; } RULIB_CATCH_ALL {} return i; }

	/// Returns the index of the <b>next</b> sequence of non-whitespace characters
	static UINT NextToken( LPCTSTR pBuf, UINT i )
	{	RULIB_TRY { return SkipWhiteSpace( pBuf, SkipNonWhiteSpace( pBuf, i ) ); } RULIB_CATCH_ALL {} return i; }

	/// Returns the index of the next line after CRLF or LF
	static UINT NextLine( LPCTSTR pBuf, UINT i )
	{	RULIB_TRY 
		{	while ( pBuf[ i ] && !IsLineBreak( &pBuf[ i ] ) ) i++;
			UINT uSkip; if ( IsLineBreak( &pBuf[ i ], &uSkip ) ) i += uSkip; 			
		} RULIB_CATCH_ALL {}
		return i;
	}

	/// Returns non-zero if the specified character is found in the buffer
	static BOOL FindChar( LPCTSTR pBuf, char ch )
	{	RULIB_TRY 
		{ 	UINT i = 0; 
			while ( pBuf[ i ] && pBuf[ i ] != ch ) i++; 
			return ( ch && pBuf[ i ] == ch );
		} RULIB_CATCH_ALL { return FALSE; }
		return FALSE;
	}

	/// Returns non-zero if the specified character is found in the buffer
	static BOOL FindChar( LPCTSTR pBuf, char ch, UINT uMax )
	{	RULIB_TRY 
		{ 	UINT i = 0; 
			while ( i < uMax && pBuf[ i ] != ch ) i++; 
			return ( ch && pBuf[ i ] == ch );			
		} RULIB_CATCH_ALL { return FALSE; }
		return FALSE;
	}

	/// Skips occurences of characters in pChars in pBuf.
	/**
		\return Returns the offset of the first character not in pChars or end of string
	*/
	static UINT Skip( LPCTSTR pBuf, LPCTSTR pChars )
	{	DWORD i = 0;
		RULIB_TRY
		{	while ( FindChar( pChars, pBuf[ i ] ) ) i++; 
		} RULIB_CATCH_ALL { return 0; }
		return i;
	}

	/// Skips occurences of characters in pChars in pBuf.
	/**
		\return Returns the offset of the first character in pChars or end of string
	*/
	static UINT FindChars( LPCTSTR pBuf, LPCTSTR pChars )
	{	DWORD i = 0;
		RULIB_TRY
		{	while ( pBuf[ i ] && !FindChar( pChars, pBuf[ i ] ) ) i++;
		} RULIB_CATCH_ALL { return 0; }
		return i;
	}

};
