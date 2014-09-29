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
#ifndef _TMEM_HEADER_
#define _TMEM_HEADER_

#define RELEASE( x )	( ( x != NULL ) ? delete x, x = NULL : NULL )

#ifndef TRACE
#define TRACE
#endif

//==================================================================
// TMem
//
/// General purpose memory allocation
/**
	General purpose memory allocation
*/
//==================================================================
template<class T> class TMem
{

public:


	//==============================================================
	// TMem()
	//==============================================================
	/// Constructs an object wrapping an array of the specified size
	/**
		\param [in] size	-	Number of elements to allocate
	*/
	TMem( unsigned long size = 0 ) { pData = NULL; dwSize = 0; allocate( size ); }

	//==============================================================
	// TMem()
	//==============================================================
	/// Constructs an object wrapping the specified array
	/**
		\param [in] ptr		-	Pointer to array
		\param [in] size	-	Number of array elements in ptr
	*/
	TMem( T* ptr, unsigned long size = 1 ) { pData = ptr; dwSize = size; }

	/// Destructor
	~TMem() { destroy(); }

	//==============================================================
	// destroy()
	//==============================================================
	/// Releases encapsulated array
	void destroy() 
	{ 	if ( dwSize && ( pData != NULL ) ) 
		{ 	RULIB_TRY
			{	if ( dwSize == 1 ) 
					delete pData; 
				else delete [] pData; 
			} RULIB_CATCH_ALL { }
		}
		pData = NULL; dwSize = 0;  
	}
	

	//==============================================================
	// _allocate()
	//==============================================================
	/// Allocates an array of the specified size
	/**
		\param [in] size	-	Desired array size in elements
		
		\return Non-zero if success
	
		\see 
	*/
	T* _allocate( unsigned long size = 1 )
	{
		// Punt if invalid size
		if ( !size ) return NULL;

		T* ptr = NULL;

		// Allocate memory
		RULIB_TRY
		{	if ( size == 1 ) ptr = new T; 
			else ptr = new T[ size ];
		} RULIB_CATCH_ALL { ptr = NULL; }

		// return success status
		return ptr; 
	}

	//==============================================================
	// allocate()
	//==============================================================
	/// Allocates an array of the specified size
	/**
		\param [in] size	-	Desired array size in elements
		
		\return Non-zero if success
	
		\see 
	*/
	bool allocate( unsigned long size = 1 ) 
	{
		// Do we already have what we need?
		if ( size == dwSize ) return true;

		// Lose the current buffer
		destroy(); 

		// Punt if invalid size
		if ( !size ) return false;

		// Get the memory
		pData = _allocate( size );
		if ( pData == NULL ) return false;

		// Save size
		dwSize = size;

		// return success status
		return true; 
	}


	//==============================================================
	// ensure()
	//==============================================================
	/// Ensures there is <b>at least</b> the specified number of elements in the array
	/**
		\param [in] size	-	Desired array size in elements
		
		If the array size is <b>at least</b> as large as size, this
		function simply returns non-zero.  Otherwise it attempts to
		allocate an array of specified size.

		\return Non-zero if success
	
		\see 
	*/
	bool ensure( unsigned long size = 1 ) 
	{
		// Do we already have what we need?
		if ( size <= dwSize ) return true;

		// return success status
		return allocate( size ); 
	}

	//==============================================================
	// Zero()
	//==============================================================
	/// Zeros the array.  Call only for POD arrays
	void Zero()
	{	if ( pData && size() ) memset( (LPVOID)pData, 0, ( sizeof( T ) * size() ) );
	}

	//==============================================================
	// Set()
	//==============================================================
	/// Sets all memory locations to a specific value.  POD arrays only!
	/**
		\param [in] ch	-	Value to fill memory
	*/
	void Set( BYTE ch )
	{	if ( pData && size() ) memset( (LPVOID)pData, ch, ( sizeof( T ) * size() ) );
	}

	//==============================================================
	// operator =()
	//==============================================================
	/// Copies the specified array
	/**
		\param [in] x	-	Source array to copy
		
		\warning POD arrays only!

		\return Reference to this object
	
		\see 
	*/
	TMem< T >& operator = ( TMem< T >& x ) { copy( x ); return *this; }

	//==============================================================
	// copy()
	//==============================================================
	/// Copies the specified array
	/**
		\param [in] x	-	Source array to copy
		
		\warning POD arrays only!

		\return Non-zero if success
	
		\see 
	*/
	bool copy( TMem< T >& x ) 
	{	if ( !allocate( x.size() ) ) return false;
		if ( size() != x.size() ) return false;
		if ( size() ) memcpy( (LPVOID)pData, (T*)x, ( sizeof( T ) * size() ) );
		return true;
	}

	//==============================================================
	// copy()
	//==============================================================
	/// Copies the specified binary buffer.  POD arrays only!
	/**
		\param [in] ptr		-	Pointer to buffer
		\param [in] len		-	Number of bytes in ptr

		\warning POD arrays only!
		
		\return Non-zero if success
	
		\see 
	*/
	bool copy( const T *ptr, unsigned long len )
	{	if ( ptr == NULL ) { destroy(); return false; }
		if ( !allocate( len ) ) return false;
		memcpy( pData, ptr, len * sizeof( T ) );
		return true;
	}

	//==============================================================
	// clip()
	//==============================================================
	/// Clips the buffer to the specified size
	/**
		\param [in] len		-	Position to clip buffer
		\param [in] str		-	Optional append string
		
		\warning POD arrays only!

		\return Non-zero if success
	
		\see 
	*/
	bool clip( unsigned long len, char *str = NULL )
	{	if ( size() < len + 1 ) return false;
		( (char*)pData )[ len ] = 0;
		if ( str == NULL || *str == 0 ) return true;
		unsigned long i = strlen( str );
		if ( i > len ) return true;
		strcpy( &( (char*)pData )[ len - i ], str );
		return true;
	}

	// Copy from buffer
	//==============================================================
	// put()
	//==============================================================
	/// Copies the specified data into the buffer.  POD arrays only!
	/**
		\param [in] buf		-	Buffer to copy
		\param [in] size	-	Number of bytes in buf
		
		\warning POD arrays only!

		\return Non-zero if success
	
		\see 
	*/
	bool put( const void * buf, unsigned long size )
	{	if ( size > dwSize && !allocate( size ) ) return false;
		if ( pData == NULL || buf == NULL || size == 0 ) return false;		
		memcpy( pData, buf, ( size <= ( dwSize * sizeof( T ) ) ) ? size : ( dwSize * sizeof( T ) ) ); return true; }

	//==============================================================
	// get()
	//==============================================================
	/// Returns the specified number of bytes from the array
	/**
		\param [in] buf		-	Buffer that receives data
		\param [in] size	-	Size of buffer in buf
		
		\return Non-zero if success
	
		\see 
	*/
	bool get( void * buf, unsigned long size )
	{	if ( pData == NULL || buf == NULL || size == 0 ) return false;		
		memcpy( buf, pData, ( size <= ( dwSize * sizeof( T ) ) ) ? size : ( dwSize * sizeof( T ) ) ); return true; }

	//==============================================================
	// grow()
	//==============================================================
	/// Grows the array and preserves data with a binary copy.
	/**
		\param [in] size	-	New array size
		
		This function does nothing if the array is already at least
		as large as size.

		\warning POD arrays only!

		\return Non-zero if success
	
		\see 
	*/
	bool grow( unsigned long size )
	{
		// Are we already big enough?
		if ( size <= dwSize ) return TRUE;

		// Allocate memory
		T* nptr = _allocate( size );
		if ( nptr == NULL ) return false;

		// Copy old buffer
		memcpy( nptr, pData, ( dwSize * sizeof( T ) ) );

		// Lose old buffer
		destroy();

		// Set new values
		pData = nptr;
		dwSize = size;

		return true;
	}

	//==============================================================
	// realloc()
	//==============================================================
	/// Resizes the array and preserves data with a binary copy.
	/**
		\param [in] size	-	New array size
		
		\warning POD arrays only!

		\return Non-zero if success
	
		\see 
	*/
	bool realloc( unsigned long size )
	{
		// Allocate memory
		T* nptr = _allocate( size );
		if ( nptr == NULL ) return false;

		DWORD copy = size;
		if ( copy > dwSize ) copy = dwSize;

		// Copy old buffer
		memcpy( nptr, pData, ( copy * sizeof( T ) ) );

		// Lose old buffer
		destroy();

		// Set new values
		pData = nptr;
		dwSize = size;

		return true;
	}

	//==============================================================
	// size()
	//==============================================================
	// Returns the size of the array in elements
	unsigned long size() { return dwSize; }

	// Attach / Detach
	//==============================================================
	// attach()
	//==============================================================
	/// Attaches to the specified buffer
	/**
		\param [in] x		-	Pointer to element buffer
		\param [in] size	-	Number of elements in x
	*/
	void attach( T* x, unsigned long size = 1 ) 
	{	destroy(); if ( x != NULL && size ) { pData = x; dwSize = size; } }

	//==============================================================
	// detach()
	//==============================================================
	/// Detaches from the internal array without releasing the memory
	/**
		\return Pointer to the internal array
	
		\see 
	*/
	T* detach() { T* ret = pData; pData = NULL; dwSize = 0; return ret; } 

	//==============================================================
	// attach()
	//==============================================================
	/// Attaches to the specified memory object buffer
	/**
		\param [in] x	-	Buffer to assume control of.  This object
							will <b>not</b> contain a valid buffer
							after this call.
		
		\return Size of the array in elements
	
		\see 
	*/
	unsigned long attach( TMem< T >& x )
	{	destroy(); pData = x.pData; dwSize = x.dwSize; x.detach(); return dwSize; }

// This suppresses the compiler warning when this
// template is used with a common data type
// **** There is a bug in MSVC++ 5.0 that may cause it to choke on
//		these pragmas.  If this happens you'll have to comment them 
//		and live with the warnings
//
// This *sometimes* brings up a bug in the microsoft VC++ 5.0 compiler
// so we have to live with the warning here
// Hopefully be able to re-enable these pragmas in VC++ 6.0 (nope), 7 (no), 8 (no), 9?
//#pragma warning( disable : 4284 )
//	T* operator ->() { return pData; } 
//#pragma warning( default : 4284 )

	//==============================================================
	// operator *()
	//==============================================================
	/// Returns a pointer to the array
	T& operator *() { return *pData; }

	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a reference to the specified array element
	/**
		\param [in] i	-	Array element index
		
		If the index is out of range, the first element is returned.

		\return Reference to the specified element.
	
		\see 
	*/
	T& operator []( const unsigned long i ) { if ( i < dwSize ) return pData[ i ]; else return pData[ 0 ]; }

	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a reference to the specified array element
	/**
		\param [in] i	-	Array element index
		
		If the index is out of range, the first element is returned.

		\return Reference to the specified element.
	
		\see 
	*/
	T& operator []( const long i ) { if ( i >= 0 && (DWORD)i < dwSize ) return pData[ i ]; else return pData[ 0 ]; }

	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a reference to the specified array element
	/**
		\param [in] i	-	Array element index
		
		If the index is out of range, the first element is returned.

		\return Reference to the specified element.
	
		\see 
	*/
	T& operator []( const int i ) { if ( i >= 0 && (DWORD)i < dwSize ) return pData[ i ]; else return pData[ 0 ]; }

	//==============================================================
	// operator &()
	//==============================================================
	/// Returns a reference to the array 
	T* operator &() { return pData; }

	//==============================================================
	// operator T*()
	//==============================================================
	/// Returns a pointer to the array 
	operator T*() { return pData; }

	//==============================================================
	// operator T()
	//==============================================================
	/// Returns the first element in the array
	operator T() { return *pData; }
	
	//==============================================================
	// ptr()
	//==============================================================
	/// Returns a pointer to the array 
	/**
		\param [in] offset	-	Pointer offset of array element

		\return Pointer to the specified array element
	*/
	T* ptr( unsigned long offset = 0 ) { return &pData[ offset ]; }

	//==============================================================
	// str()
	//==============================================================
	/// Returns a pointer to the array cast as a character string (char*)
	/**
		\param [in] offset	-	Pointer offset of array element

		\return Pointer to the specified array element cast to (char*)
	*/
	char* str( unsigned long offset = 0 ) { return (char*)&pData[ offset ]; }

	//==============================================================
	// element_size()
	//==============================================================
	/// Returns the Number of bytes in a single array element
	DWORD element_size() { return sizeof( T ); }

	//==============================================================
	// crc()
	//==============================================================
	/// Returns the CRC of the buffer
	DWORD crc() 
	{	if ( !pData ) return 0;
#if !defined( __GNUC__ )
		return CWinFile::CRC32( 0, (LPBYTE)pData, dwSize * sizeof( T ) ); 
#else
		return 0;
#endif
	}

protected:

	/// Pointer to the array
	T*				pData;

	/// Number of elements in the array
	unsigned long	dwSize;

}; // end class


