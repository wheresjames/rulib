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
// Bmp.h: interface for the CBmp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMP_H__12AC4722_7CED_11D3_BF29_00104B2C9CFA__INCLUDED_)
#define AFX_BMP_H__12AC4722_7CED_11D3_BF29_00104B2C9CFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef CBMP_ERR_BASE
#define CBMP_ERR_BASE				2000
#endif

#define CBMP_ERR_NONE				0
#define CBMP_ERRSTR_NONE			"No Error"
#define CBMP_ERR_FILEERROR			( CBMP_ERR_BASE + 0 )
#define CBMP_ERRSTR_FILEERROR		"File Error"
#define CBMP_ERR_FILESIZE			( CBMP_ERR_BASE + 1 )
#define CBMP_ERRSTR_FILESIZE		"File is too small to be a DIB"
#define CBMP_ERR_FILEREADERROR		( CBMP_ERR_BASE + 2 )
#define CBMP_ERRSTR_FILEREADERROR	"Error reading data from file"
#define CBMP_ERR_FILETYPE			( CBMP_ERR_BASE + 3 )
#define CBMP_ERRSTR_FILETYPE		"File is not a DIB"
#define CBMP_ERR_MEMORY				( CBMP_ERR_BASE + 4 )
#define CBMP_ERRSTR_MEMORY			"Insufficient memory"
#define CBMP_ERR_INVALIDHDC			( CBMP_ERR_BASE + 5 )
#define CBMP_ERRSTR_INVALIDHDC		"Invalid HDC"
#define CBMP_ERR_NOBMP				( CBMP_ERR_BASE + 6 )
#define CBMP_ERRSTR_NOBMP			"A DIB has not been loaded"
#define CBMP_ERR_FILEWRITEERROR		( CBMP_ERR_BASE + 7 )
#define CBMP_ERRSTR_FILEWRITEERROR	"Error writing data to file"
#define CBMP_ERR_BMPLT24BIT			( CBMP_ERR_BASE + 8 )
#define CBMP_ERRSTR_BMPLT24BIT		"Image must be at least 24 bits"
#define CBMP_ERR_GDI				( CBMP_ERR_BASE + 9 )
#define CBMP_ERRSTR_GDI				"Windows GDI error"
//#define CBMP_ERR_
//#define CBMP_ERRSTR_


#define CBMP_DIB_HEADER_MARKER		0x4D42
#define CBMP_PALVERSION				0x300
#define CBMP_IS_WIN30_DIB(pBMI)		((*(LPDWORD)(pBMI)) == sizeof(BITMAPINFOHEADER))
#define CBMP_DIBWIDTH(pBMI)			( CBMP_IS_WIN30_DIB(pBMI)								\
									? (DWORD)((LPBITMAPINFOHEADER)pBMI)->biWidth		\
									: (DWORD)((LPBITMAPCOREHEADER)pBMI)->bcWidth )
#define CBMP_DIBHEIGHT(pBMI)		( CBMP_IS_WIN30_DIB(pBMI)								\
									? (DWORD)((LPBITMAPINFOHEADER)pBMI)->biHeight		\
									: (DWORD)((LPBITMAPCOREHEADER)pBMI)->bcHeight )
#define CBMP_PALETTESIZE(pBMI)		( CBMP_IS_WIN30_DIB(pBMI)												\
									? (DWORD)(CBmp::GetColorCount(pBMI) * sizeof(RGBQUAD))		\
									: (DWORD)(CBmp::GetColorCount(pBMI) * sizeof(RGBTRIPLE)) )
#define CBMP_SCANWIDTH(bits)		( ( (bits) + 31 ) / 32 * 4 )
#define CBMP_ISCOMPRESSED(pBMI)		( ( pBMI->biCompression == BI_RLE8 ) || \
							        ( pBMI->biCompression == BI_RLE4 ) )

