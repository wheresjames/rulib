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
// RegKey.h: interface for the CRKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGKEY_H__477E05A1_7DAB_4329_8422_555016A90E0C__INCLUDED_)
#define AFX_REGKEY_H__477E05A1_7DAB_4329_8422_555016A90E0C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "LList.h"

/// Encapsulates the data for a single registry value
/**
	\see CRKey
*/
typedef struct tagREGVALUE : LLISTINFO
{
	/// Value type
	DWORD		type;

	/// Value size
	DWORD		size;

	union
	{
		/// Value data pointer
		LPVOID		data;

		/// Value data pointer
		LPSTR		pcdata;

		/// Value data pointer
		LPBYTE		pbdata;	
		
	}; // end union

} REGVALUE; // end typedef struct
typedef REGVALUE* LPREGVALUE;

//==================================================================
// CRKey
//
/// Contains a list of registry values for a single key.
/**
	This class implements a list of registry values for a single key.
	It also serves as a gereral purpose property bag for linear
	value heirarchies.
	
*/
//==================================================================
class CRKey : public CHList    
{
public:

	//==============================================================
	// AddFile()
	//==============================================================
	/// Reads the contents of a file in as a variable
	/**
		\param [in] pName		-	Value name
		\param [in] pFilename	-	Filename to read
		
		\return Pointer to new REGVALUE object if success
	
		\see 
	*/
	LPREGVALUE AddFile(LPCTSTR pName, LPCTSTR pFilename )
	{	return Add( REG_BINARY, pName, pFilename, strlen( pFilename ), TRUE ); } 
	
	//==============================================================
	// DeleteValue()
	//==============================================================
	/// Deletes the named value from the list
	/**
		\param [in] pValue	-	Value name
		
		\return Non-zero if value was deleted
	
		\see 
	*/
	BOOL DeleteValue( LPCTSTR pValue );

	//==============================================================
	// EncodeUrl()
	//==============================================================
	/// Encodes the variables as a URL GET string
	/**
		\param [out] pPipe			-	CPipe object that receives the data
		\param [in] dwEncoding		-	The type of url encoding to use
										-	0	=	None
										-	1	=	Normal URL
										-	2	=	Double URL encoding 
		\param [in] chSepNameVal	-	The separator between the name
										and value tokens.  Such as the 
										<b>=</b> in <b>name=value</b>.
		\param [in] chSepValues		-	The separator between the name
										and value pairs.  Such as the 
										<b>&</b> in 
										<b>name1=value1&name2=value2</b>.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EncodeUrl( CPipe *pPipe, DWORD dwEncoding = 1, char chSepNameVal = '=', char chSepValues = '&' );

    std::string sEncodeUrl( DWORD dwEncoding = 1, char chSepNameVal = '=', char chSepValues = '&' )
    {
        CPipe pipe;
        if ( !EncodeUrl( &pipe, dwEncoding, chSepNameVal, chSepValues ) )
            return "";
        pipe.Write( "\x0", 1 );
        return std::string( (LPCTSTR)pipe.GetBuffer(), pipe.GetBufferSize() );
    }

	//==============================================================
	// EncodeHttpHeaders()
	//==============================================================
	/// Encodes the variables as HTTP request headers
	/**
		\param [out] pPipe	-	CPipe object that receives the data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EncodeHttpHeaders( CPipe *pPipe );

	//==============================================================
	// IsSet()
	//==============================================================
	/// Non-zero if specified value exists
	/**
		\param [in] pName	-	Value name
		
		\return Non-zero if value exists
	
		\see 
	*/
	BOOL IsSet( LPCTSTR pName );

