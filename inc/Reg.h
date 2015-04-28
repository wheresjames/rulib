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
// Reg.h: interface for the CReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REG_H__3E9A3CA2_DEB1_4A35_BA2E_55108A0BE5EA__INCLUDED_)
#define AFX_REG_H__3E9A3CA2_DEB1_4A35_BA2E_55108A0BE5EA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/// Structure containing registry key information
/**
	\see CReg
*/
typedef struct tagREGKEY : LLISTINFO
{
	/// Registry key class
	CRKey		*key;

} REGKEY; // end typedef struct
typedef REGKEY* LPREGKEY;
typedef LPREGKEY HREGKEY;

//==================================================================
// CReg
//
/// This class contains a list of registry objects
/**
	Encapsulates the functionality of the Windows registry.  Also
	functions nicely as a general purpose property bag which can
	save / restore values to / from the Windows registry as well as
	disk file formats.	
*/
//==================================================================
class CReg : public CHList  
{

public:

	//==============================================================
	// ReadInline()
	//==============================================================
	/// Decodes 2D URL encoded data
	/**
		\param [in] pStr	-	Pointer to 2D URL string

		This function reverses the encoding by EncodeUrl()
		
		\return Non-zero if success
	
		\see EncodeUrl()
	*/
	BOOL ReadInline( LPCTSTR pStr ) { return ReadInline( (LPBYTE)pStr, strlen( pStr ) ); }

	//==============================================================
	// ReadInline()
	//==============================================================
	/// Decodes 2D URL encoded data
	/**
		\param [in] pBuf	-	Pointer to 2D URL string
		\param [in] dwSize	-	Length of string

		This function reverses the encoding by EncodeUrl()
		
		\return Non-zero if success
	
		\see EncodeUrl()
	*/
	BOOL ReadInline( LPBYTE pBuf, DWORD dwSize );

	//==============================================================
	// EncodeUrl()
	//==============================================================
	/// Encodes all data keys in 2D URL encoding
	/**
		\param [out] pPipe			-	Output buffer
		\param [in] chSepNameVal	-	Separator between name and value
		\param [in] chSepValues		-	Separator between values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EncodeUrl( CPipe *pPipe, char chSepNameVal = '=', char chSepValues = '&'  );

	//==============================================================
	// EncodeUrl()
	//==============================================================
	/// Encodes the specified key in URL format
	/**
		\param [out] pPipe			-	Output buffer
		\param [in] pKey			-	Name of key to encode
		\param [in] chSepNameVal	-	Separator between name and value
		\param [in] chSepValues		-	Separator between values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EncodeUrl( CPipe *pPipe, LPCTSTR pKey, char chSepNameVal = '=', char chSepValues = '&'  );

	//==============================================================
	// sEncodeUrl()
	//==============================================================
	/// Encodes all data keys in 2D URL encoding
	/**
		\param [in] chSepNameVal	-	Separator between name and value
		\param [in] chSepValues		-	Separator between values
		
		\return Encoded string if success, otherwise an empty string
	
		\see 
	*/
    std::string sEncodeUrl( char chSepNameVal = '=', char chSepValues = '&'  )
    {   CPipe pipe;
        if ( !EncodeUrl( &pipe, chSepNameVal, chSepValues ) )
            return "";
        pipe.Write( "\x0", 1 );        
        return (LPCTSTR)pipe.GetBuffer();
    }

	//==============================================================
	// EncodeJson()
	//==============================================================
	/// Encodes all data keys into a JSON string
	/**
		\param [in] s		- String buffer to work with
		\param [in] tabs	- How far in to tab the data
		\param [in] array	- Non-zero if it should be a flat array,
							  Zero to encode a keyed map
		
		\return Encoded string if success, otherwise an empty string
	
		\see 
	*/
	std::string& EncodeJson( std::string &s, int tabs = 0, int array = 0 );

