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
// MultiStore.h: interface for the CMultiStore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTISTORE_H__C89D9291_C0E3_45E3_BF7F_F9F60BE3A88C__INCLUDED_)
#define AFX_MULTISTORE_H__C89D9291_C0E3_45E3_BF7F_F9F60BE3A88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// Structure describing storage slot
/**
	\see CMultiStore
*/
typedef struct tagSMultiStoreInfo
{	DWORD	dwIndex;
	DWORD	dwMemSize;
	DWORD	dwDiskSize;
} SMultiStoreInfo, *LPSMultiStoreInfo;

//==================================================================
// CMultiStore
//
/// Provides index data storage
/**
	This class provides a fast way to index a large amount of data.
	You can specify a portion of data that is stored in memory, and
	another that is stored on disk.  The design is so that data can
	be accessed quickly using a linear style key.	
*/
//==================================================================
class CMultiStore  
{

public:

	//==============================================================
	// GetPrevIndexPtr()
	//==============================================================
	/// Returns pointer to previous index pointer
	/**
		\param [in] dwIndex		-	Index
		
		\return Pointer to data structure or NULL if not found
	
		\see 
	*/
	LPSMultiStoreInfo GetPrevIndexPtr( DWORD dwIndex ) { return GetPrevIndexPtr( dwIndex, &m_pvIndex ); }

	//==============================================================
	// GetPrevIndexPtr()
	//==============================================================
	/// Returns pointer to previous index pointer
	/**
		\param [in] dwIndex		-	Index
		\param [in] pvIndex		-	Root node structure
		\param [in] dwDepth		-	Current search depth
		
		\return Pointer to data structure or NULL if not found
	
		\see 
	*/
	LPSMultiStoreInfo GetPrevIndexPtr( DWORD dwIndex, LPVOID *pvIndex, DWORD dwDepth = 0  );

	//==============================================================
	// GetNextIndexPtr()
	//==============================================================
	/// Returns pointer to next index pointer
	/**
		\param [in] dwIndex		-	Index
		
		\return Pointer to data structure or NULL if not found
	
		\see 
	*/
	LPSMultiStoreInfo GetNextIndexPtr( DWORD dwIndex ) { return GetNextIndexPtr( dwIndex, &m_pvIndex ); }

	//==============================================================
	// GetPrevIndexPtr()
	//==============================================================
	/// Returns pointer to next index pointer
	/**
		\param [in] dwIndex		-	Index
		\param [in] pvIndex		-	Root node structure
		\param [in] dwDepth		-	Current search depth
		
		\return Pointer to data structure or NULL if not found
	
		\see 
	*/
	LPSMultiStoreInfo GetNextIndexPtr( DWORD dwIndex, LPVOID *pvIndex, DWORD dwDepth = 0  );

	//==============================================================
	// GetIndexPtr()
	//==============================================================
	/// Gets structure at specified index
	/**
		\param [in] dwIndex		-	Index
		\param [in] bCreate		-	Non-zero to create if not found.
		
		\return Pointer to data structure or NULL if not error
	
		\see 
	*/
	LPSMultiStoreInfo GetIndexPtr( DWORD dwIndex, BOOL bCreate = TRUE ) { return GetIndexPtr( dwIndex, &m_pvIndex, bCreate ); }

	//==============================================================
	// GetIndexPtr()
	//==============================================================
	/// Returns the structure at the specified index
	/**
		\param [in] dwIndex		-	Index
		\param [in] pvIndex		-	Index buffer pointer
		\param [in] bCreate		-	Non-zero to create if not found
		\param [in] dwDepth		-	Current search depth
		
		\return Pointer to data structure or NULL if not error
	
		\see 
	*/
	LPSMultiStoreInfo GetIndexPtr( DWORD dwIndex, LPVOID *pvIndex, BOOL bCreate = TRUE, DWORD dwDepth = 0  );

	//==============================================================
	// DestroyIndex()
	//==============================================================
	/// Destroys the specified index
	/**
		\param [in] pvIndex		-	Index pointer
		\param [in] dwDepth		-	Current search depth
		
	*/
	void DestroyIndex( LPVOID *pvIndex, DWORD dwDepth = 0 );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the index and all resources
	void Destroy();