	//==============================================================
	// RestoreWindowPos()
	//==============================================================
	/// Restores a window position from a value
	/**
		\param [in] hWnd		-	Handle to window that is restored
		\param [in] pName		-	Name of value containing data
		\param [in] bSize		-	Non-zero to restore the window size
		\param [in] bPosition	-	Non-zero to restore the window position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RestoreWindowPos( HWND hWnd, LPCTSTR pName, BOOL bSize = TRUE, BOOL bPosition = TRUE );

	//==============================================================
	// SaveWindowPos()
	//==============================================================
	/// Saves the window position into a value
	/**
		\param [in] hWnd	-	Handle to window
		\param [in] pName	-	Name of the value
		
		\return Non-zero if window position is saved
	
		\see 
	*/
	BOOL SaveWindowPos( HWND hWnd, LPCTSTR pName );

	//==============================================================
	// GetValuePtr()
	//==============================================================
	/// Returns a pointer to the specifed value data
	/**
		\param [in] pName	-	Name of the value to return
		
		\return Pointer to value data
	
		\see 
	*/
	void* GetValuePtr( LPCTSTR pName );

	//==============================================================
	// GetValueSize()
	//==============================================================
	/// Returns the number of bytes in the value
	/**
		\param [in] pName	-	Value name
		
		\return Number of bytes in the value buffer
	
		\see 
	*/
	DWORD GetValueSize( LPCTSTR pName );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the specified group from the CCfgFile class
	/**
		\param [in] pCfg		    -	CCfgFile class pointer
		\param [in] pGroup		    -	Name of group to copy.
        \param [in] bMakeStrings    -   Converts each type field to 
                                        a string if non-zero.
		
		This is for converting between the two value types.

		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( CCfgFile *pCfg, LPCTSTR pGroup, BOOL bMakeStrings = FALSE );

	//==============================================================
	// Update()
	//==============================================================
	/// Adds values in specified key that are missing from this key
	/**
		\param [in] pRk		-	Key containing new values
		
		Adds only values in pRk that are missing from this key.

		\return Non-zero if success
	
		\see 
	*/
	BOOL Update( CRKey *pRk );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the values from one key
	/**
		\param [in] pRk		-	Source key
		\param [in] bMerge	-	Non-zero to merge key values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( CRKey *pRk, BOOL bMerge = FALSE );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the values from a property bag
	/**
		\param [in] pPb		-	Property bag
		\param [in] bMerge	-	Non-zero to merge key values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( CScsPropertyBag *pPb, BOOL bMerge = FALSE );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves to a property bag
	/**
		\param [in] pPb	-   Property bag
		
		\return Non-zero if success
	
		\see 
	*/
    BOOL Save( CScsPropertyBag *pPb );


	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the value of the specified Windows registry key
	/**
		\param [in] hKey		-	Key handle
		\param [in] pKey		-	Key name
		\param [in] pValue		-	Key value name
		\param [in] pData		-	New value data
		\param [in] dwSize		-	Number of bytes in pData
		\param [in] dwType		-	Value type
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetValue( HKEY hKey, LPCTSTR pKey, LPCTSTR pValue, LPVOID pData, DWORD dwSize, DWORD dwType = REG_SZ );

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value of the specified Windows registry key
	/**
		\param [in] hKey		-	Key handle
		\param [in] pKey		-	Key name
		\param [in] pValue		-	Key value name
		\param [in] pData		-	Receives value data
		\param [in] dwSize		-	Size of buffer in pData
		\param [in] pdwType		-	Receives value type
		
		\return Number of bytes copied to pData, zero if error
	
		\see 
	*/
	static DWORD GetValue( HKEY hKey, LPCTSTR pKey, LPCTSTR pValue, LPVOID pData, DWORD dwSize, LPDWORD pdwType = NULL );

	//==============================================================
	// Parse()
	//==============================================================
	/// Parses a name-value pair and adds to the list
	/**
		\param [in] buf		-	Buffer to process
		\param [in] size	-	Number of bytes in buf
		\param [in] sep		-	name-value pair separator
		
		Example:
		\code
		"name=value"
		"param1=100,param2=\"hello world\""
		\endcode

		\return Non-zero if success
	
		\see 
	*/
	BOOL Parse( LPCTSTR buf, DWORD size,  char sep = ',' );

