// md5.cpp - modified by Wei Dai from Colin Plumb's public domain md5.c
// any modifications are placed in the public domain
// Further modifications by Robert Umbehant placed in the public domain

#include "StdAfx.h"
//#include "Wincrypt.h"

void CMD5::Init()
{_STT();
	m_hash[ 0 ] = 0x67452301L;
	m_hash[ 1 ] = 0xefcdab89L;
	m_hash[ 2 ] = 0x98badcfeL;
	m_hash[ 3 ] = 0x10325476L;

	m_total = 0;
}

template <class T> inline T rotlFixed(T x, unsigned int y)
{_STT();
	return ( x << y ) | ( x >> ( sizeof( T ) * 8 - y ) );
}

void CMD5::Transform (DWORD *hash, const DWORD *in)
{_STT();
// #define F1(x, y, z) (x & y | ~x & z)
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
	w = rotlFixed(w + f(x, y, z) + data, s) + x

    DWORD a, b, c, d;

	a = hash[ 0 ];
	b = hash[ 1 ];
	c = hash[ 2 ];
	d = hash[ 3 ];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	hash[ 0 ]+=a;
	hash[ 1 ]+=b;
	hash[ 2 ]+=c;
	hash[ 3 ]+=d;
}

BOOL CMD5::Transform(LPBYTE buf, DWORD size)
{_STT();
	DWORD i = 0;
	DWORD left = m_total & 0x3f;

	// Track total bytes hashed
	m_total += size;

	// Anything left over from last time?
	if ( left > 0 )
	{
		// How many new bytes
		DWORD copy = size;
		if ( size > ( 64 - left ) ) copy = 64 - left;
		memcpy( &m_buf[ left ], buf, copy );
		left += copy;

		// Enough to use?
		if ( left < 64 ) return TRUE;

		// Transform and empty buffer
		vTransform( (LPDWORD)m_buf );
		left = 0;

		// Take away used bytes
		i += copy;
	
	} // end if

	// Transform blocks
	while ( ( size - i ) >= 64 )
	{	vTransform( (LPDWORD)&buf[ i ] );
		i += 64;
	} // end while

	// All done?
	if ( ( size - i ) == 0 ) return TRUE;

	// Copy left over bytes
	left = size - i;
	memcpy( m_buf, (LPVOID)&buf[ i ], left );

	return TRUE;
}

BOOL CMD5::End()
{_STT();
	DWORD left = m_total & 0x3f;

	// Whatever
	m_buf[ left++ ] = 0x80;

	// Go ahead and transform if not enough room for size
	if ( left > 56 )
	{	if ( left < 64 ) memset( &m_buf[ left ], 0, 64 - left );
		vTransform( (LPDWORD)m_buf );
		left = 0;
	} // end if

	// Pad out to 56 bytes
	if ( left < 56 ) memset( &m_buf[ left ], 0, 56 - left );

	// Set size (again whatever)
	( (LPDWORD)m_buf )[ 14 ] = m_total << 3;
	( (LPDWORD)m_buf )[ 15 ] = m_total >> 29;

	// Transform the data
	vTransform( (LPDWORD)m_buf ); 	
	memcpy( m_digest, m_hash, sizeof( m_digest ) );
	ByteReverse( (LPBYTE)m_digest, 4 );

	// Don't leave input data laying around
	ZeroMemory( m_buf, sizeof( m_buf ) );

	return TRUE;
}

void CMD5::ByteReverse (LPBYTE buf, DWORD longs)
{_STT();

	for ( DWORD i = 0; i < longs; i++ )
	{
		DWORD t =	(DWORD)buf[ 0 ] << 24 | 
					(DWORD)buf[ 1 ] << 16 |
					(DWORD)buf[ 2 ] << 8 | 
					(DWORD)buf[ 3 ];
		*(LPDWORD)buf = t;
		buf += 4;
	} // end for
}

