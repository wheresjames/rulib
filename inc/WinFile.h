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
// WinFile.h: interface for the CWinFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINFILE_H__3B34FB3B_B0B7_11D2_88EB_00104B2C9CFA__INCLUDED_)
#define AFX_WINFILE_H__3B34FB3B_B0B7_11D2_88EB_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CWF_STRSIZE		1024
#if( CWF_STRSIZE < MAX_PATH )
#error CWF_STRSIZE is too small!
#endif

//#define CWF_SUPPORTENCRYPTION
#undef CWF_SUPPORTENCRYPTION

#ifdef CWF_SUPPORTENCRYPTION
#include "MD5.h"
#define HASHBUFFERSIZE		32
#endif

#define VERSION( maj, min ) MAKELONG( min, maj )
#define MINVERSION( ver ) LOWORD( ver )
#define MAJVERSION( ver ) HIWORD( ver )

//#define DYNAMIC_CRC_TABLE
#undef DYNAMIC_CRC_TABLE

/// Open file parameters
/**
	This is used by CWinFile

	\see CWinFile
*/
typedef struct tagOPENFILEPARAMS
{
	/// Desired file access
	DWORD					dwDesiredAccess;

	/// File share mode
	DWORD					dwShareMode;

	/// Security attributes
	LPSECURITY_ATTRIBUTES	lpSecurityAttributes;

	/// Create disposition, over-write, etc...
	DWORD					dwCreationDisposition;

	/// File attribute flags
	DWORD					dwFlagsAndAttributes;

	/// Handle to template file
	HANDLE					hTemplateFile;

} OPENFILEPARAMS; // typedef struct
typedef OPENFILEPARAMS* LPOPENFILEPARAMS;

// For getting special folders
#define CSIDL_USER_NONE					0xff00
#define CSIDL_USER_SYSTEM				0xff01
#define CSIDL_USER_WINDOWS				0xff02
#define CSIDL_USER_TEMP					0xff03
#define CSIDL_USER_CURRENT				0xff04
#define CSIDL_USER_DEFDRIVE				0xff05

#define CSIDL_FLAG_VIRTUAL				0x00000001

#define CWINFILE_DEFAULTCACHESIZE		1024

/// Contains url information
/**
	\see CWinFile
*/
typedef struct tagURLINFO
{
	/// CrackUrl() structure
	URL_COMPONENTS	urlc;

	/// Protocol scheme, http, ftp, https, etc...
	char			szScheme[ MAX_PATH ];

	/// Scheme id
	DWORD			dwScheme;

	/// Network host name
	char			szHostName[ MAX_PATH ];

	/// TCP port
	DWORD			dwPort;

	/// Username
	char			szUserName[ MAX_PATH ];

	/// Password
	char			szPassword[ MAX_PATH ];

	/// URL file path
	char			szUrlPath[ MAX_PATH * 4 ];

	/// GET data
	char			szExtraInfo[ MAX_PATH * 4 ];

} URLINFO; // end typedef struct
typedef URLINFO* LPURLINFO;


//==================================================================
// CWinFile
//
/// Windows file API wrapper
/**
	This class wraps the windows file API.  It also provides quite a
	few handy static functions for file and file name manipulation.	
*/
//==================================================================
class CWinFile  
{

public:

	//==============================================================
	// Close()
	//==============================================================
	/// Closes the file
	void Close();

	/// Default constructor
	CWinFile();

	//==============================================================
	// CWinFile()
	//==============================================================
	/// Constructs a CWinFile object wrapping an existing file handle
	/**
		\param [in] hFile	-	Existing file handle
	*/
	CWinFile( HANDLE hFile ) { CWinFile(); Attach( hFile ); }

	/// Destructor
	~CWinFile();

	// For attaching and detaching form file handles
	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches to an existing file handle
	/**
		\param [in] hFile	-	Existing file handle
	*/
	void Attach( HANDLE hFile )
	{	Close(); m_hFile = hFile; }

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from file handle without closing the handle.
	void Detach() { m_hFile = INVALID_HANDLE_VALUE; }

	//==============================================================
	// IsOpen()
	//==============================================================
	/// Returns non-zero if the file is open
	BOOL IsOpen() { return ( m_hFile != INVALID_HANDLE_VALUE ); }
	
	//==============================================================
	// GetHandle()
	//==============================================================
	/// Returns the curent file handle
	HANDLE GetHandle() { return m_hFile; }

	//==============================================================
	// HANDLE()
	//==============================================================
	/// Returns the curent file handle
	operator HANDLE() { return GetHandle(); }

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the size of the current file in bytes
	DWORD Size() 
	{	DWORD size = GetFileSize( m_hFile, NULL ); 
		if ( size == INVALID_FILE_SIZE ) return 0; else return size; }
	
	//==============================================================
	// GetPtrPos()
	//==============================================================
	/// Gets the current 32-bit file pointer position
	/**
		\param [in] ptr		-	Receives the current file pointer position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetPtrPos( LPDWORD ptr );

	//==============================================================
	// GetPtrPos()
	//==============================================================
	/// Gets the current 64-bit file pointer position
	/**
		\param [in] ptr		-	Receives the current file pointer position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetPtrPos( LONGLONG *ptr );

	//==============================================================
	// GetPtrPos()
	//==============================================================
	/// Returns the current 64-bit file pointer position
	/**
		\return 64-bit file pointer position
	
		\see 
	*/
	LONGLONG GetPtrPos();

