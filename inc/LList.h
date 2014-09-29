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
// LList.h: interface for the CLList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LLIST_H__02D05671_F6C3_40AA_B998_4CBDFCD46238__INCLUDED_)
#define AFX_LLIST_H__02D05671_F6C3_40AA_B998_4CBDFCD46238__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/// Provides linked list element variables
/**
	To use the CLList class, derive a data type
	from this class.  Derived structures must 
	contain POD data only since constructors
	will not be called.

	\warning Derive POD stuctures only!!!
*/
typedef struct tagLLISTINFO
{
	/// Size of this structure
	DWORD			dwObjSize;
	
	/// User DWORD value
	LPVOID			user;

	/// Size of key in vpkey
	DWORD			ksize;

	/// Key pointers
	union
	{	LPVOID		vpkey;
		TCHAR		*cpkey;
		LPBYTE		bpkey;
	};

	/// Next element in the list
	tagLLISTINFO	*pNext;

	/// Previous element in the list
	tagLLISTINFO	*pPrev;


} LLISTINFO; // end typedef struct
typedef LLISTINFO* LPLLISTINFO;
typedef void* HLITEM;

/// Double linked list
typedef struct tagDLLISTINFO : LLISTINFO
{
	/// Number of items in sub list
	DWORD			dwSubList;

	/// Parent list
	tagDLLISTINFO	*pParent;

	/// Pointer to head element in sub list
	tagDLLISTINFO	*pHead;

	/// Pointer to tail element in sub list
	tagDLLISTINFO	*pTail;

} DLLISTINFO; // end typedef struct
typedef DLLISTINFO* LPDLLISTINFO;


//==================================================================
// CLList
//
/// Provides simple sortable linked list
/**
	This class is very old but provides basic linked list functionality.
	For new code, use ATL7 classes if available.  If you need random
	access ability, use CHList instead.
*/
//==================================================================
class CLList  
{
public:

	//==============================================================
	// Find()
	//==============================================================
	/// 
	/**
		\param [in] user	-	Searches for a list element based on
								a user DWORD value.
		
		\warning This function is slow since it must traverse the list

		\return Pointer to list item if found, otherwise NULL
	
		\see 
	*/
	virtual void* Find( LPVOID user );

	//==============================================================
	// IFind()
	//==============================================================
	/// Searches for a list element matching the specified key string.
	/**
		\param [in] pKey	-	Case insensitive key to search for.
		
		\warning This function is slow since it must traverse the list

		\return Pointer to list item if found, otherwise NULL
	
		\see 
	*/
	virtual void* IFind( LPCTSTR pKey );

	//==============================================================
	// Find()
	//==============================================================
	/// Searches for a matching binary key
	/**
		\param [in] pKey	-	Key buffer
		\param [in] dwKey	-	Number of bytes in pKey
		
		\warning This function is slow since it must traverse the list

		\return Pointer to list item if found, otherwise NULL
	
		\see 
	*/
	virtual void* Find( const void * pKey, DWORD dwKey = 0 );

	//==============================================================
	// InitObject()
	//==============================================================
	/// Initializes a list element
	/**
		\param [in] node	-	Pointer to list element

		Over-ride to provide custom list element initialization.
		
		\return Non-zero if success
	
		\see DeleteObject()
	*/
	virtual BOOL InitObject( void *node );

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Destroys a single list element
	/**
		\param [in] node	-	Pointer to list element

		Over-ride to provide custom list element destruction

		\see InitObject()
	*/
	virtual void DeleteObject( void *node );

	//==============================================================
	// New()
	//==============================================================
	/// Creates a new list element
	/**
		\param [in] init		-	Initialization data
		\param [in] dwUser		-	User value
		\param [in] pKey		-	Element key
		\param [in] dwKeySize	-	Number of bytes in pKey
		
		\return Pointer to new list element or NULL if error
	
		\see 
	*/
	virtual void* New( const void *init = NULL, LPVOID dwUser = 0, const void* pKey = NULL, DWORD dwKeySize = 0 );