	//==============================================================
	// Replace()
	//==============================================================
	/// Replaces escape tokens in pDst with variable values
	/**
		\param [in] pSrc	-	Source buffer
		\param [out] pDst	-	Receives processed data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Replace( LPCTSTR pSrc, LPCTSTR pDst );

	//==============================================================
	// Replace()
	//==============================================================
	/// Replaces escape tokens in pDst with variable values
	/**
		\param [out] out		-	Receives processed data
		\param [out] op			-	Receives number of bytes written to out
		\param [in] in			-	Input buffer
		\param [in] dwin		-	Number of bytes in in
		\param [in] pBegin		-	Pointer to escape token start string
		\param [in] pEnd		-	Pointer to escape token end string
		\param [out] pBreak		-	Receives break point token
		\param [out] params		-	Receives break parameters
		\param [out] pdwBreak	-	Receives break point offset
		\param [in] sep			-	Parameter separator
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Replace(CPipe *out, LPDWORD op, LPCTSTR in, DWORD dwin, LPCTSTR pBegin, LPCTSTR pEnd, LPSTR pBreak, CVar *params, LPDWORD pdwBreak, char sep = '&' );

	//==============================================================
	// Replace()
	//==============================================================
	/// 
	/**
		\param [out] out		-	Receives processed data
		\param [out] op			-	Receives number of bytes written to out
		\param [in] dwout		-	Size of buffer in out
		\param [in] in			-	Input buffer
		\param [in] dwin		-	Number of bytes in in
		\param [in] pStop		-	Pointer to escape token start string
		\param [in] pBreak		-	Pointer to escape token end string
		\param [out] pdwBreak	-	Receives break point offset
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Replace( LPSTR out, LPDWORD op, DWORD dwout, LPCTSTR in, DWORD dwin, LPCTSTR pStop = NULL, LPSTR pBreak = NULL, LPDWORD pdwBreak = NULL )
	{	return Replace( out, op, dwout, in, dwin, pStop, pBreak, this, pdwBreak ); }

	//==============================================================
	// Replace()
	//==============================================================
	/// 
	/**
		\param [out] out		-	Receives processed data
		\param [out] op			-	Receives number of bytes written to out
		\param [in] dwout		-	Size of buffer in out
		\param [in] in			-	Input buffer
		\param [in] dwin		-	Number of bytes in in
		\param [in] pStop		-	Pointer to escape token start string
		\param [in] pBreak		-	Pointer to escape token end string
		\param [out] params		-	Receives break parameters
		\param [out] pdwBreak	-	Receives break point offset
		
		\return 
	
		\see 
	*/
	BOOL Replace( LPSTR out, LPDWORD op, DWORD dwout, LPCTSTR in, DWORD dwin, LPCTSTR pStop, LPSTR pBreak, CRKey *params, LPDWORD pdwBreak );

	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to buffer
	/**	
		\param [out] dst	-	Destination buffer
		\param [in] ptr		-	Start offset in dst
		\param [in] max		-	Size of buffer in dst
		\param [in] src		-	Source buffer
		\param [in] size	-	Number of bytes in src
		
		\return	Offset of last byte in dst
	
		\see 
	*/
	DWORD Write(LPVOID dst, DWORD ptr, DWORD max, const void *src, DWORD size = 0);

	//==============================================================
	// DeleteSubKeys()
	//==============================================================
	/// Deletes the sub keys of specified key in Windows Registry
	/**
		\param [in] hRoot	-	Key root
		\param [in] pKey	-	Sub key name
		
		\return Non-zero if key is deleted
	
		\see 
	*/
	static BOOL DeleteSubKeys( HKEY hRoot, LPCTSTR pKey );	

