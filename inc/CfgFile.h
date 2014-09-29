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
// CfgFile.h: interface for the CCfgFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFGFILE_H__2A7126D3_54F9_11D5_82D7_FFFFFF000000__INCLUDED_)
#define AFX_CFGFILE_H__2A7126D3_54F9_11D5_82D7_FFFFFF000000__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CFG_STRSIZE		1024

#define CFG_USER		0x1000

#define CFG_VOID		0x0000
#define CFG_DWORD		0x0001
#define CFG_STR			0x0002
#define CFG_BINARY		0x0003
#define CFG_DOUBLE		0x0004


/// Configuration file element information
/**
	\see CCfgFile
*/
typedef struct tagCFGELEMENTINFO
{
	/// Element name
	TCHAR				name[ CFG_STRSIZE ];

	/// Element data type
	DWORD				type;

	/// Element data size
	DWORD				size;

	/// Pointer to element data
	LPVOID				value;

	/// Pointer to next element info structure
	tagCFGELEMENTINFO	*pNext;

	/// Pointer to previous element info structure
	tagCFGELEMENTINFO	*pPrev;

} CFGELEMENTINFO; // end typedef struct
typedef CFGELEMENTINFO*	LPCFGELEMENTINFO;

/// Configuration file group information
/**
	\see CCfgFile
*/
typedef struct tagCFGGROUPINFO
{
	/// Group name
	TCHAR				name[ CFG_STRSIZE ];

	/// Variable list size
	DWORD				size;

	/// Pointer to first element info structure in the list
	LPCFGELEMENTINFO	head;

	/// Pointer to last element info structure in the list
	LPCFGELEMENTINFO	tail;	

	/// Pointer to the next group info sturcture
	tagCFGGROUPINFO		*pNext;

	/// Pointer to the last group info structure
	tagCFGGROUPINFO		*pPrev;

} CFGGROUPINFO; // end typedef struct
typedef CFGGROUPINFO*	LPCFGGROUPINFO;
typedef LPCFGGROUPINFO 	HGROUP;

class CRKey;

//==================================================================
// CCfgFile
//
/// This class reads and writes files similar to Windows ini files.
/**
	This class will read file formats similar to the Windows ini files.
	This class was not written specifically for this purpose and there
	may be differences.  The intent was simply to implement a human
	readable configuration file.  For accessing Windows ini files, you
	should use the Windows API functions instead.
*/
//==================================================================
class CCfgFile  
{
public:

	//==============================================================
	// DeleteElements()
	//==============================================================
	/// Deletes all elements of the specified group
	/**
		\param [in] hGroup	-	Handle to the group.
	*/
	void DeleteElements( HGROUP hGroup );

	//==============================================================
	// WriteToMem()
	//==============================================================
	/// Writes the specified data to the specified memory buffer
	/**
		\param [in] buf		-	Memory buffer pointer.
		\param [in] size	-	Size of the buffer in buf.
		\param [in] src		-	Data to write.
		\param [in] srcsize	-	Size of the buffer in src.

		\return Returns the number of bytes written to buf.
	*/
	DWORD WriteToMem( LPBYTE buf, DWORD size, LPCTSTR src, DWORD srcsize );

	//==============================================================
	// SaveToMem()
	//==============================================================
	/// Creates a configuration file in memory
	DWORD SaveToMem( LPBYTE buf, DWORD len, BOOL bHeader = TRUE, BOOL bIniFormat = FALSE );

	//==============================================================
	// RestoreWindowPos()
	//==============================================================
	/// Restores the position of a window from a configuration file entry
	/**
		\param [in] hWnd		-	Handle to the window that will be restored.
		\param [in] pGroup		-	Pointer to a string naming the settings group.
		\param [in] pName		-	Pointer to a string nameing the setting.
		\param [in] bSize		-	Set to non-zero to have the window resized.
		\param [in] bPosition	-	Set to non-zero to have the window repositioned.
		\param [in] bToolWindow	-	Set to non-zero if the window is a tool window
		\param [in] bTopmost	-	Set to non-zero to restore the Z-Order position.

		\return Returns non-zero on success.
	*/
	BOOL RestoreWindowPos( HWND hWnd, LPCTSTR pGroup, LPCTSTR pName, BOOL bSize = TRUE, BOOL bPosition = TRUE, BOOL bToolWindow = FALSE, BOOL bTopmost = FALSE );