	//==============================================================
	// SetObject()
	//==============================================================
	/// Sets the data inside a list element
	/**
		\param [in] obj			-	Pointer to list element
		\param [in] init		-	Pointer to initialization data
		\param [in] dwUser		-	User value
		\param [in] pKey		-	Key data
		\param [in] dwKeySize	-	Number of bytes in pKey
		
		\return Pointer in obj
	
		\see ResetObject()
	*/
	void* SetObject( void *obj, const void *init = NULL, LPVOID dwUser = 0, const void* pKey = NULL, DWORD dwKeySize = 0 );
	
	//==============================================================
	// ResetObject()
	//==============================================================
	/// 
	/**
		\param [in] obj			-	Pointer to list element
		\param [in] init		-	Pointer to initialization data
		\param [in] dwUser		-	User value
		\param [in] pKey		-	Key data
		\param [in] dwKeySize	-	Number of bytes in pKey
		
		\return Pointer in obj
	
		\see SetObject()
	*/
	void* ResetObject( void *obj, const void *init = NULL, LPVOID dwUser = 0, const void* pKey = NULL, DWORD dwKeySize = 0 )
	{	DeleteObject( obj ); return SetObject( obj, init, dwUser, pKey, dwKeySize ); }
	
	//==============================================================
	// Destroy()
	//==============================================================
	/// Deletes all elements in the list
	virtual void Destroy();

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the CLList object
	/**
		\param [in] bDestroy	-	Set to non-zero to destroy existing list
		
	*/
	void Init( BOOL bDestroy = TRUE );

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Provides the size of a single list element structure
	/**
		Over-ride this function and provide the size of your custom structure.

		\return The size of a single list element structure
	*/
	virtual DWORD GetObjSize() { return sizeof( LLISTINFO ); }

	//==============================================================
	// Zero()
	//==============================================================
	/// Initializes the custom portion of a list element
	/**
		\param [in] ptr		-	List element pointer
		\param [in] size	-	Size of the list element
	*/
	void Zero( void *ptr, DWORD size )
	{	if ( size > sizeof( LLISTINFO ) ) 
			ZeroMemory( &( (LPBYTE)ptr )[ sizeof( LLISTINFO ) ], size - sizeof( LLISTINFO ) );
	} 

	//==============================================================
	// MoveToBottom()
	//==============================================================
	/// Moves the specified element to the bottom of the list
	/**
		\param [in] ptr		-	List element
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL MoveToBottom( void *ptr );

	//==============================================================
	// MoveToTop()
	//==============================================================
	/// Moves the specified element to the top of the list
	/**
		\param [in] ptr		-	List element
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL MoveToTop( void *ptr );

	//==============================================================
	// Append()
	//==============================================================
	/// Appends the specified element after another node in the list
	/**		
		\param [in] ptr		-	Pointer to element to append
		\param [in] after	-	Node after which to append
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Append( void *ptr, void *after );

	//==============================================================
	// Insert()
	//==============================================================
	/// Inserts the specified element before another node in the list
	/**
		\param [in] ptr		-	Pointer to element that is inserted
		\param [in] before 	-	Node before which ptr is inserted
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Insert( void *ptr, void *before );

	//==============================================================
	// MoveDown()
	//==============================================================
	/// Moves a list element down one position in the list
	/**
		\param [in] ptr		-	Pointer to list element
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL MoveDown( void *ptr );

	//==============================================================
	// MoveUp()
	//==============================================================
	/// Moves an element up one position in the list
	/**
		\param [in] ptr		-	Pointer to list element
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL MoveUp( void *ptr );

	//==============================================================
	// Delete()
	//==============================================================
	/// Removes an element from the list
	/**
		\param [in] ptr			-	Pointer to list element
		\param [in] bDelete		-	Set to non-zero to destroy the element

		If bDelete is zero, the list element is removed from the list,
		but not destroyed.  It may then be inserted at another point
		in the list.
	*/
	virtual void Delete( void *ptr, BOOL bDelete = TRUE );

