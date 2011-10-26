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
// Base64.h: interface for the CBase64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE64_H__ACCCA643_84BA_4E20_AD79_32ED4D40ED8A__INCLUDED_)
#define AFX_BASE64_H__ACCCA643_84BA_4E20_AD79_32ED4D40ED8A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CBASE64_LINELEN		76

//==================================================================
// CBase64
//
/// For standard base-64 encoding and decoding 
/**
	All functions of this class are static.  It is therefore, not 
	necessary to create an instance of this class.
*/
//==================================================================
class CBase64  
{

public:

	/// Default Constructor
	CBase64();

	/// Destructor
	virtual ~CBase64();


	//==============================================================
	// Encode()
	//==============================================================
	/// Encode data as base-64 string with CRLF
	/**
		\param [in] dst		- Receives encoded string
		\param [in] dstsize	- Size of dst
		\param [in] src		- Data to be encoded
		\param [in] srcsize	- Size of src

		\return Returns number of bytes encoded
 
		\warning For multiple calls, size must be divisible by four
	*/
	static DWORD Encode( LPSTR dst, DWORD dstsize, LPBYTE src, DWORD srcsize );

	//==============================================================
	// Decode()
	//==============================================================
	/// Decode base-64 string
	/**
		\param [in] src				- Data to be decoded
		\param [in] srcsize			- Size of src
		\param [in] dst				- Receives decoded data
		\param [in] dstsize			- Size of dst
		\param [out] done			- Set to non-zero when decoding is done

		\return Returns number of bytes decoded
	*/
	static BOOL Decode( LPCTSTR src, LPDWORD srcsize, LPBYTE dst, LPDWORD dstsize, LPDWORD done );


	//==============================================================
	// GetEncodedSize()
	//==============================================================
	/// Calculates size of encoded string
	/**
		\param [in] size	- Size of unencoded data

		\return Returns calculated size of encoded string

		\warning For multiple calls, must be divisible by four
	*/
	static DWORD GetEncodedSize( DWORD size ) 
	{	DWORD total = ( size * 8 ) / 6;
		switch ( size % 3 )
		{	case 1 : total++; case 2 : total++; }
		total += ( total / CBASE64_LINELEN ) * 2;
		return total;
	}

	//==============================================================
	// GetDecodedSize()
	//==============================================================
	/// Calculates size of decoded string
	/**
		\param [in] size	- Size of encoded data

		\return Returns calculated size of unencoded string

		\warning For multiple calls, must be divisible by four
	*/
	static DWORD GetDecodedSize( DWORD size ) 
	{	if ( ( size & 0x03 ) != 0 ) return ( ( size * 6 ) / 8 ) + 1;
		return  ( size * 6 ) / 8; }

	/// Base-64 encoding lookup table
	static BYTE m_b64encode[];

	/// Base-64 decode lookup table
	static BYTE m_b64decode[];

};

#endif // !defined(AFX_BASE64_H__ACCCA643_84BA_4E20_AD79_32ED4D40ED8A__INCLUDED_)