	//==============================================================
	// SetPtrPos()
	//==============================================================
	/// Sets the 64-bit file pointer position
	/**
		\param [in] offset	-	64-bit file offset pointer
		\param [in] ref		-	Relative specifier for offset
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPtrPos( LONGLONG offset, DWORD ref )
	{	if ( !IsOpen() ) return FALSE;
		LONG lHi = (DWORD)( ( offset >> 32 ) & 0xffffffff );
		return ( SetFilePointer( m_hFile, (LONG)( offset & 0xffffffff ), &lHi, ref ) != 0xffffffff || GetLastError() == NO_ERROR ); 
	}

	//==============================================================
	// SetPtrPosBegin()
	//==============================================================
	/// Sets the 64-bit file pointer position relative to the beginning of the file
	/**
		\param [in] offset	-	64-bit file offset pointer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPtrPosBegin( LONGLONG offset = 0 )
	{	if ( !IsOpen() ) return FALSE; 
		LONG lHi = (DWORD)( ( offset >> 32 ) & 0xffffffff );
		BOOL ret = ( SetFilePointer( m_hFile, (LONG)( offset & 0xffffffff ), &lHi, FILE_BEGIN ) != 0xffffffff || GetLastError() == NO_ERROR ); 
#ifdef CWF_SUPPORTENCRYPTION
		if ( ret ) CryptoSetPos( GetPtrPos() );
#endif
		return ret;
	}

	//==============================================================
	// SetPtrPosEnd()
	//==============================================================
	/// Sets the 64-bit file pointer position relative to the end of the file
	/**
		\param [in] offset	-	64-bit file offset pointer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPtrPosEnd( LONGLONG offset = 0 )
	{	if ( !IsOpen() ) return FALSE;
		LONG lHi = (DWORD)( ( offset >> 32 ) & 0xffffffff );
		BOOL ret = ( SetFilePointer( m_hFile, (LONG)( offset & 0xffffffff ), &lHi, FILE_END ) != 0xffffffff || GetLastError() == NO_ERROR ); 
#ifdef CWF_SUPPORTENCRYPTION
		if ( ret ) CryptoSetPos( GetPtrPos() );
#endif
		return ret;
	}

	//==============================================================
	// SetPtrPosCur()
	//==============================================================
	/// Sets the 64-bit file pointer position relative to the current file pointer
	/**
		\param [in] offset	-	64-bit file offset pointer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPtrPosCur( LONGLONG offset = 0 )
	{	if ( !IsOpen() ) return FALSE;
		LONG lHi = (DWORD)( ( offset >> 32 ) & 0xffffffff );
		BOOL ret = ( SetFilePointer( m_hFile, (LONG)( offset & 0xffffffff ), &lHi, FILE_CURRENT ) != 0xffffffff || GetLastError() == NO_ERROR );
#ifdef CWF_SUPPORTENCRYPTION
		if ( ret ) CryptoSetPos( GetPtrPos() );
#endif
		return ret;
	}


	//==============================================================
	// IsPath()
	//==============================================================
	/// Returns non-zero if there is a non-zero length file path
	BOOL IsPath() { return ( m_szPath[ 0 ] != 0x0 ); }

	//==============================================================
	// SetPath()
	//==============================================================
	/// Sets the current file path
	/**
		\param [in] ptr		-	Pointer to file path string
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPath( LPCTSTR ptr )
	{	if ( IsOpen() ) return FALSE; 
		if ( ptr == NULL ) { m_szPath[ 0 ] = 0x0; return TRUE; }
		strcpy( m_szPath, ptr ); 
		return TRUE; 
	}

	//==============================================================
	// GetPath()
	//==============================================================
	/// Retrieves the current file path string
	/**
		\param [in] ptr		-	Buffer that receives the file path string
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetPath( LPSTR ptr ) 
	{	if ( !IsPath() ) return FALSE; 
		if ( ptr != NULL ) { strcpy( ptr, m_szPath ); return TRUE; } 
		else return FALSE; 
	}
	
	//==============================================================
	// GetPath()
	//==============================================================
	/// Returns a pointer to the current file path
	LPCTSTR GetPath() { return m_szPath; }

	//==============================================================
	// SaveDWORD()
	//==============================================================
	/// Writes a DWORD value into the file
	/**
		\param [in] dw	-	DWORD value to write
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveDWORD( DWORD dw )
	{ return Write( (LPVOID)&dw, sizeof( DWORD ) ); }

	//==============================================================
	// RestoreDWORD()
	//==============================================================
	/// Reads a DWORD value from the current file
	/**
		\param [in] pdw		-	Recieves the DWORD value
		
		\return 
	
		\see 
	*/
	BOOL RestoreDWORD( LPDWORD pdw )
	{ return Read( (LPVOID)pdw, sizeof( DWORD ) ); }

	//==============================================================
	// SetCacheSize()
	//==============================================================
	/// Sets the size of the file cache
	/**
		\param [in] read	-	Read cache size
		\param [in] write	-	Write cache size
	*/
	void SetCacheSize( DWORD read, DWORD write ) 
	{	FlushCache(); m_dwRdCacheSize = read; m_dwWrCacheSize = write; }

	//==============================================================
	// FlushCache()
	//==============================================================
	/// Flushes the file cache
	void FlushCache() { FlushReadCache(); FlushWriteCache(); }

	//==============================================================
	// EnableCache()
	//==============================================================
	/// Enables software file cache
	/**
		\param [in] enable	-	Non-zero to enable the file cache
	*/
	void EnableCache( BOOL enable ) 
	{	FlushCache(); m_bEnableCache = enable; }
	
	//==============================================================
	// Flush()
	//==============================================================
	/// Flushes the Windows file buffers
	void Flush() { FlushFileBuffers( m_hFile ); }

public:

	//==============================================================
	// DeleteDirectory()
	//==============================================================
	/// Deletes the specified directory
	/**
		\param [in] pDir						-	Directory to delete
		\param [in] bEmptyOnly					-	Non-zero to delete only the 
													contents of the specified directory.
		\param [in] bSubDirectories				-	Non-zero to delete the sub all sub
													directories.
		\param [in] bSubDirectoriesAndFilesOnly	-	Deletes only sub directories.  That
													is, it does not delete files in the
													specified directories.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DeleteDirectory( LPCTSTR pDir, BOOL bEmptyOnly = FALSE, BOOL bSubDirectories = TRUE, BOOL bSubDirectoriesAndFilesOnly = FALSE );

    static BOOL sDeleteDirectory( const std::string &sDir )
    {   return DeleteDirectory( sDir.c_str() ); }

	//==============================================================
	// CreateDirectory()
	//==============================================================
	/// Creates the specified directory
	/**
		\param [in] pDir	-	Directory to create

		This can create any number of nested directories.
		
		\return 
	
		\see 
	*/
	static BOOL CreateDirectory( LPCTSTR pDir );

    static BOOL sCreateDirectory( const std::string &sDir )
    {   return CreateDirectory( sDir.c_str() ); }

    static LPCTSTR chreplace( LPSTR pStr, TCHAR chFind, TCHAR chReplace )
    {   for ( long i = 0; i < CWF_STRSIZE && pStr[ i ]; i++ )
            if ( pStr[ i ] == chFind ) pStr[ i ] = chReplace;
        return pStr;
    }

	//==============================================================
	// GetSaveFileName()
	//==============================================================
	/// Uses the windows file name dialog box to retrieve a filename
	/**
		\param [in] pFileName	-	Receives the filename
		\param [in] pTitle		-	Title for dialog box
		\param [in] pFilter		-	File type filters
		\param [in] pDefExt		-	Default extension
		\param [in] hOwner		-	Handle to parent window
		\param [in] bPreview	-	Non-zero for integrated file preview
		\param [in] dwFlags		-	Optional file flags

		Example: 

			char szImage[ MAX_PATH ] = "";
			BOOL bImageFileChoosen = 
				GetSaveFileName( szImage,	"Save image capture",
											"Joint Photographic Experts Group (*.jpg)\x0*.jpg\x0"
											"Portable Network Graphic (*.png)\x0*.png\x0"
											"Windows bitmap (*.bmp)\x0*.bmp\x0",
											"jpg", GetSafeHwnd() );
	  
		\return Non-zero if a file was selected, otherwise zero
	
		\see 
	*/
	static BOOL GetSaveFileName( LPSTR pFileName, LPCTSTR pTitle = NULL, 
								 LPCTSTR pFilter = NULL, LPCTSTR pDefExt = NULL,
								 HWND hOwner = NULL, BOOL bPreview = FALSE,
								 DWORD dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST );

    static std::string sGetSaveFileName( LPCTSTR pFilename, LPCTSTR pTitle, LPCTSTR pFilter )
    {   TCHAR szFile[ CWF_STRSIZE ] = _T( "" );
        if ( pFilename ) strcpy_sz( szFile, pFilename );
        TCHAR szFilter[ CWF_STRSIZE ] = _T( "All Files (*.*)|*.*|" );
        if ( pFilter ) strcpy_sz( szFilter, pFilter );
        chreplace( szFilter, _T( '|' ), _T( '\x0' ) );
        if ( !GetSaveFileName( szFile, pTitle, szFilter ) ) return "";
        return szFile;
    }


