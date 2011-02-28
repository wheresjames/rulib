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
// XpTheme.h: interface for the CXpTheme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPTHEME_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_)
#define AFX_XPTHEME_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <uxtheme.h>
#include <tmschema.h>

#define ixptf_PFNOPENTHEMEDATA							0
#define ixptf_PFNCLOSETHEMEDATA							1
#define ixptf_PFNDRAWTHEMEBACKGROUND					2
#define ixptf_PFNDRAWTHEMETEXT							3
#define ixptf_PFNGETTHEMEBACKGROUNDCONTENTRECT			4
#define ixptf_PFNGETTHEMEBACKGROUNDEXTENT				5
#define	ixptf_PFNGETTHEMEPARTSIZE						6
#define ixptf_PFNGETTHEMETEXTEXTENT						7
#define ixptf_PFNGETTHEMETEXTMETRICS					8
#define ixptf_PFNGETTHEMEBACKGROUNDREGION				9
#define ixptf_PFNHITTESTTHEMEBACKGROUND					10
#define ixptf_PFNDRAWTHEMEEDGE							11
#define ixptf_PFNDRAWTHEMEICON							12
#define ixptf_PFNISTHEMEPARTDEFINED						13
#define ixptf_PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT	14
#define ixptf_PFNGETTHEMECOLOR							15
#define ixptf_PFNGETTHEMEMETRIC							16
#define ixptf_PFNGETTHEMESTRING							17
#define ixptf_PFNGETTHEMEBOOL							18
#define ixptf_PFNGETTHEMEINT							19
#define ixptf_PFNGETTHEMEENUMVALUE						20
#define ixptf_PFNGETTHEMEPOSITION						21
#define ixptf_PFNGETTHEMEFONT							22
#define ixptf_PFNGETTHEMERECT							23
#define ixptf_PFNGETTHEMEMARGINS						24
#define ixptf_PFNGETTHEMEINTLIST						25
#define ixptf_PFNGETTHEMEPROPERTYORIGIN					26
#define ixptf_PFNSETWINDOWTHEME							27
#define ixptf_PFNGETTHEMEFILENAME						28
#define ixptf_PFNGETTHEMESYSCOLOR						29
#define ixptf_PFNGETTHEMESYSCOLORBRUSH					30
#define ixptf_PFNGETTHEMESYSBOOL						31
#define ixptf_PFNGETTHEMESYSSIZE						32
#define ixptf_PFNGETTHEMESYSFONT						33
#define ixptf_PFNGETTHEMESYSSTRING						34
#define ixptf_PFNGETTHEMESYSINT							35
#define ixptf_PFNISTHEMEACTIVE							36
#define ixptf_PFNISAPPTHEMED							37
#define ixptf_PFNGETWINDOWTHEME							38
#define ixptf_PFNENABLETHEMEDIALOGTEXTURE				39
#define ixptf_PFNISTHEMEDIALOGTEXTUREENABLED			40
#define ixptf_PFNGETTHEMEAPPPROPERTIES					41
#define ixptf_PFNSETTHEMEAPPPROPERTIES					42
#define ixptf_PFNGETCURRENTTHEMENAME					43
#define ixptf_PFNGETTHEMEDOCUMENTATIONPROPERTY			44
#define ixptf_PFNDRAWTHEMEPARENTBACKGROUND				45
#define ixptf_PFNENABLETHEMING							46

#define ixptf_PFNSIZE									47


#define CXPTHEME_MAXERR		128

//==================================================================
// CXpTheme
//
/// Wraps the Windows XP theme API
/**
	  Provides a thin wrapper for the Windows XP theme API.  The main
	  benefit of this class is that it accesses the Windows XP theme
	  API using dynamic binding.  That means your app can use XP
	  elements in XP, while still supporting earlier versions of 
	  Windows that do not support themes.
*/
//==================================================================
class CXpTheme  
{

public:

	int GetThemeMetric( HDC hDC, int iPartId, int iStateId, int iPropId, int iDefault );

	static BOOL IsWindowThemed( HWND hWnd );
	static HTHEME GetWindowTheme( HWND hWnd );
	static BOOL SetThemeAppProperties( DWORD dwFlags );
	static DWORD GetThemeAppProperties();
	static BOOL IsThemeDialogTextureEnabled( HWND hWnd );
	static BOOL IsAppThemed();
	static BOOL IsThemeActive();
	