	//==============================================================
	// EncodeJson()
	//==============================================================
	/// Encodes all data keys into a JSON string
	/**
		\param [in] tabs	- How far in to tab the data
		\param [in] array	- Non-zero if it should be a flat array,
							  Zero to encode a keyed map
		
		\return Encoded string if success, otherwise an empty string
	
		\see 
	*/
	std::string EncodeJson( int tabs = 0, int array = 0 )
	{	std::string s; return EncodeJson( s, tabs, array ); }

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the specified CReg object
	/**
		\param [in] pReg	-	Source CReg object
		\param [in] bMerge	-	Non-zero to merge into current data
		
		This function copies all keys in pReg into this object.

		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( CReg *pReg, BOOL bMerge = FALSE );

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
	// FindWhere()
	//==============================================================
	/// Searches for a key containing the specified value name and value
	/**
		\param [in] pName		-	Name to search for
		\param [in] dwValue		-	Value to search for
		
		\return Pointer to key or NULL if not found
	
		\see 
	*/
	LPREGKEY FindWhere( LPCTSTR pName, DWORD dwValue );

	//==============================================================
	// FindWhere()
	//==============================================================
	/// Searches for a key containing the specified value name and value
	/**
		\param [in] pName		-	Name to search for
		\param [in] pValue		-	Value to search for
		
		\return Pointer to key or NULL if not found
	
		\see 
	*/
	LPREGKEY FindWhere( LPCTSTR pName, LPCTSTR pValue );

	//==============================================================
	// IsSet()
	//==============================================================
	/// Returns non zero if the specified value exists
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		
		\return Non-zero if the value exists
	
		\see 
	*/
	BOOL IsSet( LPCTSTR pKey, LPCTSTR pName );

	//==============================================================
	// RenameKey()
	//==============================================================
	/// Renames a key
	/**
		\param [in] pOld	-	Old name
		\param [in] pNew	-	New name
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RenameKey( LPCTSTR pOld, LPCTSTR pNew );

	//==============================================================
	// RestoreSettings()
	//==============================================================
	/// Restores settings from a registry file
	/**
		\param [in] pFile		-	Registry filename
		\param [in] pBak		-	Optional backup file name
		\param [in] bSafety		-	If non-zero a value will be set
									in the registry file to detect
									partial saves and use the backup
									file if this occurs.
		
		The purpose of this function is to help reduce the likely
		hood of currupted configuration file by using a backup
		file containing the same information.

		\return Non-zero if success
	
		\see 
	*/
	BOOL RestoreSettings( LPCTSTR pFile, LPCTSTR pBak = NULL, BOOL bSafety = FALSE );

	//==============================================================
	// SaveSettings()
	//==============================================================
	/// Saves the registry values to a disk file
	/**
		\param [in] pFile	-	Disk filename
		\param [in] pBak	-	Optional backup file name

		Saves data to a configuration file.  Also saves to a backup
		file to be used in case the main file is currupted.
		
		\return 
	
		\see 
	*/
	BOOL SaveSettings( LPCTSTR pFile, LPCTSTR pBak = NULL);

	//==============================================================
	// Parse()
	//==============================================================
	/// Parses comma separated parameter value string
	/**
		\param [in] pStr	-	Value string
		\param [in] size	-	Number of bytes in pStr
		\param [in] sep		-	Value separator
		
		Example:
		\code
		"param1=100,param2=test,param3=\"hello world\""
		\endcode

		\return Non-zero if success
	
		\see 
	*/
	BOOL Parse( LPCTSTR pStr, DWORD size, char sep = ',' );

	//==============================================================
	// RestoreWindowPos()
	//==============================================================
	/// Restores a window position from a reg value
	/**
		\param [in] hWnd		-	Handle to window that is restored
		\param [in] pGroup		-	Key name
		\param [in] pName		-	Value name
		\param [in] bSize		-	Non-zero to resize window
		\param [in] bPosition	-	Non-zero to reposition window
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RestoreWindowPos( HWND hWnd, LPCTSTR pGroup, LPCTSTR pName, BOOL bSize = TRUE, BOOL bPosition = TRUE );

	//==============================================================
	// SaveWindowPos()
	//==============================================================
	/// Saves the position of a window
	/**
		\param [in] hWnd		-	Handle to window to save
		\param [in] pGroup		-	Key name
		\param [in] pName		-	Value name
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveWindowPos( HWND hWnd, LPCTSTR pGroup, LPCTSTR pName );

	//==============================================================
	// DeleteValue()
	//==============================================================
	/// Deletes the named value from the list
	/**
		\param [in] pKey		-	Key name
		\param [in] pValue		-	Value name
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DeleteValue( LPCTSTR pKey, LPCTSTR pValue );

	//==============================================================
	// Enum()
	//==============================================================
	/// Enumerates keys with the same name
	/**
		\param [in] pKey	-	Key name to find
		\param [in] hRk		-	Handle to starting key or NULL to find
								first occurence.
		
		This function can be used to enumerate through multiple keys
		with the same name

		\return Handle to reg structure or NULL if not found
	
		\see 
	*/
	HREGKEY Enum( LPCTSTR pKey, HREGKEY hRk );