	//==============================================================
	// SaveWindowPos()
	//==============================================================
	/// Saves the position of a window into a configuration file entry
	/**
		\param [in] hWnd		-	Handle to the window that will be restored.
		\param [in] pGroup		-	Pointer to a string naming the settings group.
		\param [in] pName		-	Pointer to a string naming the setting.
		\param [in] bToolwindow	-	Set to non-zero if the window is a tool window

		\return Returns non-zero on success.
	*/
	BOOL SaveWindowPos( HWND hWnd, LPCTSTR pGroup, LPCTSTR pName, BOOL bToolwindow = FALSE );

	//==============================================================
	// RemoveElement()
	//==============================================================
	/// Removes an element from the configuration list
	/**
		\param [in] pGroup		-	Pointer to a string naming the settings group.
		\param [in] pElement	-	Pointer to a string naming the setting.

		\return Returns non-zero on success.
	*/
	BOOL RemoveElement( LPCTSTR pGroup, LPCTSTR pElement );

	//==============================================================
	// VerifyGroup()
	//==============================================================
	/// Verifies that a handle to a group is in fact valid.
	/**
		\param [in] hGroup		-	Pointer to a group handle.

		\return Returns hGroup if it is valid otherwise NULL.
	*/
	HGROUP VerifyGroup( HGROUP hGroup );
	
	//==============================================================
	// RemoveGroup()
	//==============================================================
	/// Removes a group from the configuration list
	/**
		\param [in] pGroup	-	Pointer to a string naming the settings group.

		\return Returns non-zero on success.
	*/
	BOOL RemoveGroup( LPCTSTR pGroup );

	//==============================================================
	// RemoveGroup()
	//==============================================================
	/// Removes a group from the configuration list
	/**
		\param [in] node	-	Handle to group

		\return Returns non-zero on success.
	*/
	BOOL RemoveGroup( HGROUP node );

	//==============================================================
	// AddGroup()
	//==============================================================
	/// Adds the group if it does not already exist.
	/**
		\param [in] pGroup	-	Pointer to a string naming the group that is to be added.

		\return Returns the handle to the new or existing group, or NULL if failure.
	*/
	HGROUP AddGroup( LPCTSTR pGroup );

	//==============================================================
	// FindGroup()
	//==============================================================
	/// Returns the handle to the specified group.
	/**
		\param [in] pGroup	-	Pointer to a string naming the group to find.

		\return Returns the handle to the group if found, otherwise NULL.
	*/
	HGROUP FindGroup( LPCTSTR pGroup );

	//==============================================================
	// AddLine()
	//==============================================================
	/// Processes a single line of a configuration file.
	/**
		\param [in] pLine	-	Pointer to a string containing the line to process and add.

		\return Non-zero if success.
	*/
	BOOL AddLine( LPSTR pLine );

	//==============================================================
	// LoadFromMem()
	//==============================================================
	/// Loads a configuration file from a memory buffer.
	/**
		\param [in] buf		-	Pointer to the buffer containing the configuration file data.
		\param [in] size	-	Size of the buffer in buf.
		\param [in] bMerge	-	Set to non-zero if you want to merge the configuration file
								into the existing configuration.
	*/
	BOOL LoadFromMem( LPBYTE buf, DWORD size, BOOL bMerge = FALSE );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads a configuration file from disk.
	/**
		\param [in] pFile	-	Filename of the configuration file that will be read.
		\param [in] bMerge	-	Set to non-zero if you want to merge the configuration file
								into the existing configuration.
	*/
	BOOL Load( LPCTSTR pFile, BOOL bMerge = FALSE );

	//==============================================================
	// GetMinDeCanonicalizeBufferSize()
	//==============================================================
	/// Returns the minimum size buffer needed to store the decoded string
	/**
		\param [in] size	-	Size of the encoded string.

		\return The maximum size of the decoded string.

		\see DeCanonicalizeBuffer()
	*/
	static DWORD GetMinDeCanonicalizeBufferSize( DWORD size )
	{	return size + 1; }

	//==============================================================
	// DeCanonicalizeBuffer()
	//==============================================================
	/// Decodes buffer, that is, converts all escape characters to actual values.
	/**
		\param [in] str		-	The string to decode.
		\param [out] buf	-	Buffer to hold decoded string.
		\param [in] max		-	The size of the buffer in buf.
		\param [out] size	-	The number of bytes written to buf.
	
		\return Non-zero if success.

		\see CanonicalizeBuffer()
	*/
	static BOOL DeCanonicalizeBuffer( LPCTSTR str, LPBYTE buf, DWORD max, LPDWORD size );