BOOL CMD5::Transform(LPCTSTR pString)
{_STT();
	return Transform( (LPBYTE)pString, strlen( pString ) );
}


BOOL CMD5::TransformFile(LPCTSTR pFile)
{_STT();
	// Attempt to open the file
	HANDLE hFile = CreateFile( pFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD	read = 0;
	DWORD	size = 64 * 64;
	BYTE	buf[ 64 * 64 ];
	BOOL    success;

	// For each block
	while ( ( success = ReadFile( hFile, buf, size, &read, NULL ) ) && read == size )
		Transform( buf, size );

	// Close the file handle
	CloseHandle( hFile );

	// Check for file error
	if ( !success ) return FALSE;

	// Process last block if any
	if ( read > 0 ) Transform( buf, size );

	return TRUE;
}

void CMD5::CalcOffsetMask( DWORD bit, LPDWORD offset, LPBYTE mask )
{_STT();
	// Calculate byte offset
	*offset = ( bit & 0xfffffff8 ) >> 3;

	// Calculate mask
	*mask = 0x01 << ( bit & 0x07 );
}

DWORD CMD5::GetBit( LPBYTE buf, DWORD bit )
{_STT();
	return ( buf[ ( bit & 0xfffffff8 ) >> 3 ] & ( 0x01 << ( bit & 0x07 ) ) );
}

// Obviously, can only read max of 32 bits
DWORD CMD5::GetValue( LPBYTE buf, DWORD bitoff, DWORD bits )
{_STT();
	DWORD value = 0;
	DWORD mask = 1;

	for ( DWORD i = 0; i < bits; i++ )
	{
		if ( GetBit( buf, ( bitoff + i ) ) )
			value |= mask;
		mask <<= 1;
	} // end for

	return value;
}

BOOL CMD5::HashToString(LPBYTE hash, DWORD hashbits, LPSTR str, DWORD size)
{_STT();
	DWORD i = 0;
	DWORD b = 0;

	// Sanity check
	if ( hash == NULL || hashbits == 0 || str == NULL || size < ( ( hashbits / 6 ) + 2 ) )
		return FALSE;

	while ( i < hashbits )
	{
		// Get value of next six bits
		DWORD	usebits = ( hashbits - i ) >= 6 ? 6 : hashbits - i;
		str[ b ] = (BYTE)GetValue( hash, i, usebits );

		// Convert to ASCII characters
		if ( str[ b ] < 10 ) str[ b ] += '0';
		else if ( str[ b ] < ( 10 + 26 ) ) str[ b ] += 'A' - 10;
		else if ( str[ b ] < ( 10 + 26 + 26 ) ) str[ b ] += 'a' - 10 - 26;
		else if ( str[ b ] < ( 10 + 26 + 26 + 1 ) ) str[ b ] = '-';
		else if ( str[ b ] < ( 10 + 26 + 26 + 1 + 1 ) ) str[ b ] = '+';
		else return FALSE;

		// Update variables
		b++; i += usebits;

	} // end while

	// NULL terminate
	str[ b ] = 0;

	return TRUE;
}

void CMD5::SetBit( LPBYTE buf, DWORD bit, DWORD set )
{_STT();
	if ( set ) buf[ ( bit & 0xfffffff8 ) >> 3 ] |= ( 0x01 << ( bit & 0x07 ) );
	else buf[ ( bit & 0xfffffff8 ) >> 3 ] &= ~( 0x01 << ( bit & 0x07 ) );
}

// Obviously, can only set max of 32 bits
DWORD CMD5::SetValue( LPBYTE buf, DWORD bitoff, DWORD bits, DWORD value )
{_STT();
	DWORD mask = 1;

	// For each bit
	for ( DWORD i = 0; i < bits; i++ )
	{
		// Set bit state
		SetBit( buf, ( bitoff + i ), ( value & mask ) );
		
		// Update mask
		mask <<= 1;

	} // end for

	return value;
}

BOOL CMD5::StringToHash(LPBYTE hash, DWORD hashbits, LPCTSTR str)
{_STT();
	DWORD i = 0;
	DWORD b = 0;

	// Sanity check
	if ( hash == NULL || hashbits == 0 || str == NULL )
		return FALSE;

	while ( i < hashbits )
	{
		BYTE ch;

		// Convert to number
		if ( str[ b ] >= '0' && str[ b ] <= '9' ) ch = str[ b ] - '0';
		else if ( str[ b ] >= 'A' && str[ b ] <= 'Z' ) ch = str[ b ] - 'A' + 10;
		else if ( str[ b ] >= 'a' && str[ b ] <= 'z' ) ch = str[ b ] - 'a' + 10 + 26;
		else if ( str[ b ] == '-' ) ch = 62;
		else if ( str[ b ] == '+' ) ch = 63;
		else return FALSE;

		// Set value
		DWORD	usebits = ( hashbits - i ) >= 6 ? 6 : hashbits - i;
		SetValue( hash, i, usebits, ch );

		// Update variables
		b++; i += usebits;

	} // end while

	return TRUE;
}


void CMD5::Random(LPVOID buf, DWORD size)
{_STT();
	// *** Get data for random seed

	// *** This stuff should be hard to reproduce

	// The bits are my estimation on how well one could guess
	// Hopefully they are pesimistic

	HWND hWnd = GetActiveWindow();				// 8-bits
	DWORD procid = GetCurrentProcessId();		// 8-bits
	DWORD thread = GetCurrentThreadId();		// 8-bits

	DWORD memory = (DWORD)this;					// 16-bits
	char *ptr = new char[ 16 ];					// 16-bits
	if ( ptr != NULL ) delete [] ptr;
	memory ^= (DWORD)ptr;
												// +- 1 hour
	DWORD tickcount = GetTickCount();			// 21-bits

	SYSTEMTIME	st;								// +- 1-day
	GetLocalTime( &st );						// 26-bits

	POINT pt;
	GetCursorPos( &pt );						// 16-bits 

												// Total: 119-bits

	// *** This stuff can be easily reproduced

	char computer[ MAX_COMPUTERNAME_LENGTH + 1 ] = { 0 };
	DWORD csize = sizeof( computer );
	if ( !GetComputerName( computer, &csize ) )
		*computer = 0;

	char name[ 512 ] = { 0 };
	DWORD nsize = sizeof( name );
	if ( !GetUserName( name, &nsize ) )
		*name = 0;

	SYSTEM_INFO si;
	GetSystemInfo( &si );

	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof( os );
	GetVersionEx( &os );

	// *** end collect random data

	// Did the user give us anything?
	if( buf != NULL )
	{	if ( size == 0 ) size = strlen( (LPCTSTR)buf );
		Transform( buf, size );
	} // end if

/*	RULIB_TRY
	{
		// First let's ask microsoft for the required data
		HCRYPTPROV hCryptProv = NULL;
		if ( CryptAcquireContext( &hCryptProv, NULL, NULL, PROV_RSA_SIG, 0 ) )
		{
			// Hopefully will init with random value
			CryptGenRandom( hCryptProv, sizeof( m_hash ), m_hash );

			// Release the encryption context
			CryptReleaseContext( hCryptProv );

		} // end if
	} RULIB_CATCH_ALL { ASSERT( 0 ); }
*/
	// Hash random data
	Transform( &hWnd, sizeof( hWnd ) );
	Transform( &procid, sizeof( procid ) );
	Transform( &thread, sizeof( thread ) );
	Transform( &memory, sizeof( memory ) );
	Transform( &tickcount, sizeof( tickcount ) );
	Transform( &st, sizeof( st ) );
	Transform( &pt, sizeof( pt ) );

	Transform( computer, strlen( computer ) );
	Transform( name, strlen( name ) );
	Transform( &si, sizeof( si ) );
	Transform( &os, sizeof( os ) );
}

