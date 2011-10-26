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
// Text.h: interface for the CText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__03EABC95_C029_11D2_88F9_00104B2C9CFA__INCLUDED_)
#define AFX_TEXT_H__03EABC95_C029_11D2_88F9_00104B2C9CFA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Text flags
/*
#define DT_TOP              0x00000000
#define DT_LEFT             0x00000000
#define DT_CENTER           0x00000001
#define DT_RIGHT            0x00000002
#define DT_VCENTER          0x00000004
#define DT_BOTTOM           0x00000008
#define DT_WORDBREAK        0x00000010
#define DT_SINGLELINE       0x00000020
#define DT_EXPANDTABS       0x00000040
#define DT_TABSTOP          0x00000080
#define DT_NOCLIP           0x00000100
#define DT_EXTERNALLEADING  0x00000200
#define DT_CALCRECT         0x00000400
#define DT_NOPREFIX         0x00000800
#define DT_INTERNAL         0x00001000

#if(WINVER >= 0x0400)
#define DT_EDITCONTROL      0x00002000
#define DT_PATH_ELLIPSIS    0x00004000
#define DT_END_ELLIPSIS     0x00008000
#define DT_MODIFYSTRING     0x00010000
#define DT_RTLREADING       0x00020000
#define DT_WORD_ELLIPSIS    0x00040000
#endif
*/
#define EDT_EXTRA				0xfff00000
#define EDT_NOEXTRA				0x000fffff
#define EDT_MULTICENTER			0x00100000
#define EDT_ESCAPEMENTCENTER	0x00200000

//==================================================================
// CText
//
/// Encapsulates Windows GDI text drawing functions
/**
	Use this class to draw text into DC objects	
*/
//==================================================================
class CText  
{
public:

	//==============================================================
	// WriteBuffer()
	//==============================================================
	/// Provides generic buffer writing
	/**
		\param [in] out		-	Buffer receiving data
		\param [in] op		-	Starting offset in out
		\param [in] om		-	Size of buffer in out
		\param [in] buf		-	Data to write
		\param [in] size	-	Number of bytes in buf
		
		\return op + number of bytes written to out
	
		\see 
	*/
	static DWORD WriteBuffer( LPSTR out, DWORD op, DWORD om, LPCTSTR buf, DWORD size );

	//==============================================================
	// HtmlText()
	//==============================================================
	/// Adds HTML escape sequences to text
	/**
		\param [in] out				-	Buffer receiving data
		\param [in] odw				-	Size of buffer in out
		\param [in] buf				-	Data to write
		\param [in] size			-	Number of bytes in buf
		\param [in] bHyperLinks		-	Set to non-zero to resolve hyperlinks
		
		\return odw + number of bytes written to out
	
		\see 
	*/
	static DWORD HtmlText( LPSTR out, DWORD odw, LPCTSTR buf, DWORD size, BOOL bHyperLinks );

	//==============================================================
	// ChooseFont()
	//==============================================================
	/// Displays the Windows font chooser dialog box
	/**
		\param [in] hWnd	-	Parent window handle
		
		\return Non-zero if new font was choosen
	
		\see 
	*/
	BOOL ChooseFont( HWND hWnd );

	//==============================================================
	// DrawMonoSpacedText()
	//==============================================================
	/// Draws any font as monospaced
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pText	-	Text string
		\param [in] pRect	-	Position of text
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DrawMonoSpacedText( HDC hDC, LPCTSTR pText, LPRECT pRect );

	//==============================================================
	// CalcTextWidth()
	//==============================================================
	/// Calculates the width of the specified text
	/**
		\param [in] hDC		-	Handle to DC that would receive text
		\param [out] pWidth	-	Pointer that receives the text width
		
		\return Width of text in pixels
	
		\see 
	*/
	long CalcTextWidth( HDC hDC, long *pWidth = NULL );

	//==============================================================
	// CalcTextHeight()
	//==============================================================
	/// Calculates the height of the specified text
	/**
		\param [in] hDC			-	Handle to DC that would receive text
		\param [in] pHeight		-	Pointer that receives the text height
		
		\return Height of text in pixels
	
		\see 
	*/
	long CalcTextHeight( HDC hDC, long *pHeight = NULL );

	//==============================================================
	// CalcRect()
	//==============================================================
	/// Calculates the size of a text string
	/**
		\param [in] hDC		-	DC handle that would receive text
		\param [in] pText	-	Text string
		\param [in] pRect	-	Receives the calculated size of the text
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CalcRect( HDC hDC, LPCTSTR pText, LPRECT pRect );

	//==============================================================
	// GetFont()
	//==============================================================
	/// Gets a copy of the current font information
	/**
		\param [in] pFont	-	Receives font data
		
		\return Non-zero if succcess
	
		\see 
	*/
	BOOL GetFont( LPLOGFONT pFont );