	/// Constructor
	CLList();

	/// Destructor
	virtual ~CLList();

	//==============================================================
	// CLList::Put()
	//==============================================================
	/// Inserts a DWORD value into the list
	/**
		\param [in] dw	-	LPVOID value
		
		\return	Pointer to new list element, or NULL if error
	
		\see 
	*/
	void* Put(LPVOID dw)
	{	return New( NULL, dw ); }

	//==============================================================
	// CLList::Put()
	//==============================================================
	/// Inserts a binary buffer value into the list
	/**
		\param [in] pVal	-	Buffer pointer
		\param [in] size	-	Number of byte sin pVal
		
		\return	Pointer to new list element, or NULL if error
	
		\see 
	*/
	void* Put(LPVOID pVal, DWORD size)
	{	return New( NULL, NULL, pVal, size ); }

	//==============================================================
	// CLList::Put()
	//==============================================================
	/// Inserts a string into the list
	/**
		\param [in] pStr	-	NULL terminated string pointer
		
		\return	Pointer to new list element, or NULL if error
	
		\see 
	*/
	void* Put(LPCTSTR pStr)
	{	return Put( (void*)pStr, (DWORD)strlen( pStr ) ); }

	//==============================================================
	// GetKeyStr()
	//==============================================================
	/// Returns the key string for a list element
	/**
		\param [in] ptr		-	Pointer to a list element
		
		\return Pointer the the key string or NULL if none
	
		\see 
	*/
	LPCTSTR GetKeyStr( void *ptr )
	{	return ( (LPLLISTINFO)ptr )->cpkey; }

	//==============================================================
	// GetKeyPtr()
	//==============================================================
	/// Returns a pointer to the binary key data for a list element
	/**
		\param [in] ptr		-	Pointer to a list element
		
		\return Pointer to the binary key data or NULL if none
	
		\see 
	*/
	LPBYTE GetKeyPtr( void *ptr )
	{	return ( (LPLLISTINFO)ptr )->bpkey; }

	//==============================================================
	// GetKeySize()
	//==============================================================
	/// Number of bytes in the list element key
	/**
		\param [in] ptr		-	Pointer to a list element
		
		\return Number of bytes in the list element key
	
		\see 
	*/
	DWORD GetKeySize( void *ptr )
	{	return ( (LPLLISTINFO)ptr )->ksize; }

public:

	//==============================================================
	// SetKey()
	//==============================================================
	/// Sets a binary buffer to use as a list element key
	/**
		\param [in] ptr			-	Pointer to list element
		\param [in] pKey		-	Pointer to key buffer
		\param [in] dwKeySize	-	Number of bytes in pKey
		
		\return Pointer to the list element in ptr
	
		\see 
	*/
	virtual const void* SetKey( void *ptr, const void *pKey, DWORD dwKeySize = 0 );

	//==============================================================
	// DeleteAllItems()
	//==============================================================
	/// Removes all list elements
	void DeleteAllItems();

	//==============================================================
	// NewObj()
	//==============================================================
	/// Creates and initializes a new list element without inserting
	/**
		\param [in] dwSize		-	Size of the list element
		\param [in] init		-	List element initialization data
		\param [in] dwUser		-	User value
		\param [in] pKey		-	Key buffer
		\param [in] dwKeySize	-	Number of bytes in pKey
		
		\return Pointer to new list element
	
		\see 
	*/
	void* NewObj( DWORD dwSize, const void *init = NULL, LPVOID dwUser = 0, const void* pKey = NULL, DWORD dwKeySize = 0 );

	//==============================================================
	// FindByUserData()
	//==============================================================
	/// Searches the list for an element with matching user value
	/**
		\param [in] data	-	User value to search for
		
		\warning This function is slow since it must traverse the list

		\return Pointer to list element or NULL if match not found.
	
		\see 
	*/
	void* FindByUserData( LPVOID data );

