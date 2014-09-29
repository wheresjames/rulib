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

#ifndef _TLLIST_HEADER_
#define _TLLIST_HEADER_

//==================================================================
// TLList
//
/// Implements a linked list
/**
	Implements a linked list
	Obsolete, new code should use ATL7 classes or other standard
	implementation.
*/
//==================================================================
template< class T > class TLList : public CHList
{
public:

	/// Destructor
	virtual ~TLList() { Destroy(); }

	typedef struct tagSTLListInfo : LLISTINFO
	{	
		//==============================================================
		// operator T*()
		//==============================================================
		/// Returns a pointer to the list element object
		operator T*() { return p; }

		//==============================================================
		// operator ->()
		//==============================================================
		/// Differences to the list element object
		T* operator ->() { return p; }

		/// Pointer to the list element object
		T		*p;

	} STLListInfo, *LPSTLListInfo;

	/// Hash list element iterator
	typedef LPSTLListInfo iterator;

	//==============================================================
	// begin()
	//==============================================================
	/// Returns the iterator for the first element in the list
	iterator begin() { return TLList::iterator( (LPSTLListInfo)GetNext( NULL ) ); }

	//==============================================================
	// last()
	//==============================================================
	/// Returns the iterator for the last element in the list
	iterator last() { return TLList::iterator( (LPSTLListInfo)GetPrev( NULL ) ); }
	
	//==============================================================
	// end()
	//==============================================================
	/// Returns a NULL iterator
	iterator end() { return (TLList::iterator)NULL; }
	
	//==============================================================
	// next()
	//==============================================================
	/// Returns the next element in the list
	/**
		\param [in] it	-	Reference element

		If it is NULL, the head element is returned.
		
		\return Pointer to the next element or NULL if none.
	
		\see 
	*/
	iterator next( iterator it )
	{	if ( it == NULL ) return begin(); return (TLList::iterator)it->pNext; }
	
	//==============================================================
	// prev()
	//==============================================================
	/// Returns the previous element in the list
	/**
		\param [in] it	-	Reference element

		If it is NULL, the last element is returned.
		
		\return Pointer to the previous element or NULL if none.
	
		\see 
	*/
	iterator prev( iterator it )
	{	if ( it == NULL ) return last(); return (TLList::iterator)it->pPrev; }

	//==============================================================
	// push_back()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] node	-	Optional element pointer to add.  If
								node is NULL, a new element is created.
		
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back( T* node = NULL )
	{	T* pObj = node ? node : _PTR_NEW T;
		if ( pObj == NULL ) return NULL;
		LPSTLListInfo pTli = (LPSTLListInfo)New( NULL, pObj );
		if ( pTli == NULL ) { _PTR_DELETE( pObj ); return NULL; }
		return ( pTli->p = pObj );
	}

	//==============================================================
	// push_back_memcpy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] node	-	Optional element pointer that is 
								copied byte-wise into the new element.  
								Must be a POD pointer.

		\warning POD only!
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_memcpy( T* node )
	{	T* pObj = _PTR_NEW T;
		if ( pObj == NULL ) return NULL;
		LPSTLListInfo pTli = (LPSTLListInfo)New( NULL, (DWORD)pObj );
		if ( node != NULL ) memcpy( pTli->p, node, sizeof( T ) );
		if ( pTli == NULL ) { _PTR_DELETE( pObj ); return NULL; }
		return ( pTli->p = pObj );
	}

	//==============================================================
	// push_back_copy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] node	-	Optional element pointer that is 
								copied to the new element.
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_copy( T* node )
	{	T* pObj = _PTR_NEW T;
		if ( pObj == NULL ) return NULL;
		if ( node != NULL ) *pObj = *node;
		LPSTLListInfo pTli = (LPSTLListInfo)New( NULL, pObj );
		if ( pTli == NULL ) { _PTR_DELETE( pObj ); return NULL; }
		return ( pTli->p = pObj );
	}

	//==============================================================
	// erase()
	//==============================================================
	/// Removes the specified element from the list
	/**
		\param [in] it		-	Element iterator to remove
		
		\return Pointer to the previous element.  Useful when iterating elements.
	
		\see 
	*/
	iterator erase( iterator it )
	{	if ( !it ) return NULL;
		TLList::iterator pPrev = (TLList::iterator)it->pPrev;
		Delete( it ); return pPrev;
	}

	//==============================================================
	// verify()
	//==============================================================
	/// Verifies that the specified object pointer is actually in the list
	/**
		\param [in] node	-	Object pointer to verify
		
		\return Non-zero if valid list object
	
		\see 
	*/
	BOOL verify( T* node )
	{	return ( Find( (LPVOID)node ) != NULL );
	}

	//==============================================================
	// verify()
	//==============================================================
	/// Verifies that the specified iterator is actually in the list
	/**
		\param [in] it	-	Iterator pointer to verify
		
		\return Non-zero if valid list iterator
	
		\see 
	*/
	BOOL verify( iterator it )
	{	return ( VerifyPointer( it ) != NULL ); }

	//==============================================================
	// clear()
	//==============================================================
	/// Removes all elements from the list
	void clear() { Destroy(); }

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of a hash list structure in bytes
	virtual DWORD GetObjSize() { return sizeof( STLListInfo ); }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Removes all elements from the list
	virtual void Destroy() { CHList::Destroy(); }
		
	//==============================================================
	// InitObject()
	//==============================================================
	/// Provides custom initialization of new hash list structures
	/**
		\param [in] node	-	Hash list structure pointer
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL InitObject( void *node )
	{	if ( !CHList::InitObject( node ) ) return FALSE;
		return TRUE;
	}

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Provides custom destruction of hash list structures
	/**
		\param [in] node	-	Hash list structure pointer
	*/
	virtual void DeleteObject( void *node )
	{	LPSTLListInfo pTli = (LPSTLListInfo)node;
		if ( pTli == NULL ) return;
		_PTR_DELETE( pTli->p );
		CHList::DeleteObject( node );
	}

	// Allocate array
	//==============================================================
	// allocate()
	//==============================================================
	/// Creates a list with the specified number of elements
	/**
		\param [in] dwSize	-	Desired number of elements in list
		
		\return Non-zero if success
	
		\see 
	*/
	bool allocate( DWORD dwSize )
	{	clear(); while ( dwSize-- ) { if ( !push_back() ) return false; } return true; }

	//==============================================================
	// resize()
	//==============================================================
	/// Resizes a list to the specified number of elements
	/**
		\param [in] dwSize	-	Desired number of elements in list
		
		\return Non-zero if success
	
		\see 
	*/
	bool resize( DWORD dwSize )
	{	while ( Size() > dwSize ) erase( last() );
		while ( Size() < dwSize ) push_back();
		return Size() == dwSize;
	}

	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a pointer to the specified element
	/**
		\param [in] i		-	Element index

		This function can be slow since it has to iterate through the
		entire list until the specified element is reached
		
		\return Pointer to specified element.
	
		\see 
	*/
	T* operator []( unsigned long i )
	{	iterator it = begin();
		for ( it = begin(); it != end(); it = next( it ) )
			if ( i ) i--; else return *it;
		return NULL;
	}

	//==============================================================
	// find_obj()
	//==============================================================
	/// Searchs the list to find the iterator for the specified object pointer
	/**
		\param [in] pObj	-	Element object pointer
		
		\return Iterator if found, otherwise NULL
	
		\see 
	*/
	iterator find_obj( T *pObj )
	{	iterator it = NULL;
		while ( ( it = next( it ) ) != end() )
			if ( it->p == pObj ) return it;
		return NULL;
	}

private:

};

#endif // _TLLIST_HEADER_