	//==============================================================
	// GetOpenFileName()
	//==============================================================
	/// Uses the windows file name dialog box to retrieve a filename
	/**
		\param [in] pFileName	-	Receives the filename
		\param [in] pTitle		-	Title for dialog box
		\param [in] pFilter		-	File type filters
		\param [in] pDefExt		-	Default extension
		\param [in] hOwner		-	Handle to parent window
		\param [in] bPreview	-	Non-zero for integrated file preview
		\param [in] dwFlags		-	Optional file flags
		\param [in] dwBufSize	-	Size of buffer in pFileName

		Example: 

			char szImage[ MAX_PATH ] = "";
			BOOL bImageFileChoosen = 
				GetOpenFileName( szImage,	"Save image capture",
											"Joint Photographic Experts Group (*.jpg)\x0*.jpg\x0"
											"Portable Network Graphic (*.png)\x0*.png\x0"
											"Windows bitmap (*.bmp)\x0*.bmp\x0",
											"jpg", GetSafeHwnd() );
	  
		\return Non-zero if a file was selected, otherwise zero
	
		\see 
	*/
	static BOOL GetOpenFileName( LPSTR pFileName, LPCTSTR pTitle = NULL, 
								 LPCTSTR pFilter = NULL, LPCTSTR pDefExt = NULL,
								 HWND hOwner = NULL, BOOL bPreview = FALSE,
								 DWORD dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
								 DWORD dwBufSize = CWF_STRSIZE );

    static std::string sGetOpenFileName( LPCTSTR pFilename, LPCTSTR pTitle, LPCTSTR pFilter )
    {   TCHAR szFile[ CWF_STRSIZE ] = _T( "" );
        if ( pFilename && *pFilename ) strcpy_sz( szFile, pFilename );
        else GetModulePath( szFile, sizeof( szFile ) );
        TCHAR szFilter[ CWF_STRSIZE ] = _T( "All Files (*.*)|*.*|" );
        if ( pFilter ) strcpy_sz( szFilter, pFilter );
        strcat( szFilter, "||" ); chreplace( szFilter, _T( '|' ), _T( '\x0' ) );
        if ( !GetOpenFileName( szFile, pTitle, szFilter ) ) return "";
        return szFile;
    }


	//==============================================================
	// OpenTemp()
	//==============================================================
	/// Opens a temporary file
	/**
		\return Non-zero if temporary file is opened.
	
		\see 
	*/
	BOOL OpenTemp();

	//==============================================================
	// Delete()
	//==============================================================
	/// Deletes the specified filename
	/**
		\param [in] pFileName	-	Filename to delete
		
		\return Non-zero if file was deleted
	
		\see 
	*/
	static BOOL Delete( LPCTSTR pFileName )
	{ SetFileAttributes( pFileName, FILE_ATTRIBUTE_NORMAL ); return DeleteFile( pFileName ); }

    static BOOL sDelete( const std::string &sFileName )
    {   return Delete( sFileName.c_str() ); }

	//==============================================================
	// CreateTemp()
	//==============================================================
	/// Creates a temporary filename
	/**
		\param [in] pFileName	-	Receives the temporary filename
		
		This function uses the Windows temporary directory unless
		another directory is specified via SetTempPath().  If the
		temporary folder does not exist, it is created.  This function
		does not create the named file.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL CreateTemp( LPSTR pFileName );

	//==============================================================
	// IsRelative()
	//==============================================================
	/// Returns non-zero if the filename contains the relative path specifiers.
	/**
		\param [in] pFileName -	Filename
	*/
	static BOOL IsRelative( LPCTSTR pFileName );

	//==============================================================
	// WebBuildPath()
	//==============================================================
	/// Builds a path using the forward slash '/'
	/**
		\param [in] pFullPath	-	Receives the final filename
		\param [in] pPath		-	Path
		\param [in] pFileName	-	Filename
		
		The same buffer can be used for multiple parameters.

		\return Pointer in pFullPath
	
		\see 
	*/
	static LPCTSTR WebBuildPath( LPSTR pFullPath, LPCTSTR pPath, LPCTSTR pFileName );

	//==============================================================
	// BuildPath()
	//==============================================================
	/// Builds a file path
	/**
		\param [in] pFullPath	-	Receives the final filename
		\param [in] pPath		-	Path
		\param [in] pFileName	-	Filename
		\param [in] sep			-	Path separator, Backslash is the default '\'
		
		The same buffer can be used for multiple parameters.

		\return Pointer in pFullPath
	
		\see 
	*/
	static LPCTSTR BuildPath( LPSTR pFullPath, LPCTSTR pPath, LPCTSTR pFileName, char sep = '\\' );

    static std::string sBuildPath( const std::string &sPath, const std::string &sFile )
    {   std::vector< TCHAR > buf( CWF_STRSIZE + sPath.length() + sFile.length() );
        BuildPath( &buf[ 0 ], sPath.c_str(), sFile.c_str() ); return &buf[ 0 ]; 
    }

    static std::string sGetContents( const std::string &sFile )
    {   CWinFile f; if ( !f.OpenExisting( sFile.c_str() ) ) return "";
        std::vector< TCHAR > buf( f.Size() + 1 );
        DWORD dwRead = 0; f.Read( &buf[ 0 ], f.Size(), &dwRead );
        buf[ dwRead ] = 0; return &buf[ 0 ];
    }

    static UINT sPutContents( const std::string &sFile, const std::string &sContents )
    {   CWinFile f; if ( !f.OpenNew( sFile.c_str() ) ) return FALSE;
        DWORD dwWritten = 0; 
        if ( !f.Write( sContents.c_str(), sContents.length(), &dwWritten ) )
            return 0;
        return dwWritten;
    }

	//==============================================================
	// GetPathFromFileName()
	//==============================================================
	/// Extracts the path from the specified filename
	/**
		\param [in] pFileName	-	Full path to file
		\param [out] pPath		-	Receives xtracted path
		
		\return Non-zero if a path was extracted.
	
		\see 
	*/
	static BOOL GetPathFromFileName( LPCTSTR pFileName, LPSTR pPath );

    static std::string sGetPathFromFileName( LPCTSTR pFile )
    {   TCHAR szBuf[ CWF_STRSIZE ] = _T( "" );
        GetPathFromFileName( pFile, szBuf );
        return szBuf;
    }

	//==============================================================
	// GetFileNameFromPath()
	//==============================================================
	/// Extracts the filename from a specified file path
	/**
		\param [in] pPath		-	The full path to file
		\param [out] pFileName	-	Receives the extracted path
		\param [in] bNoExt		-	If non-zero, the extension is stripped
									from the filename.
	*/
	static void GetFileNameFromPath( LPCTSTR pPath, LPSTR pFileName, BOOL bNoExt = FALSE );

    static std::string sGetFileNameFromPath( LPCTSTR pPath )
    {   TCHAR szBuf[ CWF_STRSIZE ] = _T( "" );
        GetFileNameFromPath( pPath, szBuf );
        return szBuf;
    }

