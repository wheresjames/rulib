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
// Var.h: interface for the CVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VAR_H__DDB2B0F5_6A43_4A7B_BB12_8DD8082246C3__INCLUDED_)
#define AFX_VAR_H__DDB2B0F5_6A43_4A7B_BB12_8DD8082246C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VAR_VOID	0
#define VAR_STR		1
#define VAR_DWORD	2
#define VAR_DOUBLE	3
#define VAR_BINARY	4

/// Holds information on a variable
/**
	\see CVar
*/
typedef struct tagVAR : LLISTINFO
{
	/// Variable name
	char		name[ 256 ];

	/// Variable type
	DWORD		type;

	/// Size of variable data
	DWORD		size;

	/// Pointer to variable data
	LPVOID		val;

} VAR, *LPVAR; // end typedef struct


//==================================================================
// CVar
//
/// This class manages a list of variables
/**
	This class manages a list of variables.  This class is obsolete,
	new code should use CRKey or CReg instead.
*/
//==================================================================
class CVar : public CLList
{
public:

	//==============================================================
	// Write()
	//==============================================================
	/// Generic buffer writing function
	/**
		\param [in] dst		-	Destination buffer
		\param [in] ptr		-	Starting offset in buffer
		\param [in] max		-	Size of buffer in dst
		\param [in] src		-	Data to copy
		\param [in] size	-	Size of data in src

		I have a few functions like this floating around.  Need to
		consolidate my string processing functions one day.
		
		\return ptr + number of bytes written to dst
	
		\see 
	*/
	DWORD Write( LPVOID dst, DWORD ptr, DWORD max, LPVOID src, DWORD size );

	//==============================================================
	// Replace()
	//==============================================================
	/// Used for replacing tags in a string
	/**
		\param [out] out		-	Output buffer
		\param [in] op			-	Start of output buffer
		\param [in] in			-	Input buffer
		\param [in] dwin		-	size of input buffer
		\param [in] pBegin		-	Pointer to tag start string
		\param [in] pEnd		-	Pointer to tag end string
		\param [in] pBreak		-	Break strings
		\param [in] params		-	Parameters
		\param [in] pdwBreak	-	Break offset
		\param [in] sep			-	Parameter separator
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Replace(CPipe *out, LPDWORD op, LPCTSTR in, DWORD dwin, LPCTSTR pBegin, LPCTSTR pEnd, LPSTR pBreak, CVar *params, LPDWORD pdwBreak, char sep = '&' );
	
	//==============================================================
	// Replace()
	//==============================================================
	/// Used for replacing tags in a string
	/**
		\param [out] out		-	Output buffer
		\param [in] op			-	Start of output buffer
		\param [in] dwout		-	Size of buffer in out
		\param [in] in			-	Input buffer
		\param [in] dwin		-	size of input buffer
		\param [in] pBegin		-	Pointer to tag start string
		\param [in] pEnd		-	Pointer to tag end string
		\param [in] pBreak		-	Break strings
		\param [in] params		-	Parameters
		\param [in] pdwBreak	-	Break offset
		\param [in] sep			-	Parameter separator
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Replace( LPSTR out, LPDWORD op, DWORD dwout, LPCTSTR in, DWORD dwin, LPCTSTR pBegin, LPCTSTR pEnd, LPSTR pBreak, CVar *params, LPDWORD pdwBreak, char sep = '&' );

	//==============================================================
	// ReadInline()
	//==============================================================
	/// Reads inline parameters such as that used in URL's
	/**
		\param [in] buf				-	Input buffer
		\param [in] size			-	Number of bytes in buf
		\param [in] sep				-	Data separator
		\param [in] bDeCanonicalize -	Set to non-zero to decode
										escape sequences.

		Example strings: 
			
			  search=computer

			  search=computer&max_price=1000

			  location=Jacksonville%20Florida&name=Bob%20Smith&age=27
		
		\return 
	
		\see 
	*/
	BOOL ReadInline( LPBYTE buf, DWORD size, char sep = '&', BOOL bDeCanonicalize = TRUE );

	//==============================================================
	// ReadMIME()
	//==============================================================
	/// Reads MIME encoded strings or HTML headers
	/**
		\param [in] buf		-	MIME encoded data
		\param [in] size	-	Number of bytes in buf

		Example:

		Cache-Control: private
		Content-Type: text/html
		Server: GWS/2.1
		Transfer-Encoding: chunked
	  
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadMIME( LPBYTE buf, DWORD size );
	
	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Releases variable element data
	/**
		\param [in] node	-	Pointer to VAR structure
	*/
	virtual void DeleteObject( void *node );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of a VAR structure
	virtual DWORD GetObjSize() { return sizeof( VAR ); }