	static BOOL EnableThemeDialogTexture( HWND hWnd, DWORD dwFlags );
	static BOOL SetWindowTheme( HWND hWnd, LPCTSTR pszSubAppName, LPCTSTR pszSubIdList );
	static BOOL GetCurrentThemeName( LPSTR pszThemeFileName, int cchMaxNameChars, LPSTR pszColorBuff, int cchMaxColorChars, LPSTR pszSizeBuf, int cchMaxSizeChars );
	static BOOL GetThemeDocumentationProperty( LPCTSTR pszThemeName, LPCTSTR pszPropertyName, LPSTR pszValueBuff, int cchMaxValChars );
	static BOOL DrawThemeParentBackground( HWND hWnd, HDC hDC, LPRECT pRect );
	static BOOL EnableTheming( BOOL bEnable );

	
	COLORREF GetThemeColor( int iPartId, int iStateId, int iPropId, COLORREF rgbDefault );
	BOOL GetThemeSysInt( int iIntId, int *piValue );
	BOOL GetThemeSysString( int iStringId, LPSTR pszStringBuf, int cchMaxStringChars );
	BOOL GetThemeSysFont( int iFontId, LPLOGFONT plf );
	int GetThemeSysSize( int iSizeId );
	BOOL GetThemeSysBool( int iBoolId );
	HBRUSH GetThemeSysColorBrush( int iColorId );
	COLORREF GetThemeSysColor( int iColorId );
	BOOL GetThemeFilename( int iPartId, int iStateId, int iPropId, LPSTR pszThemeFileName, int cchMaxBufChars );
	BOOL GetThemePropertyOrigin( int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN *pOrigin );
	BOOL GetThemeIntList( int iPartId, int iStateId, int iPropId, INTLIST *pIntList );
	BOOL GetThemeMargins( HDC hDC, int iPartId, int iStateId, int iPropId, LPRECT prc, MARGINS *pMargins );
	BOOL GetThemeRect( int iPartId, int iStateId, int iPropId, LPRECT pRect );
	BOOL GetThemeFont( HDC hDC, int iPartId, int iStateId, int iPropId, LPLOGFONT pFont );
	BOOL GetThemePosition( int iPartId, int iStateId, int iPropId, LPPOINT pPoint );
	BOOL GetThemeEnumValue( int iPartId, int iStateId, int iPropId, int *piVal );
	int GetThemeInt( int iPartId, int iStateId, int iPropId, int iDefault );
	BOOL GetThemeInt( int iPartId, int iStateId, int iPropId, int *piVal );
	BOOL GetThemeBool( int iPartId, int iStateId, int iPropId, BOOL bDefault );
	BOOL GetThemeBool( int iPartId, int iStateId, int iPropId, BOOL *pfVal );
	BOOL GetThemeString( int iPartId, int iStateId, int iPropId, LPSTR pszBuff, int cchMaxBufChars );
	BOOL GetThemeMetric( HDC hDC, int iPartId, int iStateId, int iPropId, int *piVal );
	BOOL GetThemeColor( int iPartId, int iStateId, int iPropId, LPCOLORREF pColor );
	BOOL IsThemeBackgroundPartiallyTransparent( int iPartId, int iStateId );
	BOOL IsThemePartDefined( int iPartId, int iStateId );
	BOOL DrawThemeIcon( HDC hDC, int iPartId, int iStateId, LPRECT pRect, HIMAGELIST himl, int iImageIndex );
	BOOL DrawThemeEdge( HDC hDC, int iPartId, int iStateId, LPRECT pDestRect, UINT uEdge, UINT uFlags, LPRECT pContentRect );
	BOOL HitTestThemeBackground( HDC hDC, int iPartId, int iStateId, DWORD dwOptions, LPRECT pRect, HRGN hRgn, POINT ptTest, LPWORD pwHitTestCode );
	BOOL GetThemeBackgroundRegion( HDC hDC, int iPartId, int iStateId, LPRECT pRect, HRGN *pRegion );
	BOOL GetThemeTextMetrics( HDC hDC, int iPartId, int iStateId, LPTEXTMETRIC ptm );
	BOOL GetThemeTextExtent( HDC hDC, int iPartId, int iStateId, LPCTSTR pszText, int iCharCount, DWORD dwTextFlags, LPRECT pBoundingRect, LPRECT pExtentRect );

	BOOL GetThemePartSize( HDC hDC, int iPartId, int iStateId, LPRECT pRect, enum THEMESIZE eSize, LPSIZE psz );
	
	BOOL GetThemeBackgroundExtent( HDC hDC, int iPartId, int iStateId, LPRECT pContentRect, LPRECT pExtentRect );
	BOOL GetThemeBackgroundContentRect( HDC hDC, int iPartId, int iStateId, LPRECT pBoundingRect, LPRECT pContentRect );

	BOOL DrawThemeText( HDC hDC, int iPartId, int iStateId, LPCTSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPRECT pRect );
	BOOL DrawThemeBackground( HDC hDC, int iPartId, int iStateId, LPRECT pRect, LPRECT pClipRect );
	BOOL Close();

	BOOL Open( HWND hwnd, LPCTSTR pszClassList );

	void Destroy();

	static BOOL Init();
	static void Uninit();
	static BOOL LoadFunctionPointers();
	static LRESULT SetErr( LRESULT err );
	static LRESULT GetLastError();
	
	CXpTheme();
	CXpTheme( HWND hWnd, LPCTSTR pClass );
	virtual ~CXpTheme();

	BOOL IsDll() { return ( m_hModule != NULL ); }
	BOOL IsTheme() { return ( m_hTheme != NULL ); }

	void Attach( HTHEME hTheme, BOOL bAutoRelease = FALSE )
	{	Close(); m_hTheme = hTheme; m_bReleaseTheme = bAutoRelease; }
	void Detach() { m_hTheme = NULL; }

private:

	BOOL				m_bReleaseTheme;
	HTHEME				m_hTheme;

	static DWORD		m_dwLast;
	static LRESULT		m_hrLast[ CXPTHEME_MAXERR ];

	static HMODULE		m_hModule;
	static void*		m_ixptf[ ixptf_PFNSIZE ];

};

#endif // !defined(AFX_XPTHEME_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_)
