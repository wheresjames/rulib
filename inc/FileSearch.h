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
// FileSearch.h: interface for the CFileSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESEARCH_H__7C796E65_D1BD_43A6_BD2E_EB1DBCC91E09__INCLUDED_)
#define AFX_FILESEARCH_H__7C796E65_D1BD_43A6_BD2E_EB1DBCC91E09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LList.h"
#include "WinFile.h"

/// Holds information of file search query
typedef struct tagFILESEARCHINFO : LLISTINFO
{
	TCHAR			fname[ CWF_STRSIZE ];
	TCHAR			loc[ CWF_STRSIZE ];

} FILESEARCHINFO, *LPFILESEARCHINFO; // end typedef struct

//==================================================================
// CFileSearch
//
/// Provides file searching capability
/**
	This class can be used to search files for particular strings.
*/
//==================================================================
class CFileSearch : public CLList  
{
public:
	
	//==============================================================
	// FindString()
	//==============================================================
	/// Searches files for a particular text string
	/**
		\param [in] pText	-	Text string to search for
		\param [in] pDir	-	Directory to start search
		\param [in] pExt	-	File extensions to search
		\param [in] pLoc	-	Tracks recursive file path
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FindString( LPCTSTR pText, LPCTSTR pDir, LPCTSTR pExt, LPCTSTR pLoc = NULL );

	//==============================================================
	// AddFile()
	//==============================================================
	/// Adds a file to the list 
	/**
		\param [in] pFile	-	Filename
		\param [in] pLoc	-	File location
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddFile( LPCTSTR pFile, LPCTSTR pLoc );

	//==============================================================
	// Search()
	//==============================================================
	/// Searches for the particular text string
	/**
		\param [in] pText	-	Text to search for
		\param [in] pDir	-	Directory to start search
		\param [in] pExt	-	File extension 
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Search( LPCTSTR pText, LPCTSTR pDir, LPCTSTR pExt );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of a file search element structure
	virtual DWORD GetObjSize() { return sizeof( FILESEARCHINFO ); }

	//==============================================================
	// GetExtension()
	//==============================================================
	/// Returns the specified extension from the list
	/**
		\param [in] ext			-	Index of extension to retrieve
		\param [in] pExt		-	Buffer that receives extension
		\param [in] pExtList	-	List of extensions separated by sep
		\param [in] sep			-	Separator character
		
		\return Non-Zero if extension index is valid
	
		\see 
	*/
	static BOOL GetExtension( DWORD ext, LPSTR pExt, LPCTSTR pExtList, BYTE sep = ';' );

	//==============================================================
	// IsToken()
	//==============================================================
	/// Searches a buffer for a string
	/**
		\param [in] buf			-	Buffer to search
		\param [in] len			-	Size of buffer in buf
		\param [in] token		-	Token to search for
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL IsToken( LPCTSTR buf, DWORD len, LPCTSTR token );
	
	//==============================================================
	// FindInFile()
	//==============================================================
	/// Searches a file for a string
	/**
		\param [in] pString		-	String to find
		\param [in] pFile		-	Name of file to search
		
		\return Non-zero if string is found in file
	
		\see 
	*/
	static BOOL FindInFile( LPCTSTR pString, LPCTSTR pFile );

	//==============================================================
	// FindInMem()
	//==============================================================
	/// Searches a buffer for the specified string
	/**
		\param [in] pString		-	String to find
		\param [in] buf			-	Buffer to search
		\param [in] size		-	Size of buffer in buf
		
		\return 
	
		\see 
	*/
	static BOOL FindInMem( LPCTSTR pString, LPCTSTR buf, DWORD size );

	/// Constructor
	CFileSearch();

	/// Destructor
	virtual ~CFileSearch();


};

#endif // !defined(AFX_FILESEARCH_H__7C796E65_D1BD_43A6_BD2E_EB1DBCC91E09__INCLUDED_)