	//==============================================================
	// RemoveVar()
	//==============================================================
	/// Removes the named variable from the list
	/**
		\param [in] pVar	-	Variable name
		
		\return Non-zero if removed
	
		\see 
	*/
	BOOL RemoveVar( LPCTSTR pVar );

	//==============================================================
	// FindVar()
	//==============================================================
	///	Finds a variable in the list by name
	/**
		\param [in] pVar -	Variable name
		
		\return Pointer to variable info or NULL if not found
	
		\see 
	*/
	LPVAR FindVar( LPCTSTR pVar );

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds a variable to the list
	/**
		\param [in] pVar	-	Variable name
		\param [in] dwType	-	Variable type
		\param [in] pVal	-	Variable value
		\param [in] dwSize	-	Number of bytes in pVal
		
		\return Pointer to new variable information
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, DWORD dwType, LPVOID pVal, DWORD dwSize );

	/// Constructor
	CVar();

	/// Destructor
	virtual ~CVar();

	//==============================================================
	// GetStr()
	//==============================================================
	/// Returns string value of variable
	/**
		\param [in] pVar	-	Variable name
		\param [in] pDef	-	Default value, returned if not found
		
		\return Returns string value or pDef if not found
	
		\see 
	*/
	LPCTSTR GetStr( LPCTSTR pVar, LPCTSTR pDef = "" )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return pDef;
		if ( pv->type != VAR_STR ) return pDef;
		return (LPCTSTR)pv->val;
	}

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds string variable to the list
	/**
		\param [in] pVar	-	Variable name
		\param [in] pVal	-	String value of variable
		
		\return Pointer to variable information or NULL if error
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, LPCTSTR pVal )
	{	return AddVar( pVar, VAR_STR, (LPVOID)pVal, (DWORD)strlen( pVal ) ); }

	//==============================================================
	// GetVar()
	//==============================================================
	/// Reads variable string value into a buffer
	/**
		\param [in] pVar	-	Variable name
		\param [out] pVal	-	Receives the string value of the variable
		\param [in] size	-	Size of the buffer in pVal
		
		\return Non-zero if success, otherwise zero
	
		\see 
	*/
	BOOL GetVar( LPCTSTR pVar, LPSTR pVal, DWORD size )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return FALSE;
		if ( pv->type == VAR_DWORD )
		{	wsprintf( pVal, "%lu", pv->val );
			return TRUE;
		} // end if
		if ( pv->type != VAR_STR && pv->type != VAR_VOID ) return FALSE;
		if ( pv->size > size ) return FALSE;
		strcpy( pVal, (LPCTSTR)pv->val );
		return TRUE;
	}

	//==============================================================
	// GetDword()
	//==============================================================
	/// Returns DWORD value of variable
	/**
		\param [in] pVar	-	Variable name
		\param [in] dwDef	-	Default value, returned if not found
		
		\return DWORD value of variable or dwDef if not found
	
		\see 
	*/
	DWORD GetDword( LPCTSTR pVar, DWORD dwDef = 0 )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return dwDef;
		if ( pv->type == VAR_STR ) return strtoul( (char*)pv->val, NULL, 10 );
		else if ( pv->type != VAR_DWORD ) return dwDef;
		return (DWORD)pv->val;
	}

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds a DWORD variable to the list
	/**
		\param [in] pVar	-	Name of variable
		\param [in] dwVal	-	Initial value of variable
		
		\return Pointer to variable info or NULL if error.
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, DWORD dwVal )
	{	return AddVar( pVar, VAR_DWORD, (LPVOID)dwVal, 0 ); }

	//==============================================================
	// GetVar()
	//==============================================================
	/// Copies the DWORD value of a variable
	/**
		\param [in] pVar		-	Name of variable
		\param [in] pdwVal		-	Receives the DWORD value of the variable
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetVar( LPCTSTR pVar, LPDWORD pdwVal )
	{	if ( pdwVal == NULL ) return FALSE;
		LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return FALSE;
		if ( pv->type == VAR_STR ) *pdwVal = strtoul( (char*)pv->val, NULL, 10 );
		else if ( pv->type == VAR_DWORD || pv->type == VAR_VOID ) 
			*pdwVal = (DWORD)pv->val;
		else return FALSE;
		return TRUE;
	}

	//==============================================================
	// GetLong()
	//==============================================================
	/// Returns the long value of a variable
	/**
		\param [in] pVar	-	Name of variable
		\param [in] lDef	-	Default long value, returned if not found
		
		\return long value of variable or lDef if not found.
	
		\see 
	*/
	LONG GetLong( LPCTSTR pVar, LONG lDef = 0 )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return lDef;
		if ( pv->type == VAR_STR ) return strtoul( (char*)pv->val, NULL, 10 );
		else if ( pv->type != VAR_DWORD ) return lDef;
		return (LONG)pv->val;
	}

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds a long variable to the list
	/**
		\param [in] pVar	-	Name of variable
		\param [in] lVal	-	Initial long value
		
		\return Pointer to variable information or NULL if error
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, LONG lVal )
	{	return AddVar( pVar, VAR_DWORD, (LPVOID)lVal, 0 ); }

	//==============================================================
	// GetVar()
	//==============================================================
	/// Copies the long value of a variable
	/**
		\param [in] pVar	-	Name of variable
		\param [in] plVal	-	Receives the long value of the variable
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetVar( LPCTSTR pVar, LPLONG plVal )
	{	if ( plVal == NULL ) return FALSE;
		LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return FALSE;
		if ( pv->type == VAR_STR ) *plVal = strtoul( (char*)pv->val, NULL, 10 );
		else if ( pv->type == VAR_DWORD || pv->type == VAR_VOID ) 
			*plVal = (LONG)pv->val;
		else return FALSE;
		return TRUE;
	}

	//==============================================================
	// GetDouble()
	//==============================================================
	/// Returns the double value of a variable
	/**
		\param [in] pVar	-	Name of variable
		\param [in] dDef	-	Default double value for variable
		
		\return double value of variable or dDef if not found
	
		\see 
	*/
	double GetDouble( LPCTSTR pVar, double dDef = 0 )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return dDef;
		if ( pv->type == VAR_STR ) return strtod( (char*)pv->val, NULL );
		else if ( pv->type != VAR_DOUBLE ) return dDef;
		return *(double*)pv->val;
	}

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds a double variable to the list
	/**
		\param [in] pVar	-	Name of variable to add
		\param [in] dVal	-	Initial value of variable
		
		\return	Pointer to variable information or NULL if error
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, double dVal )
	{	return AddVar( pVar, VAR_DOUBLE, &dVal, sizeof( dVal ) ); }

	//==============================================================
	// GetVar()
	//==============================================================
	/// Copies the double value of a variable
	/**
		\param [in] pVar	-	Variable name
		\param [in] pdVal	-	Receives the double value of variable
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetVar( LPCTSTR pVar, double *pdVal )
	{	if ( pdVal == NULL ) return FALSE;
		LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return FALSE;
		if ( pv->type != VAR_DOUBLE && pv->type != VAR_VOID ) return FALSE;
		if ( pv->size != sizeof( double ) ) *pdVal = 0;
		else memcpy( pdVal, pv->val, sizeof( double ) );
		return TRUE;
	}

	//==============================================================
	// AddVar()
	//==============================================================
	/// Adds a binary variable to the list
	/**
		\param [in] pVar	-	Name of variable
		\param [in] pVal	-	Pointer to variable data
		\param [in] dwSize	-	Number of bytes in pVal
		
		\return Pointer to variable structure if success otherwise NULL
	
		\see 
	*/
	LPVAR AddVar( LPCTSTR pVar, LPVOID pVal, DWORD dwSize )
	{	return AddVar( pVar, VAR_BINARY, pVal, dwSize ); }

	//==============================================================
	// GetVar()
	//==============================================================
	/// Copies binary data from variable
	/**
		\param [in] pVar	-	Variable name
		\param [in] pVal	-	Receives the binary data
		\param [in] size	-	Size of buffer in pVal
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetVar( LPCTSTR pVar, LPVOID pVal, DWORD size )
	{	LPVAR pv = FindVar( pVar );
		if ( pv == NULL ) return FALSE;
		if ( pv->type != VAR_BINARY && pv->type != VAR_VOID ) return FALSE;
		if ( pv->size > size ) return FALSE;
		memcpy( pVal, pv->val, pv->size );
		return TRUE;
	}


};

#endif // !defined(AFX_VAR_H__DDB2B0F5_6A43_4A7B_BB12_8DD8082246C3__INCLUDED_)