	//==============================================================
	// SaveRegFile()
	//==============================================================
	/// Saves a registry formated file to a CPipe object
	/**
		\param [out] pipe	-	Handle to CPipe object
		\param [in] pKey	-	Key name to save, NULL for all keys
		\param [in] pHeader	-	Pointer to file header.  NULL for 
								default.  Set to empty string for
								none.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveRegFile( CPipe *pipe, LPCTSTR pKey = NULL, LPCTSTR pHeader = NULL );

	//==============================================================
	// GetKey()
	//==============================================================
	/// Returns a pointer to the specified key object
	/**
		\param [in] pKey	-	Name of key to find.  It is created
								if it is not found.
		
		\return	Pointer to key object or NULL if error.
	
		\see 
	*/
	CRKey* GetKey(LPCTSTR pKey);

	//==============================================================
	// FindKey()
	//==============================================================
	/// Returns a pointer to the specified key object
	/**
		\param [in] pKey	-	Name of key to find.  Function returns
								NULL if not found.
		
		\return Pointer to key object or NULL if not found.
	
		\see 
	*/
	CRKey* FindKey( LPCTSTR pKey );

	//==============================================================
	// WriteStr()
	//==============================================================
	/// Writes a string to the specified pipe
	/**
		\param [out] pipe	-	Pipe that receives string
		\param [in] pStr	-	String to write
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL WriteStr( CPipe *pipe, LPCTSTR pStr );

	//==============================================================
	// SaveRegKey()
	//==============================================================
	/// Saves a key to the specified pipe
	/**
		\param [out] pipe	-	Pipe that receives the key
		\param [in] pRk		-	Key object
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveRegKey( CPipe *pipe, LPREGKEY pRk );

	//==============================================================
	// SaveRegFile()
	//==============================================================
	/// Saves a registry formated file to the specified pipe
	/**
		\param [out] pipe	-	Pipe that receives the formated file data
		\param [in] pKey	-	Name of the key to save.  NULL to 
								save all keys.								
		\param [in] pHeader	-	Pointer to file header.  NULL for 
								default.  Set to empty string for
								none.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveRegFile(CPipe &pipe, LPCTSTR pKey = NULL, LPCTSTR pHeader = NULL);

	//==============================================================
	// SaveRegFile()
	//==============================================================
	/// Saves a registry formated file to the specified file
	/**
		\param [out] pFile	-	Filename
		\param [in] pKey	-	Name of the key to save.  NULL to 
								save all keys.								
		\param [in] pHeader	-	Pointer to file header.  NULL for 
								default.  Set to empty string for
								none.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveRegFile( LPCTSTR pFile, LPCTSTR pKey = NULL, LPCTSTR pHeader = NULL );

	//==============================================================
	// ahtoul()
	//==============================================================
	/// Converts an ASCII hex value to a binary value
	/**
		\param [in] pBuffer		-	Buffer containing ASCII hex value
		\param [in] ucBytes		-	Number of bytes to convert from
									pBuffer.
		
		\return Binary value
	
		\see 
	*/
	static DWORD ahtoul( LPCTSTR pBuffer, BYTE ucBytes );

	//==============================================================
	// ConvertData()
	//==============================================================
	/// Converts data from string to component values
	/**
		\param [out] pdwType	-	Receives the value type
		\param [out] pData		-	Receives the value data
		\param [out] pdwData	-	Receives number of bytes written to pData
		\param [in] buf			-	Contains definition string
		\param [in] size		-	Number of bytes in buf
		
		\return Index in buf of the end of the value
	
		\see 
	*/
	DWORD ConvertData( LPDWORD pdwType, LPBYTE pData, LPDWORD pdwData, LPBYTE buf, DWORD size );