#define CBMP_SETPIXEL( buf, size, scanwidth, x, y, rgb )	\
{															\
	DWORD i = ( y * scanwidth ) + ( x * 3 );				\
	if ( i < ( size - 3 ) )									\
	{	buf[ i ] = GetBValue( rgb ); 						\
		buf[ i + 1 ] = GetGValue( rgb ); 					\
		buf[ i + 2 ] = GetRValue( rgb );					\
	}														\
}

#define CBMP_GETPIXEL( buf, size, scanwidth, x, y, rgb )	\
{															\
	DWORD i = ( y * scanwidth ) + ( x * 3 );				\
	if ( i < ( size - 3 ) )									\
		rgb = RGB( buf[ i + 2 ], buf[ i + 1 ], buf[ i ] );	\
}

//==================================================================
// CBmp
//
/// Wraps a bitmap object
/**
	This class is obsolete, use CWinImg and CWinDc instead.	
*/
//==================================================================
class CBmp  
{
public:
	HRGN CreateRgnFromBitmap( COLORREF rgbTransparent, HRGN hRgn = NULL, HBITMAP hBmp = NULL, LPRECT pRect = NULL, DWORD xOffset = 0, DWORD yOffset = 0 );
	BOOL GetBmpBits( LPBYTE *buf, LPDWORD width, LPDWORD height, LPDWORD size, LPDWORD scanwidth );
	BOOL Save( LPCTSTR pFile );
	BOOL GetDibRect( LPRECT pRect );
	BOOL Draw( HDC hDC, LPRECT pRect = NULL, BOOL bStretch = FALSE );
	static DWORD GetColorCount( LPBYTE buf );
	BOOL CreatePalette();
	BOOL Load( LPCTSTR pFile );
	void Destroy();
	CBmp();
	virtual ~CBmp();

	BOOL IsDib() { return ( m_pDib != NULL ); }
	BOOL IsPalette() { return ( m_hPal != NULL ); }

	LPBYTE GetBits() 
	{	if ( m_pDib == NULL ) return NULL;
		return ( m_pDib + *(LPDWORD)m_pDib + CBMP_PALETTESIZE( m_pDib ) );
	}

	DWORD GetBitCount()
	{	if ( m_pDib == NULL ) return 0;
		return ( (LPBITMAPINFOHEADER)m_pDib )->biBitCount; }
	DWORD GetScanWidth() 
	{	if ( m_pDib == NULL ) return FALSE;
		DWORD width = GetWidth() * ( GetBitCount() / 8 ); while ( ( width ) & 0x03 ) width++;
		return width; }
	DWORD GetWidth() 
	{	if ( m_pDib == NULL ) return FALSE;
		return ( CBMP_DIBWIDTH( m_pDib ) ); }
	DWORD GetHeight() 
	{	if ( m_pDib == NULL ) return FALSE;
		return ( CBMP_DIBHEIGHT( m_pDib ) ); }

	LPBITMAPINFOHEADER GetBitmapInfoHeader() { return (LPBITMAPINFOHEADER)m_pDib; }
	LPBITMAPINFO GetBitmapInfo() { return (LPBITMAPINFO)m_pDib; }

	DWORD Size() { return m_dwSize; }
	LPBYTE Data() { return m_pDib; }

	BOOL IsRgn() { return ( m_hRgn != NULL ); }
	HRGN GetSafeHrgn() { return m_hRgn; }

private:

	BITMAPFILEHEADER	m_bfh;
	LPBYTE				m_pDib;
	DWORD				m_dwSize;