	//==============================================================
	// GetMinCanonicalizeBufferSize()
	//==============================================================
	/// Returns the minimum size buffer needed to store the encoded string
	/**
		\param [in] size	-	Size of the decoded string.

		\return The maximum size of the encoded string.

		\see CanonicalizeBuffer()
	*/
	static DWORD GetMinCanonicalizeBufferSize( DWORD size )
	{	return ( size * 3 ) + 1; }

	//==============================================================
	// CanonicalizeBuffer()
	//==============================================================
	/// Encodes buffer.  Converts certain characters to escape sequences.
	/**
		\param [out] str	-	Buffer that receives the encoded string
		\param [in] buf		-	Buffer that holds the data to be encoded
		\param [in] size	-	Size of the buffer in buf.
		
		\return Non-zero if success.
	
		\see DeCanonicalizeBuffer()
	*/
	static BOOL CanonicalizeBuffer( LPSTR str, LPBYTE buf, DWORD size );

	//==============================================================
	// FindElement()
	//==============================================================
	/// Searches for the named element
	/**
		\param [in] hGroup	-	Handle to group containing element
		\param [in] pName	-	Name of element to find.
		
		\return Pointer to element data or NULL if not found.
	
		\see 
	*/
	LPCFGELEMENTINFO FindElement( HGROUP hGroup, LPCTSTR pName );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves configuration file to disk
	/**
		\param [in] pFile	-	Disk filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Save( LPCTSTR pFile = NULL );
	
	//==============================================================
	// RemoveElement()
	//==============================================================
	/// 
	/**
		\param [in] hGroup	-	Handle to group containing element to remove.
		\param [in] node	-	Pointer to element structure to remove.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemoveElement( HGROUP hGroup, LPCFGELEMENTINFO node );
	
	//==============================================================
	// AddElement()
	//==============================================================
	/// Adds an element to the configuration
	/**
		\param [in] hGroup	-	Handle to group into which to add element.
		\param [in] name	-	Name of the new element.
		\param [in] type	-	Data type of the new element.
		\param [in] size	-	Size of the new element.
		\param [in] value	-	Initial value of the new element.
		
		\return Non-zero if success, otherwise zero.
	
		\see 
	*/
	BOOL AddElement( HGROUP hGroup, LPCTSTR name, DWORD type, DWORD size, LPVOID value );
	
	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all groups and elements
	void Destroy();

	/// Constructor
	CCfgFile();
	
	/// Destructor
	virtual ~CCfgFile();

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the total size of the list, i.e. the total number of elements.
	/**			
		\return The number of elements in the configuration.
	
		\see 
	*/
	DWORD Size() { return m_dwSize; }
	
	//==============================================================
	// GetNumGroups()
	//==============================================================
	/// Returns the total number of groups in the configuration.
	/**		
		\return The number of groups in the configuration.
	
		\see 
	*/
	DWORD GetNumGroups() { return Size(); }

	//==============================================================
	// GetNext()
	//==============================================================
	/// Gets the pointer to the next group
	/**
		\param [in] ptr		-	A group information structure
		
		\return Pointer to the next group or NULL if none.
	
		\see 
	*/
	LPCFGGROUPINFO GetNext( LPCFGGROUPINFO ptr )
	{	if ( ptr != NULL ) return ptr->pNext; return m_pHead; }

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the specified CRKey object into the specified group.
	/**
		\param [in] hGroup	-	Group that will receive the elements.
		\param [in] pRk		-	CRKey object whose elements that will be copied.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( HGROUP hGroup, CRKey *pRk );
	
	//==============================================================
	// Copy()
	//==============================================================
	/// 
	/**
		\param [in] pGroup	-	Name of group that will receive the elements.
		\param [in] pRk		-	CRKey object whose elements that will be copied.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( LPCTSTR pGroup, CRKey *pRk )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return Copy( hGroup, pRk );
	}

	//==============================================================
	// CopyGroup()
	//==============================================================
	/// Copies one group to another
	/**
		\param [in] pGroup		-	Name of group that will be copied.
		\param [in] pNewGroup	-	Name of new group to be created. 
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL CopyGroup( LPCTSTR pGroup, LPCTSTR pNewGroup );


	//==============================================================
	// GetNextElement()
	//==============================================================
	/// Returns the next element in a group
	/**
		\param [in] hGroup	-	Group containing element.
		\param [in] ptr		-	Pointer to current element.
		
		\return Pointer to the next element or NULL if none.
	
		\see 
	*/
	LPCFGELEMENTINFO GetNextElement( HGROUP hGroup, LPCFGELEMENTINFO ptr )
	{	if ( ptr != NULL ) return ptr->pNext; 
		if ( hGroup != NULL ) return hGroup->head; 
		return NULL;
	}

