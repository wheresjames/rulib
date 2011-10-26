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

#ifndef _THLIST_HEADER_
#define _THLIST_HEADER_

//==================================================================
// THList
//
/// Implements a hashed linked list
/**
	Implements a hashed linked list
	Obsolete, new code should use ATL7 classes or other standard
	implementation.
*/
//==================================================================
template< class H, class T > class THList : public CHList
{
public:

	/// Destructor
	virtual ~THList() { Destroy(); }

	/// Hash list element iterator structure
	/**
		\see THList, THList::iterator
	*/
	typedef struct tagSTHListInfo : LLISTINFO
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
		
		//==============================================================
		// key()
		//==============================================================
		/// Returns a pointer to the list element key
		H* key() { return (H*)vpkey; }

		//==============================================================
		// keylen()
		//==============================================================
		/// Returns the length of the list element key in bytes
		unsigned long keylen() { return ksize; }

	} STHListInfo, *LPSTHListInfo;

	/// Hash list element iterator
	typedef LPSTHListInfo iterator;

	//==============================================================
	// begin()
	//==============================================================
	/// Returns the iterator for the first element in the list
	iterator begin() { return THList::iterator( (LPSTHListInfo)GetNext( NULL ) ); }

	//==============================================================
	// last()
	//==============================================================
	/// Returns the iterator for the last element in the list
	iterator last() { return THList::iterator( (LPSTHListInfo)GetPrev( NULL ) ); }

	//==============================================================
	// end()
	//==============================================================
	/// Returns a NULL iterator
	iterator end() { return (THList::iterator)NULL; }

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
	{	if ( it == NULL ) return begin(); return (THList::iterator)it->pNext; }

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
	{	if ( it == NULL ) return last(); return (THList::iterator)it->pPrev; }

	//==============================================================
	// push_back()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] key		-	Element key
		\param [in] node	-	Optional element pointer to add.  If
								node is NULL, a new element is created.
		
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back( const H& key, T* node = NULL )
	{	
		// wjr 10/12/06...
		LPSTHListInfo pTli = (LPSTHListInfo)Find( &key, sizeof( H ) );
		if ( pTli != NULL )
		{
			Delete( pTli );		
		}
		// ...wjr 10/12/06
	
		/*LPSTHListInfo*/ pTli = (LPSTHListInfo)New( NULL, 0, &key, sizeof( H ) );
		if ( pTli == NULL ) { _PTR_DELETE( node ); return NULL; }
		if ( node != NULL ) pTli->p = node;
		else pTli->p = _PTR_NEW T;
		return pTli->p;
	}

	//==============================================================
	// push_back_memcpy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] key		-	Element key
		\param [in] node	-	Optional element pointer that is 
								copied byte-wise into the new element.  
								Must be a POD pointer.

		\warning POD only!
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_memcpy( const H& key, T* node )
	{	LPSTHListInfo pTli = (LPSTHListInfo)New( NULL, 0, &key, sizeof( H ) );
		if ( pTli == NULL ) NULL;
		pTli->p = _PTR_NEW T;
		if ( node != NULL ) memcpy( pTli->p, node, sizeof( T ) );
		else ZeroMemory( pTli->p, sizeof( T ) );
		return pTli->p;
	}

	//==============================================================
	// push_back_copy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] key		-	Element key
		\param [in] node	-	Optional element pointer that is 
								copied to the new element.
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_copy( const H& key, T* node )
	{	LPSTHListInfo pTli = (LPSTHListInfo)New( NULL, 0, &key, sizeof( H ) );
		if ( pTli == NULL ) NULL;
		pTli->p = _PTR_NEW T;
		if ( node != NULL ) *pTli->p = *node;
		return pTli->p;
	}


	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a pointer to the specified element
	/**
		\param [in] key		-	Element key
		
		\return Pointer to specified element.
	
		\see 
	*/
	T* operator [] ( const H& key )
	{	THList::iterator it = (THList::iterator)Find( &key, sizeof( H ) );
		if ( it == NULL ) return NULL;
		return it->p;
	}

	//==============================================================
	// get()
	//==============================================================
	/// Returns a pointer to the specified element, adding if needed
	/**
		\param [in] key		-	Element key
		\param [in] node	-	Element to add if not found
		
		\return Pointer to specified element.
	
		\see 
	*/
	T* get( const H& key, T* node = NULL )
	{	T* pT = (*this)[ key ];
		if ( pT != NULL ) return pT;
		return push_back( key, node );
	}

	//==============================================================
	// find()
	//==============================================================
	/// Locates the specified element in the list
	/**
		\param [in] key		-	Element key
		
		\return Pointer to the element.
	
		\see 
	*/
	// wjr 10/12/06...
	iterator find( const H& key )
	{	
		LPSTHListInfo pTli = (LPSTHListInfo)Find( &key, sizeof( H ) );

		if ( pTli == NULL )
			return NULL; 

		THList::iterator pIt = (THList::iterator)pTli;

		return pIt;
	}
	// ...wjr 10/12/06

	//==============================================================
	// erase()
	//==============================================================
	/// Removes the specified element from the list
	/**
		\param [in] key		-	Element key
		
		\return Pointer to the previous element.  Useful when iterating elements.
	
		\see 
	*/
	iterator erase( const H& key )
	{	LPSTHListInfo pTli = (LPSTHListInfo)Find( &key, sizeof( H ) );
		if ( pTli == NULL ) return NULL; 
		THList::iterator pPrev = (THList::iterator)pTli->pPrev;
		Delete( pTli );	return pPrev;
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
	iterator erase( const iterator it )
	{	if ( it == NULL ) return NULL;
		THList::iterator pPrev = (THList::iterator)it->pPrev;
		Delete( it );	return pPrev;
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
	{	return ( find_obj( node ) != NULL );
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
	virtual DWORD GetObjSize() { return sizeof( STHListInfo ); }

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
	{	LPSTHListInfo pTli = (LPSTHListInfo)node;
		if ( pTli == NULL ) return;
		RULIB_TRY { _PTR_DELETE( pTli->p ); } RULIB_CATCH_ALL {}
		CHList::DeleteObject( node );
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

//==================================================================
// TStrList
//
/// Implements a hashed string linked list
/**
	Implements a hashed string linked list
	Obsolete, new code should use ATL7 classes or other standard
	implementation.
*/
//==================================================================
template< class T > class TStrList : public CHList
{
public:

	/// Destructor
	virtual ~TStrList() { Destroy(); }

	/// Hash list element iterator structure
	/**
		\see THList, THList::iterator
	*/
	typedef struct tagSTStrListInfo : LLISTINFO
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

		//==============================================================
		// key()
		//==============================================================
		/// Returns a pointer to the list element key string
		LPCTSTR key() { return cpkey; }

		//==============================================================
		// keylen()
		//==============================================================
		/// Returns the length of the list element key in bytes
		unsigned long keylen() { return ksize; }

	} STStrListInfo, *LPSTStrListInfo;

	/// Hash list element iterator
	typedef LPSTStrListInfo iterator;

	//==============================================================
	// begin()
	//==============================================================
	/// Returns the iterator for the first element in the list
	iterator begin() { return TStrList::iterator( (LPSTStrListInfo)GetNext( NULL ) ); }

	//==============================================================
	// last()
	//==============================================================
	/// Returns the iterator for the last element in the list
	iterator last() { return TStrList::iterator( (LPSTStrListInfo)GetPrev( NULL ) ); }

	//==============================================================
	// end()
	//==============================================================
	/// Returns a NULL iterator
	iterator end() { return (TStrList::iterator)NULL; }
	
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
	{	if ( it == NULL ) return begin(); return (TStrList::iterator)it->pNext; }
	
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
	{	if ( it == NULL ) return last(); return (TStrList::iterator)it->pPrev; }

	//==============================================================
	// push_back()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] pKey	-	Element key
		\param [in] node	-	Optional element pointer to add.  If
								node is NULL, a new element is created.
		
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back( LPCTSTR pKey, T* node = NULL )
	{	LPSTStrListInfo pTli = (LPSTStrListInfo)New( NULL, 0, pKey );
		if ( pTli == NULL ) NULL;
		if ( node != NULL ) pTli->p = node;
		else pTli->p = _PTR_NEW T;
		return pTli->p;
	}

	//==============================================================
	// push_back_memcpy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] pKey	-	Element key
		\param [in] node	-	Optional element pointer that is 
								copied byte-wise into the new element.  
								Must be a POD pointer.

		\warning POD only!
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_memcpy( LPCTSTR pKey, T* node )
	{	LPSTStrListInfo pTli = (LPSTStrListInfo)New( NULL, 0, pKey );
		if ( pTli == NULL ) NULL;
		pTli->p = _PTR_NEW T;
		if ( node != NULL ) memcpy( pTli->p, node, sizeof( T ) );
		else ZeroMemory( pTli->p, sizeof( T ) );
		return pTli->p;
	}

	//==============================================================
	// push_back_copy()
	//==============================================================
	/// Adds an element to the end of the list
	/**
		\param [in] pKey	-	Element key
		\param [in] node	-	Optional element pointer that is 
								copied to the new element.
	  
		\return Pointer to newly added element
	
		\see 
	*/
	T* push_back_copy( LPCTSTR pKey, T* node )
	{	LPSTStrListInfo pTli = (LPSTStrListInfo)New( NULL, 0, pKey );
		if ( pTli == NULL ) NULL;
		pTli->p = _PTR_NEW T;
		if ( node != NULL ) *pTli->p = *node;
		return pTli->p;
	}

	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a pointer to the specified element
	/**
		\param [in] pKey		-	Element key
		
		\return Pointer to specified element.
	
		\see 
	*/
	T* operator [] ( LPCTSTR pKey )
	{	TStrList::iterator it = (TStrList::iterator)Find( pKey );
		if ( it == NULL ) return NULL;
		return it->p;
	}

	//==============================================================
	// get()
	//==============================================================
	/// Returns a pointer to the specified element, adding if needed
	/**
		\param [in] pKey	-	Element key
		\param [in] node	-	Element to add if not found
		
		\return Pointer to specified element.
	
		\see 
	*/
	T* get( LPCTSTR pKey, T* node = NULL )
	{	T* pT = (*this)[ pKey ];
		if ( pT != NULL ) return pT;
		return push_back( pKey, node );
	}

	//==============================================================
	// erase()
	//==============================================================
	/// Removes the specified element from the list
	/**
		\param [in] pKey		-	Element key string
		
		\return Pointer to the previous element.  Useful when iterating elements.
	
		\see 
	*/
	iterator erase( LPCTSTR pKey )
	{	LPSTStrListInfo pTli = (LPSTStrListInfo)Find( pKey );
		if ( pTli == NULL ) return NULL; 
		TStrList::iterator pNext = (TStrList::iterator)pTli->pNext;
		Delete( pTli );	return pNext;
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
	iterator erase( const iterator it )
	{	if ( it == NULL ) return NULL;
		TStrList::iterator pNext = (TStrList::iterator)it->pNext;
		Delete( it );	return pNext;
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
	{	return ( find_obj( node ) != NULL );
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
	virtual DWORD GetObjSize() { return sizeof( STStrListInfo ); }

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
	{	LPSTStrListInfo pTli = (LPSTStrListInfo)node;
		if ( pTli == NULL ) return;
		_PTR_DELETE( pTli->p );
		CHList::DeleteObject( node );
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


#endif // _THLIST_HEADER_
