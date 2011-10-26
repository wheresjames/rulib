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
// Text.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include <Math.h>

#ifdef DEBUG_NEW
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CText::CText()
{
	m_hFont = NULL;

	// Setup for default fonts
	Default();
}

CText::~CText()
{
	Destroy();
}

BOOL CText::SetFont(DWORD dwSize, LPCTSTR pFont )
{
	HFONT	hFont;

	// Zero size or NULL pFont means use the default
	if ( dwSize == 0 || pFont == NULL )
	{
		Destroy();
		return TRUE;
	} // end if

	// Attempt to create a new font
	hFont = CreateFont(	dwSize, 0, 
						m_nEscapement, m_nEscapement,
						m_nWeight, 
						m_bItalic, m_bUnderline, m_bStrikeOut,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH, pFont );
	// Did we get the font
	if ( hFont == NULL ) return FALSE;

	// Lose the old font
	Destroy();

	// Save font information
	m_logfont.lfHeight = dwSize; 
	m_logfont.lfWidth = 0;
	m_logfont.lfEscapement = m_nEscapement;
	m_logfont.lfOrientation = 0;
	m_logfont.lfWeight = m_nWeight;
	m_logfont.lfItalic = m_bItalic;
	m_logfont.lfUnderline = m_bUnderline;
	m_logfont.lfStrikeOut = m_bStrikeOut;
	m_logfont.lfCharSet = ANSI_CHARSET;
	m_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logfont.lfQuality = DEFAULT_QUALITY;
	m_logfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy( m_logfont.lfFaceName, pFont );

	// Save our new font
	m_hFont = hFont;

	return TRUE;

}

void CText::Destroy()
{
	if ( m_hFont != NULL ) 
	{
		DeleteObject( m_hFont );
		m_hFont = NULL;
	} // end if
	ZeroMemory( &m_logfont, sizeof( m_logfont ) );
}

BOOL CText::DrawText( HDC hDC, LPCTSTR pText, LPRECT pRect )
{
	RECT		rect;
	HFONT		oldFont = NULL;
	COLORREF	oldTextColor;

	// Sanity check
	if ( hDC == NULL || pText == NULL || pRect == NULL )
		return FALSE;

	if ( m_rgbColor != MAXDWORD )
	{
		oldTextColor = ::GetTextColor( hDC );
		::SetTextColor( hDC, m_rgbColor );
	} // end if

	// Have we been given a font to use?
	if ( m_hFont != NULL ) oldFont = (HFONT)::SelectObject( hDC, m_hFont );

	// Transparent text
	int mode = ::GetBkMode( hDC );
	::SetBkMode( hDC, TRANSPARENT );
	
	// Adjust for margins
	CopyRect( &rect, pRect );
	InflateRect( &rect, -m_xMargin, -m_yMargin );

	DWORD flags = ( m_dwFlags & EDT_NOEXTRA );
	if ( ( m_dwFlags & EDT_MULTICENTER ) != 0 )
	{
		flags &= ~( DT_TOP | DT_VCENTER | DT_BOTTOM );
		CalcRect( hDC, pText, &rect );

		long tw = rect.right - rect.left + 2;
		long th = rect.bottom - rect.top + 2;
		long rw = pRect->right - pRect->left;
		long rh = pRect->bottom - pRect->top;

		long xoff = 0, yoff = 0;
		if (	( m_dwFlags & EDT_ESCAPEMENTCENTER ) != 0 &&
				m_logfont.lfEscapement > 0 )
		{	
			long deg = m_logfont.lfEscapement % 3600;
			float rad = float( deg ) / ( float( 57.29582791 ) * float( 10 ) );

			xoff = -long( float( ( tw >> 1 ) ) * cos( rad ) ) + ( tw >> 1 );
			xoff -= long( float( ( th >> 1 ) ) * sin( rad ) );
			yoff = -long( float( ( th >> 1 ) ) * cos( rad ) ) + ( th >> 1 );
			yoff += long( float( ( tw >> 1 ) ) * sin( rad ) );

		} // end if

		rect.left = pRect->left + ( ( rw - tw ) >> 1 ) + xoff;
		rect.right = rect.left + tw;
		rect.top = pRect->top + ( ( rh - th ) >> 1 ) + yoff;
		rect.bottom = rect.top + th;

	} // end if

	// Draw the text
	BOOL ret = ( ::DrawTextEx( hDC, (char*)pText, strlen( pText ), &rect, flags, NULL ) != 0 );

	// Restore bk mode
	::SetBkMode( hDC, mode );

	// Restore the text color
	if ( m_rgbColor != MAXDWORD ) ::SetTextColor( hDC, oldTextColor );

	// Restore the old font
	if ( m_hFont != NULL ) ::SelectObject( hDC, oldFont );

	return ret;
}