	//==============================================================
	// AddKey()
	//==============================================================
	/// Adds the named key object to the list
	/**
		\param [in] pKey	-	Key name
		
		\return Pointer to newly created key object
	
		\see 
	*/
	LPREGKEY AddKey( LPCTSTR pKey );

	//==============================================================
	// GetToken()
	//==============================================================
	/// Extracts a token from a given string
	/**
		\param [in] pToken		-	Receives string token
		\param [in] dwTokenSize	-	Size of buffer in pToken
		\param [in] buf			-	Source string buffer
		\param [in] size		-	Number of bytes in buf
		\param [in] ucTerm		-	Token terminator
		
		\return Offset of the end of the token in buf
	
		\see 
	*/
	static DWORD GetToken( LPBYTE pToken, DWORD dwTokenSize, LPBYTE buf, DWORD size, BYTE ucTerm );
	
	//==============================================================
	// SkipWhiteSpace()
	//==============================================================
	/// Skips the white space in buffer
	/**
		\param [in] buf					-	Buffer to process
		\param [in] size				-	Number of bytes in buf
		\param [in] i					-	Starting offset in buf
		\param [in] bIncludeNewLine		-	Non-zero to skip new line
											characters as well.
		
		\return Offset of first non-white space character
	
		\see 
	*/
	static DWORD SkipWhiteSpace( LPBYTE buf, DWORD size, DWORD i, BOOL bIncludeNewLine = FALSE );
	
	//==============================================================
	// SkipNonWhiteSpace()
	//==============================================================
	/// Skips the white space in buffer
	/**
		\param [in] buf					-	Buffer to process
		\param [in] size				-	Number of bytes in buf
		\param [in] i					-	Starting offset in buf
		
		\return Offset of first white space character
	
		\see 
	*/
	static DWORD SkipNonWhiteSpace(LPBYTE buf, DWORD size, DWORD i );

	//==============================================================
	// IsWhiteSpace()
	//==============================================================
	/// Returns non-zero if specified character is a white space character
	/**
		\param [in] buf					-	Pointer to character
		\param [in] bIncludeNewLine		-	Non-zero if new line characters
											should be considered white space.
		
		\return Non-zero if character is white space.
	
		\see 
	*/
	static BOOL IsWhiteSpace( LPBYTE buf, BOOL bIncludeNewLine = FALSE ) 
	{	if ( buf ) return IsWhiteSpace( *buf, bIncludeNewLine ); return FALSE; }

	//==============================================================
	// IsWhiteSpace()
	//==============================================================
	/// Returns non-zero if specified character is a white space character
	/**
		\param [in] ch					-	Character to check
		\param [in] bIncludeNewLine		-	Non-zero if new line characters
											should be considered white space.
		
		\return Non-zero if character is white space.
	
		\see 
	*/
	static BOOL IsWhiteSpace( BYTE ch, BOOL bIncludeNewLine = FALSE );

	//==============================================================
	// NextToken()
	//==============================================================
	/// Returns the offset of the next token in specified buffer
	/**
		\param [in] buf		-	Buffer pointer
		\param [in] size	-	Number of bytes in buf
		\param [in] i		-	Starting offset in buf
		
		\return Offset of token in buf
	
		\see 
	*/
	static DWORD NextToken(LPBYTE buf, DWORD size, DWORD i);

	//==============================================================
	// NextLine()
	//==============================================================
	/// Returns the offset of the next line in specified buffer
	/**
		\param [in] buf		-	Buffer pointer
		\param [in] size	-	Number of bytes in buf
		\param [in] i		-	Starting offset in buf
		
		\return Offset of the start of next line in buf
	
		\see 
	*/
	static DWORD NextLine( LPBYTE buf, DWORD size, DWORD i );

	//==============================================================
	// GetNonWhiteSpace()
	//==============================================================
	/// Returns the first non-white space token
	/**
		\param [out] pToken		-	Receives the token
		\param [in] dwTokenSize -	Size of buffer in pToken
		\param [in] pBuf		-	Source buffer
		\param [in] dwSize		-	Number of bytes in pBuf
		\param [in] i			-	Starting offset in pBuf
		
		\return Offset in pBuf of the end of the found token
	
		\see 
	*/
	static DWORD GetNonWhiteSpace( LPBYTE pToken, DWORD dwTokenSize, LPBYTE pBuf, DWORD dwSize, DWORD i );

