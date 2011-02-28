// WinPrint.cpp: implementation of the CWinPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CWinPrint::m_bAbort = FALSE;

CWinPrint::CWinPrint()
{
	m_nDocId = 0;
	m_nPageId = 0;

	ZeroMemory( &m_pd, sizeof( m_pd ) );

#if defined ( RULIB_USE_PRINTDLG )
	ZeroMemory( &m_pdx, sizeof( m_pdx ) );
#endif

	ZeroMemory( &m_psd, sizeof( m_psd ) );

	ZeroMemory( &m_rect, sizeof( m_rect ) );
}

CWinPrint::~CWinPrint()
{
	Destroy();
}

void CWinPrint::Destroy()
{
	// End page
	EndPage();

	// End printing
	EndDoc();

	// Delete the print dc
	if ( m_pd.hDC ) DeleteDC( m_pd.hDC );

	ZeroMemory( &m_pd, sizeof( m_pd ) );

#if defined ( RULIB_USE_PRINTDLG )

	if ( m_pdx.hDC ) DeleteDC( m_pdx.hDC );

	ZeroMemory( &m_pdx, sizeof( m_pdx ) );

#endif

	ZeroMemory( &m_psd, sizeof( m_psd ) );

	ZeroMemory( &m_rect, sizeof( m_rect ) );
}

BOOL CWinPrint::PrintDlg(HWND hOwner, LONG lCopies, LONG lFrom, LONG lTo, LONG lMin, LONG lMax)
{

// Use new version instead
//#if defined ( RULIB_USE_PRINTDLG )

//	return PrintDlgEx( hOwner, lCopies, lFrom, lTo, lMin, lMax );

//#endif

	Destroy();

	// Fill in print information
	m_pd.lStructSize = sizeof( m_pd ); 
	m_pd.Flags = PD_RETURNDC; 
	m_pd.hwndOwner = hOwner; 
	m_pd.nFromPage = (WORD)lFrom; 
	m_pd.nToPage = (WORD)lTo; 
	m_pd.nMinPage = (WORD)lMin; 
	m_pd.nMaxPage = (WORD)lMax; 
	m_pd.nCopies = (WORD)lCopies; 

	// Attempt to get printer
	if ( !::PrintDlg( &m_pd ) ) return FALSE;

	// Reset the abort flag
	m_bAbort = FALSE;

	// Set abort procedure
	SetAbortProc( m_pd.hDC, &CWinPrint::AbortProc );

	return TRUE;
}

BOOL CWinPrint::PageSetup(HWND hOwner)
{
	m_psd.lStructSize = sizeof( m_psd );
	m_psd.hwndOwner = hOwner;

	if ( !::PageSetupDlg( &m_psd ) ) return FALSE;

	return TRUE;
}

#if defined ( RULIB_USE_PRINTDLG )

BOOL CWinPrint::PrintDlgEx(HWND hOwner, LONG lCopies, LONG lFrom, LONG lTo, LONG lMin, LONG lMax)
{
	Destroy();

	PRINTPAGERANGE ppr = { lFrom, lTo };

	// Fill in print information
	m_pdx.lStructSize = sizeof( m_pdx ); 
	m_pdx.hwndOwner = hOwner; 
	m_pdx.Flags = PD_ALLPAGES | PD_RETURNDC; 
	m_pdx.nMinPage = lMin; 
	m_pdx.nMaxPage = lMax; 
	m_pdx.nCopies = lCopies; 
	m_pdx.nStartPage = START_PAGE_GENERAL;

	// From to?
	if ( ( lFrom || lTo ) && lFrom != lTo )
	{	m_pdx.nMaxPageRanges = 1;
		m_pdx.lpPageRanges = &ppr;
	} // end if

	// Attempt to get printer
	if ( PD_RESULT_PRINT != ::PrintDlgEx( &m_pdx ) ) 
		return FALSE;

	// Reset the abort flag
	m_bAbort = FALSE;

	// Set abort procedure
	SetAbortProc( m_pdx.hDC, &CWinPrint::AbortProc );

	return TRUE;
}

#endif


BOOL CWinPrint::StartDoc( LPCTSTR pName )
{
	// Get print dc
	HDC hDc = GetPrintHdc();
	if ( !hDc ) return FALSE;

	DOCINFO di;
	ZeroMemory( &di, sizeof( di ) );
	di.cbSize = sizeof( di );
	di.lpszDocName = pName;

	// Start the print job
	m_nDocId = ::StartDoc( hDc, &di );

	return 0 < m_nDocId;
}

BOOL CWinPrint::EndDoc()
{
	// Document in progress?
	if ( 0 >= m_nDocId ) return FALSE;

	// Kill print job id
	m_nDocId = 0;

	// Get print dc
	HDC hDc = GetPrintHdc();
	if ( !hDc ) return FALSE;

	::EndDoc( hDc );

	return TRUE;
}

BOOL CWinPrint::StartPage()
{
	// Get print dc
	HDC hDc = GetPrintHdc();
	if ( !hDc ) return FALSE;

	// Start the print job
	m_nPageId = ::StartPage( hDc );
	if ( 0 >= m_nPageId ) return FALSE;

	// Update device size
	GetDeviceSize();

	return TRUE;
}

BOOL CWinPrint::EndPage()
{
	// Page in progress?
	if ( 0 >= m_nPageId ) return FALSE;

	// Kill print job id
	m_nPageId = 0;

	// Get print dc
	HDC hDc = GetPrintHdc();
	if ( !hDc ) return FALSE;

	// End print page
	return ::EndPage( hDc );
}

LPRECT CWinPrint::GetDeviceSize()
{
	// Get print dc
	HDC hDc = GetPrintHdc();
	if ( !hDc ) return FALSE;

	// Get device size
	SetRect( &m_rect,	0, 0, 
						::GetDeviceCaps( hDc, HORZRES ), 
						::GetDeviceCaps( hDc, VERTRES ) );

	return &m_rect;
}

BOOL CALLBACK CWinPrint::AbortProc( HDC hPrintDC, int nCode )
{
	MSG msg; 

	// Process messages
	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
	{	TranslateMessage( &msg ); 
		DispatchMessage( &msg ); 
	} // end while

	return TRUE; 
}