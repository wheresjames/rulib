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
// Module.h: interface for the CModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULE_H__8B7971B8_0827_45AF_A00E_F03BD2E14BB3__INCLUDED_)
#define AFX_MODULE_H__8B7971B8_0827_45AF_A00E_F03BD2E14BB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// Holds library function information
/**
	\see CModule
*/
typedef struct tagFUNCTIONINFO : LLISTINFO
{
	/// Function address
	void		*addr;

} FUNCTIONINFO, *LPFUNCTIONINFO; // end typedef struct

//==================================================================
// CModule
//
/// Encapsulates the Windows LoadLibrary() API functions
/**
	Encapsulates the Windows LoadLibrary() API functions.  Provides
	a simple way to load Windows DLLs and execute functions.	
*/
//==================================================================
class CModule : public CHList  
{
public:

	//==============================================================
	// Addr()
	//==============================================================
	/// Returns the function address for specified index
	/**
		\param [in] i	-	Function index
		
		\return Pointer to function or NULL if none
	
		\see 
	*/
	LPVOID Addr( DWORD i );

	//==============================================================
	// Addr()
	//==============================================================
	/// Returns a pointer to the specified function name
	/**
		\param [in] pFunctionName	-	Function name
		
		\return Pointer to function or NULL if not found
	
		\see 
	*/
	LPVOID Addr( LPCTSTR pFunctionName );

	//==============================================================
	// AddFunction()
	//==============================================================
	/// Adds the specified function to the function list
	/**
		\param [in] pFunctionName	-	Function name
		
		\return Pointer to function address or NULL if not found
	
		\see 
	*/
	void* AddFunction( LPCTSTR pFunctionName );

	//==============================================================
	// LoadLibrary()
	//==============================================================
	/// Loads the specified module
	/**
		\param [in] pFile	-	Module filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL LoadLibrary( LPCTSTR pFile );

	//==============================================================
	// GetLibraryPath()
	//==============================================================
	/// Returns the file path of the currently loaded module.
	LPCTSTR GetLibraryPath() { return m_sFile; }

	/// Default constructor
	CModule();

	//==============================================================
	// CModule()
	//==============================================================
	/// Constructs object and loads the specified module
	/**
		\param [in] pFile	-	Module filename
	*/
	CModule( LPCTSTR pFile );

	/// Destructor
	virtual ~CModule();

	//==============================================================
	// Destory()
	//==============================================================
	/// Unloads the module and releases associated resources
	virtual void Destroy();

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size, in bytes, of a module function structure 
	virtual DWORD GetObjSize() { return sizeof( FUNCTIONINFO ); }


	//==============================================================
	// operator []
	//==============================================================
	/// Returns the function address for specified index
	/**
		\param [in] i	-	Function index
		
		\return Pointer to function or NULL if none
	
		\see 
	*/
	void* operator [] ( unsigned long i ) 
	{	void *ptr = Addr( i ); 
		if ( ptr == NULL ) throw "0:CModule: Invalid function pointer"; 
		return ptr; }

	//==============================================================
	// GetModuleHandle()
	//==============================================================
	/// Returns the loaded modules handle
	HMODULE GetModuleHandle() { return m_hModule; }

	//==============================================================
	// IsLoaded()
	//==============================================================
	/// Returns non-zero if there is a valid module handle
	BOOL IsLoaded() { return ( m_hModule != NULL ); }

	//==============================================================
	// HashTable()
	//==============================================================
	/// Returns a hash of the function address table for verification
	DWORD HashTable() { return m_ptrs.crc(); }

private:

	/// Current module handle
	HMODULE			m_hModule;

	/// Module function pointer array
	TMem< void* >	m_ptrs;

	/// Currently loaded file name
	CStr			m_sFile;

};

#endif // !defined(AFX_MODULE_H__8B7971B8_0827_45AF_A00E_F03BD2E14BB3__INCLUDED_)
