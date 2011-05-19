/*------------------------------------------------------------------
// ScsSerialize.h
// Copyright (c) 2006 
// Robert Umbehant
// rumbehant@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or 
// without modification, are permitted for commercial and 
// non-commercial purposes, provided that the following 
// conditions are met:
//
// * Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to 
//   endorse or promote products derived from this software without 
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#pragma once

// Disable VC6 length warning
#if defined( _MSC_VER ) && _MSC_VER <= 1200
#	pragma warning( disable : 4786 )
#endif

#include <map>
#include <string>

#ifndef tscsStringMacros

#if defined( TCHAR )
#   define tscsTCHAR    TCHAR
#elif defined( _UNICODE ) || defined( UNICODE )
#   define tscsTCHAR    wchar_t
#else
#   define tscsTCHAR    char
#endif

#   define tscsTT( c, s ) ( 1 == sizeof( c ) ? ( ( c* )s ) : ( ( c* )L##s ) )
#   define tscsTC( c, s ) ( 1 == sizeof( c ) ? ( s ) : ( L##s ) )

#   define tscsStringMacros

#   define tscsStrWToStr8( s )          scsStrToStr< char, wchar_t >( s )
#   define tscsStr8ToStrW( s )          scsStrToStr< wchar_t, char >( s )

#   define tscsStr8ToBin( s )           ( s )
#   define tscsStrWToBin( s )           scsStrToBin< wchar_t >( s )

#   define tscsBinToStr8( s )           ( s )
#   define tscsBinToStrW( s )           scsStrToStr< wchar_t, char >( s )

#if defined( _UNICODE ) || defined( UNICODE )

#       define tscsStrToStr8( s )       scsStrToStr< char, wchar_t >( s )
#       define tscsStr8ToStr( s )       scsStrToStr< wchar_t, char >( s )

#       define tscsStrToStrW( s )       ( s )
#       define tscsStrWToStr( s )       ( s )

#       define tscsStrToBin( s )        scsStrToBin< wchar_t >( s )
#       define tscsBinToStr( s )        scsStrToStr< wchar_t, char >( s )

#   else

#       define tscsStrToStr8( s )       ( s )
#       define tscsStr8ToStr( s )       ( s )

#       define tscsStrToStrW( s )       scsStrToStr< wchar_t, char >( s )
#       define tscsStrWToStr( s )       scsStrToStr< char, wchar_t >( s )

#       define tscsStrToBin( s )        ( s )
#       define tscsBinToStr( s )        ( s )

#   endif

#endif

//==================================================================
// TScsPropertyBag
//
/// Implements a multi-dimensional property bag with nested serialization
/**

	This class provides functionality for a multi-dimensional
	property bag.  It also provides automatic type conversions
	and, hopefully, easily ported serialization.

	Typical use

	CScsPropertyBag arr1, arr2;

	arr1[ "A" ][ "AA" ] = "Hello World!";
	arr1[ "A" ][ "AB" ] = (long)1;
	arr1[ "B" ][ "BA" ] = (double)3.14159;
	
	for ( long i = 0; i < 4; i++ )
		arr1[ "list" ][ i ] = i * 2;

	// Encode
	CScsPropertyBag::t_String str = arr.serialize();

	// Let's have a look at the encoded string...
	TRACE( str.c_str() ); TRACE( _T( "\n" ) );

	// Decode
	arr2.deserialize( str );	

	// 'Hello World!' check...
  	TRACE( arr2[ "A" ][ "AA" ] ); TRACE( _T( "\n" ) );

	// Get long value
	long lVal = arr2[ "A" ][ "AB" ].ToLong();

	// Get double
	double dVal = arr2[ "B" ][ "BA" ].ToDouble();

	// Get string value
	LPCTSTR pString = arr2[ "list" ][ 0 ];

*/
//==================================================================
template < class T > class TScsPropertyBag
{
public:

	//==================================================================
	// CAutoMem
	//
	/// Just a simple auto pointer
	/**
		This class is a simple auto pointer.  It has properties that I
		particularly like for this type of job.  I'll quit making my
		own when boost comes with VC...		
	*/
	//==================================================================
	template < class T > class CAutoMem
	{
		public:

			/// Default constructor
			CAutoMem() { m_p = NULL; }

			/// Destructor
			~CAutoMem() { release(); }

			/// Release allocated object
			void release() { if ( m_p ) { delete m_p; m_p = NULL; } }

			/// Returns a pointer to encapsulated object
			T& Obj() { if ( !m_p ) m_p = new T; return *m_p; }

			/// Returns a pointer to encapsulated object
			T* Ptr() { if ( !m_p ) m_p = new T; return m_p; }

			/// Returns a reference to the encapsulated object
			operator T&() { return Obj(); }
			
			/// Returns a reference to the encapsulated object
			T& operator *() { return Obj(); }
			
			/// Returns a pointer to the encapsulated object
			T* operator ->() { return Ptr(); }

			/// Returns non-zero if valid object
			int IsValid() { return NULL != m_p; }

		private:

			/// Contains a pointer to the controlled object
			T		*m_p;
			
	};

	/// Unicode friendly string
	typedef std::basic_string< T > t_String;

	/// Our multi-dimensional string array type
	typedef std::map< t_String, CAutoMem< TScsPropertyBag< T > > > t_StringArray;

	/// Iterator type
	typedef typename t_StringArray::iterator iterator;

public:

	iterator begin() 
    {   return m_lstSub.begin(); }

	iterator end() 
    {   return m_lstSub.end(); }
	
	iterator next( iterator it )
	{
		// First
		if ( end() == it ) 
			return begin();

		// Next
		it++;
		return it;		
	}

	size_t count() 
    {   return m_lstSub.size(); }

	/// Default constructor
	TScsPropertyBag() 
    {}

    /// Destructor
    virtual ~TScsPropertyBag() 
    {   Destroy(); }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all memory resources and prepares class for reuse.
    void Destroy()
    {   m_str.release();
	    m_lstSub.clear();
    }

	//==============================================================
	// TScsPropertyBag()
	//==============================================================
	/// Constructos object from encoded string
	/**
		\param [in] sStr	-	Encoded array
	*/
	TScsPropertyBag( t_String sStr )
	{	deserialize( sStr );
	}

	//==============================================================
	// TScsPropertyBag()
	//==============================================================
	/// Copy constructor
	/**
		\param [in] rPb	-	Property bag to copy
	*/
	TScsPropertyBag( TScsPropertyBag &rPb )
    {   Copy( rPb ); }

	//==============================================================
	// operator = ()
	//==============================================================
	/// Assignment operator
	/**
		\param [in] rPb	-	Property bag to copy
	*/
	TScsPropertyBag& operator = ( TScsPropertyBag &rPb )
    {   return Copy( rPb ); }

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies another property bag
	/**
		\param [in] rPb 	-	Property bag to copy
        \param [in] bMerge  -   Non-zero if contents of rPb should
                                be merged with the current property
                                bag.
	*/
	TScsPropertyBag& Copy( TScsPropertyBag &rPb, BOOL bMerge = FALSE )
	{
        if ( !bMerge ) 
            Destroy();

        for ( iterator it = rPb.begin(); rPb.end() != it; it = rPb.next( it ) )
        {
            // Copy array
            if ( it->second->IsArray() )
                (*this)[ it->first ].Copy( it->second.Obj() );

            // Copy single value
            else
                (*this)[ it->first ] = it->second->Val();

        } // for

        return *this;
	}


	//==============================================================
	// IsStdChar()
	//==============================================================
	/// Returns non-zero if the character does *not* need encoding
	/**
		\param [in] ch	-	Character to check
	*/
	static int IsStdChar( T ch ) 
	{	return	( tscsTC( T, 'a' ) <= ch && tscsTC( T, 'z' ) >= ch ) ||
				( tscsTC( T, 'A' ) <= ch && tscsTC( T, 'Z' ) >= ch ) ||
				( tscsTC( T, '0' ) <= ch && tscsTC( T, '9' ) >= ch ) ||
				tscsTC( T, '_' ) == ch || tscsTC( T, '-' ) == ch || 
				tscsTC( T, '.' ) == ch;
	}


	//==============================================================
	// urlencode()
	//==============================================================
	/// Returns URL encoded version of a string.
	/**
		\param [in] sStr	-	String to encode
		
		\return TSend_string object containing encoded string
	*/
	static t_String urlencode( t_String sStr )
	{
		t_String sRes;
		long lLen = sStr.length(), i = 0;

		T tmp[ 256 ];
		while ( i < lLen )
		{
			if ( IsStdChar( sStr[ i ] ) ) sRes += sStr[ i ];
			else
			{	StrFmt( tmp, tscsTT( T, "%%%02lX" ), (long)sStr[ i ] );
				sRes += tmp;
			} // end else			 

			i++;

		} // end while

		return sRes;
	}

	//==============================================================
	// urldecode()
	//==============================================================
	/// Decodes URL encoded string
	/**
		\param [in] sStr	-	URL encoded string to decode
		
		\return Decoded string
	*/
	static t_String urldecode( t_String sStr )
	{
		t_String sRes;
		long lLen = sStr.length(), i = 0;

		T tmp[ 256 ];
		while ( i < lLen )
		{
			if ( tscsTC( T, '%' ) != sStr[ i ] ) sRes += sStr[ i ];
			else
			{
				tmp[ 0 ] = sStr[ ++i ]; tmp[ 1 ] = sStr[ ++i ]; tmp[ 2 ] = 0;

				sRes += (T)( StrToULong( tmp, 16 ) );

			} // end else			 

			i++;

		} // end while

		return sRes;
	}

	//==============================================================
	// serialize()
	//==============================================================
	/// Serializes the array
	/**
		\return Serialized array.
	
		\see 
	*/
	t_String serialize()
	{
		t_String sRes;
		
		// Just return our value if we're not an array
		if ( !IsArray() ) 
            return m_str.Obj();

		// Iterator
		t_StringArray::iterator pos = m_lstSub.begin();

		// For each array element
		while ( pos != m_lstSub.end() )
		{
			// Add separator if needed
			if ( sRes.length() ) 
                sRes += tscsTC( T, ',' );

			sRes += urlencode( pos->first );

			// Is it an array?
			if ( pos->second.Obj().IsArray() )
			{ 
				sRes += tscsTC( T, '{' ); 
				sRes += pos->second.Obj().serialize(); 
				sRes += tscsTC( T, '}' ); 
			}
				
			// Serialize the value
			else 
                sRes += tscsTC( T, '=' ), 
                sRes += urlencode( pos->second.Obj().ToStr() );

			// Next array element
			pos++;

		} // end while
		
		return sRes;
	}


	//==============================================================
	// deserialize()
	//==============================================================
	/// Deserializes an array from string
	/**
		\param [in] sStr	-	Serialized array string.
		\param [in] bMerge	-	Non-zero if array should be merged
								into current data.  Set to zero to
								replace current array.
		\param [in] pLast	-	Receives the number of bytes decoded.
		\param [in] pPs		-	Property bag that receives any decoded
								characters.  We could also have just
								called this function on the object,
								but this way provides a little extra
								flexibility for later.
		
		\return Number of items deserialized.
	
		\see 
	*/
	long deserialize( t_String sStr, long bMerge = FALSE, long *pLast = NULL, TScsPropertyBag *pPs = NULL )
	{
		// Ensure object
		if ( !pPs )
		{	pPs = this;
			sStr += tscsTT( T, "," );
		} // end if

		// Do we want to merge?
		if ( !bMerge ) 
            pPs->Destroy();
		
		long lItems = 0;
		long lLen = sStr.length(), s = 0, e = 0;

		while ( e < lLen )
		{
			switch( sStr[ e ] )
			{
				case tscsTC( T, ',' ) : case tscsTC( T, '}') :
				{
					if ( 1 < e - s )
					{
						// Find '='
						long a = s; 
                        while ( a < e && tscsTC( T, '=' ) != sStr[ a ] ) 
                            a++;

						t_String sKey, sVal;

						// First character is separator
						if ( a == s ) 
                            sKey = urldecode( t_String( &sStr.c_str()[ s + 1 ], e - s - 1 ) );

						else 
                            sKey = urldecode( t_String( &sStr.c_str()[ s ], a - s ) );
						
						// Single token
						if ( 1 >= e - a ) 
                            (*pPs)[ sKey ] = tscsTT( T, "" );

						// Both tokens present
						else 
                            (*pPs)[ sKey ] = urldecode( t_String( &sStr.c_str()[ a + 1 ], e - a - 1 ) );

						// Count one item
						lItems++;

					} // end if

					// Next element
					s = e + 1;

					// Time to exit?
					if ( tscsTC( T, '}' ) == sStr[ e ] )
					{	if ( pLast ) 
                            *pLast = e + 1; 
                        return lItems; 
                    } // end if

				} break;

				case tscsTC( T, '{' ) :
				{			   
					// Get key
					t_String sKey = urldecode( t_String( &sStr.c_str()[ s ], e - s ) );

					// Do we have a key?
					if ( sKey.length() )
					{
						// This will point to the end of the array we're about to decode
						LONG lEnd = 0;

						// Get the sub array
						lItems += deserialize( t_String( &sStr.c_str()[ e + 1 ] ), TRUE, &lEnd, &(*pPs)[ sKey ] );

						// Skip the array we just decoded
						e += lEnd;

					} // end if

					// Skip this token
					s = e + 1; 

				} break;

			} // end switch

			// Next i
			e++;

		} // end while

		// Just save the string if we found no items
		if ( !lItems && lLen ) 
        {   *pPs = sStr; lItems++; }

		return lItems;
	}


	//==============================================================
	// operator []()
	//==============================================================
	/// Indexes into sub array
	/**
		\param [in] pKey	-	Index key
		
		\return Reference to sub class.
	
		\see 
	*/
    TScsPropertyBag& operator []( LPCTSTR pKey ) 
    {   return m_lstSub[ pKey ]; }

	//==============================================================
	// operator []()
	//==============================================================
	/// Indexes into sub array
	/**
		\param [in] sKey	-	Index key
		
		\return Reference to sub class.
	
		\see 
	*/
	TScsPropertyBag& operator []( t_String sKey ) 
    {   return m_lstSub[ sKey.c_str() ]; }

	//==============================================================
	// operator []()
	//==============================================================
	/// Indexes into sub array
	/**
		\param [in] n	-	Index key
		
		\return Reference to sub class.
	
		\see 
	*/
	TScsPropertyBag& operator []( long n ) 
	{	T szKey[ 256 ] = { 0 };
		StrFmt( szKey, tscsTT( T, "%li" ), n );
		return m_lstSub[ szKey ]; 
	}

	//==============================================================
	// operator []()
	//==============================================================
	/// Indexes into sub array
	/**
		\param [in] n	-	Index key
		
		\return Reference to sub class.
	
		\see 
	*/
	TScsPropertyBag& operator []( unsigned long n ) 
	{	T szKey[ 256 ] = { 0 };
		StrFmt( szKey, tscsTT( T, "%lu" ), n );
		return m_lstSub[ szKey ]; 
	}

	//==============================================================
	// operator []()
	//==============================================================
	/// Indexes into sub array
	/**
		\param [in] n	-	Index key
		
		\return Reference to sub class.
	
		\see 
	*/
	TScsPropertyBag& operator []( double n ) 
	{	T szKey[ 256 ] = { 0 };
		StrFmt( szKey, tscsTT( T, "%g" ), n );
		return m_lstSub[ szKey ]; 
	}

	//==============================================================
	// operator = ()
	//==============================================================
	/// Conversion from string object
	t_String operator = ( t_String sStr ) 
	{	m_str.Obj() = sStr.c_str(); return m_str.Obj(); }


	//==============================================================
	// operator = ()
	//==============================================================
	/// Conversion from string
	t_String operator = ( const T *pStr ) 
	{	m_str.Obj() = pStr; return m_str.Obj(); }


	//==============================================================
	// operator = ()
	//==============================================================
	/// Conversion from long
	t_String operator = ( long lVal )
	{	T num[ 256 ] = { 0 };
		StrFmt( num, tscsTT( T, "%li" ), lVal );
		m_str.Obj() = num; return m_str.Obj(); 
	}

	//==============================================================
	// operator = ()
	//==============================================================
	/// Conversion from unsigned long
	t_String operator = ( unsigned long ulVal )
    {	T num[ 256 ] = { 0 };
		StrFmt( num, tscsTT( T, "%lu" ), ulVal );
		m_str.Obj() = num; return m_str.Obj(); 
	}

	//==============================================================
	// operator = ()
	//==============================================================
	/// Conversion from double
	t_String operator = ( double dVal )
	{	T num[ 256 ] = { 0 };
		StrFmt( num, tscsTT( T, "%g" ), dVal );
		m_str.Obj() = num; return m_str.Obj(); 
	}

	//==============================================================
	// Val()
	//==============================================================
	/// Returns string object reference
	t_String& Val() 
    {   return m_str.Obj(); }

	//==============================================================
	// LPCTSTR()
	//==============================================================
	/// Conversion to string
	operator const T *() 
    {   return ToStr(); }


	//==============================================================
	// ToStr()
	//==============================================================
	/// Returns local string object
	const T * ToStr() 
    {   return m_str.Obj().c_str(); }

	//==============================================================
	// ToLong()
	//==============================================================
	/// Converts to long
	long ToLong() 
    {   return StrToLong( ToStr(), 10 ); }

	//==============================================================
	// ToULong()
	//==============================================================
	/// Converts to unsigned long
	long ToULong() 
    {   return StrToULong( ToStr(), 10 ); }

	//==============================================================
	// ToDouble()
	//==============================================================
	/// Converts to double
	double ToDouble() 
    {   return StrToDouble( ToStr(), NULL ); }

	//==============================================================
	// IsArray()
	//==============================================================
	/// Returns non-zero if array elements are present
	int IsArray() 
    {   return 0 < m_lstSub.size(); }

	//==============================================================
	// IsKey()
	//==============================================================
	/// Returns non-zero if the specified key is valid
    int IsKey( T *pKey )
    {   return ( m_lstSub.end() != m_lstSub.find( pKey ) ); }

	//==============================================================
	// IsValidKey()
	//==============================================================
	/// Returns non-zero if the specified key is valid
    int IsValidKey( T *pKey )
    {   iterator it = m_lstSub.find( pKey );
        if ( m_lstSub.end() == it )
            return 0;
        return it->second->Val().length();
    }

    //==============================================================
	// Unset()
	//==============================================================
	/// Returns non-zero if the specified key is valid
    int Unset( T *pKey )
    {   iterator it = m_lstSub.find( pKey );
        if ( m_lstSub.end() != it ) 
            m_lstSub.erase( it ); 
    }

public:

    // *** Multi-byte
    static int StrFmt( char * pDest, const char * pFormat, ... )
    {   return vsprintf( pDest, pFormat, (va_list)( &pFormat + 1 ) ); }

    static long long StrToInt64( const char * x_pStr, unsigned int x_uRadix ) 
    {   return _strtoi64( x_pStr, NULL, x_uRadix ); }

    static unsigned long long StrToUInt64( const char * x_pStr, unsigned int x_uRadix ) 
    {   return _strtoui64( x_pStr, NULL, x_uRadix ); }

    static long StrToLong( const char * x_pStr, unsigned int x_uRadix ) 
    {   return strtol( x_pStr, NULL, x_uRadix ); }

    static unsigned long StrToULong( const char * x_pStr, unsigned int x_uRadix ) 
    {   return strtoul( x_pStr, NULL, x_uRadix ); }

    static double StrToDouble( const char * x_pStr ) 
    {   return strtod( x_pStr, NULL ); }


    // *** Unicode
    static int StrFmt( const wchar_t * pDest, const wchar_t * pFormat, ... )
    {   return vswprintf( pDest, pFormat, (va_list)( &pFormat + 1 ) ); }

    static long long StrToInt64( const wchar_t * x_pStr, unsigned int x_uRadix ) 
    {   return _wcstoi64( x_pStr, NULL, x_uRadix ); }

    static unsigned long long StrToUInt64( const wchar_t * x_pStr, unsigned int x_uRadix ) 
    {   return _wcstoui64( x_pStr, NULL, x_uRadix ); }

    static long StrToLong( const wchar_t * x_pStr, unsigned int x_uRadix ) 
    {   return wcstol( x_pStr, NULL, x_uRadix ); }

    static unsigned long StrToULong( const wchar_t * x_pStr, unsigned int x_uRadix ) 
    {   return wcstoul( x_pStr, NULL, x_uRadix ); }

    static double StrToDouble( const wchar_t * x_pStr ) 
    {   return wcstod( x_pStr, NULL ); }

public:

    t_String tab( UINT uTabs )
    {   t_String s; while ( uTabs-- ) s += tscsTT( T, "    " ); return s; }
    
    t_String print( BOOL bShowValues = TRUE, TScsPropertyBag *pPb = NULL, UINT uDepth = 0 )
    {
        if ( !pPb )
            pPb = this;

        t_String ret;
        for ( iterator it = pPb->begin(); it != pPb->end(); it = pPb->next( it ) )
        {
            // First key
            ret += tab( uDepth ) + it->first + tscsTT( T, " = " );

            if ( it->second->IsArray() )

                ret += tscsTT( T, "\n" )
                       + tab( uDepth + 1 ) + tscsTT( T, "{\n" )
                       + print( bShowValues, it->second.Ptr(), uDepth + 2 )
                       + tscsTT( T, "\n" ) + tab( uDepth + 1 ) + tscsTT( T, "}\n" );

            else
                ret += ( bShowValues ? it->second->Val() : tscsTT( T, "...\n" ) );

        } // end for

        return ret;
    }

private:

	/// Our value
	CAutoMem< t_String >			m_str;

	/// Array of strings
	t_StringArray					m_lstSub; 

};

/// Property bag types
/** \see TScsPropertyBag */

typedef TScsPropertyBag< tscsTCHAR >    CScsPropertyBag;
typedef TScsPropertyBag< char >         CScsPropertyBag8;
typedef TScsPropertyBag< wchar_t >      CScsPropertyBagW;

// Convert string the hard way
template < typename T_RET, typename T_IN >
    std::basic_string< T_RET > scsStrToStr( std::basic_string< T_IN > s )
    {
        // Straight forward truncate or pad conversion
        std::basic_string< T_RET > ret;
        ret.reserve( s.length() );
        for ( unsigned int i = 0; i < s.length(); i++ )
            ret += (T_RET)s[ i ];

        return ret;
    }

// Binary conversion
template < typename T_IN >
    std::basic_string< char > scsStrToBin( std::basic_string< T_IN > s )
    {
        std::basic_string< char > ret;
        ret.reserve( s.length() * sizeof( T_IN ) );
        for ( unsigned int i = 0; i < s.length(); i++ )
        {
            // Binary conversion, preserves all data
            char *p = (char*)&s[ i ];
            for ( unsigned int b = 0; b < sizeof( T_IN ); b++ )
                ret += (T_RET)s[ b ];

        } // end for

        return ret;
    }