	//==============================================================
	// ReleaseStr()
	//==============================================================
	/// Releases a previously allocated string
	/**
		\param [in] pBuf	-	Pointer to string
	*/
	void ReleaseStr( LPSTR *pBuf );

	//==============================================================
	// AllocateStr()
	//==============================================================
	/// Allocates a buffer for a string and initializes
	/**
		\param [in] pBuf	-	Receives pointer to buffer
		\param [in] pStr	-	Pointer to initial value of string
		
		\return Pointer to newly allocated string buffer
	
		\see 
	*/
	LPCTSTR AllocateStr( LPSTR *pBuf, LPCTSTR pStr );

	//==============================================================
	// ReleaseObject()
	//==============================================================
	/// Releases resources allocated for a single list element
	/**
		\param [in] node	-	Pointer to a list element
	*/
	void ReleaseObject( void *node );

	//==============================================================
	// VerifyPointer()
	//==============================================================
	/// Verifys that a pointer is valid by searching for a match in the list
	/**
		\param [in] ptr		-	Pointer to a list element

		\warning This is slow since it must traverse the list until a match is found.
		
		\return Pointer in ptr if valid or NULL if not found
	
		\see 
	*/
	virtual void* VerifyPointer( void *ptr );

	//==============================================================
	// GetByIndex()
	//==============================================================
	/// Retrieves a list element by its position in the list
	/**
		\param [in] i	-	List element position
		
		\warning This function is slow since it must traverse the list

		\return Pointer to a list element or NULL if index was invalid
	
		\see 
	*/
	virtual void* GetByIndex( DWORD i );

	//==============================================================
	// Randomize()
	//==============================================================
	/// Randomizes the position of all list elements
	/**
		\param [in] seed	-	Random seed
		\param [in] loops	-	Number of times to randomize the list
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Randomize( DWORD seed, DWORD loops = 1 );

	//==============================================================
	// Link()
	//==============================================================
	/// Reliniks a list element array
	/**
		\param [in] list	-	List element array
		\param [in] size	-	Size of the array in list
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Link( LPLLISTINFO *list, DWORD size );

	//==============================================================
	// Compare()
	//==============================================================
	/// Compares two list items to determine which should come first
	/**
		\param [in] p1	-	First list element
		\param [in] p2	-	Second list element
		
		\return 1 if p1 should come first, -1 if p2 should come first, 0 if equal
	
		\see 
	*/
	long Compare( LPLLISTINFO p1, LPLLISTINFO p2 );

	//==============================================================
	// rQuickSort()
	//==============================================================
	/// Provies a recursive quick sort
	/**
		\param [in] list	-	Pointer to list array
		\param [in] left	-	Left offset into list
		\param [in] right	-	Right offset into list
	*/
	void rQuickSort( LPLLISTINFO *list, DWORD left, DWORD right );

	//==============================================================
	// QuickSort()
	//==============================================================
	/// Provides a non-recursive quick sort
	/**
		\param [in] list	-	Pointer to list array
		\param [in] left	-	Left offset into list
		\param [in] right	-	Right offset into list
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL QuickSort( LPLLISTINFO *list, DWORD left, DWORD right );

	//==============================================================
	// QuickSort()
	//==============================================================
	/// Performs a quick sort on the list
	/**
		\param [in] bRecursive	-	Set to non-zero to use the stack for sorting
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL QuickSort( BOOL bRecursive = TRUE );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads binary formated list from memory buffer
	/**
		\param [in] buf			-	Pointer to binary data
		\param [in] size		-	Number of bytes in buf
		\param [in] bMerge		-	Non-zero to merge into current list
		
		\return Number of bytes read
	
		\see 
	*/
	virtual DWORD Load( LPBYTE buf, DWORD size, BOOL bMerge = FALSE );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves binary formated list into a memory buffer
	/**
		\param [in] buf		-	Buffer that receives binary formated list
		\param [in] size	-	Size of buffer in buf
		
		\return Number of bytes written to buffer
	
		\see 
	*/
	virtual DWORD Save( LPBYTE buf, DWORD size );
	