	//==============================================================
	// ChooseFont()
	//==============================================================
	/// Displays Windows font chooser dialog box
	/**
		\param [in] pFont	-	Pointer to LOGFONT structure
		\param [in] hWnd	-	Handle to parent window
		\param [in] bInit	-	True if pFont contains initial font info
		\param [in] pRGB	-	Pointer to variable that receives font color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ChooseFont( LPLOGFONT pFont, HWND hWnd = NULL, BOOL bInit = FALSE, COLORREF *pRGB = NULL );

	//==============================================================
	// SetFont()
	//==============================================================
	/// Sets the font
	/**
		\param [in] pFont	-	Pointer to LOGFONT structure describing font
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetFont( LPLOGFONT pFont );

	//==============================================================
	// Default()
	//==============================================================
	/// Sets default text attributes
	void Default();

	//==============================================================
	// DrawText()
	//==============================================================
	/// Draws the specified text into a Device Context
	/**
		\param [in] hDC		-	DC handle that receives text
		\param [in] pText	-	Text to draw into hDC
		\param [in] pRect	-	Position for text
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DrawText(	HDC hDC, LPCTSTR pText, LPRECT pRect );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources allocated for this class
	void Destroy();

	//==============================================================
	// SetFont()
	//==============================================================
	/// Sets a font by font name and size
	/**
		\param [in] dwSize	-	Point size for font
		\param [in] pFont	-	Name of the font
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetFont( DWORD dwSize, LPCTSTR pFont ); 

	/// Constructor
	CText();

	/// Destructor
	virtual ~CText();

	//==============================================================
	// SetXMargin()
	//==============================================================
	/// Sets the horizontal text margin
	/**
		\param [in] x	-	Horizontal text margin
	*/
	void SetXMargin( long x ) { m_xMargin = x; }

	//==============================================================
	// SetYMargin()
	//==============================================================
	/// Sets vertical text margin
	/**
		\param [in] y	-	Vertical text margin
	*/
	void SetYMargin( long y ) { m_yMargin = y; }

	//==============================================================
	// SetMargins()
	//==============================================================
	/// Sets the text margins
	/**
		\param [in] x	-	Horizontal text margin
		\param [in] y	-	Vertical text margin
	*/
	void SetMargins( long x, long y ) { m_xMargin = x; m_yMargin = y; }

//------------------------------------------------------------------
// Must be followed by a call to SetFont() or no effect
//------------------------------------------------------------------

	//==============================================================
	// SetEscapement()
	//==============================================================
	/// Sets the text rotation
	/**
		\param [in] v	-	New escapement value

		This is basically the rotation angle in degrees
	*/
	void SetEscapement( int v = 0 ) { m_nEscapement = v; }

	//==============================================================
	// SetWeight()
	//==============================================================
	/// Sets the text weight
	/**
		\param [in] w	-	Font weight
	*/
	void SetWeight( int w = FW_NORMAL ) { m_nWeight = w; }

	//==============================================================
	// SetItalic()
	//==============================================================
	/// Enables / disables italic text
	/**
		\param [in] b	-	Non-zero for italic text, zero for 
							normal text
	*/
	void SetItalic( BOOL b = FALSE ) { m_bItalic = b; }

	//==============================================================
	// SetUnderline()
	//==============================================================
	/// Enables / disables underlined text
	/**
		\param [in] b	-	Non-zero for underlined text, zero for 
							normal text
	*/
	void SetUnderline( BOOL b = FALSE ) { m_bUnderline = b; }

	//==============================================================
	// SetStrikeOut()
	//==============================================================
	/// Enables / disables striked out text
	/**
		\param [in] b	-	Non-zero for striked out text, zero for
							normal text
	*/
	void SetStrikeOut( BOOL b = FALSE ) { m_bStrikeOut = b; }

//------------------------------------------------------------------

//------------------------------------------------------------------
// Effective next call to DrawText()
//------------------------------------------------------------------

	//==============================================================
	// SetColor()
	//==============================================================
	/// Sets the text color
	/**
		\param [in] col	-	New color for text
	*/
	void SetColor( COLORREF col = MAXDWORD ) { m_rgbColor = col; }

	//==============================================================
	// GetColor()
	//==============================================================
	/// Returns current text color
	COLORREF GetColor() { return m_rgbColor; }