	//==============================================================
	// ProcessLine()
	//==============================================================
	/// Processes a single line from a Windows registry formated buffer
	/**
		\param [in] buf		-	Pointer to line
		\param [in] size	-	Number of bytes in buf
		
		Processes a single line from buf then returns.

		\return Offset of end of line proccessed in buf
	
		\see 
	*/
	DWORD ProcessLine( LPBYTE buf, DWORD size );

	//==============================================================
	// LoadRegFromMem()
	//==============================================================
	/// Loads Windows registry file formated data from memory buffer
	/**
		\param [in] buf		-	Buffer containing data
		\param [in] size	-	Number of bytes in buf
		\param [in] bMerge	-	Non-zero to merge data into current
								list.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL LoadRegFromMem( LPBYTE buf, DWORD size, BOOL bMerge = FALSE );

	//==============================================================
	// LoadRegFile()
	//==============================================================
	/// Loads Windows registry file formated data from disk file
	/**
		\param [in] pFile	-	Filename
		\param [in] pKey	-	Key to load.  NULL for all keys
		\param [in] bMerge	-	Non-zero to merge data into current
								list.
		
		\return Non-zero if success
		
		\return 
	
		\see 
	*/
	BOOL LoadRegFile( LPCTSTR pFile, LPCTSTR pKey = NULL, BOOL bMerge = FALSE );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves a Windows Registry formated file to disk
	/**
		\param [in] pFile	-	Filename
		
		\return Non-zero if success
	
		\see 
	*/
	virtual DWORD Save( LPCTSTR pFile );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads Windows registry file formated data from disk file
	/**
		\param [in] pFile	-	Filename
		\param [in] bMerge	-	Non-zero to merge data into current
								list.
		
		\return Non-zero if success
		
		\return 
	
		\see 
	*/
	virtual DWORD Load( LPCTSTR pFile, BOOL bMerge = FALSE  );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Release all keys and resources
	virtual void Destroy();

	//==============================================================
	// InitObject()
	//==============================================================
	/// Provides custom initialization for a key structure
	/**
		\param [in] node	-	Pointer to key structure
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL InitObject( void *node );

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Provides custom destruction for a key structure
	/**
		\param [in] node	-	Key structure pointer
	*/
	virtual void DeleteObject( void *node );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of a key structure
	virtual DWORD GetObjSize() { return sizeof( REGKEY ); }

	//==============================================================
	// GetDouble()
	//==============================================================
	/// Returns the double representation of a key value
	/**
		\param [in] pKey		-	Key name
		\param [in] pName		-	Value name
		\param [in] dDefault	-	Default value
		
		\return Value or dDefault
	
		\see 
	*/
	double GetDouble( LPCTSTR pKey, LPCTSTR pName, double dDefault = 0 )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return dDefault;
		return prk->GetDouble( pName, dDefault );
	}

	//==============================================================
	// GetLong()
	//==============================================================
	/// Returns the long representation of a key value
	/**
		\param [in] pKey		-	Key name
		\param [in] pName		-	Value name
		\param [in] lDefault	-	Default value
		
		\return Value or lDefault
	
		\see 
	*/
	long GetLong( LPCTSTR pKey, LPCTSTR pName, long lDefault = 0 )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return lDefault;
		return prk->GetLong( pName, lDefault );
	}

	//==============================================================
	// GetDword()
	//==============================================================
	/// Returns the DWORD representation of a key value
	/**
		\param [in] pKey		-	Key name
		\param [in] pName		-	Value name
		\param [in] dwDefault	-	Default value
		
		\return Value or dwDefault
	
		\see 
	*/
	DWORD GetDword( LPCTSTR pKey, LPCTSTR pName, DWORD dwDefault = 0 )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return dwDefault;
		return prk->GetDword( pName, dwDefault );
	}

	//==============================================================
	// GetSz()
	//==============================================================
	/// Returns the string representation of a key value
	/**
		\param [in] pKey		-	Key name
		\param [in] pName		-	Value name
		\param [in] pDefault	-	Default value
		
		\return Value or pDefault
	
		\see 
	*/
	LPCTSTR GetSz( LPCTSTR pKey, LPCTSTR pName, LPCTSTR pDefault = "" )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return pDefault;
		return prk->GetSz( pName, pDefault );
	}

	//==============================================================
	// Get()
	//==============================================================
	/// Returns the string representation of a key value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [out] str	-	Buffer that receives the string value
		\param [in] size	-	Size of buffer in str
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Get( LPCTSTR pKey, LPCTSTR pName, LPSTR str, DWORD size )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Get( pName, str, size ) != NULL );
	}
	
	//==============================================================
	// Get()
	//==============================================================
	/// Returns the DWORD representation of a key value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [out] pdw	-	Buffer that receives the value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Get( LPCTSTR pKey, LPCTSTR pName, LPDWORD pdw )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Get( pName, pdw ) != NULL );
	}
	
	//==============================================================
	// Get()
	//==============================================================
	/// Returns the binary representation of a key value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] ptr		-	Buffer that receives the binary value
		\param [in] size	-	Size of buffer in ptr
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Get( LPCTSTR pKey, LPCTSTR pName, LPVOID ptr, DWORD size )
	{	CRKey *prk = FindKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Get( pName, ptr, size ) != NULL );
	}

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the string representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] str		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, LPCTSTR str )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, str ) != NULL );
	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the string representation of the specified GUID
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] pGuid	-	GUID to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, const GUID *pGuid )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, pGuid ) != NULL );
	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the DWORD representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] dw		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