	HPALETTE			m_hPal;

public:
	BOOL LoadFromHDC( HDC hDC );
	BOOL Create( LPBITMAPINFO pbi );
	BOOL Line( long x1, long y1, long x2, long y2, COLORREF rgb );
	BOOL LoadFromFastDraw();
	BOOL TlFastDraw( HDC hDC, LPRECT pRect, DWORD dwTL, BOOL bStretch );
	BOOL CreateFastDraw( HDC hDC, DWORD x, DWORD y );
	void ReleaseFastDraw();
	BOOL FastDraw( HDC hDC, LPRECT pRect, BOOL bStretch = TRUE );
	void ReleaseMask();
	BOOL CreateMask( HDC hDC = NULL, HBITMAP hBmp = NULL );
	BOOL Copy( CBmp *pBmp, DWORD x, DWORD y );
	BOOL CreateRgn(  LPRECT pRect, COLORREF rgbTransparent = RGB( 0, 0, 0 ) );
	BOOL CutOut( HWND hWnd, LPRECT pRect, BOOL bAddRgn = FALSE );
	void DestroyRgn();
	BOOL LoadFromBuffer( LPBYTE buf, DWORD size );
	BOOL FxGrayscale( BYTE r = 0, BYTE g = 0, BYTE b = 0 );
	BOOL FxDither( DWORD strength );
	BOOL FxBlur( DWORD strength );
	BOOL FxSmooth( DWORD strength, DWORD threshold );
	BOOL GetBmpMetrics( LPBYTE *buf, LPDWORD width, LPDWORD height, LPDWORD size, LPDWORD scanwidth );
	BOOL CreateBitmap( DWORD width, DWORD height, DWORD bitsperpixel );
	COLORREF GetPixel( DWORD x, DWORD y );
	void SetPixel( DWORD x, DWORD y, COLORREF clr );
	BOOL Load( HBITMAP hBmp );
	HBITMAP CreateHBITMAP( HDC hDC = NULL );
	static LPCTSTR GetErrorString( DWORD err );

	LPCTSTR GetErrorString() { return GetErrorString( m_dwErr ); }
	DWORD SetError( DWORD err ) { return ( m_dwErr = err ); }
	DWORD GetError() { return m_dwErr; }

	void SetTransparency( BOOL b ) 
	{	m_bTransparent = b; /*if ( b ) CreateMask(); else ReleaseMask();*/ }
	BOOL GetTransparency() { return m_bTransparent; }
	void SetTransparentColor( COLORREF col ) { m_rgbTransparent = col; }
	COLORREF GetTransparentColor() { return m_rgbTransparent; }

	BOOL IsMask() { return ( m_hMaskDC != NULL ); }

	BOOL IsFastDraw() { return ( m_hFastDC != NULL ); }
	HDC GetFastDC() { return m_hFastDC; }
	HBITMAP GetFastBmp() { return m_hFastBmp; }
	LPRECT GetFastRect(){ return &m_rectFast; }

	BOOL LineTo( long x, long y, COLORREF rgb );
	void MoveTo( long x, long y )
	{	m_drawx = x; m_drawy = y; }
	long GetDrawX() { return m_drawx; }
	long GetDrawY() { return m_drawy; }

	void SetChromaErr( LONG pos, LONG neg )
	{	m_lCPos = pos; m_lCNeg = neg; }
	LONG GetPosChromaErr() { return m_lCPos; }
	LONG GetNegChromaErr() { return m_lCNeg; }

private:
	DWORD	m_dwErr;
	char	m_szErr[ 256 ];

	// Bitmap cutout region
	HRGN			m_hRgn;
	RECT			m_rectRgn;

	// Transparency
	BOOL			m_bChroma;
	BOOL			m_bTransparent;
	COLORREF		m_rgbTransparent;
	LONG			m_lCPos;
	LONG			m_lCNeg;
	HDC				m_hMaskDC;
	HBITMAP			m_hMaskBmp;
	HBITMAP			m_hMaskOld;

	// Fast draw
	HDC				m_hFastDC;
	HBITMAP			m_hFastBmp;
	HBITMAP			m_hFastOld;
	RECT			m_rectFast;

	long			m_drawx;
	long			m_drawy;
};

#endif // !defined(AFX_BMP_H__12AC4722_7CED_11D3_BF29_00104B2C9CFA__INCLUDED_)