//==================================================================
// TPodAutoPtr
//
/// Automatic memory management for Plain Old Data (POD) types
/**
	Provides automatic memory management for Plain Old Data (POD) types
*/
//==================================================================
template<class T> class TPodAutoPtr : public TMem< BYTE >
{

public:

	//==============================================================
	// operator *()
	//==============================================================
	/// Returns a reference to the first object
	T& operator *() { return (T&)*pData; }

	//==============================================================
	// operator *()
	//==============================================================
	/// Returns a pointer to the first object
	T* operator &() { return (T*)pData; }

	//==============================================================
	// operator ->()
	//==============================================================
	/// Differences the first object
	T* operator ->() { return (T*)pData; }

};

//==================================================================
// TAutoPtr
//
/// Automatic memory release by calling delete on destruction.
/**
	Provides automatic memory release by calling delete on destruction
*/
//==================================================================
template<class T> class TAutoPtr : public TMem< T >
{

public:

	//==============================================================
	// TAutoPtr()
	//==============================================================
	/// Constructs an object and allocates the specified number of objects
	/**
		\param [in] size	-	Array size in elements
	*/
	TAutoPtr( unsigned long size = 0 ) :
		TMem< T >( size ) { }

	//==============================================================
	// TAutoPtr()
	//==============================================================
	/// Constructs an object and wraps the specified array
	/**
		\param [in] ptr		-	Object array pointer
		\param [in] size	-	Array size in elements
	*/
	TAutoPtr( T* ptr, unsigned long size = 1 ) :
		TMem< T >( ptr, size ) { }

	//==============================================================
	// operator *()
	//==============================================================
	/// Returns a reference to the first object
	T& operator *() { return (T&)*TMem< T >::pData; }

	//==============================================================
	// operator &()
	//==============================================================
	/// Returns a pointer to the first object
	T* operator &() { return (T*)TMem< T >::pData; }

	//==============================================================
	// operator ->()
	//==============================================================
	/// Differences the first object
	T* operator ->() { return (T*)TMem< T >::pData; }

	//==============================================================
	// operator =()
	//==============================================================
	/// Attaches to the specified object
	/**
		\param [in] ptr		-	Object that is attached

		\return Value in ptr
	*/
	T* operator =( T *ptr ) { return attach( ptr ); }

};

//==================================================================
// TAutoResetEvent
//
/// Automatic event reset by calling ResetEvent() on destruction
/**
	Provides automatic event reset by calling ResetEvent() on destruction
*/
//==================================================================
template<class T> class TAutoResetEvent
{

public:

	//==============================================================
	// TAutoResetEvent()
	//==============================================================
	/// Wraps the specified event
	/**
		\param [in] hEvent	-	Windows event handle
	*/
	TAutoResetEvent( T hEvent ) { m_hEvent = hEvent; }

	/// Destructor
	~TAutoResetEvent() { Reset(); }

	//==============================================================
	// Reset()
	//==============================================================
	/// Resets the event and detaches
	void Reset() 
	{	if ( m_hEvent ) ResetEvent( m_hEvent ); m_hEvent = NULL; }

	//==============================================================
	// operator *()
	//==============================================================
	/// Returns a reference to the event
	T& operator *() { return m_hEvent; }

	//==============================================================
	// operator &()
	//==============================================================
	/// Returns a pointer to the event
	T* operator &() { return &m_hEvent; }

private:

	/// Event handle
	T		m_hEvent;

};

#endif // _TMEM_HEADER