	//==============================================================
	// Open()
	//==============================================================
	/// Initializes the index for use
	/**
		\param [in] pFile		-	Disk file
		\param [in] dwMaxMem	-	Memory required per item
		\param [in] dwMaxDisk	-	Disk space required per item
		\param [in] dwBits		-	Number of bits to unroll with the 
									index table
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Open( LPCTSTR pFile, DWORD dwMaxMem, DWORD dwMaxDisk, DWORD dwBits = 4 );

	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from the specified index
	/**
		\param [in] dwIndex		-	Index offset
		\param [out] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [out] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] dwDisk		-	Size of the buffer in pDisk;
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Read( DWORD dwIndex, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk )
	{	LPSMultiStoreInfo pMsi = GetIndexPtr( dwIndex, FALSE );
		if ( pMsi == NULL ) return FALSE;
		return Read( pMsi, pMem, dwMem, pDisk, dwDisk );
	}

	//==============================================================
	// ReadNext()
	//==============================================================
	/// Reads the next record after specified index
	/**
		\param [in] dwIndex		-	Index offset
		\param [out] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [out] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] dwDisk		-	Size of the buffer in pDisk;
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadNext( DWORD dwIndex, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk )
	{	LPSMultiStoreInfo pMsi = GetNextIndexPtr( dwIndex );
		if ( pMsi == NULL ) return FALSE;
		return Read( pMsi, pMem, dwMem, pDisk, dwDisk );
	}

	//==============================================================
	// ReadPrev()
	//==============================================================
	/// Reads the record before the specified index
	/**
		\param [in] dwIndex		-	Index offset
		\param [out] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [out] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] dwDisk		-	Size of the buffer in pDisk;
		
		\return Non-zero if success
	
		\see 
		
	*/
	BOOL ReadPrev( DWORD dwIndex, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk )
	{	LPSMultiStoreInfo pMsi = GetPrevIndexPtr( dwIndex );
		if ( pMsi == NULL ) return FALSE;
		return Read( pMsi, pMem, dwMem, pDisk, dwDisk );
	}

	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to the specified index
	/**
		\param [in] dwIndex		-	Index offset
		\param [in] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [in] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] dwDisk		-	Size of the buffer in pDisk.
		\param [in] bOverwrite	-	Non-zero to overwrite record.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( DWORD dwIndex, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk, BOOL bOverwrite = TRUE )
	{	LPSMultiStoreInfo pMsi = GetIndexPtr( dwIndex );
		if ( pMsi == NULL ) return FALSE;
		return Write( pMsi, pMem, dwMem, pDisk, dwDisk, bOverwrite );
	}

	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from the specified memory structure
	/**
		\param [in] pMsi		-	Pointer to data structure
		\param [out] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [out] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] dwDisk		-	Size of the buffer in pDisk;
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Read( LPSMultiStoreInfo pMsi, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk );

	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to the specified memory structure
	/**
		\param [in] pMsi		-	Pointer to data structure
		\param [out] pMem		-	Receives memory structure. Can be
									NULL if not required.
		\param [in] dwMem		-	Size of buffer in pMem
		\param [out] pDisk		-	Receives disk structure.  Can be 
									NULL if not required.  Leaving this
									structure NULL avoids the disk
									access.
		\param [in] bOverwrite	-	Non-zero to overwrite record.
		\param [in] dwDisk		-	Size of the buffer in pDisk;
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( LPSMultiStoreInfo pMsi, void *pMem, DWORD dwMem, void *pDisk, DWORD dwDisk, BOOL bOverwrite = TRUE );

	/// Default constructor
	CMultiStore();

	/// Destructor
	virtual ~CMultiStore();

	//==============================================================
	// MemPtr()
	//==============================================================
	/// Returns a pointer to the user memory space
	/**
		\param [in] pMsi	-	Pointer to data structure
		
		\return Pointer to user memory space associated with this structure
	
		\see 
	*/
	LPVOID MemPtr( LPSMultiStoreInfo pMsi )
	{	return ( (LPBYTE)pMsi ) + sizeof( SMultiStoreInfo ); }

private:

	/// Pointer to root index level
	LPVOID					m_pvIndex;

	/// Encapsulates disk storage file
	CWinFile				m_store;

	/// Number of records
	DWORD					m_dwRecord;

	/// Size of user memory space
	DWORD					m_dwMaxMem;

	/// Size of user disk space
	DWORD					m_dwMaxDisk;

	/// Bits used for each indexing level
	DWORD					m_dwBits;

	/// Max depth of index
	DWORD					m_dwDepth;

	/// Size of each memory node
	DWORD					m_dwNodeSize;

};

#endif // !defined(AFX_MULTISTORE_H__C89D9291_C0E3_45E3_BF7F_F9F60BE3A88C__INCLUDED_)
