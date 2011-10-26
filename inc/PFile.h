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
// PFile.h: interface for the CPFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PFILE_H__6A08ECF2_76C9_45A3_A388_2638405E5074__INCLUDED_)
#define AFX_PFILE_H__6A08ECF2_76C9_45A3_A388_2638405E5074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pipe.h"

//==================================================================
// CPFile
//
/// Extends the CPipe class adding disk file functionality
/**
	Extends the CPipe class to support disk access.  You can use
	this whenever a pipe is required, and the output will be directed
	to a disk file instead of memory.
*/
//==================================================================
class CPFile : public CPipe  
{
public:

	//==============================================================
	// CPFile()
	//==============================================================
	/// Constructs the object and initializes the file interface
	/**
		\param [in] pFile	-	Filename
		\param [in] access	-	File access flags
		\param [in] bNew	-	Non-zero to truncate existing file
	*/
	CPFile( LPCTSTR pFile, DWORD access, BOOL bNew );

	//==============================================================
	// CPFile()
	//==============================================================
	/// Constructs the object and initializes the file interface
	/**
		\param [in] pFile	-	Filename
		\param [in] pKey	-	Encryption key
		\param [in] access	-	File access flags
		\param [in] bNew	-	Non-zero to truncate existing file
		
		\return 
	
		\see 
	*/
	CPFile( LPCTSTR pFile, LPCTSTR pKey, DWORD access, BOOL bNew );

	/// Default constructor
	CPFile();

	/// Destructor
	virtual ~CPFile();

	//==============================================================
	// ProcessWrite()
	//==============================================================
	/// Called when data is written to the pipe
	/**
		\param [in] buf			-	Buffer containing write data
		\param [in] size		-	Number of bytes in buf
		\param [out] written	-	Number of bytes written
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL ProcessWrite( const void *buf, DWORD size, LPDWORD written );

	//==============================================================
	// ProcessRead()
	//==============================================================
	/// Called when data is read from the pipe
	/**
		\param [out] buf		-	Buffer that receives read data
		\param [in] size		-	Size of buffer in buf
		\param [out] read		-	Number of bytes read into buf
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL ProcessRead( LPVOID buf, DWORD size, LPDWORD read );

	//==============================================================
	// File()
	//==============================================================
	/// Returns the encapsulated CWinFile object
	/**
		\return Pointer to encapsulated CWinFile object.
	
		\see 
	*/
	CWinFile& File() { return m_file; }

private:

	/// Object containing the file for data access
	CWinFile	m_file;

};

#endif // !defined(AFX_PFILE_H__6A08ECF2_76C9_45A3_A388_2638405E5074__INCLUDED_)