//	BOOL Set( LPCTSTR pKey, LPCTSTR pName, DWORD dw )
//	{	CRKey *prk = GetKey( pKey );
//		if ( prk == NULL ) return FALSE;
//		return ( prk->Set( pName, dw ) != NULL );
//	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the double representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] d		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, double d )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, d ) != NULL );
	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the long representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] l		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
//	BOOL Set( LPCTSTR pKey, LPCTSTR pName, long l )
//	{	CRKey *prk = GetKey( pKey );
//		if ( prk == NULL ) return FALSE;
//		return ( prk->Set( pName, l ) != NULL );
//	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the int representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] i		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, int i )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, i ) != NULL );
	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the int representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] i		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, unsigned int i )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, i ) != NULL );
	} // end Set

#if !defined( __GNUC__ )

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the int representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] i		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, DWORD i )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, i ) != NULL );
	} // end Set

#endif
	
	//==============================================================
	// Set()
	//==============================================================
	/// Sets the unsigned int representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] u		-	Value to set
		
		\return	Non-zero if success
	
		\see 
	*/
//	BOOL Set( LPCTSTR pKey, LPCTSTR pName, UINT u )
//	{	CRKey *prk = GetKey( pKey );
//		if ( prk == NULL ) return FALSE;
//		return ( prk->Set( pName, u ) != NULL );
//	} // end Set

	//==============================================================
	// Set()
	//==============================================================
	/// Sets the binary representation of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		\param [in] ptr		-	Binary value
		\param [in] size	-	Number of bytes in ptr
		
		\return	Non-zero if success
		
		\return 
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, LPVOID ptr, DWORD size )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Set( pName, ptr, size ) != NULL );
	} // end Set

	//==============================================================
	// Get()
	//==============================================================
	/// De-serializes data to the specified pReg object
	/**
		\param [in] pKey	-	Key name.
		\param [in] pName	-	Variable name.
		\param [out] pReg	-	Pointer to CReg object.
		\param [in] bMerge	-	Non-zero to merge settins with existing
								values in pReg.
		
		\return 
	
		\see 
	*/
	BOOL Get( LPCTSTR pKey, LPCTSTR pName, CReg *pReg, BOOL bMerge = FALSE );
	
	//==============================================================
	// Set()
	//==============================================================
	/// Serializes the CReg object and stores it.
	/**
		\param [in] pKey	-	Key name.
		\param [in] pName	-	Variable name.
		\param [in] pReg	-	Pointer to CReg object.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, CReg *pReg );

	//==============================================================
	// Get()
	//==============================================================
	/// 
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Name of the variable containing 
								serialized variables.
		\param [out] pRk	-	Pointer to CRKey object that receives
								the de-serialized variables.
		
		\return Non-zero if success
	
		\see Set()
	*/
	BOOL Get( LPCTSTR pKey, LPCTSTR pName, CRKey *pRk )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return ( prk->Get( pName, pRk ) != 0 );
	}

    std::string sGet( const SQCHAR *pKey, const SQCHAR *pName )
    {   return GetSz( pKey, pName ); }

    BOOL sSet( const SQCHAR *pKey, const SQCHAR *pName, const SQCHAR *pVal )
    {   return Set( pKey, pName, pVal ); }

    BOOL sIsSet( const SQCHAR *pKey, const SQCHAR *pName )
    {   return IsSet( pKey, pName ); }


	//==============================================================
	// Set()
	//==============================================================
	/// 
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Name of the variable.
		\param [in] pRk		-	Pointer to CRKey object that contains
								the variables to serialize and store.
		
		\return Non-zero if success
	
		\see Get()
	*/
	BOOL Set( LPCTSTR pKey, LPCTSTR pName, CRKey *pRk )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return FALSE;
		return prk->Set( pName, pRk );
	}
	
	//==============================================================
	// GetValueSize()
	//==============================================================
	/// Returns the size of the specified value
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		
		\return Size of the specified value, or zero if error
	
		\see 
	*/
	DWORD GetValueSize( LPCTSTR pKey, LPCTSTR pName )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return 0;
		return prk->GetValueSize( pName );
	} // end GetValueSize

	//==============================================================
	// GetValuePtr()
	//==============================================================
	/// Returns a pointer to the value data
	/**
		\param [in] pKey	-	Key name
		\param [in] pName	-	Value name
		
		\return Pointer to the specified value data, or NULL if error
	
		\see 
	*/
	void* GetValuePtr( LPCTSTR pKey, LPCTSTR pName )
	{	CRKey *prk = GetKey( pKey );
		if ( prk == NULL ) return NULL;
		return prk->GetValuePtr( pName );
	} // end GetValueSize
	
	//==============================================================
	// GetKey()
	//==============================================================
	/// Copies the specified key from the Windows registry
	/**
		\param [in] hKey		-	Root registry key
		\param [in] pKey		-	Sub key path
		\param [in] dwSubkeys	-	Depth of sub keys to copy
		\param [in] bMerge		-	Non-zero to merge
		\param [in] pKeyname	-	Copied key name
		
		\return 
	
		\see 
	*/
	BOOL GetKey( HKEY hKey, LPCTSTR pKey, DWORD dwSubkeys = MAXDWORD, BOOL bMerge = FALSE, LPCTSTR pKeyname = NULL );

	/// Default constructor
	CReg();

	/// Destructor
	virtual ~CReg();

	//==============================================================
	// RegKeyFromHandle()
	//==============================================================
	/// Returns the CRKey object encapsulated in a key structure
	/**
		\param [in] hRegKey		-	Handle to key structure
		
		\return CRKey pointer or NULL if error
	
		\see 
	*/
	static CRKey* RegKeyFromHandle( HREGKEY hRegKey ) 
	{	if ( hRegKey == NULL ) return NULL; return hRegKey->key; }

	//==============================================================
	// SetMinSize()
	//==============================================================
	/// Specifies the minimum size allocated for each key item
	/**
		\param [in] min		-	Minimum size in bytes
	*/
	void SetMinSize( DWORD min ) { m_dwMinSize = min; }

	//==============================================================
	// InplaceDecode
	//==============================================================
	/// Decodes %hh sequences in a string without copying.
	/**
		\param [in,out]	pStr	-	The string to decode.
		\param [in]		dwMax	-	Maximum number of bytes to decode.
									If dwMax is zero, a pStr must be
									NULL terminated.

		The return string will always be equal to or less than the
		length of the input string.

		\return The length of the decoded string
	*/
	static DWORD InplaceDecode( LPTSTR pStr, DWORD dwMax = 0 );

private:

	/// Minimum value allocation size
	DWORD			m_dwMinSize;

	/// Current key pointer
	LPREGKEY		m_pCurKey;

};

#endif // !defined(AFX_REG_H__3E9A3CA2_DEB1_4A35_BA2E_55108A0BE5EA__INCLUDED_)
