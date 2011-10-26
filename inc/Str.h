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
// Str.h: interface for the CStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STR_H__CB5C360E_5AEB_44B7_A016_D25C716BFD1E__INCLUDED_)
#define AFX_STR_H__CB5C360E_5AEB_44B7_A016_D25C716BFD1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CStr
//
/// String class
/**
	Old class encapsulating a string.  Originally did not use TMem,
	but I updated it a little.  Your much better off using ATL7 
	or MFC string class in new code.	
*/
//==================================================================
class CStr : public TMem< char >  
{
public:

	//==============================================================
	// Set()
	//==============================================================
	/// Sets a double value into the string
	/**
		\param [in] n			-	double value
		\param [in] pTemplate	-	conversion template for sprintf
		
		\return Pointer to converted string
	
		\see 
	*/
	LPCTSTR Set( double n, LPCTSTR pTemplate = "%f0.2" );

	/// Constructor
	CStr();

	/// Destructor
	virtual ~CStr();

	//==============================================================
	// CStr()
	//==============================================================
	/// Constructs a string and initializes
	/**
		\param [in] pStr	-	Initial value for string
	*/
	CStr( LPCTSTR pStr ) { copy( pStr ); }

	//==============================================================
	// CStr()
	//==============================================================
	/// Constructs a string initialized with a DWORD value
	/**
		\param [in] dw	-	DWORD value that is converted to string
	*/
	CStr( unsigned long dw ) { *this = dw; }

	//==============================================================
	// CStr()
	//==============================================================
	/// Constructs a string and initializes to a long value
	/**
		\param [in] l	-	long value that is converted to a string
	*/
	CStr( long l ) { *this = l; }

	//==============================================================
	// CStr()
	//==============================================================
	/// Constructs a string and initializes to a double value
	/**
		\param [in] d	-	double value that is converted to string
	*/
	CStr( double d ) { *this = d; }

	//==============================================================
	// LPCTSTR()
	//==============================================================
	/// Returns a pointer to the encapsulated string
	operator LPCTSTR() { if ( ptr() == NULL ) return ""; return ptr(); }

	//==============================================================
	// str()
	//==============================================================
	/// Returns a pointer to the encapsulated string ofset
	/**
		\param [in] offset	-	Offset from start of string 
		
		\return Pointer to specified byte in string
	
		\see 
	*/
	char* str( unsigned long offset = 0 ) 
	{	if ( ptr() == NULL ) return ""; 
		if ( offset > strlen() ) offset = strlen(); return ptr( offset ); 
	}
	
	//==============================================================
	// copy()
	//==============================================================
	/// Copies NULL terminated string
	/**
		\param [in] ptr		-	Pointer to string buffer to copy
		
		\return Non-zero if success
	
		\see 
	*/
	bool copy( const char *ptr )
	{	unsigned long len = ::strlen( ptr );
		if ( !TMem< char >::copy( ptr, len + 1 ) ) return false;
		pData[ len ] = 0; return true;
	}

	//==============================================================
	// copy()
	//==============================================================
	/// Copies unterminated string
	/**
		\param [in] ptr		-	Pointer to string
		\param [in] len		-	Number of bytes to copy
		
		\return Non-zero if success
	
		\see 
	*/
	bool copy( const char *ptr, unsigned long len )
	{	if ( !TMem< char >::allocate( len + 1 ) ) return false;
		memcpy( pData, ptr, len );
		pData[ len ] = 0; return true;
	}

	//==============================================================
	// operator =()
	//==============================================================
	/// Assigns string value of unsigned long
	CStr& operator = ( unsigned long n );

	//==============================================================
	// operator =()
	//==============================================================
	/// Assigns string value of long
	CStr& operator = ( long n );

	//==============================================================
	// operator =()
	//==============================================================
	/// Assigns value of NULL terminated string
	CStr& operator = ( LPCTSTR str );

	//==============================================================
	// operator =()
	//==============================================================
	/// Assigns string value of double
	CStr& operator = ( double n );

	//==============================================================
	// operator +=()
	//==============================================================
	/// Appends string value of unsigned long
	CStr& operator += ( unsigned long n );

	//==============================================================
	// operator +=()
	//==============================================================
	/// Appends string value of long
	CStr& operator += ( long n );

	//==============================================================
	// operator +=()
	//==============================================================
	/// Appends value of NULL terminated string
	CStr& operator += ( LPCTSTR str );

	//==============================================================
	// operator +=()
	//==============================================================
	/// Appends string value of double
	CStr& operator += ( double n );

	//==============================================================
	// strlen()
	//==============================================================
	/// Returns the length of the encapsulated string
	DWORD strlen() 
	{	if ( size() == 0 || ptr() == NULL ) return 0; 
		return size() - 1; 
	}

};

#endif // !defined(AFX_STR_H__CB5C360E_5AEB_44B7_A016_D25C716BFD1E__INCLUDED_)