void CText::Default()
{
	// Default font
	Destroy();

	// Default margins
	m_xMargin = 1;
	m_yMargin = 1;

	// Default font stuff
	m_nWeight = FW_NORMAL;
	m_nEscapement = 0;

	m_bItalic = FALSE;
	m_bUnderline = FALSE;
	m_bStrikeOut = FALSE;

	SetFlags();
	SetColor();

	m_txtheight = 0;
	m_txtwidth = 0;
	SetRect( &m_txtbox, 0, 0, 0, 0 );
	m_monotab = 4;

	ZeroMemory( &m_monowidth, sizeof( m_monowidth ) );
}

BOOL CText::SetFont(LPLOGFONT pFont)
{
	HFONT hFont;

	// Zero size or NULL pFont means use the default
	if ( pFont == NULL || *pFont->lfFaceName == NULL )
	{
		Destroy();
		return TRUE;
	} // end if

	// Attempt to create a new font
	hFont = CreateFontIndirect( pFont );

	// Did we get the font
	if ( hFont == NULL ) return FALSE;

	// Lose the old font
	Destroy();

	// Save font information
	memcpy( &m_logfont, pFont, sizeof( LOGFONT ) );

	// Save our new font
	m_hFont = hFont;

	return TRUE;
}

BOOL CText::ChooseFont(LPLOGFONT pFont, HWND hWnd, BOOL bInit, COLORREF *pRGB )
{
	if ( pFont == NULL ) return FALSE;

	CHOOSEFONT	cf;

	cf.lStructSize = sizeof( cf );
	cf.hwndOwner = hWnd;
	cf.hDC = NULL;
	cf.lpLogFont = pFont;
	cf.iPointSize = 0;	
	cf.Flags = CF_EFFECTS | CF_FORCEFONTEXIST | CF_SCREENFONTS;
	if ( bInit ) cf.Flags |= CF_INITTOLOGFONTSTRUCT;
	if ( pRGB != NULL ) cf.rgbColors = *pRGB;
	else cf.rgbColors = RGB( 0, 0, 0 );
	cf.lCustData = NULL;
	cf.lpfnHook = NULL;
	cf.lpTemplateName = NULL;
	cf.hInstance = NULL;
	cf.lpszStyle = NULL;
	cf.nFontType = 0;
	cf.nSizeMin = 0;
	cf.nSizeMax = 0;

	BOOL ret = ::ChooseFont( &cf );
	if ( !ret ) return FALSE;

	if ( pRGB != NULL ) *pRGB = cf.rgbColors;

	return TRUE;
}

BOOL CText::GetFont(LPLOGFONT pFont)
{
	if ( pFont == NULL || m_hFont == NULL ) return FALSE;

	memcpy( pFont, &m_logfont, sizeof( LOGFONT ) );

	return TRUE;
}

BOOL CText::CalcRect(HDC hDC, LPCTSTR pText, LPRECT pRect)
{
	HFONT		oldFont = NULL;

	// Sanity check
	if ( hDC == NULL || pText == NULL || pRect == NULL )
		return FALSE;

	// Have we been given a font to use?
	if ( m_hFont != NULL ) oldFont = (HFONT)::SelectObject( hDC, m_hFont );

	// Adjust for margins
	InflateRect( pRect, -m_xMargin, -m_yMargin );
	
	// Calculate the bounding rectangle
	DWORD flags = ( m_dwFlags & EDT_NOEXTRA ) | DT_CALCRECT;
	if ( ( m_dwFlags & EDT_MULTICENTER ) != 0 )
		flags &= ~( DT_TOP | DT_VCENTER | DT_BOTTOM );

	BOOL ret = ( ::DrawTextEx( hDC, (char*)pText, strlen( pText ), pRect, flags, NULL ) != 0 );

	// Restore the old font
	if ( m_hFont != NULL ) ::SelectObject( hDC, oldFont );

	return ret;
}

