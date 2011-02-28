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

// {00000000-0000-0000-0000-000000000000}
static const GUID CLSID_ZERO = 
{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };


#ifdef _DEBUG
#define _vTRY()			if ( 1 )
#define _vCATCH( p )	else if ( 0 )
#else
#define	_vTRY()			try
#define _vCATCH( p )	catch( p )
#endif

// Usefull macros
#define MIN_VAL( a, b )			( a < b ? a : b )
#define MAX_VAL( a, b )			( a > b ? a : b )
#define MIN_VAL_3D( a, b, c )	( a < b ? ( a < c ? a : c ) : ( b < c ? b : c ) )
#define MAX_VAL_3D( a, b, c )	( a > b ? ( a > c ? a : c ) : ( b > c ? b : c ) )
#define DIF_VAL( a, b )			( a > b ? a - b : b - a )
#define SET_IF( a, b, c )		( a = ( a == b ) ? c : a )
#define SET_IF_NOT( a, b, c )	( a = ( a != b ) ? c : a )

// Memory macros ( watch out for side effects! )
#define _PTR_NEW				new
#define _PTR_DELETE( p )		( p ? ( delete p, p = NULL ) : NULL )
#define _PTR_DELETE_ARR( p )	( p ? ( delete[] p, p = NULL ) : NULL )
#define _PTR_RELEASE( p )		( p ? ( p->Release(), p = NULL ) : NULL )

// Rectangle macros
#define RW( rect )		( rect.right - rect.left )
#define RH( rect )		( rect.bottom - rect.top )
#define PRW( rect )		( rect->right - rect->left )
#define PRH( rect )		( rect->bottom - rect->top )
#define RPT( rect )		rect.left, rect.top, rect.right, rect.bottom
#define RWH( rect )		rect.left, rect.top, RW( rect ), RH( rect )

#define SCANWIDTH( w, bpp )	( ( ( w * bpp ) + 3 ) & ( ~3 ) ) 

#define RANGE( val, min, max ) ( ( val < min ) ? min : ( val > max ) ? max : val )
#define SET_RANGE( val, min, max ) ( val = RANGE( val, min, max ) )

#define SCALE( v, cur, des ) ( ( v * des ) / cur )
#define MSCALE( v, min1, max1, min2, max2 ) ( min2 + ( ( ( v - min1 ) * max2 ) / max1 ) )

#define FAST_SMOOTH( v1, v2, weight ) ( ( ( v1 << weight ) - v1 + v2 ) >> weight )
#define SMOOTH( v1, v2, weight ) ( ( v1 * weight + v2 ) / ( weight + 1 ) )

#define ABSVAL( v ) ( ( v >= 0 ) ? v : -v )

#define AVERAGE_ANGLE( a, b, w ) ( a + ( ( b - a ) / w ) )

#define SWAP_INT( a, b ) ( a ^= b, b ^= a, a ^= b )
#define R_SWAP( a, b ) 

//#define strcpy_max( str1, str2, max )	\
//	{ try{ ZeroMemory( str1, max ), strncpy( str1, str2, max - 1 ), str1[ max - 1 ] = NULL; } catch(...){} }
static void strcpy_max( LPSTR str1, LPCTSTR str2, DWORD max )
{ { RULIB_TRY{ strncpy( str1, str2, max - 1 ), str1[ max - 1 ] = NULL; } RULIB_CATCH_ALL{} } }
#define strcpy_sz( str1, str2 )	strcpy_max( str1, str2, sizeof( str1 ) )

// Sets a bit in DWORD
#define BSETDW( b, dwrd )	( dwrd |= 0x00000001 << b )

// Clears a bit in DWORD
#define BCLRDW( b, dwrd ) ( dwrd &= ~( 0x00000001 << b ) )

// Toggles a bit in DWORD
#define BTOGDW( b, dwrd ) ( dwrd ^= ( 0x00000001 << b ) )

// Gets the value of a particular bit
#define BGETDW( b, dwrd ) ( ( ( 0x00000001 << b ) & dwrd ) != 0 )
   
#define DEG_TO_RADIANS		( (double)0.00000001745329251994 )
#define EARTH_RADIUS		( (double)63781.5 )
#define GREAT_CIRCLE_DISTANCE( x1, y1, x2, y2 )																											\
	( EARTH_RADIUS * acos(	cos( y1 * DEG_TO_RADIANS ) * cos( x1 * DEG_TO_RADIANS ) * cos( y2 * DEG_TO_RADIANS ) * cos( x2 * DEG_TO_RADIANS ) +		\
							sin( y1 * DEG_TO_RADIANS ) * sin( x1 * DEG_TO_RADIANS ) * sin( y2 * DEG_TO_RADIANS ) * sin( x2 * DEG_TO_RADIANS ) +		\
							sin( x1 * DEG_TO_RADIANS ) * sin( x2 * DEG_TO_RADIANS ) ) )

// Mask
#define BSET( w, m, s ) ( ( s != 0 ) ? w |= m : w &= ~m; )
#define BSETON( w, m ) ( w |= m )
#define BSETOFF( w, m ) ( w &= ~m )
#define BGET( w, m ) ( ( w & m ) != 0 )
#define BTOG( w, m ) ( w ^= m )
#define MATCHBIT( w1, w2, m ) ( w1 &= ~m, w1 |= ( w2 & m ) )

// Octal
#define OCT(n) 0##n
// Binary Numbers
// Use _BIN4(1,0,1,0)
#define _BIN4(n1,n2,n3,n4) ((n1<<3)+(n2<<2)+(n3<<1)+n4)
// Use _BIN8(1,0,1,0,1,0,1,0)
#define _BIN8(n1,n2,n3,n4,n5,n6,n7,n8) ((_BIN4(n1,n2,n3,n4)<<4)+_BIN4(n5,n6,n7,n8))
// Use _BIN16(1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0)
#define _BIN16(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16) \
					( (_BIN8(n1,n2,n3,n4,n5,n6,n7,n8)<<8) + _BIN8(n9,n10,n11,n12,n13,n14,n15,n16) )

// Use BIN4(1010)
#define BIN4(n) _BIN4(((OCT(n)&01000)==01000),((OCT(n)&0100)==0100),\
							((OCT(n)&010)==010),((OCT(n)&01)==01))
// Use BIN8(1010,0101)
#define BIN8(n1,n2) ((BIN4(n1)<<4)+BIN4(n2))
// Use BIN16(1010,0101,1010,0101)
#define BIN16(n1,n2,n3,n4) ((BIN8(n1,n2)<<8)+BIN8(n3,n4))

// Used to track a timer value
// tm = timer variable
// to = timeout in units
// count = current timer value in units
#define CHECKTIMEOUT( tm, to, count )					\
			( ( tm < count || tm > ( count + to ) ) ?	\
				( ( tm = ( count + to ) ) ? 1 : 1 ) : 0 )

#define IS_VALID_TIMER( tm, to, count ) ( tm >= count && tm <= ( count + to ) )