	//==============================================================
	// GetRelativePath
	//==============================================================
	/// Returns the relative path from pPath1 to pPath2
	/**
		\param [out] pRelativePath  -   Receives the relative path.
		\param [in] pPath1	        -	From path
		\param [in] pPath2		    -	To path
									    
	*/
	static BOOL GetRelativePath( LPSTR pRelativePath, LPCTSTR pPath1, LPCTSTR pPath2 );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies data from the specified file
	/**
		\param [in] hFile		-	Handle to open file
		\param [in] dwCopy		-	Number of bytes to copy
		\param [out] pdwCopied	-	Number of bytes actually copied
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( HANDLE hFile, DWORD dwCopy, LPDWORD pdwCopied = NULL);

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies data from one file to another
	/**
		\param [in] hSource		-	Source file handle
		\param [out] hTarget	-	Destination file handle
		\param [in] dwCopy		-	Number of bytes to copy
		\param [out] pdwCopied	-	Number of bytes actually copied
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Copy( HANDLE hSource, HANDLE hTarget, DWORD dwCopy, LPDWORD pdwCopied = NULL );

	//==============================================================
	// CopyFile()
	//==============================================================
	/// Copies the entire contents of the specified file handle
	/**
		\param [in] hFile	-	Handle to open file
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CopyFile( HANDLE hFile );

	//==============================================================
	// CopyFile()
	//==============================================================
	/// Copies the entire contents of one file into another 
	/**
		\param [in] hSource		-	Source file handle
		\param [out] hTarget	-	Target file handle
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL CopyFile( HANDLE hSource, HANDLE hTarget );

	//==============================================================
	// RestoreStruct()
	//==============================================================
	/// Reads file data into a buffer pointer
	/**
		\param [out] pStruct	-	Receives file data
		\param [in] dwLen		-	Number of bytes to copy
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RestoreStruct( LPVOID pStruct, DWORD dwLen = 0 );

	//==============================================================
	// SaveStruct()
	//==============================================================
	/// Writes data from buffer into file
	/**
		\param [in] pStruct		-	Pointer to data
		\param [in] dwLen		-	Number of bytes to copy from pStruct
		
		\return 
	
		\see 
	*/
	BOOL SaveStruct( LPVOID pStruct, DWORD dwLen = 0 );

	//==============================================================
	// Write()
	//==============================================================
	/// Writes data into file
	/**
		\param [in] pBuf			-	Pointer to data to write
		\param [in] dwLen			-	Number of bytes in pBuf to write
		\param [out] pdwWritten		-	Number of bytes written to file
		\param [in] bBypassCache	-	Non-zero to bypass disk cache
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL Write( const void * pBuf, DWORD dwLen = 0, LPDWORD pdwWritten = NULL, BOOL bBypassCache = FALSE );

	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from file
	/**
		\param [out] pBuf			-	Receives data read from file
		\param [in] dwLen			-	Size of buffer in pBuf
		\param [out] pdwRead		-	Receives number of bytes read
		\param [in] bBypassCache	-	Non-zero to bypass disk cache
		
		\return 
	
		\see 
	*/
	virtual BOOL Read( LPVOID pBuf, DWORD dwLen, LPDWORD pdwRead = NULL, BOOL bBypassCache = FALSE );
	
	//==============================================================
	// Open()
	//==============================================================
	/// Opens the specified file
	/**
		\param [in] pFilename	-	Filename to open
		\param [in] access		-	Desired access, 0xffffffff for default access.
		
		\return Non-zero if file is opened
	
		\see 
	*/
	virtual BOOL Open( LPCTSTR pFilename = NULL, DWORD access = 0xffffffff );

	//==============================================================
	// OpenExisting()
	//==============================================================
	/// Opens an existing file.  Fails if file does not exist.
	/**
		\param [in] pFilename	-	Filename to open
		\param [in] access		-	Desired access, 0xffffffff for default access.
		
		\return Non-zero if a file is opened.
	
		\see 
	*/
	BOOL OpenExisting( LPCTSTR pFilename = NULL, DWORD access = 0xffffffff );

	//==============================================================
	// OpenNew()
	//==============================================================
	/// Opens a new file.  If file exists it is truncated.
	/**
		\param [in] pFilename	-	Filename to open
		\param [in] access		-	Desired access, 0xffffffff for default access.
		
		\return Non-zero if a file is opened.
	
		\see 
	*/
	BOOL OpenNew( LPCTSTR pFilename = NULL, DWORD access = 0xffffffff );

	//==============================================================
	// OpenAlways()
	//==============================================================
	/// Opens new or existing file.
	/**
		\param [in] pFilename	-	Filename to open
		\param [in] access		-	Desired access, 0xffffffff for default access.

		If the file does not exist it is created.  If the file exists,
		it is opened and the file pointer is set to the end of the file.
	  
		\return Non-zero if a file is opened.
	
		\see 
	*/
	BOOL OpenAlways( LPCTSTR pFilename = NULL, DWORD access = 0xffffffff );

	//==============================================================
	// DefaultOpenParams()
	//==============================================================
	/// Sets default file open parameters
	void DefaultOpenParams();

	/// File open parameters
	OPENFILEPARAMS		m_ofp;

	/// File security attributes
	SECURITY_ATTRIBUTES	m_saSecurityAttributes;

public:

	//==============================================================
	// VerboseCleanup()
	//==============================================================
	/// Attempts to delete temporary directories and files
	/**
		\param [in] hWnd		-	Handle to parent window. Can be NULL.
		\param [in] dwAttempts	-	Number of attempts to delete
		
		This function attempts to delete named temporary files and 
		folders.  If there is a failure, the function prompts the user
		for desired action with a standard windows retry/cancel/ignore
		dialog box.

		\return Non-zero if all directories and files are deleted.
	
		\see 
	*/
	static BOOL VerboseCleanup( HWND hWnd = NULL, DWORD dwAttempts = 5 );

	//==============================================================
	// CreateTempDir()
	//==============================================================
	/// Creates the named temporary directory
	/**
		\param [in] pDir	-	Temporary directory
		
		This directory will be used to hold temporary files.  It will
		also be the target of a cleanup.

		\return Non-zero if directory is created or already exists.
	
		\see 
	*/
	static BOOL CreateTempDir( LPSTR pDir );

	//==============================================================
	// IsTempPath()
	//==============================================================
	/// Returns non-zero if a custom temporary path is specified
	static BOOL IsTempPath() { return ( *m_szTempPath != 0x0 ); }

	//==============================================================
	// CleanTempPath()
	//==============================================================
	/// Deletes temporary files from custom temporary path
	/**
		\return Non-zero if files are deleted
	*/
	static BOOL CleanTempPath();

	//==============================================================
	// DeleteTempPath()
	//==============================================================
	/// Deletes the custom temporary path and all contained files
	/**
		\return Non-zero if deleted
	*/
	static BOOL DeleteTempPath();

	//==============================================================
	// SetTempPath()
	//==============================================================
	/// Sets the custom temporary path
	/**
		\param [in] pPath	-	Temporary path 
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL SetTempPath( LPCTSTR pPath = NULL );

	//==============================================================
	// GetTempPath()
	//==============================================================
	/// Returns a pointer to the custom temporary path
	static LPCTSTR GetTempPath() { return m_szTempPath; }

public:

	//==============================================================
	// FlushWriteCache()
	//==============================================================
	/// Flushes the software write cache
	void FlushWriteCache();

	//==============================================================
	// FlushReadCache()
	//==============================================================
	/// Flushes the software read cache
	void FlushReadCache();

	//==============================================================
	// ReadCache()
	//==============================================================
	/// Reads data from the cache
	/**
		\param [out] pBuffer	-	Receives data
		\param [in] dwLength	-	Length of buffer in pBuffer
		\param [out] pdwRead	-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadCache( LPVOID pBuffer, DWORD dwLength, LPDWORD pdwRead );

	//==============================================================
	// WriteCache()
	//==============================================================
	/// Writes data to cache
	/**
		\param [in] pBuffer		-	Pointer to data
		\param [in] dwLength	-	Number of bytes in pBuffer
		\param [out] pdwWritten	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL WriteCache( const void * pBuffer, DWORD dwLength, LPDWORD pdwWritten );

	//==============================================================
	// DoesExist()
	//==============================================================
	/// Returns non-zero if the specified file or directory exists
	/**
		\param [in] pFilename	-	Filename
		
		\return non-zero if file or directory exists
	
		\see 
	*/
	static BOOL DoesExist( LPCTSTR pFilename );

