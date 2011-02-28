// WinPrint.h: interface for the CWinPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPRINT_H__99A96FAC_8D3C_4D64_BF3A_4BE57DF408E0__INCLUDED_)
#define AFX_WINPRINT_H__99A96FAC_8D3C_4D64_BF3A_4BE57DF408E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if 0x0501 <= WINVER && defined( PRINTDLGEX )
#	define RULIB_USE_PRINTDLG
#endif


//==================================================================
// CWinPrint
//
/// Thin wrapper around the Windows Printing API
/**

	Example:

  \code

	CWinPrint wp;
	if ( wp.PrintDlg( hWnd ) )
	{
		// Start print document
		wp.StartDoc( "My Picture" );

		// First page
		wp.StartPage();

		// Draw page
		DrawMyImage( wp.GetPrintHdc(), wp.GetPrintRect() );

		// End first page
		wp.EndPage();

		// End document
		wp.EndDoc();

	} // end if

  \endcode
	
*/
//==================================================================
class CWinPrint  
{
public:

	//==============================================================
	// GetDeviceSize()
	//==============================================================
	/// Calculates the device image size in pixels
	/**		
		\return Pointer to RECT containing image size
	*/
	LPRECT GetDeviceSize();

	//==============================================================
	// EndPage()
	//==============================================================
	/// Ends printing for current page
	/**
		\return Non-zero if success
	*/
	BOOL EndPage();

	//==============================================================
	// StartPage()
	//==============================================================
	/// Starts printing on a particular page
	/**
		\return Non-zero if success
	*/
	BOOL StartPage();
	
	//==============================================================
	// EndDoc()
	//==============================================================
	/// Ends printing for the current document
	/**
		\return Non-zero if success
	*/
	BOOL EndDoc();

	//==============================================================
	// StartDoc()
	//==============================================================
	/// Starts print job
	/**
		\param [in] pName	-	Print job name.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL StartDoc( LPCTSTR pName );
	
	//==============================================================
	// Destroy()
	//==============================================================
	/// Ends printing and resets print device information.
	void Destroy();

	//==============================================================
	// PageSetup()
	//==============================================================
	/// Displays the page setup dialog box to the user.
	/**
		\param [in] hOwner	-	Handle to parent window.

		Results are stored in m_pd.		
		
		\return	Non-zero if user clicks 'Ok'.
	
		\see 
	*/
	BOOL PageSetup( HWND hOwner );

	//==============================================================
	// PrintDlg()
	//==============================================================
	/// Displays the print dialog box to the user
	/**
		\param [in] hOwner		-	Handle to parent window.
		\param [in] lCopies		-	Default number of copies
		\param [in] lFrom		-	Default start page
		\param [in] lTo			-	Default end page
		\param [in] lMin		-	First page
		\param [in] lMax		-	Last page
		
		\return Non-zero if the user clicks 'Print'.
	
		\see 
	*/
	BOOL PrintDlg( HWND hOwner, LONG lCopies = 1, LONG lFrom = 1, LONG lTo = 1, LONG lMin = 0, LONG lMax = 1 );
	
#if defined ( RULIB_USE_PRINTDLG )

	//==============================================================
	// PrintDlgEx()
	//==============================================================
	/// Displays the new print dialog box.
	/**
		\param [in] hOwner		-	Handle to parent window.
		\param [in] lCopies		-	Default number of copies
		\param [in] lFrom		-	Default start page
		\param [in] lTo			-	Default end page
		\param [in] lMin		-	First page
		\param [in] lMax		-	Last page
		
		\return Non-zero if the user clicks 'Print'.
	
		\see 
	*/
	BOOL PrintDlgEx( HWND hOwner, LONG lCopies = 1, LONG lFrom = 1, LONG lTo = 1, LONG lMin = 0, LONG lMax = 0 );

#endif

	/// Default constructor
	CWinPrint();
	
	/// Destructor
	virtual ~CWinPrint();

	//==============================================================
	// GetPrintHdc()
	//==============================================================
	/// Returns the print HDC.  Must call PrintDlg() before using.
	/**		
		\return HANDLE to a printer DC
	
		\see 
	*/
	HDC GetPrintHdc() 
	{
		if ( m_pd.hDC ) return m_pd.hDC;

#if defined ( RULIB_USE_PRINTDLG )

		if ( m_pdx.hDC ) return m_pdx.hDC;

#endif

		return NULL;
	}

	//==============================================================
	// GetPrintRect()
	//==============================================================
	/// Returns the size of the print device context.
	/**
		
		Must call PrintDlg or PrintDlgEx before using this function.

		\return RECT structure containing size of the printer device image.
	
		\see 
	*/
	LPRECT GetPrintRect() { return &m_rect; }

	//==============================================================
	// SupportsBitBlt()
	//==============================================================
	/// Returns non-zero if the printer supports BitBlt().
	BOOL SupportsBitBlt() 
	{	return 0 != ( GetDeviceCaps( GetPrintHdc(), RASTERCAPS ) & RC_BITBLT ); }

	//==============================================================
	// GetPrintDlgInfo()
	//==============================================================
	/// Returns reference to PRINTDLG structure
	PRINTDLG& GetPrintDlgInfo() { return m_pd; }

	//==============================================================
	// GetPageSetupInfo()
	//==============================================================
	/// Returns reference to PAGESETUPDLG structure
	PAGESETUPDLG& GetPageSetupInfo() { return m_psd; }

#if defined ( RULIB_USE_PRINTDLG )

	//==============================================================
	// GetPrintDlgExInfo()
	//==============================================================
	/// Returns reference to PRINTDLGEX structure
	PRINTDLGEX& GetPrintDlgExInfo() { return m_pdx; }

#endif

	//==============================================================
	// IsDocumentPrinting()
	//==============================================================
	/// Returns non-zero if a document is being printed.
	BOOL IsDocumentPrinting() { return 0 < m_nDocId; }

	//==============================================================
	// IsPagePrinting()
	//==============================================================
	/// Returns non-zero if a page is being printed.
	BOOL IsPagePrinting() { return 0 < m_nPageId; }


	//==============================================================
	// AbortProc()
	//==============================================================
	/// Called by Windows when a print job is to be cancelled
	/**
		\param [in] hPrintDC	-	Print DC
		\param [in] nCode		-	Error code
		
		\return 
	
		\see 
	*/
	static BOOL CALLBACK AbortProc( HDC hPrintDC, int nCode );


	//==============================================================
	// CancelPrinting()
	//==============================================================
	/// Cancels any current print job
	static void CancelPrinting() { m_bAbort = TRUE; }

private:

	/// PRINTDLG structure
	PRINTDLG					m_pd;

	/// PAGESETUPDLG structure
	PAGESETUPDLG				m_psd;

#if defined ( RULIB_USE_PRINTDLG )

	/// PRINTDLGEX structure
	PRINTDLGEX					m_pdx;

#endif

	/// Greater than zero if document is being printed
	int							m_nDocId;

	/// Greater than zero if page is being printed
	int							m_nPageId;

	/// Size of the printer device image
	RECT						m_rect;

	/// Non-zero to abort printing
	static BOOL					m_bAbort;
	
};

#endif // !defined(AFX_WINPRINT_H__99A96FAC_8D3C_4D64_BF3A_4BE57DF408E0__INCLUDED_)