	//==============================================================
	// DeleteKey()
	//==============================================================
	/// Deletes the specified key
	/**
		\param [in] hKey		-	Handle to root key
		\param [in] pKey		-	Key name
		\param [in] bSubKeys	-	Non-zero to delete sub keys, if
									zero, only values are deleted.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DeleteKey( HKEY hKey, LPCTSTR pKey, BOOL bSubKeys = TRUE );

	//==============================================================
	// UpdateData()
	//==============================================================
	/// Updates key or value location
	/**
		\param [in] bRead		-	Non-zero to read value, zero to write value.
		\param [in] pName		-	Value name
		\param [in,out] pDw		-	Pointer to value buffer
		\param [in] dwDefault	-	Default value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL UpdateData( BOOL bRead, LPCTSTR pName, LPDWORD pDw, DWORD dwDefault = 0 )
	{	if ( pDw == NULL ) return FALSE;
		if ( bRead ) 
		{	if ( Get( pName, pDw ) ) return TRUE; 
			*pDw = dwDefault; return FALSE; 
		}
		return Set( pName, *pDw ) != 0; 
	}

	//==============================================================
	// UpdateData()
	//==============================================================
	/// Updates key or value location
	/**
		\param [in] bRead		-	Non-zero to read value, zero to write value.
		\param [in] pName		-	Value name
		\param [in,out] ptr		-	Pointer to value buffer
		\param [in] size		-	Size of buffer in ptr
		\param [in] pDefault	-	Pointer to default value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL UpdateData( BOOL bRead, LPCTSTR pName, void *ptr, DWORD size, void *pDefault = NULL )
	{	if ( ptr == NULL || size == 0 ) return FALSE;
		if ( bRead ) 
		{	if ( Get( pName, ptr, size ) ) return TRUE; 
			if ( pDefault != NULL ) memcpy( ptr, pDefault, size ); 
			return FALSE; 
		}
		else return Set( pName, ptr, size ) != 0;
	}

	//==============================================================
	// UpdateData()
	//==============================================================
	/// Updates key or value location
	/**
		\param [in] bRead		-	Non-zero to read value, zero to write value.
		\param [in] pName		-	Value name
		\param [in,out] pStr	-	Pointer to value buffer
		\param [in] size		-	Size of buffer in pStr
		\param [in] pDefault	-	Default value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL UpdateData( BOOL bRead, LPCTSTR pName, LPSTR pStr, DWORD size, LPCTSTR pDefault = "" )
	{	if ( pStr == NULL || size == 0 ) return FALSE;
		if ( bRead ) 
		{	if ( Get( pName, pStr, size ) ) return TRUE;
			if ( pDefault != NULL ) strcpy( pStr, pDefault );
			return FALSE;			
		} // end if
		else return Set( pName, pStr ) != 0; 
	}

	//==============================================================
	// Get()
	//==============================================================
	/// 
	/**
		\param [in] pName	-	Variable name.
		\param [out] pRk	-	Pointer to CRKey object that receives
								the de-serialized variables.
		
		\return Non-zero if success
	
		\see Set()
	*/
	BOOL Get( LPCTSTR pName, CRKey *pRk );

