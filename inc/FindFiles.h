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
// FindFiles.h: interface for the CFindFiles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDFILES_H__ADD77EB0_0605_43FE_B217_FB9D6A429561__INCLUDED_)
#define AFX_FINDFILES_H__ADD77EB0_0605_43FE_B217_FB9D6A429561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CFindFiles
//
/// Simple wrapper for the Windows Find Files API
/**
	Use this class to search for files and directories.

	To search sub-folders as well, use TRecursiveFindFiles.

	Typical example:

  \code

	CFindFiles ff;

	if ( ff.FindFirst( "C:\\", "*.*" ) )
		do
		{

			if ( ff.IsDirectory() ) ; // Use folder

			else ; // Use file

		} while ( ff.FindNext() );

  \endcode

*/
//==================================================================
class CFindFiles  
{
public:

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources used by this class instance
	void Destroy();

	//==============================================================
	// BuildPaths()
	//==============================================================
	/// Builds paths to current file
	/**		
		\return Non-zero if success
	*/
	BOOL BuildPaths();

	//==============================================================
	// FindNext()
	//==============================================================
	/// Finds the next matching file
	/**		
		\return Non-zero if another file is found
	*/
	BOOL FindNext();

	//==============================================================
	// FindFirst()
	//==============================================================
	/// Finds the first matching file
	/**
		\param [in] pDir		-	Directory to start search
		\param [in] pMask		-	File search mask

		\return Non-zero if a matching file was found
	
		\see 
	*/
	BOOL FindFirst( LPCTSTR pDir, LPCTSTR pMask = "*.*" );

    BOOL sFindFirst( const std::string &sDir, const std::string &sMask )
    {   return FindFirst( sDir.c_str(), sMask.c_str() ); }

	/// Constructor
	CFindFiles();

	//==============================================================
	// FindFirst()
	//==============================================================
	/// Construcs object and finds the first matching file
	/**
		\param [in] pDir		-	Directory to start search
		\param [in] pMask		-	File search mask
		
		\return Non-zero if a matching file was found
	
		\see 
	*/
	CFindFiles( LPCTSTR pDir, LPCTSTR pMask = "*.*" );

	/// Destructor
	virtual ~CFindFiles();

	//==============================================================
	// GetPath()
	//==============================================================
	/// Returns the path to the found file
	LPCTSTR GetPath() { return m_szPath; }

	//==============================================================
	// GetFullPath()
	//==============================================================
	/// Returns the complete filename for the found file
	LPCTSTR GetFullPath() { return m_szFullPath; }

	//==============================================================
	// GetFileName()
	//==============================================================
	/// Returns the filename for the current matching file
	LPCTSTR GetFileName() { return m_wfd.cFileName; }

	//==============================================================
	// GetFileAttributes()
	//==============================================================
	/// Returns the file attributes of the current matching file
	DWORD GetFileAttributes() { return m_wfd.dwFileAttributes; }