	//==============================================================
	// GetExtension()
	//==============================================================
	/// Returns a pointer to the file extension in pFile
	/**
		\param [in] pFile	-	Filename

		The returned pointer is actually an offset into pFile.
		
		\return Pointer to extension if found, otherwise NULL
	
		\see 
	*/
	static LPCTSTR GetExtension( LPCTSTR pFile );

	//==============================================================
	// GetBrowseDir()
	//==============================================================
	/// Displays the Windows Folder picker
	/**
		\param [in] hWnd	-	Handle to parent window
		\param [in] pTitle	-	Title for picker window
		\param [out] pDir	-	Receives the folder name
		\param [in] pInit	-	Initial folder, can be NULL
		
		\return Non-zero if a folder is selected
	
		\see 
	*/
	static BOOL GetBrowseDir( HWND hWnd, LPCTSTR pTitle, LPSTR pDir, LPCTSTR pInit = NULL );

	//==============================================================
	// BrowseCallbackProc()
	//==============================================================
	/// Callback procedure used by GetBrowseDir() to set the initial folder
	/**
		\param [in] hwnd	-	Handle to window
		\param [in] uMsg	-	Message ID
		\param [in] lParam	-	WPARAM
		\param [in] lpData	-	LPARAM
		
		\return Windows return code
	
		\see GetBrowseDir()
	*/
	static int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData ); 

	//==============================================================
	// GetSpecialFolder()
	//==============================================================
	/// Returns the path to special windows folders
	/**
		\param [in] nID		-	Special folder ID
		\param [out] buf	-	Receives the path to the specified folder
		
		Codes with possible example folders...
			
			<b>Extra folders</b>

			-	CSIDL_USER_SYSTEM	-	System 32 folder
										-	C:\\Windows\\System32

			-	CSIDL_USER_WINDOWS	-	Windows folder
										-	C:\\Windows

			-	CSIDL_USER_TEMP		-	Windows temporary directory
										-	C:\\Temp

			-	CSIDL_USER_CURRENT	-	Current folder
					
			-	CSIDL_USER_DEFDRIVE	-	Drive that windows is installed on.
										-	C:\\

			<b>Default folders</b>
			
			-	CSIDL_ALTSTARTUP				-	File system directory that corresponds to the 
													user's nonlocalized Startup program group. 

			-	CSIDL_APPDATA					-	File system directory that serves as a common 
													repository for application-specific data. 
			
			-	CSIDL_BITBUCKET					-	File system directory containing file objects 
													in the user's Recycle Bin. The location of this 
													directory is not in the registry; it is marked 
													with the hidden and system attributes to prevent
													the user from moving or deleting it. 
			
			-	CSIDL_COMMON_ALTSTARTUP			-	File system directory that corresponds to the 
													nonlocalized Startup program group for all users. 
			
			-	CSIDL_COMMON_DESKTOPDIRECTORY	-	File system directory that contains files and 
													folders that appear on the desktop for all users. 
			
			-	CSIDL_COMMON_FAVORITES			-	File system directory that serves as a common 
													repository for all users' favorite items. 
			
			-	CSIDL_COMMON_PROGRAMS			-	File system directory that contains the 
													directories for the common program groups that 
													appear on the Start menu for all users. 
			
			-	CSIDL_COMMON_STARTMENU			-	File system directory that contains the programs 
													and folders that appear on the Start menu for 
													all users. 
			
			-	CSIDL_COMMON_STARTUP			-	File system directory that contains the programs 
													that appear in the Startup folder for all users.  
			
			-	CSIDL_CONTROLS					-	Virtual folder containing icons for the Control 
													Panel applications. 
			
			-	CSIDL_COOKIES					-	File system directory that serves as a common 
													repository for Internet cookies. 
			
			-	CSIDL_DESKTOP					-	Windows Desktop—virtual folder at the root of 
													the namespace. 
			
			-	CSIDL_DESKTOPDIRECTORY			-	File system directory used to physically store 
													file objects on the desktop (not to be confused 
													with the desktop folder itself). 
			
			-	CSIDL_DRIVES					-	My Computer—virtual folder containing everything 
													on the local computer: storage devices, printers,
													and Control Panel. The folder may also contain 
													mapped network drives. 
			
			-	CSIDL_FAVORITES					-	File system directory that serves as a common 
													repository for the user's favorite items. 
			
			-	CSIDL_FONTS						-	Virtual folder containing fonts. 
			
			-	CSIDL_HISTORY					-	File system directory that serves as a common 
													repository for Internet history items. 
			
			-	CSIDL_INTERNET					-	Virtual folder representing the Internet. 
			
			-	CSIDL_INTERNET_CACHE			-	File system directory that serves as a common 
													repository for temporary Internet files. 
			
			-	CSIDL_NETHOOD					-	File system directory containing objects that 
													appear in the network neighborhood. 
			
			-	CSIDL_NETWORK					-	Network Neighborhood Folder—virtual folder 
													representing the top level of the network 
													hierarchy. 
			
			-	CSIDL_PERSONAL					-	File system directory that serves as a common 
													repository for documents. 
			
			-	CSIDL_PRINTERS					-	Virtual folder containing installed printers. 
			
			-	CSIDL_PRINTHOOD					-	File system directory that serves as a common 
													repository for printer links. 
			
			-	CSIDL_PROGRAMS					-	File system directory that contains the user's 
													program groups (which are also file system 
													directories). 
			
			-	CSIDL_RECENT					-	File system directory that contains the user's 
													most recently used documents. 
			
			-	CSIDL_SENDTO					-	File system directory that contains Send To 
													menu items. 
			
			-	CSIDL_STARTMENU					-	File system directory containing Start menu 
													items. 
			
			-	CSIDL_STARTUP					-	File system directory that corresponds to the 
													user's Startup program group. The system starts 
													these programs whenever any user logs onto 
													Windows NT or starts Windows 95. 
			
			-	CSIDL_TEMPLATES					-	File system directory that serves as a common 
													repository for document templates. 

		\return Non-zero if valid folder is returned.
	
		\see 
	*/
	static BOOL GetSpecialFolder( int nID, LPSTR buf );

    static std::string sGetSpecialFolder( long lID )
    {   TCHAR szBuf[ CWF_STRSIZE ] = _T( "" );
        if ( !GetSpecialFolder( (int)lID, szBuf ) )
			return _T( "" );
        return szBuf;
    }


	//==============================================================
	// ShowLastError()
	//==============================================================
	/// Displays the last Windows error returned by ::GetLastError()
	/**
		\param [in] pTitle	-	Title for the dialog box.
		\param [in] pAdd	-	String to add to end of error
		\param [in] type	-	Dialog box type
		
		\return Return code from dialog box
	
		\see 
	*/
	static int ShowLastError( LPCTSTR pTitle, LPCTSTR pAdd = NULL, UINT type = MB_OK | MB_ICONEXCLAMATION);

	//==============================================================
	// GetSystemErrorMsg()
	//==============================================================
	/// Returns a human readable string describing a Windows error code
	/**
		\param [in] err			-	Windows error code
		\param [out] pMsg		-	Receives the error string
		\param [in] pTemplate	-	Defines the format of the returned string
		
		\return Non-zero if err is a valid Windows error code
	
		\see 
	*/
	static BOOL GetSystemErrorMsg( DWORD err, LPSTR pMsg, LPCTSTR pTemplate = "Error Code: %lu\r\n%s" );

	//==============================================================
	// GetDiskSpace()
	//==============================================================
	/// Returns the total disk space on the specified drive
	/**
		\param [in] pDrive	-	Drive to query
		\param [out] total	-	Receives the total disk space
		\param [out] free	-	Receives the free disk space
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetDiskSpace( LPCTSTR pDrive, double *total, double *free );

	//==============================================================
	// CreateSizeString()
	//==============================================================
	/// Creates a shortened string representing the specified quantity
	/**
		\param [out] pString	-	Receives the shortened string
		\param [in] size		-	Value to convert

		Example returned strings:
				
			-	1.32 MB
			-	345 KB
			-	534.3 GB
			-	600 TB
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL CreateSizeString( LPSTR pString, double size );

#ifdef CWF_SUPPORTENCRYPTION

	//==============================================================
	// CryptoSetKey()
	//==============================================================
	/// Sets a string to seed the encrpytion key
	/**
		\param [in] key		-	Encryption key seed
	*/
	void CryptoSetKey( LPCTSTR key ) 
	{	strcpy_sz( m_szKey, key ); }

	//==============================================================
	// CryptoSetPos()
	//==============================================================
	/// Sets the position of the encoder / decoder buffer
	/**
		\param [in] pos		-	File position
	*/
	void CryptoSetPos( DWORD pos );

	//==============================================================
	// CryptoDoHash()
	//==============================================================
	/// Calculates the hash for the current file position
	/**
		\param [in] buf		-	Generates hash for buffer
		\param [in] size	-	Number of bytes in buf
		
		\return 
	
		\see 
	*/
	void CryptoDoHash( LPBYTE buf, DWORD size );

	//==============================================================
	// CryptoRefillHashBuffer()
	//==============================================================
	/// Fills the file hash buffer when it runs out
	void CryptoRefillHashBuffer();