long CText::CalcTextHeight( HDC hDC, long *pHeight )
{
	DWORD	h = 0;
	RECT	rect;

	// Zero width array if needed
	if ( pHeight != NULL ) ZeroMemory( pHeight, sizeof( long ) * 256 );

	if ( pHeight == NULL )
	{
		SetRect( &rect, 0, 0, 10000, 10 );

		// Get the height
		if ( !CalcRect( hDC, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*(),./;'[]", &rect ) ) return 0;

		// Save height
		h = RH( rect );

	} // end if

	else
	{
		DWORD t;
		char  c[ 2 ] = { 0, 0 };
		for ( c[ 0 ] = ' '; c[ 0 ] <= '~'; c[ 0 ]++ )
		{	
			// Get character width
			SetRect( &rect, 0, 0, 10000, 10 );
			if ( CalcRect( hDC, c, &rect ) )
			{
				t = rect.right - rect.left;

				// Save in array if any
				if ( pHeight != NULL ) pHeight[ c[ 0 ] ] = t;

				// Track largest width
				if ( t > h ) h = t;

			} // end if

		} // end for

	} // end else

	// Return the new height
	return h;
}

long CText::CalcTextWidth(HDC hDC, long *pWidth)
{
	RECT	rect;

	// Zero width array if needed
	if ( pWidth != NULL ) ZeroMemory( pWidth, sizeof( long ) * 256 );

	// Get the width of the largest character
	DWORD w = 0, t;
	char  c[ 2 ] = { 0, 0 };
	for ( c[ 0 ] = ' '; c[ 0 ] <= '~'; c[ 0 ]++ )
	{	
		// Get character width
		SetRect( &rect, 0, 0, 10000, 10 );
		if ( CalcRect( hDC, c, &rect ) )
		{
			t = rect.right - rect.left;

			// Save in array if any
			if ( pWidth != NULL ) pWidth[ c[ 0 ] ] = t;

			// Track largest width
			if ( t > w ) w = t;
		} // end if

	} // end for

	// Return the largest width
	return w;
}

BOOL CText::DrawMonoSpacedText(HDC hDC, LPCTSTR pText, LPRECT pRect)
{
	RECT		txt;
	char		t[ 2 ] = { 0, 0 };
	HFONT		oldFont = NULL;
	COLORREF	oldTextColor;

	// Do we need to setup?
	if ( m_txtwidth == 0 || m_txtheight == 0 )
	{	SetMonoSpacedText( hDC );
		if ( m_txtwidth == 0 || m_txtheight == 0 ) return FALSE;
	} // end if

	// Have we been given a font to use?
	if ( m_hFont != NULL ) oldFont = (HFONT)::SelectObject( hDC, m_hFont );

	if ( m_rgbColor != MAXDWORD )
	{
		oldTextColor = ::GetTextColor( hDC );
		::SetTextColor( hDC, m_rgbColor );
	} // end if

	// Transparent text
	int mode = ::GetBkMode( hDC );
	::SetBkMode( hDC, TRANSPARENT );

	// Draw the text mono-spaced
	DWORD i = 0;
	DWORD size = strlen( pText );
	char last = 0;
	long x = pRect->left;
	long y = pRect->top;

	while ( i < size )
	{
		// No need to keep going if past the bottom of rect
		if ( y >= pRect->bottom ) return TRUE;

		// Create one character string
		t[ 0 ] = pText[ i++ ];

		if ( t[ 0 ] >= ' ' )
		{
			// Where will the text go?
			txt.left = x; txt.right = x + m_txtwidth;
			txt.top = y; txt.bottom = y + m_txtheight;

			// Center character
			txt.left += ( m_txtwidth - m_monowidth[ t[ 0 ] ] ) >> 1;

			// Draw text if anyone could see it
			if ( txt.left < pRect->right && txt.top < pRect->bottom )
				DrawTextEx( hDC, t, 1, &txt, DT_NOCLIP, NULL );

			// Move over one character width
			x += m_txtwidth;

		} // end if

		// Carriage return
		else if ( t[ 0 ] == '\r' ) y += m_txtheight;

		// Line feed
		else if ( t[ 0 ] == '\n' ) 
		{
			// Auto detect CRLF or new line
			if ( last != '\r' ) y += m_txtheight;
			x = pRect->left;
		} // end else if

		// Tab character
		else if ( t[ 0 ] == '\t' ) x += ( m_txtwidth * m_monotab );

		// Track last character
		last = t[ 0 ];
		
	} // end for

	// Restore bk mode
	::SetBkMode( hDC, mode );

	// Restore the text color
	if ( m_rgbColor != MAXDWORD ) ::SetTextColor( hDC, oldTextColor );

	// Restore the old font
	if ( m_hFont != NULL ) ::SelectObject( hDC, oldFont );

	return TRUE;
}

BOOL CText::ChooseFont(HWND hWnd)
{
	if ( !ChooseFont( &m_logfont, hWnd, TRUE, &m_rgbColor ) )
		return FALSE;

	SetFont( &m_logfont );
	SetColor( m_rgbColor );

	return TRUE;
}


DWORD CText::HtmlText(LPSTR out, DWORD odw, LPCTSTR buf, DWORD size, BOOL bHyperLinks)
{
	if ( out == NULL || odw == NULL || buf == NULL || size == 0 ) 
		return FALSE;

	DWORD	opos = 0;
	DWORD	i = 0;
	DWORD 	ptr = 0;
	char	buffer[ 4096 ];
	DWORD	max = sizeof( buffer ) >> 2;
	DWORD	maxcopy = max + ( max >> 1 );

	while ( buf[ i ] != 0x0 && i < size )
	{
		// Check for hyper links
		if (	bHyperLinks &&
				(
					!strnicmp( &buf[ i ], "https://", 8 ) ||
					!strnicmp( &buf[ i ], "http://", 7 ) ||
					!strnicmp( &buf[ i ], "ftp://", 6 ) ||
					!strnicmp( &buf[ i ], "mailto:", 7 ) ||
					!strnicmp( &buf[ i ], "news:", 7 ) ||
					!strnicmp( &buf[ i ], "www.", 4 ) ||
					!strnicmp( &buf[ i ], "ftp.", 4 ) )
				)
		{
			// Start the link
			strcpy( &buffer[ ptr ], "<a target=\"_Launch\" href=\"" ); ptr += 26;

			// Must copy the protocol if not specified
			if ( !strnicmp( &buf[ i ], "mailto:", 7 ) )
			{/*	strcpy( &buffer[ ptr ], "http://" ); ptr += 7; */}
			else if ( !strnicmp( &buf[ i ], "news:", 5 ) )
			{	strcpy( &buffer[ ptr ], "http://" ); ptr += 7; }
			else if ( !strnicmp( &buf[ i ], "www.", 4 ) ) 
			{	strcpy( &buffer[ ptr ], "http://" ); ptr += 7; }
			else if ( !strnicmp( &buf[ i ], "ftp.", 4 ) )
			{	strcpy( &buffer[ ptr ], "ftp://" ); ptr += 6; }

			// Find start and end to link
			DWORD start = i, end = i;
			while( end < size && buf[ end ] > ' ' && buf[ end ] < '~' ) end++;

			// Back up over punctuation
			while( end > start && 
					( buf[ end ] < '0' || buf[ end ] > '9' ) &&
					( buf[ end ] < 'a' || buf[ end ] > 'z' ) &&
					( buf[ end ] < 'A' || buf[ end ] > 'Z' ) &&
					buf[ end ] != '_' && buf[ end ] != '/' ) end--;

			// Copy the link
			while( ptr < maxcopy && i <= end ) buffer[ ptr++ ] = buf[ i++ ];

			// close tag
			strcpy( &buffer[ ptr ], "\">" ); ptr += 2;

			// Go ahead and flush
			opos = WriteBuffer( out, opos, odw, buffer, ptr );
			ptr = 0;

			// Now copy the link as text
			if ( odw > opos )
				opos += HtmlText( &out[ opos ], odw - opos, &buf[ start ], ( end + 1 ) - start, FALSE );

			// end tag
			strcpy( &buffer[ ptr ], "</a>" ); ptr += 4;

		} // end if

		// Check for e-mail addresses
		if (	bHyperLinks &&
				(
					buf[ i ] > ' ' && buf[ i ] < '~' && 
					( 
						( buf[ i ] >= '0' && buf[ i ] <= '9' ) ||
						( buf[ i ] >= 'a' && buf[ i ] <= 'z' ) ||
						( buf[ i ] >= 'A' && buf[ i ] <= 'Z' ) ||
						buf[ i ] == '_'
					)
				)
			)
		{
			DWORD c = i;

			BOOL bEmail = FALSE;
			while( !bEmail && buf[ c ] > ' ' && buf[ c ] < '~' )
				if ( buf[ c++ ] == '@' ) bEmail = TRUE;

			if ( bEmail )
			{
				// Start the link
				strcpy( &buffer[ ptr ], "<a target=\"_Launch\" href=\"mailto:" ); ptr += 33;

				// Find start and end to link
				DWORD start = i, end = i;
				while( end < size && buf[ end ] > ' ' && buf[ end ] < '~' ) end++;

				// Back up over punctuation
				while( end > start && 
						( buf[ end ] < '0' || buf[ end ] > '9' ) &&
						( buf[ end ] < 'a' || buf[ end ] > 'z' ) &&
						( buf[ end ] < 'A' || buf[ end ] > 'Z' ) &&
						buf[ end ] != '_' && buf[ end ] != '/' ) end--;

				// Copy the link
				while( ptr < maxcopy && i <= end ) buffer[ ptr++ ] = buf[ i++ ];

				// close tag
				strcpy( &buffer[ ptr ], "\">" ); ptr += 2;

				// Go ahead and flush
				opos = WriteBuffer( out, opos, odw, buffer, ptr );
				ptr = 0;

				// Now copy the link as text
				if ( odw > opos )
					opos += HtmlText( &out[ opos ], odw - opos, &buf[ start ], ( end + 1 ) - start, FALSE );

				// end tag
				strcpy( &buffer[ ptr ], "</a>" ); ptr += 4;


			} // end if

		} // end if

		if ( buf[ i ] == '<' )
		{	strcpy( &buffer[ ptr ], "&lt;" ); ptr += 4; }

		else if ( buf[ i ] == '&' )
		{	strcpy( &buffer[ ptr ], "&amp;" ); ptr += 5; }

		else if ( buf[ i ] == '>' )
		{	strcpy( &buffer[ ptr ], "&gt;" ); ptr += 4; }

		else if ( buf[ i ] == '\"' )
		{	strcpy( &buffer[ ptr ], "&quot;" ); ptr += 6; }

		else if ( buf[ i ] == ' ' && i > 0 && buf[ i - 1 ] == ' ' )
		{	strcpy( &buffer[ ptr ], "&nbsp;" ); ptr += 6; }

		else if ( buf[ i ] >= ' ' && buf[ i ] <= '~' )
			buffer[ ptr++ ] = buf[ i ];

		else if ( buf[ i ] == '\n' )
		{	strcpy( &buffer[ ptr ], "<br>\r\n" ); ptr += 6; }

		// Flush to disk
		if ( ptr > max )
		{	opos = WriteBuffer( out, opos, odw, buffer, ptr ); ptr = 0; }

		i++;

	} // end while

	// Write out last bit of data if any
	if ( ptr !=0 ) opos = WriteBuffer( out, opos, odw, buffer, ptr );

	return opos;
}

DWORD CText::WriteBuffer(LPSTR out, DWORD op, DWORD om, LPCTSTR buf, DWORD size)
{	
	// Sanity check
	if ( out == NULL || buf == NULL || size == 0 || op >= om ) return 0;
	if ( size > ( om - op ) ) size = om - op;

	// Copy the data
	memcpy( &out[ op ], buf, size );

	// Return new pointer
	return ( op + size );	
}