	//==============================================================
	// GetValueSize()
	//==============================================================
	/// Returns the size of the data in the specified element
	/**
		\param [in] hGroup	-	Handle to group containing element
		\param [in] pName	-	Pointer to name of the element.
		
		\return Size of the data.
	
		\see 
	*/
	DWORD GetValueSize( HGROUP hGroup, LPCTSTR pName )
	{	LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return 0;
		return pcei->size;
	}

	//==============================================================
	// GetValueSize()
	//==============================================================
	/// Returns the size of the data in the specified element
	/**
		\param [in] pGroup	-	Name of the group containing element
		\param [in] pName	-	Pointer to name of the element.
		
		\return Size of the data.
	
		\see 
	*/
	DWORD GetValueSize( LPCTSTR pGroup, LPCTSTR pName )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return 0;
		return GetValueSize( hGroup, pName );
	}

	//==============================================================
	// GetValuePtr()
	//==============================================================
	/// Returns a pointer to the data of the specified element.
	/**
		\param [in] hGroup	-	Handle to group containing the element.
		\param [in] pName	-	The name of the element.
		
		\return Pointer to the element data.
	
		\see 
	*/
	LPVOID GetValuePtr( HGROUP hGroup, LPCTSTR pName )
	{	LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return NULL;
		return pcei->value;
	}

	//==============================================================
	// GetValuePtr()
	//==============================================================
	/// Returns a pointer to the data of the specified element.
	/**
		\param [in] pGroup	-	Name of the group containing the element.
		\param [in] pName	-	The name of the element.
		
		\return Pointer to the element data.
	
		\see 
	*/
	LPVOID GetValuePtr( LPCTSTR pGroup, LPCTSTR pName )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return NULL;
		return GetValuePtr( hGroup, pName );
	}

	//==============================================================
	// GetSz()
	//==============================================================
	/// Returns a pointer to a string representation of an element.
	/**
		\param [in] pGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] pDefault	-	Default value for element.
		
		\return Pointer to string representation of element or pDefault if failure.
	
		\see 
	*/
	LPCTSTR GetSz( LPCTSTR pGroup, LPCTSTR pName, LPCTSTR pDefault = "" )
	{	HGROUP hGroup = FindGroup( pGroup ); 
		if ( hGroup == NULL ) return pDefault;
		return GetSz( hGroup, pName, pDefault );
	}

	//==============================================================
	// GetSz()
	//==============================================================
	/// Returns a pointer to a string representation of an element.
	/**
		\param [in] hGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] pDefault	-	Default value for element.
		
		\return Pointer to string representation of element or pDefault if failure.
	
		\see 
	*/
	LPCTSTR GetSz( HGROUP hGroup, LPCTSTR pName, LPCTSTR pDefault = "" )
	{	LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return pDefault;
		else if ( pcei->type == CFG_STR || pcei->type == CFG_VOID )
		{	if ( pcei->value != NULL ) return (LPCTSTR)pcei->value; return pDefault; }
		else if ( pcei->type == CFG_DWORD )
		{	wsprintf( m_szNum, "%lu", RUPTR2DW(pcei->value) ); return m_szNum; }
		else if ( pcei->type == CFG_DOUBLE )
		{	sprintf( m_szNum, "%f", *( (double*)pcei->value ) ); return m_szNum; }
		return pDefault;
	}

	//==============================================================
	// GetDword()
	//==============================================================
	/// DWORD representation of element
	/**
		\param [in] pGroup		-	Group containing element
		\param [in] pName		-	Name of element.
		\param [in] dwDefault	-	Default value for element.
		
		\return DWORD value for element or dwDefault if error.
	
		\see 
	*/
	DWORD GetDword( LPCTSTR pGroup, LPCTSTR pName, DWORD dwDefault = 0 )
	{	GetValue( pGroup, pName, &dwDefault ); return dwDefault;
	}

	//==============================================================
	// GetDword()
	//==============================================================
	/// DWORD representation of element
	/**
		\param [in] hGroup		-	Group containing element
		\param [in] pName		-	Name of element.
		\param [in] dwDefault	-	Default value for element.
		
		\return DWORD value for element or dwDefault if error.
	
		\see 
	*/
	DWORD GetDword( HGROUP hGroup, LPCTSTR pName, DWORD dwDefault = 0 )
	{	GetValue( hGroup, pName, &dwDefault ); return dwDefault;
	}

	//==============================================================
	// GetDouble()
	//==============================================================
	/// double representation of element
	/**
		\param [in] pGroup		-	Group containing element
		\param [in] pName		-	Name of element.
		\param [in] dDefault	-	Default value for element.
		
		\return double value for element or dwDefault if error.
	
		\see 
	*/
	double GetDouble( LPCTSTR pGroup, LPCTSTR pName, double dDefault = 0 )
	{	GetValue( pGroup, pName, &dDefault ); return dDefault;
	}

	//==============================================================
	// GetDouble()
	//==============================================================
	/// double representation of element
	/**
		\param [in] hGroup		-	Group containing element
		\param [in] pName		-	Name of element.
		\param [in] dwDefault	-	Default value for element.
		
		\return double value for element or dwDefault if error.
	
		\see 
	*/
	double GetDouble( HGROUP hGroup, LPCTSTR pName, double dDefault = 0 )
	{	GetValue( hGroup, pName, &dDefault ); return dDefault;
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Set binary variables
	/**
		\param [in] hGroup	-	Group containing element
		\param [in] pName	-	Name of the element to set
		\param [in] pValue	-	New value data of element.
		\param [in] dwSize	-	Size of the buffer in pValue.
		
		\return 
	
		\see 
	*/
	BOOL SetValue( HGROUP hGroup, LPCTSTR pName, LPVOID pValue, DWORD dwSize )
	{	if ( pValue == NULL || dwSize == 0 ) return FALSE;
		return AddElement( hGroup, pName, CFG_BINARY, dwSize, pValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Set binary variables
	/**
		\param [in] pGroup	-	Group containing element
		\param [in] pName	-	Name of the element to set
		\param [in] pValue	-	New value data of element.
		\param [in] dwSize	-	Size of the buffer in pValue.
		
		\return 
	
		\see 
	*/
	BOOL SetValue( LPCTSTR pGroup, LPCTSTR pName, LPVOID pValue, DWORD dwSize )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return SetValue( hGroup, pName, pValue, dwSize );
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value data for the specified element
	/**
		\param [in] hGroup	-	Handle to group containing element.
		\param [in] pName	-	Name of the element.
		\param [out] pValue	-	Receives the value data for the element.
		\param [in] dwSize	-	Size of the buffer in pValue.

		Call with pValue set to NULL to retreive the required buffer size.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( HGROUP hGroup, LPCTSTR pName, LPVOID pValue, DWORD dwSize )
	{
		LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if (	pcei == NULL || 
				( pcei->type != CFG_BINARY && pcei->type != CFG_VOID ) ||
				pcei->value == NULL )
			return FALSE;
		if ( pcei->size < dwSize ) dwSize = pcei->size + 1;
		memcpy( pValue, pcei->value, dwSize );
		return TRUE;
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value data for the specified element
	/**
		\param [in] pGroup	-	Name of the group containing element.
		\param [in] pName	-	Name of the element.
		\param [out] pValue	-	Receives the value data for the element.
		\param [in] dwSize	-	Size of the buffer in pValue.

		Call with pValue set to NULL to retreive the required buffer size.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( LPCTSTR pGroup, LPCTSTR pName, LPVOID pValue, DWORD dwSize )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return GetValue( hGroup, pName, pValue, dwSize );
	}
	
	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the string representation for an element
	/**
		\param [in] hGroup	-	Group containing the element.
		\param [in] pName	-	Name of the element.
		\param [in] pValue	-	String value of the element.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( HGROUP hGroup, LPCTSTR pName, LPCTSTR pValue )
	{	if ( pValue == NULL ) return FALSE;
		return AddElement( hGroup, pName, CFG_STR, strlen( pValue ), (LPVOID)pValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the string representation for an element
	/**
		\param [in] pGroup	-	Group containing the element.
		\param [in] pName	-	Name of the element.
		\param [in] pValue	-	String value of the element.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( LPCTSTR pGroup, LPCTSTR pName, LPCTSTR pValue )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return SetValue( hGroup, pName, pValue );
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value data for the specified element
	/**
		\param [in] hGroup	-	Handle of the group containing element.
		\param [in] pName	-	Name of the element.
		\param [out] pValue	-	Receives the string value for the element.
		\param [in] dwSize	-	Size of the buffer in pValue.

		Call with pValue set to NULL to retreive the required buffer size.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( HGROUP hGroup, LPCTSTR pName, LPSTR pValue, DWORD dwSize )
	{
		LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if (	pcei == NULL || 
				( pcei->type != CFG_STR && pcei->type != CFG_VOID ) )
			return FALSE;
		if ( pcei->size == 0 || pcei->value == NULL )
			pValue[ 0 ] = 0;
		else
		{
			if ( pcei->size < dwSize ) dwSize = pcei->size + 1;
			memcpy( pValue, pcei->value, dwSize );
			if ( dwSize ) pValue[ dwSize - 1 ] = 0;
			else pValue[ dwSize ] = 0;
		} // end else
		return TRUE;
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value data for the specified element
	/**
		\param [in] pGroup	-	Name of the group containing element.
		\param [in] pName	-	Name of the element.
		\param [out] pValue	-	Receives the string value for the element.
		\param [in] dwSize	-	Size of the buffer in pValue.

		Call with pValue set to NULL to retreive the required buffer size.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( LPCTSTR pGroup, LPCTSTR pName, LPSTR pValue, DWORD dwSize )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return GetValue( hGroup, pName, pValue, dwSize );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the DWORD representation of the specified element
	/**
		\param [in] hGroup	-	Handle to the group containing the element
		\param [in] pName	-	Name of the element.
		\param [in] dwValue -	New DWORD value for element
		
		\return Non-Zero if success.
	
		\see 
	*/
	BOOL SetValue( HGROUP hGroup, LPCTSTR pName, DWORD dwValue )
	{	
		return AddElement( hGroup, pName, CFG_DWORD, 0, (LPVOID)dwValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the DWORD representation of the specified element
	/**
		\param [in] pGroup	-	Name of the group containing the element
		\param [in] pName	-	Name of the element.
		\param [in] dwValue -	New DWORD value for element
		
		\return Non-Zero if success.
	
		\see 
	*/
	BOOL SetValue( LPCTSTR pGroup, LPCTSTR pName, DWORD dwValue )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return SetValue( hGroup, pName, dwValue );
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Retreives the DWORD representation of the specified element
	/**
		\param [in] hGroup		-	Handle to the group containing the element.
		\param [in] pName		-	Name of the element.
		\param [out] pdwValue	-	Receives the DWORD value of the element.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetValue( HGROUP hGroup, LPCTSTR pName, LPDWORD pdwValue )
	{
		LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return FALSE;
		if ( pcei->type == CFG_DWORD || pcei->size == 0 )
			*pdwValue = RUPTR2DW(pcei->value);
		else
		{	TCHAR *end;
			*pdwValue = strtoul( (char*)pcei->value, &end, 10 );
		} // end else
		return TRUE;
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Retreives the DWORD representation of the specified element
	/**
		\param [in] pGroup		-	Name of the group containing the element.
		\param [in] pName		-	Name of the element.
		\param [out] pdwValue	-	Receives the DWORD value of the element.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetValue( LPCTSTR pGroup, LPCTSTR pName, LPDWORD pdwValue )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return GetValue( hGroup, pName, pdwValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the long representation of an element
	/**
		\param [in] hGroup	-	Handle to the group containing element.
		\param [in] pName	-	Name of the element to set.
		\param [in] lValue	-	New long value of the element
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( HGROUP hGroup, LPCTSTR pName, long lValue )
	{	
		return AddElement( hGroup, pName, CFG_DWORD, 0, (LPVOID)lValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the long representation of an element
	/**
		\param [in] pGroup	-	Name of the group containing element.
		\param [in] pName	-	Name of the element to set.
		\param [in] lValue	-	New long value of the element
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( LPCTSTR pGroup, LPCTSTR pName, long lValue )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return SetValue( hGroup, pName, lValue );
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the long representation of the specified element
	/**
		\param [in] hGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] plValue		-	Receives the long representation of the element.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( HGROUP hGroup, LPCTSTR pName, long *plValue )
	{
		LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return FALSE;
		if ( pcei->type == CFG_DWORD || pcei->size == 0 )
			*plValue = (LONG)RUPTR2INT(pcei->value);
		else
		{	char *end;
			*plValue = strtol( (char*)pcei->value, &end, 10 );
		} // end else
		return TRUE;
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the long representation of the specified element
	/**
		\param [in] pGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] plValue		-	Receives the long representation of the element.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL GetValue( LPCTSTR pGroup, LPCTSTR pName, long *plValue )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return GetValue( hGroup, pName, plValue );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the double representation of the element
	/**
		\param [in] hGroup	-	Group containing the element.
		\param [in] pName	-	Name of the element.
		\param [in] dValue	-	double representation of the element
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( HGROUP hGroup, LPCTSTR pName, double dValue )
	{	
		TCHAR num[ 256 ];
		sprintf( num, "%f", dValue );
		return AddElement( hGroup, pName, CFG_STR, strlen( num ), (LPVOID)num );
	}

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the double representation of the element
	/**
		\param [in] pGroup	-	Group containing the element.
		\param [in] pName	-	Name of the element.
		\param [in] dValue	-	double representation of the element
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SetValue( LPCTSTR pGroup, LPCTSTR pName, double dValue )
	{	HGROUP hGroup = AddGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return SetValue( hGroup, pName, dValue );
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Retreives the double representation of the specified element.
	/**
		\param [in] hGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] pdValue		-	Receives the double representation.
		
		\return Non-zero if success, otherwise zero
	
		\see 
	*/
	BOOL GetValue( HGROUP hGroup, LPCTSTR pName, double *pdValue )
	{
		LPCFGELEMENTINFO pcei = FindElement( hGroup, pName );
		if ( pcei == NULL ) return FALSE;
		TCHAR *end;
		*pdValue = strtod( (TCHAR*)pcei->value, &end );
		return TRUE;
	}

	//==============================================================
	// GetValue()
	//==============================================================
	/// Retreives the double representation of the specified element.
	/**
		\param [in] pGroup		-	Group containing the element.
		\param [in] pName		-	Name of the element.
		\param [in] pdValue		-	Receives the double representation.
		
		\return Non-zero if success, otherwise zero
	
		\see 
	*/
	BOOL GetValue( LPCTSTR pGroup, LPCTSTR pName, double *pdValue )
	{	HGROUP hGroup = FindGroup( pGroup );
		if ( hGroup == NULL ) return FALSE;
		return GetValue( hGroup, pName, pdValue );
	}

	//==============================================================
	// SetFileName()
	//==============================================================
	/// Sets the default filename for the configuration.
	/**
		\param [in] file	-	The file name of the configuration.
	*/
	void SetFileName( LPCTSTR file ) 
	{ if ( file == NULL ) *m_szFileName = 0; else strcpy( m_szFileName, file ); }

	//==============================================================
	// SetKey()
	//==============================================================
	/// Sets the encryption / decryption key for the configuration file.
	/**
		\param [in] pKey	-	The encryption / decryption key for the file.
	*/
	void SetKey( LPCTSTR pKey )
	{	strcpy( m_szKey, pKey ); }

	//==============================================================
	// SetTerminator()
	//==============================================================
	/// Sets the default line terminiation character
	/**
		\param [in] term	-	The default line terminiation character
		
		\return 
	
		\see 
	*/
	void SetTerminator( BYTE term ) { m_utTerm = term; }

private:

	/// Line termination character
	TCHAR				m_utTerm;

	/// Encrypt / decrypt key
	TCHAR				m_szKey[ CFG_STRSIZE ];

	/// Total number of elements.
	DWORD				m_dwSize;

	/// Pointer to the first element in the list
	LPCFGGROUPINFO		m_pHead;

	/// Pointer to the last element in the list.
	LPCFGGROUPINFO		m_pTail;

	/// Number of errors made while reading file.
	DWORD				m_dwErrors;

	/// Handle to the current group.
	HGROUP				m_hCurGroup;

	/// Default filename
	TCHAR				m_szFileName[ CFG_STRSIZE ];

	/// Temporary buffer used to hold string conversions.
	TCHAR				m_szNum[ 256 ];
};

#endif // !defined(AFX_CFGFILE_H__2A7126D3_54F9_11D5_82D7_FFFFFF000000__INCLUDED_)