	//==============================================================
	// SetColor()
	//==============================================================
	/// Sets the text flags
	/**
		\param [in] f	-	Text flags
	*/
	void SetFlags( DWORD f =	DT_CENTER | DT_VCENTER | 
								DT_WORDBREAK | DT_NOCLIP |
								DT_SINGLELINE ) 
	{	m_dwFlags = f; }

	//==============================================================
	// GetFlags()
	//==============================================================
	/// Returns current text flags
	DWORD GetFlags() { return m_dwFlags; }

//------------------------------------------------------------------

	//==============================================================
	// CalcTextBox()
	//==============================================================
	/// Calculates the size of a single text character
	/**
		\param [in] hDC		-	DC handle where text would be drawn
	*/
	void CalcTextBox( HDC hDC )
	{
		m_txtwidth = CalcTextWidth( hDC, m_monowidth );
		m_txtheight = CalcTextHeight( hDC );
		SetRect( &m_txtbox, 0, 0, m_txtwidth, m_txtheight );
	}

	//==============================================================
	// SetMonoSpacedText()
	//==============================================================
	/// Sets up for drawing mono-spaced font for any font type
	/**
		\param [in] hDC		-	DC handle that will receive text
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetMonoSpacedText( HDC hDC ) 
	{
		// Use screen DC if one is not provided
		BOOL bReleaseDC = FALSE;
		if ( hDC == NULL ) 
		{	if ( ( hDC = GetDC( NULL ) ) == NULL ) return FALSE;
			bReleaseDC = TRUE;
		} // end if

		CalcTextBox( hDC ); 
		
		// Release DC if needed
		if ( bReleaseDC ) ReleaseDC( NULL, hDC );

		return TRUE;
	}

	//==============================================================
	// SetMonoSpacedTextMetrics()
	//==============================================================
	/// Sets the width and height of a mono-spaced character
	/**
		\param [in] w	-	Horizontal size of a single character
		\param [in] h	-	Vertical size of a single character
	*/
	void SetMonoSpacedTextMetrics( long w, long h ) 
	{	m_txtwidth = w; m_txtheight = h; SetRect( &m_txtbox, 0, 0, w, h ); }

	//==============================================================
	// SetMonoSpacedTabs()
	//==============================================================
	/// Size of a mono spaced tab character
	/**
		\param [in] t	-	Size of a tab character
	*/
	void SetMonoSpacedTabs( DWORD t ) { m_monotab = t; }

	//==============================================================
	// GetMonoTextWidth()
	//==============================================================
	/// Returns the mono-spaced width of a single character
	DWORD GetMonoTextWidth() { return m_txtwidth; }

	//==============================================================
	// GetMonoTextHeight()
	//==============================================================
	/// Returns the mono-spaced height of a single character
	DWORD GetMonoTextHeight() { return m_txtheight; }

	//==============================================================
	// LPLOGFONT()
	//==============================================================
	/// Returns a pointer to the LOGFONT structure for the current font
	operator LPLOGFONT() { return &m_logfont; }

	//==============================================================
	// HFONT()
	//==============================================================
	/// Returns the HFONT handle of the current font
	operator HFONT() { return m_hFont; }

	//==============================================================
	// IsFont()
	//==============================================================
	/// Returns non-zero if a font is loaded
	BOOL IsFont() { return ( m_hFont != NULL ); }

	//==============================================================
	// GetLogFont()
	//==============================================================
	/// Returns a pointer to the LOGFONT structure for the current font
	LPLOGFONT GetLogFont() { return &m_logfont; }

private:

	/// Currently loaded font
	HFONT		m_hFont;

	/// Horizontal text margin
	long		m_xMargin;

	/// Vertical text margin
	long		m_yMargin;
	
	/// Text rotation
	int			m_nEscapement;

	/// Text weight
	int			m_nWeight;

	/// Non-zero for italic text
	BOOL		m_bItalic;

	/// Non-zero for underlined text
	BOOL		m_bUnderline;

	/// Non-zero for striked out text
	BOOL		m_bStrikeOut;

	/// Text drawing flags
	DWORD		m_dwFlags;

	/// Current text color
	COLORREF	m_rgbColor;

	/// Currently loaded font information
	LOGFONT		m_logfont;

	/// Mono-spaced text width
	long		m_txtwidth;

	/// Mono-spaced text height
	long		m_txtheight;

	/// Mono-spaced text dimensions
	RECT		m_txtbox;

	/// Mono-spaced tab width
	DWORD		m_monotab;

	/// Widths for each mono-spaced character
	long		m_monowidth[ 256 ];

};

#endif // !defined(AFX_TEXT_H__03EABC95_C029_11D2_88F9_00104B2C9CFA__INCLUDED_)