	//==============================================================
	// Set()
	//==============================================================
	/// 
	/**
		\param [in] pName	-	Variable name.
		\param [in] pRk		-	Pointer to CRKey object that contains
								the variables to serialize and store.
		
		\return Non-zero if success
	
		\see Get()
	*/
	BOOL Set( LPCTSTR pName, CRKey *pRk );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the DWORD representation of the named value
	/**
		\param [in] prv		-	Pointer to reg value structure
		\param [out] pdw	-	Receives the value
		
		\return Pointer in prv
	
		\see 
	*/
	LPREGVALUE Get( LPREGVALUE prv, LPDWORD pdw );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the DWORD representation of the named value
	/**
		\param [in] pName	-	Value name
		\param [out] pdw	-	Receives the value
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPCTSTR pName, LPDWORD pdw );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the long representation of the named value
	/**
		\param [in] prv		-	Pointer to reg value structure
		\param [out] pl		-	Receives the value
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPREGVALUE prv, LPLONG pl )	
	{	return Get( prv, (LPDWORD)pl ); }

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the long representation of the named value
	/**
		\param [in] pName	-	Value name
		\param [out] pl		-	Receives the value
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPCTSTR pName, LPLONG pl )
	{	return Get( pName, (LPDWORD)pl ); }

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the double representation of the named value
	/**
		\param [in] prv		-	Pointer to reg value structure
		\param [out] pd		-	Receives the value
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPREGVALUE prv, double *pd );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the double representation of the named value
	/**
		\param [in] pName	-	Value name
		\param [out] pd		-	Receives the value
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPCTSTR pName, double *pd );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the binary representation of the named value
	/**
		\param [in] prv		-	Pointer to reg value structure
		\param [out] ptr	-	Receives the value
		\param [in] size	-	Size of buffer in ptr
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get(LPREGVALUE prv, void *ptr, DWORD size);

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the binary representation of the named value
	/**
		\param [in] pName	-	Value name
		\param [out] ptr	-	Receives the value
		\param [in] size	-	Size of buffer in ptr
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPCTSTR pName, void *ptr, DWORD size );

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the string representation of the named value
	/**
		\param [in] prv		-	Pointer to reg value structure
		\param [out] str	-	Receives the value
		\param [in] size	-	Size of buffer in str
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get(LPREGVALUE prv, LPSTR str, DWORD size);

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the string representation of the named value
	/**
		\param [in] pName	-	Value name
		\param [out] str	-	Receives the value
		\param [in] size	-	Size of buffer in str
		
		\return Pointer to value key structure
	
		\see 
	*/
	LPREGVALUE Get( LPCTSTR pName, LPSTR str, DWORD size );

	//==============================================================
	// GetSz()
	//==============================================================
	/// Returns a pointer to the string representation of value
	/**
		\param [in] prv			-	Pointer to reg value structure
		\param [in] pDefault 	-	Default value
		
		\return Pointer to value or pDefault
	
		\see 
	*/
	LPCTSTR GetSz(LPREGVALUE prv, LPCTSTR pDefault = "" );

	//==============================================================
	// GetSz()
	//==============================================================
	/// Returns a pointer to the string representation of value
	/**
		\param [in] pName		-	Value name
		\param [in] pDefault 	-	Default value
		
		\return Pointer to value or pDefault
	
		\see 
	*/
	LPCTSTR GetSz( LPCTSTR pName, LPCTSTR pDefault = "" );
	
	//==============================================================
	// GetDword()
	//==============================================================
	/// Returns the DWORD representation of value
	/**
		\param [in] prv			-	Pointer to reg value structure
		\param [in] dwDefault 	-	Default value
		
		\return Pointer to value or dwDefault
	
		\see 
	*/
	DWORD GetDword( LPREGVALUE prv, DWORD dwDefault = 0 );

	//==============================================================
	// GetDword()
	//==============================================================
	/// Returns the DWORD representation of value
	/**
		\param [in] pName		-	Value name
		\param [in] dwDefault 	-	Default value
		
		\return Pointer to value or dwDefault
	
		\see 
	*/
	DWORD GetDword( LPCTSTR pName, DWORD dwDefault = 0 );

	//==============================================================
	// GetLong()
	//==============================================================
	/// Returns the long representation of value
	/**
		\param [in] prv			-	Pointer to reg value structure
		\param [in] lDefault 	-	Default value
		
		\return Pointer to value or lDefault
	
		\see 
	*/
	LONG GetLong( LPREGVALUE prv, LONG lDefault = 0 )
	{	return (LONG)GetDword( prv, (DWORD)lDefault ); }