	//==============================================================
	// IsDirectory()
	//==============================================================
	/// Returns non-zero if the current match is a directory
	BOOL IsDirectory() 
	{	if ( m_wfd.dwFileAttributes == MAXDWORD ) return FALSE;
		return ( m_wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0; }
	
    BOOL sIsDirectory() { return IsDirectory(); }

	//==============================================================
	// IsDirectory()
	//==============================================================
	/// Returns non-zero if the specified path is a directory
	/**
		\param [in] pDir	-	Directory path
		
		\return Non-zero if the specified path is a directory
	
		\see 
	*/
	static BOOL IsDirectory( LPCTSTR pDir )
	{	DWORD dwAttributes = ::GetFileAttributes( pDir );
		if ( dwAttributes == MAXDWORD ) return FALSE;
		return ( dwAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0; }

	//==============================================================
	// WIN32_FIND_DATA*()
	//==============================================================
	/// Returns pointer to WIN32_FIND_DATA structure
	operator WIN32_FIND_DATA*() { return &m_wfd; }


	//==============================================================
	// Wfd()
	//==============================================================
	/// Returns pointer to WIN32_FIND_DATA structure
	WIN32_FIND_DATA* Wfd() { return &m_wfd; }

private:

	/// Currently open find handle
	HANDLE				m_hFind;

	/// Windows find files data structure
	WIN32_FIND_DATA		m_wfd;

	/// Buffer containing path to file
	TCHAR				m_szPath[ CWF_STRSIZE ];

	/// Buffer containing full file path
	TCHAR				m_szFullPath[ CWF_STRSIZE ];
	
};

//==================================================================
// TRecursiveFindFiles
//
/// Creates an array of CFindFiles to support recursive searches
/**

	The functions in this class, by design, exactly match CFindFiles
	so it is a drop in replacement.

	This is a template class because I didn't want to use dynamic 
	memory allocation since this could slow down the searches.  I 
	also don't know the maximum folder depth you may want to parse.  
	Using a 'large enough' number like 32 or 64 would result
	in a pretty large amount of memory.  So I allow (force) you to
	decide for yourself.

	I also failed to supply a default to avoid causing any pain
	to those that may not otherwise realize that the depth is 
	limited.

	At the time I am writing this, each depth will cost you 
	2,376 bytes.  sizeof( CFindFiles )

	Typical example:

  \code

	TRecursiveFindFiles< 16 > rff;

	if ( rff.FindFirst( "C:\\", "*.*" ) )
		do
		{

			if ( rff.IsDirectory() ) ; // Use folder

			else ; // Use file

		} while ( rff.FindNext() );

  \endcode

*/
//==================================================================
template < const long c_lMaxDepth > class TRecursiveFindFiles  
{
public:

	/// Default constructor
	TRecursiveFindFiles()  { m_dwDepth = 0; *m_szSearchMask = 0; }
	
	/// Destructor
	virtual ~TRecursiveFindFiles() {}

	//==============================================================
	// TRecursiveFindFiles()
	//==============================================================
	/// Construcs object and finds the first matching file
	/**
		\param [in] pDir		-	Directory to start search
		\param [in] pMask		-	File search mask
		
		\return Non-zero if a matching file was found
	
		\see 
	*/
	TRecursiveFindFiles( LPCTSTR pDir, LPCTSTR pMask = "*.*" )
	{	m_dwDepth = 0; *m_szSearchMask = 0; FindFirst( pDir, pMask ); }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources used by this class instance
	void Destroy()
	{
		// Validate depth
		if ( c_lMaxDepth <= m_dwDepth )
			m_dwDepth = c_lMaxDepth;

		do 
		{				
			// Destroy this depth
			m_ff[ m_dwDepth ].Destroy();

			// Next depth
			if ( m_dwDepth ) m_dwDepth--;

		} while ( m_dwDepth );
	}

	//==============================================================
	// BuildPaths()
	//==============================================================
	/// Builds paths to current file
	/**		
		\return Non-zero if success
	*/
	BOOL BuildPaths() { return m_ff[ m_dwDepth ].BuildPath(); }

	//==============================================================
	// FindNext()
	//==============================================================
	/// Finds the next matching file
	/**		
		\return Non-zero if another file is found
	*/
	BOOL FindNext()
	{
		// Ensure valid depth
		if ( c_lMaxDepth <= m_dwDepth ) 
			m_dwDepth = c_lMaxDepth - 1;

		// Recurse into directory if this is one
		if ( m_ff[ m_dwDepth ].IsDirectory() )
		{
			// Do we have another valid depth
			DWORD dwNext = m_dwDepth + 1;
			if ( c_lMaxDepth > dwNext )
			{
				// See if we can find any files in this folder
				if ( m_ff[ dwNext ].FindFirst( m_ff[ m_dwDepth ].GetFullPath(), m_szSearchMask ) )
				{
					// Accept this depth as current
					m_dwDepth = dwNext;

					return TRUE;

				} // end if

			} // end if

		} // end if

		// Try to find another file
		for( ; ; )
		{
			// Is there another file at this depth?
			if ( m_ff[ m_dwDepth ].FindNext() )
				return TRUE;

			// Any higher ground?
			if ( !m_dwDepth ) return FALSE;

			// Next depth
			m_dwDepth--;

		} // end for

		// Unreachable...
		return FALSE;
	}

	//==============================================================
	// FindFirst()
	//==============================================================
	/// Finds the first matching file
	/**
		\param [in] pDir		-	Directory to start search
		\param [in] pMask		-	File search mask
		
		\return Non-zero if a matching file was found
	
		\see 
	*/
	BOOL FindFirst( LPCTSTR pDir, LPCTSTR pMask = "*.*" )
	{
		// We have to save the search mask for later
		strcpy_sz( m_szSearchMask, pMask );

		// Pass it on
		return m_ff[ m_dwDepth ].FindFirst( pDir, pMask ); 
	}


	//==============================================================
	// GetPath()
	//==============================================================
	/// Returns the path to the found file
	LPCTSTR GetPath() { return m_ff[ m_dwDepth ].GetPath(); }

	//==============================================================
	// GetFullPath()
	//==============================================================
	/// Returns the complete filename for the found file
	LPCTSTR GetFullPath() { return m_ff[ m_dwDepth ].GetFullPath(); }

	//==============================================================
	// GetFileName()
	//==============================================================
	/// Returns the filename for the current matching file
	LPCTSTR GetFileName() { return m_ff[ m_dwDepth ].GetFileName(); }

	//==============================================================
	// GetFileAttributes()
	//==============================================================
	/// Returns the file attributes of the current matching file
	DWORD GetFileAttributes() { return m_ff[ m_dwDepth ].GetFileAttributes(); }

	//==============================================================
	// IsDirectory()
	//==============================================================
	/// Returns non-zero if the current match is a directory
	BOOL IsDirectory() { return m_ff[ m_dwDepth ].IsDirectory(); }
	
	//==============================================================
	// WIN32_FIND_DATA*()
	//==============================================================
	/// Returns pointer to WIN32_FIND_DATA structure
	operator WIN32_FIND_DATA*() { return m_ff[ m_dwDepth ].Wfd(); }


	//==============================================================
	// Wfd()
	//==============================================================
	/// Returns pointer to WIN32_FIND_DATA structure
	WIN32_FIND_DATA* Wfd() { return m_ff[ m_dwDepth ].Wfd(); }


private:

	/// Current folder depth
	DWORD				m_dwDepth;

	/// CFindFiles object array
	CFindFiles			m_ff[ c_lMaxDepth ];

	/// Search mask
	TCHAR				m_szSearchMask[ CWF_STRSIZE ];

};


#endif // !defined(AFX_FINDFILES_H__ADD77EB0_0605_43FE_B217_FB9D6A429561__INCLUDED_)