#else
	void CryptoSetKey( LPCTSTR key ) {}
	void CryptoSetPos( DWORD pos ) {}
	void CryptoDoHash( LPBYTE buf, DWORD size ) {}
	void CryptoRefillHashBuffer() {}
#endif

private:

	/// Handle to encapsulated file
	HANDLE				m_hFile;

	/// Full file path
	char				m_szPath[ CWF_STRSIZE ];

	/// Path to temporary folder
	static char			m_szTempPath[ CWF_STRSIZE ];

	// Non-zero to enable file cache
	BOOL				m_bEnableCache;

	/// Write cache position pointer
	DWORD				m_dwWrCachePtr;

	/// Pointer to write cache buffer
	LPBYTE				m_pucWrCache;

	/// Size of buffer in m_pucWrCache
	DWORD				m_dwWrCacheSize;

	/// Read cache position pointer
	DWORD				m_dwRdCachePtr;

	/// Number of bytes in the read caceh
	DWORD				m_dwRdCacheBytes;

	/// Pointer to read cache buffer
	LPBYTE				m_pucRdCache;

	/// Size of buffer in m_pucRdCache
	DWORD				m_dwRdCacheSize;

#ifdef CWF_SUPPORTENCRYPTION

	/// Calculates hash for encryption
	CMD5	m_md5;

	/// Offset into hash buffer
	DWORD	m_dwHashPtr;

	/// Hash buffer
	BYTE	m_pHashBuf[ HASHBUFFERSIZE ];

	/// Hash seed
	char	m_szKey[ CWF_STRSIZE ];

#endif