	//==============================================================
	// GetLong()
	//==============================================================
	/// Returns the long representation of value
	/**
		\param [in] pName		-	Value name
		\param [in] lDefault 	-	Default value
		
		\return Pointer to value or lDefault
	
		\see 
	*/
	LONG GetLong( LPCTSTR pName, LONG lDefault = 0 )
	{	return (LONG)GetDword( pName, (DWORD)lDefault ); }

	//==============================================================
	// GetDouble()
	//==============================================================
	/// Returns the double representation of value
	/**
		\param [in] prv			-	Pointer to reg value structure
		\param [in] dDefault 	-	Default value
		
		\return Pointer to value or dDefault
	
		\see 
	*/
	double GetDouble( LPREGVALUE prv, double dDefault = 0 );

	//==============================================================
	// GetDouble()
	//==============================================================
	/// Returns the double representation of value
	/**
		\param [in] pName		-	Pointer to reg value structure
		\param [in] dDefault 	-	Default value
		
		\return Pointer to value or dDefault
	
		\see 
	*/
	double GetDouble( LPCTSTR pName, double dDefault = 0 );

	//==============================================================
	// GetKeyFromName()
	//==============================================================
	/// Converts a string name of Windows registry root key to key handle
	/**
		\param [in] pName		-	Key name string
		\param [out] phKey		-	Receives key handle
		\param [out] pKey		-	Receives full key name
		
		\return Returns non-zero if success
	
		\see 
	*/
	BOOL GetKeyFromName( LPCTSTR pName, PHKEY phKey, LPSTR pKey );

	//==============================================================
	// GetKey()
	//==============================================================
	/// Reads the specifed key fron the Windows registry
	/**
		\param [in] hKey	-	Handle to root key
		\param [in] pKey	-	Pointer to key name
		\param [in] bMerge	-	Non-zero to merge into current list
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetKey( HKEY hKey, LPCTSTR pKey, BOOL bMerge = FALSE );

	//==============================================================
	// Add()
	//==============================================================
	/// Adds the specified binary value to the list
	/**
		\param [in] dwType		-	Value type
		\param [in] pName		-	Value name
		\param [in] pValue		-	Value data
		\param [in] dwValue		-	Number of bytes in pValue
		\param [in] bFile		-	Non-zero if pValue is a filename
		
		\return Pointer to REGVALUE structure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Add( DWORD dwType, LPCTSTR pName,  const void * pValue, DWORD dwValue, BOOL bFile = FALSE );

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the double representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] dValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, double dValue )
	{	char num[ 256 ]; sprintf( num, "%f", dValue );
		return Set( pName, num ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the DWORD representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] dwValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, DWORD dwValue )
	{	return Add( REG_DWORD, pName, &dwValue, sizeof( dwValue ) ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the long representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] lValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, LONG lValue )
	{	return Add( REG_DWORD, pName, (LPDWORD)&lValue, sizeof( lValue ) ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the int representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] iValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, int iValue )
	{	return Set( pName, (LONG)iValue ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the short representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] iValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, short iValue )
	{	return Set( pName, (LONG)iValue ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the unsigned int representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] uValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, UINT uValue )
	{	return Set( pName, (DWORD)uValue ); }
	
	//==============================================================
	// Set()
	//==============================================================
	/// Sets the string representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] pValue	-	New value
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, LPCTSTR pValue )
	{	if ( pValue == NULL ) return Add( REG_SZ, pName, NULL, 0 );
		return Add( REG_SZ, pName, (LPVOID)pValue, strlen( pValue ) ); }

	//==============================================================
	// SetStr()
	//==============================================================
	/// Sets the string representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] pValue	-	New value
		\param [in] dwLen	-	Length of the string
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE SetStr( LPCTSTR pName, LPCTSTR pValue, DWORD dwLen )
	{	if ( !pValue || !dwLen ) return Add( REG_SZ, pName, NULL, 0 );
		return Add( REG_SZ, pName, (LPVOID)pValue, dwLen ); }

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the binary representation of a value
	/**
		\param [in] pName	-	Value name
		\param [in] pValue	-	New value
		\param [in] dwValue	-	Size of value in pValue
		
		\return Pointer to REGVALUE strucure if success, otherwise NULL
	
		\see 
	*/
	LPREGVALUE Set( LPCTSTR pName, const void * pValue, DWORD dwValue )
	{	return Add( REG_BINARY, pName, pValue, dwValue ); }

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Provides custom destruction for REGVALUE structures
	/**
		\param [in] node	-	Pointer to REGVALUE structure
	*/
	virtual void DeleteObject( void *node );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of a REGVALUE structure
	/**
		\return Size of a REGVALUE structure in bytes
	*/
	virtual DWORD GetObjSize() { return sizeof( REGVALUE ); }

