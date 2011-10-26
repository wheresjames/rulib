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
// HList.h: interface for the CHList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLIST_H__35705236_7C90_46E6_ACA2_E228CBF1E1E5__INCLUDED_)
#define AFX_HLIST_H__35705236_7C90_46E6_ACA2_E228CBF1E1E5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "LList.h"

/// Hashed linked list information structure
typedef struct tagHLISTINFO
{
	DWORD			hash;

	LPLLISTINFO		plli;

	tagHLISTINFO	*pNext;
	tagHLISTINFO	*pPrev;

} HLISTINFO; // end typedef struct
typedef HLISTINFO* LPHLISTINFO;

//==================================================================
// CHList
//
/// Hashed linked list
/**
	This class encapsulates a hashed linked list.  Though used
	extensively in this library, this class is very old.  I suggest
	you use ATL7 classes if available.	
*/
//==================================================================
class CHList : public CLList  
{
public:

	//==============================================================
	// SetKey()
	//==============================================================
	/// Sets the key on a linked list element
	/**
		\param [in] ptr			-	Pointer to linked list element
		\param [in] pKey		-	Buffer containing the key
		\param [in] dwKeySize	-	Size of buffer in pKey
		
		\return Pointer to linked list element
	
		\see 
	*/
	virtual const void* SetKey( void *ptr, const void *pKey, DWORD dwKeySize = 0 );

	//==============================================================
	// DeleteAllItems()
	//==============================================================
	/// Removes all elements from the list
	virtual void DeleteAllItems();

	//==============================================================
	// RemoveHash()
	//==============================================================
	/// Removes the hash for the specified element from the hash table
	/**
		\param [in] plli	-	Linked list element
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemoveHash( LPLLISTINFO plli );

	//==============================================================
	// Find()
	//==============================================================
	/// Finds an element in the list by its hash value
	/**
		\param [in] dwKey	-	Hash
		
		\return Pointer to list element if found, otherwise NULL
	
		\see 
	*/
	void* Find( DWORD dwKey )
	{	return Find( &dwKey, sizeof( dwKey ) ); }

	//==============================================================
	// Find()
	//==============================================================
	/// Finds an element in a list by hashing a string value
	/**
		\param [in] pKey	-	String to hash
		
		\return Pointer to list element if found, otherwise NULL
	
		\see 
	*/
	void* Find( LPCTSTR pKey )
	{	if ( !pKey || !*pKey ) return NULL;
		return Find( pKey, strlen( pKey ) ); }

	//==============================================================
	// Find()
	//==============================================================
	/// Finds an element in the list by hashing a binary buffer
	/**
		\param [in] pKey	-	Pointer to key data
		\param [in] dwKey	-	Number of bytes in pKey
		
		\return Pointer to list element if found, otherwise NULL
	
		\see 
	*/
	virtual void* Find( const void * pKey, DWORD dwKey = 0 );

	//==============================================================
	// AddHash()
	//==============================================================
	/// Adds a hash for a list element to the hash table
	/**
		\param [in] plli	-	Pointer to a list element
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddHash( LPLLISTINFO plli );
	
	//==============================================================
	// ReleaseHashList()
	//==============================================================
	/// Deletes the entire hash table
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	void ReleaseHashList();

	//==============================================================
	// Create()
	//==============================================================
	/// Creates an initial hash table
	/**
		\param [in] size	-	Initial size of hash table

		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( DWORD size = 8 );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the hash table and all list elements.
	virtual void Destroy();

	//==============================================================
	// InitObject()
	//==============================================================
	/// Initializes a single list element and adds to hash table
	/**
		\param [in] node	-	Pointer to list element
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL InitObject( void *node );

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Deletes a single list element and removes from hash table
	/**
		\param [in] node	-	Pointer to list element
	*/
	virtual void DeleteObject( void *node );	

	/// Constructor
	CHList();

	/// Destructor
	virtual ~CHList();

	//==============================================================
	// Crc32()
	//==============================================================
	/// Calculates a 32-bit CRC that is used as a hash
	/**
		\param [in] crc		-	Initial CRC value
		\param [in] vbuf	-	Buffer to hash
		\param [in] size	-	Number of bytes in vbuf
		
		\return Updated CRC value
	
		\see 
	*/
	static DWORD Crc32( DWORD crc, LPVOID vbuf, DWORD size );

	/// Static CRC table
	static DWORD crc_table[ 256 ];

	//==============================================================
	// GetHash()
	//==============================================================
	/// Calculates the hash value of a list element
	/**
		\param [in] plli	-	Pointer to a list element
		
		\return List element hash
	
		\see 
	*/
	DWORD GetHash( LPLLISTINFO plli )
	{	if ( plli == NULL ) return 0;
		else if ( plli->vpkey == NULL || plli->ksize == 0 ) 
			return GetHash( plli->user );
        return GetHash( plli->bpkey, plli->ksize );
	}

	//==============================================================
	// GetHash()
	//==============================================================
	/// Calculates the hash value of a binary buffer
	/**
		\param [in] buf		-	Buffer pointer
		\param [in] size	-	Number of bytes in buf
		
		\return Hash value
	
		\see 
	*/
	DWORD GetHash( const void *buf, DWORD size )
	{	if ( buf == NULL || size == 0 ) return 0;
		return ( Crc32( 0, (LPBYTE)buf, size ) & m_dwMask ); 
	}
		
	//==============================================================
	// GetHash()
	//==============================================================
	/// Gets the hash value of a DWORD value
	/**
		\param [in] dw	-	DWORD value
		
		\return Hash value
	
		\see 
	*/
	DWORD GetHash( LPVOID dw )
	{	if ( dw == 0 ) return 0;
		return ( Crc32( 0, (LPBYTE)&dw, sizeof( dw ) ) & m_dwMask ); 
	}


private:

	/// Hash mask
	DWORD			m_dwMask;

	/// Size of hash table
	DWORD			m_dwSize;

	/// Number of bits in hash to use in lookups
	DWORD			m_dwBits;

	/// Pointer to the hash table
	LPHLISTINFO		*m_pList;

};

#endif // !defined(AFX_HLIST_H__35705236_7C90_46E6_ACA2_E228CBF1E1E5__INCLUDED_)