	//==============================================================
	// Load()
	//==============================================================
	/// Loads binary formated list from disk file
	/**
		\param [in] pFile	-	Disk filename
		\param [in] bMerge	-	Non-zero to merge into current list
		
		\return Number of bytes read from file
	
		\see 
	*/
	virtual DWORD Load( LPCTSTR pFile, BOOL bMerge = FALSE );

	//==============================================================
	// Save()
	//==============================================================
	/// Writes binary encoded list to a disk file
	/**
		\param [in] pFile	-	Disk filename
		
		\return Number of bytes written to file
	
		\see 
	*/
	virtual DWORD Save( LPCTSTR pFile );

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the number of elements in the list
	DWORD Size() { return m_dwSize; }

	//==============================================================
	// GetNext()
	//==============================================================
	/// Returns the next list item, use for enumerating the list forward
	/**
		\param [in] ptr		-	Pointer to list element

		If ptr is NULL, the head element is returned.

		Example:
		\code
	LPLLISTINFO pLli = NULL;
	while ( NULL != ( pLli = (LPLLISTINFO)GetNext( pLli ) ) )
		; // Process list element
		\endcode
		
		\return Next element or NULL if none.
	
		\see 
	*/
	void * GetNext( void * ptr )
	{	LPLLISTINFO node = (LPLLISTINFO)ptr;
		if ( node != NULL ) 
			return node->pNext; 
		return m_pHead; }

	//==============================================================
	// GetPrev()
	//==============================================================
	/// Returns the previous list item, use for enumerating the list backward
	/**
		\param [in] ptr		-	Pointer to list element

		If ptr is NULL, the tail element is returned.

		Example:
		\code
	LPLLISTINFO pLli = NULL;
	while ( NULL != ( pLli = (LPLLISTINFO)GetPrev( pLli ) ) )
		; // Process list element
		\endcode
		
		\return Previous element or NULL if none.
	
		\see 
	*/
	void * GetPrev( void * ptr )
	{	LPLLISTINFO node = (LPLLISTINFO)ptr;
		if ( node != NULL ) return node->pPrev; return m_pTail; }

	//==============================================================
	// SetUserData()
	//==============================================================
	/// Sets the user DWORD value into a list element
	/**
		\param [in] ptr		-	Pointer to list element
		\param [in] data	-	Number of bytes in ptr.
	*/
	void SetUserData( void * ptr, LPVOID data )
	{	LPLLISTINFO node = (LPLLISTINFO)ptr;
		if ( node != NULL ) node->user = data; }

	//==============================================================
	// GetUserData()
	//==============================================================
	/// Returns the user DWORD value in a list element
	/**
		\param [in] ptr		-	Pointer to a list element
		
		\return User DWORD value
	
		\see 
	*/
	LPVOID GetUserData( void * ptr )
	{	LPLLISTINFO node = (LPLLISTINFO)ptr;
		if ( node != NULL ) return node->user; 
		return 0; 
	}

	//==============================================================
	// CTlLock&()
	//==============================================================
	/// Returns pointer to the thread lock
	operator CTlLock&() { return m_lock; }

	//==============================================================
	// CTlLock*()
	//==============================================================
	/// Returns pointer to the thread lock
	CTlLock* GetLock() { return &m_lock; }

private:

	/// Number of elements in the list
	DWORD			m_dwSize;

	/// Pointer to the head element
	LPLLISTINFO		m_pHead;

	/// Pointer to the tail element
	LPLLISTINFO		m_pTail;

	/// Thread lock
	CTlLock			m_lock;

	/// Thread lock timeout
	DWORD			m_dwLockTimeout;

};

#endif // !defined(AFX_LLIST_H__02D05671_F6C3_40AA_B998_4CBDFCD46238__INCLUDED_)