	//==============================================================
	// SetName()
	//==============================================================
	/// Sets the name of this key
	/**
		\param [in] pName	-	Key name
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetName( LPCTSTR pName );

	//==============================================================
	// GetName()
	//==============================================================
	/// Returns the name of the current key
	/**		
		\return Pointer to buffer containing key name
	*/
	LPCTSTR GetName() { if ( m_name.ptr() == NULL ) return ""; return m_name; }

	/// Default constructor
	CRKey();

	/// Destructor
	virtual ~CRKey();

	//==============================================================
	// GetKeyName()
	//==============================================================
	/// Returns a string describing the specified key
	/**
		\param [in] hKey	-	Root key
		\param [in] pKey	-	Key path
		\param [in] pName	-	Receives human readable name for key
		
		hKey must specify a Windows registry root key

		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetKeyName( HKEY hKey, LPCTSTR pKey, LPSTR pName );

	//==============================================================
	// SetMinSize()
	//==============================================================
	/// Sets the minimum memory allocation size for a value
	/**
		\param [in] min		-	Minimum allocation size in bytes
	*/
	void SetMinSize( DWORD min ) { m_dwMinSize = min; }

	//==============================================================
	// ReadInline()
	//==============================================================
	/// Reads name-value pairs from a HTTP GET URL style encoding
	/**
		\param [in] pStr	-	Parameters
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadInline( LPCTSTR pStr ) { return ReadInline( (LPBYTE)pStr, strlen( pStr ) ); }

	//==============================================================
	// ReadInline()
	//==============================================================
	/// Reads name-value pairs from a HTTP GET URL style encoding
	/**
		\param [in] buf				-	Buffer containing parameter string
		\param [in] size			-	Number of bytes in buf
		\param [in] sep				-	name-value pair separator, usually '&'
		\param [in] bDeCanonicalize	-	Non-zero to decode escaped values
		
		\return 
	
		\see 
	*/
	BOOL ReadInline(LPBYTE buf, DWORD size, char sep = '&', BOOL bDeCanonicalize = TRUE);

	//==============================================================
	// ReadMIME()
	//==============================================================
	/// Reads name-value pairs from a MIME or HTTP header style encoding
	/**
		\param [in] pStr	-	String containing values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadMIME( LPCTSTR pStr ) { return ReadMIME( (LPBYTE)pStr, strlen( pStr ) ); }

	//==============================================================
	// ReadMIME()
	//==============================================================
	/// Reads name-value pairs from a MIME or HTTP header style encoding
	/**
		\param [in] buf		-	Buffer containing values
		\param [in] size	-	Number of bytes in buf
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadMIME(LPBYTE buf, DWORD size);

private:

	/// Minimum memory allocation size for values
	DWORD	m_dwMinSize;

	/// Key name
	CStr	m_name;

	/// Temporary buffer for string converted values
	CStr	m_num;

};

#endif // !defined(AFX_REGKEY_H__477E05A1_7DAB_4329_8422_555016A90E0C__INCLUDED_)