public:

	//==============================================================
	// GetCacheFileName()
	//==============================================================
	/// Generates a repeatable but indecipherable file name from pFilename
	/**
		\param [in] pFilename		-	Filename
		\param [in] pDir			-	Direcory name
		\param [out] pCacheFilename	-	Receives the encoded file name
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetCacheFileName( LPCTSTR pFilename, LPCTSTR pDir, LPSTR pCacheFilename );

	//==============================================================
	// Crc16()
	//==============================================================
	/// Updates a 16-bit CRC
	/**
		\param [in] b		-	Source byte for updating
		\param [in] crc		-	Current CRC
		
		\return New CRC
	
		\see 
	*/
	static UINT Crc16( BYTE b, UINT crc )
	{	UINT y;
		for( y = 0; y < 8; y++ ) 
		{	if ( ( crc ^ b ) & 1 ) { crc >>= 1; crc ^= 0x8408; }
			else { crc >>= 1; }
			b >>= 1; 
		}
		return crc;
	}

	//==============================================================
	// MatchPattern()
	//==============================================================
	/// Attempts to match filename to a pattern
	/**
		\param [in] pFile		-	Filename
		\param [in] pPattern	-	Pattern for matching
		
		Uses standard wild card characters

		Example patterns:
				
			-	*.*
			-	*.jpg
			-	Backup???.bak
			-	C:\\html_files\\Bob*.htm

		\return Non-zero if filename matches the pattern
	
		\see 
	*/
	static BOOL MatchPattern( LPCTSTR pFile, LPCTSTR pPattern );

	//==============================================================
	// MatchPatterns()
	//==============================================================
	/// Attempts to match filename against multiple patterns
	/**
		\param [in] pFile		-	Filename
		\param [in] pPattern	-	Pattern for matching
		\param [in] cSep		-	Character separating patterns.  Defaults to ';'
		\param [in] bAll		-	If non-zero, filename must match ALL patterns.
		
		Uses standard wild card characters

		Example patterns:
				
			-	*.*
			-	*.jpg
			-	Backup???.bak
			-	C:\\html_files\\Bob*.htm
  
		\return If bAll is zero, returns non-zero if the filename matches <b>any</b> of the patterns.
				If bAll is non-zero, returns non-zero if the filename matches <b>all</b> of the patterns.
	
		\see 
	*/
	static BOOL MatchPatterns(LPCTSTR pFile, LPCTSTR pPattern, char cSep = ';', BOOL bAll = FALSE);

	//==============================================================
	// IsValidPathChar()
	//==============================================================
	/// Returns non-zero if the character is a valid file path character
	/**
		\param [in] ch	-	Character to test

		This function returns zero if the character cannot be used
		in a Windows file path.
		
		\return Non-zero if ch is a valid file path character.
	
		\see 
	*/
	static BOOL IsValidPathChar( BYTE ch );

	//==============================================================
	// IsValidFileChar()
	//==============================================================
	/// Returns non-zero if the character is a valid filename character
	/**
		\param [in] ch	-	Character to test

		This function returns zero if the character cannot be used
		in a Windows filename.
		
		\return Non-zero if ch is a valid filename character.
	
		\see 
	*/
	static BOOL IsValidFileChar( BYTE ch );

	//==============================================================
	// GetFileCount()
	//==============================================================
	/// 
	/**
		\param [in] pPath 
		\param [in] pMask 
		
		\return 
	
		\see 
	*/
	static DWORD GetFileCount( LPCTSTR pPath, LPCTSTR pMask = "*.*" );

	//==============================================================
	// CopyFolder()
	//==============================================================
	/// Copies the files in one folder to another
	/**
		\param [in] pSrc			-	Source folder name
		\param [in] pDst			-	Destination folder name
		\param [in] bSubFolders		-	Non-zero to copy sub folders
		
		\return Non-zero if files are copied
	
		\see 
	*/
	static BOOL CopyFolder( LPCTSTR pSrc, LPCTSTR pDst, BOOL bSubFolders = TRUE );

	//==============================================================
	// IsFile()
	//==============================================================
	/// Returns non-zero if the named path is a file.
	/**
		\param [in] pFile	-	Filename

		This function returns zero if the named path is a directory.
		
		\return Non-zero if the named path is a file
	
		\see 
	*/
	static BOOL IsFile( LPCTSTR pFile );

	//==============================================================
	// CmpExt()
	//==============================================================
	/// Compares the named files extension
	/**
		\param [in] pFile	-	Filename
		\param [in] pExt	-	Extension
		\param [in] len		-	Number of bytes in pExt to compare
		
		Example:
		\code			
			if ( CmpExt( "Airplane.jpg", "jpg" ) )
				; // Named file is a JPEG image
		\endcode

		\return Non-zero if match
	
		\see 
	*/
	static BOOL CmpExt( LPCTSTR pFile, LPCTSTR pExt, DWORD len = 3 );

	//==============================================================
	// GetPathElement()
	//==============================================================
	/// Returns the specified path element
	/**
		\param [out] pElement	-	Receives the element
		\param [in] pPath		-	Path to analyze
		\param [in] dwElement	-	Index of the element
		\param [out] pLeft		-	Number of elements remaining
		\param [in] bReverse	-	Reverse index direction

		Examples:

		\code

		char element[ 256 ];

		GetPathElement( element, "one/two/three/four", 1 );

		// element contains "two"

		GetPathElement( element, "one/two/three/four", 1, NULL, TRUE );

		// element contains "three"
			
		\endcode
		
		\return 
	
		\see 
	*/
	static LPCTSTR GetPathElement( LPSTR pElement, LPCTSTR pPath, DWORD dwElement, LPDWORD pLeft = NULL, BOOL bReverse = FALSE );
	
	//==============================================================
	// Quote()
	//==============================================================
	/// Quotes a given string escaping as needed
	/**
		\param [out] dst	-	Receives quoted string
		\param [in] src		-	Unquoted string
		\param [in] ch		-	Quote character
		\param [in] esc		-	Escape character
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Quote( LPSTR dst, LPCTSTR src = NULL, char ch = '\"', char esc = '\\' );

	//==============================================================
	// Unquote()
	//==============================================================
	/// Unquotes a string, converting escape sequences as needed
	/**
		\param [out] dst	-	Receives unquoted string
		\param [in] src		-	Quoted string
		\param [in] ch		-	Quote character
		\param [in] esc		-	Escape character
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Unquote( LPSTR dst, LPCTSTR src = NULL, char ch = '\"', char esc = '\\' );
	
	//==============================================================
	// ExtractResource()
	//==============================================================
	/// Extracts resource data from the specified module to named file
	/**
		\param [in] pFile		-	Module filename
		\param [in] pResource	-	Resource name
		\param [in] pType		-	Resource type
		\param [in] hModule		-	Module handle
		\param [in] bOverwrite	-	Non-zero to overwrite existing file
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ExtractResource( LPCTSTR pFile, LPCTSTR pResource, LPCTSTR pType, HMODULE hModule = NULL, BOOL bOverwrite = TRUE );
	
	//==============================================================
	// ChangeExtension()
	//==============================================================
	/// Changes the extension of the specified file
	/**
		\param [in,out] pFile	-	Filename whose extension is changed
		\param [in] pExt		-	New extension
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ChangeExtension( LPSTR pFile, LPCTSTR pExt );
	
	//==============================================================
	// GetSpecialTextName()
	//==============================================================
	/// Gets the name of specified special text
	/**
		\param [in] i		-	Index of special text
		\param [out] buf	-	Receives special text name
		\param [in] size	-	Size of buffer in buf
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetSpecialTextName( DWORD i, LPSTR buf, DWORD size );
	
	//==============================================================
	// GetSpecialText()
	//==============================================================
	/// Gets the specified special text
	/**
		\param [in] i		-	Index of special text
		\param [out] buf	-	Receives special text
		\param [in] size	-	Size of buffer in buf
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetSpecialText( DWORD i, LPSTR buf, DWORD size );
	
	//==============================================================
	// GetSubPath()
	//==============================================================
	/// Gets specified depth sub path
	/**
		\param [in] dwFolders	-	Depth to retreive
		\param [in] pPath		-	Source path
		\param [out] pSub		-	Receives sub path

		Example:
		\code
		
		char szPath[ 256 ];

		GetSubPath( 2, "C:\\one\\two\\three", szPath );

		// szPath contains "two\\three"

		\endcode
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetSubPath( DWORD dwFolders, LPCTSTR pPath, LPSTR pSub );
	
	//==============================================================
	// GetRootPath()
	//==============================================================
	/// 
	/**
		\param [in] dwFolders	-	Depth to retreive
		\param [in] pPath		-	Source path
		\param [out] pRoot		-	Receives root path

		Example:
		\code
		
		char szPath[ 256 ];

		GetSubPath( 2, "C:\\one\\two\\three", szPath );

		// szPath contains "C:\\one"

		\endcode
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetRootPath( DWORD dwFolders, LPCTSTR pPath, LPSTR pRoot );
	
	//==============================================================
	// GetIconForFile()
	//==============================================================
	/// Returns the system icon for the specified filename
	/**
		\param [in] pFile		-	Filename
		\param [in] bLargeIcon	-	Non-zero for large icon, zero for
									small icon.
		
		Creates copy of the system icon.  When the icon is no longer
		needed, delete it with ::DestroyIcon().

		\return Handle to system icon for file
	
		\see 
	*/
	static HICON GetIconForFile( LPCTSTR pFile, BOOL bLargeIcon = TRUE );
	
	//==============================================================
	// TokenFromTextString()
	//==============================================================
	/// Grabs a token from a text string 
	/**
		\param [in] pText				-	Text string
		\param [out] pToken				-	Receives token
		\param [in] pdwNext				-	Offset to next token
		\param [in] bUseDOSCommandRules -	Non zero to use DOS command
											line rules.
		
		\return Non-zero if token is copied
	
		\see 
	*/
	static BOOL TokenFromTextString( LPCTSTR pText, LPSTR pToken, LPDWORD pdwNext = NULL, BOOL bUseDOSCommandRules = TRUE);
	
	//==============================================================
	// GetExePath()
	//==============================================================
	/// Gets the folder path for the current application
	/**
		\param [in] buf		-	Receives application folder
		\param [in] size	-	Size of buffer in buf
		
		If the application path is "c:/myprogram/myapp.exe"

		This function returns "c:/myprogram"

		\return Pointer in buf
	
		\see 
	*/
	static LPCTSTR GetExePath( LPSTR buf, DWORD size = CWF_STRSIZE );
	
	//==============================================================
	// GetModulePath()
	//==============================================================
	/// Gets the folder path for the current module
	/**
		\param [in] buf		-	Receives application folder
		\param [in] size	-	Size of buffer in buf
		
		If the module path is "c:/myprogram/myapp.dll"

		This function returns "c:/myprogram"

		\return Pointer in buf
	
		\see 
	*/
	static LPCTSTR GetModulePath( LPSTR buf, DWORD size = CWF_STRSIZE );
	
    static std::string sGetModulePath()
    {   TCHAR szPath[ CWF_STRSIZE ] = _T( "" );
        return GetModulePath( szPath, sizeof( szPath ) );
    }

	//==============================================================
	// GetUniqueFileName()
	//==============================================================
	/// Appends numbers to file to achieve a unique file name
	/**
		\param [out] pUnique	-	Receives unique file name
		\param [in] pFile		-	Filename
		\param [in] bNoExt		-	Set to non-zero to strip the extension
		
		Example:

		\code

		char szPath[ 256 ];

		GetUniqueFileName( szPath, "Image.jpg" );

		// szPath may contain "Image (2).jpg" if "Image.jpg" and 
		// "Image (1).jpg" already exist.

		\endcode

		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetUniqueFileName( LPSTR pUnique, LPCTSTR pFile, BOOL bNoExt = FALSE );
	
	//==============================================================
	// CleanChars()
	//==============================================================
	///	Replaces invalid file name characters with substitutes
	/**
		\param [in] pFile	-	Filename to clean
		
		\code

		char szPath[ 256 ] = "This ? file | name < contains > illegal * characters.txt";
		
		CleanChars( szPath );

		// szPath now contains "This _ file ! name [ contains ] illegal - characters.txt"
		// which is a valid file name

		\endcode

		\return Non-zero if success
	
		\see 
	*/
	static BOOL CleanChars( LPSTR pFile );
	
	//==============================================================
	// CrackUrl()
	//==============================================================
	/// Breaks the specified URL down into it's components
	/**
		\param [in] pUrl	-	URL to decompose
		\param [in] pui		-	Structure containing URL components
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL CrackUrl( LPCTSTR pUrl, LPURLINFO pui );
	
	//==============================================================
	// CreateLink()
	//==============================================================
	/// Creates a shortcut to the specified item
	/**
		\param [in] pPath	-	Path to file object
		\param [in] pLink	-	Shortcut path
		\param [in] pDesc	-	Description for shortcut
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL CreateLink( LPCTSTR pPath, LPCTSTR pLink, LPCTSTR pDesc );
	
	//==============================================================
	// GetMultiSelectFileName()
	//==============================================================
	/// Gets individual file names returned from multiple selected files
	/**
		\param [in] pFilenames	-	Multiple file names
		\param [in] dwMax		-	Size of buffer in pFilenames
		\param [out] pFile		-	Receives full path to single file
		\param [in] dwIndex		-	Index of the file to return
		\param [in] ucSep		-	Separator between file names

		The GetSaveFileName() and GetOpenFileName() functions can return
		multiple files.  This allows you to build the complete path for 
		each file.

		To get each file, call this function incrementing dwIndex each
		time until it returns false.
		
		\return Non-zero if valid file name is returned
	
		\see 
	*/
	static BOOL GetMultiSelectFileName( LPCTSTR pFilenames, DWORD dwMax, LPSTR pFile, DWORD dwIndex, BYTE ucSep = 0 );
	
	//==============================================================
	// DeletePath()
	//==============================================================
	/// Removes the specified path, can delay until reboot if in use
	/**
		\param [in] pPath		-	Path to folder or file to delete
		\param [in] bReboot		-	If non-zero, and pPath is in use,
									pPath is deleted at the next 
									system reboot.
		\param [in] bEmptyOnly	-	Deletes the named path only if it
									is empty.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DeletePath( LPCTSTR pPath, BOOL bReboot, BOOL bEmptyOnly );
	
	//==============================================================
	// GetSpecialFolderName()
	//==============================================================
	/// Gets the human readable name of a Windows special folder
	/**
		\param [in] nID			-	Special folder ID
		\param [out] pName		-	Receives the name of the special folder
		\param [in] pdwFlags 	-	Special folder flags
		\param [in] dwIndex		-	One-based index of special folder to retrieve.
		\param [in] pnID		-	Receives the special folder ID of dwIndex
		
		This function provides a way to get the special folder name, as
		well as functionality for enumerating all folder types

		Getting a special folder name
  		\code

		char szName[ 256 ];

		// Getting a particular folder
		GetSpecialFolderName( CSIDL_DESKTOP, szName );
		// szName contains "Desktop"


		// Enumerating all special folders
		int nId;
		for ( UINT i = 1; GetSpecialFolderName( 0, szName, i, &nId ); i++ )
			; // szName and nId describe indexed folder		

		\endcode


		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetSpecialFolderName( int nID, LPSTR pName, LPDWORD pdwFlags = NULL, DWORD dwIndex = 0, int *pnID = NULL );

#ifdef DYNAMIC_CRC_TABLE

	//==============================================================
	// CreateCRC32Table()
	//==============================================================
	/// Creates a static CRC table to speed CRC calculations
	static void CreateCRC32Table();

	/// Dynamic CRC table
	static LPDWORD crc_table;

#else

	/// Static CRC table
	static DWORD crc_table[ 256 ];

#endif

	//==============================================================
	// LoadResource()
	//==============================================================
	/// Loads a resource from the specifed module into memory 
	/**
		\param [in] pResource	-	Resource name
		\param [out] pPtr		-	Receives the resource data
		\param [out] pdwSize	-	Receives the number of bytes copied to pPtr
		\param [in] pType		-	Resource type
		\param [in] hModule		-	Handle to module containing resource
		
		Call with pPtr set to NULL to receive the required buffer length
		in pdwSize.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL LoadResource( LPCTSTR pResource, LPBYTE pPtr, LPDWORD pdwSize, LPCTSTR pType, HMODULE hModule = NULL );

	//==============================================================
	// ValidateModuleCRC32()
	//==============================================================
	/// Validates the CRC of the specified module
	/**
		\param [in] hModule		-	Handle to module to validate
		\param [in] pCrc		-	Receives the CRC.  Can be NULL
		
		This function calculates the CRC for the module file, then 
		compares it with the last four bytes in the file which are 
		assumed to be the CRC.  You can add the CRC to the file by 
		calling AddCRC32().  

		\warning	This is a somewhat outdated way of validating an
					executable file.  New programs should use Digital
					Signatures.

		\return Non-zero if CRC is valid
	
		\see 
	*/
	static BOOL ValidateModuleCRC32( HMODULE hModule = NULL, LPDWORD pCrc = NULL );

	//==============================================================
	// ValidateCRC32()
	//==============================================================
	/// Validates the crc of the given file
	/**
		\param [in] pFile -	Filename to validate
		\param [in] pCrc 
		
		This function calculates the CRC for the file, then 
		compares it with the last four bytes in the file which are 
		assumed to be the CRC.  You can add the CRC to the file by 
		calling AddCRC32().  

		\return Non-zero if CRC is valid
	
		\see 
	*/
	static BOOL ValidateCRC32( LPCTSTR pFile, LPDWORD pCrc = NULL );

	//==============================================================
	// AddCRC32()
	//==============================================================
	/// Calculates the CRC of a file and adds it to the end of the file
	/**
		\param [in] pFile	-	File to update
		
		This function calculates the CRC for the file, then adds
		the CRC (four bytes) to the end of the file

		\return Non-zero if success
	
		\see 
	*/
	static BOOL AddCRC32( LPCTSTR pFile );

	//==============================================================
	// CRC32()
	//==============================================================
	/// Updates the CRC with the specified buffer
	/**
		\param [in] crc		-	Initial CRC value
		\param [in] buf		-	Pointer to input buffer
		\param [in] size	-	Number of bytes in buf
		
		\return Updated CRC value
	
		\see 
	*/
	static DWORD CRC32( DWORD crc, LPBYTE buf, DWORD size );

	/// Static array of custom colors used by the Windows color picker
	static COLORREF	m_rgbCustomColors[ 16 ];

	//==============================================================
	// ChooseColor()
	//==============================================================
	/// Shows the Windows color picker dialog box
	/**
		\param [in,out] pCol		-	Receives the customized color
		\param [in]		hOwner		-	Handle to parent window
		\param [in]		dwFlags		-	Color dialog box flags
		
		\return Non-zero if a new color is chosen.
	
		\see 
	*/
	static BOOL ChooseColor( COLORREF *pCol, HWND hOwner = NULL, DWORD dwFlags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT );
};

#endif // !defined(AFX_WINFILE_H__3B34FB3B_B0B7_11D2_88EB_00104B2C9CFA__INCLUDED_)
