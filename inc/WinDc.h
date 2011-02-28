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
// WinDc.h: interface for the CWinDc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDC_H__4F8FB0B2_A5C5_11D4_8214_0050DA205C15__INCLUDED_)
#define AFX_WINDC_H__4F8FB0B2_A5C5_11D4_8214_0050DA205C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSCANTYPE_GRID		0
#define MSCANTYPE_POINT		1

// Intensity
#define WDC_CALC_INTENSITY( r, g, b )	( b + ( g << 1 ) + r )

// Saturation
#define WDC_CALC_SATURATION( r, g, b ) ( MAX_VAL_3D( r, g, b ) - MIN_VAL_3D( r, g, b ) )

// Hue
#define WDC_MAX_HUESCALE				10000
#define WDC_MAX_HUESCALE_2				( WDC_MAX_HUESCALE >> 1 )
#define WDC_CALC_HUE( h, r, g, b )																				\
	( !( h = ( ( WDC_MAX_HUESCALE_2 * r ) >> 8 ) + ( ( WDC_MAX_HUESCALE_2 * g ) >> 8 ) + ( ( WDC_MAX_HUESCALE_2 * b ) >> 8 ) ) ?	\
	WDC_MAX_HUESCALE_2 : ( ( ( WDC_MAX_HUESCALE_2 * g ) >> 8 ) * WDC_MAX_HUESCALE_2 + ( ( WDC_MAX_HUESCALE_2 * r ) >> 8 ) * WDC_MAX_HUESCALE ) / h )


/// Describes frame motion information
/**
	\see CWinDc
*/
typedef struct tagMOTION
{
	/// Non-zero if motion detection is enabled
	BOOL			bEnable;

	/// Pixel grid to check
	long			grid;			// 10

	/// Smoothing applied to samples
	long			smoothing;		// 8

	/// Pixel motion threshold
	long			threshold;		// 10

	/// Image scan type
	long			scantype;

	/// Brightness threshold
	long			thbright;		// 500

	/// Detected image brightness level
	long			sensebright;	

	/// Motion threshold
	long			thmotion;		// 500

	/// Detected motion level
	long			sensemotion;

	/// Last frame containing motion (Globally unique value)
	DWORD			dwMotionFrame;

	/// Pointer to motion data
	long			*plMotion;

	/// Pointer to pixel difference data
	long			*plDelta;

	/// Maximum pixel difference
	long			lMaxDelta;

	/// Motion buffer size
	DWORD			dwMotion;

	/// Motion buffer width
	long			lxMotion;

	/// Motion buffer height
	long			lyMotion;

	/// Number of frames proccessed
	DWORD			dwMotionCycles;

} MOTION, *LPMOTION; // end typedef struct


//==================================================================
// CWinDc
//
/// Wraps a windows DC
/**
	This class wraps a Windows DC and provides lots of image
	manipulation functions.  

	Out of the whole library, I find this class to be the most 
	useful.
*/
//==================================================================
class CWinDc  
{
public:

	//==============================================================
	// BitBlt()
	//==============================================================
	/// Copies to a DC using specified operation
	/**
		\param [in] hDst		-	Handle to destination device context.
		\param [in] pDst		-	Destination rect.
		\param [in] pSrc		-	Source rect.
		\param [in] bStretch	-	Non-zero to stretch image to pDst
		\param [in] dwFlags		-	Copy flags
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL BitBlt( HDC hDst, LPRECT pDst, LPRECT pSrc, BOOL bStretch = FALSE, DWORD dwFlags = SRCCOPY );

	//==============================================================
	// Assume()
	//==============================================================
	/// Takes over the device context of another class
	/**
		\param [in] pDc		-	CWinDc source object
	*/
	void Assume( CWinDc *pDc );

	//==============================================================
	// GetMetrics()
	//==============================================================
	/// Gets the parameters of the current DC
	/**
		\param [in] pBits			-	Receives pointer to the image.  
										Must use CreateDIBSection() 
										for this to be valid.
		\param [in] plWidth			-	Receives the width of the image.
		\param [in] plHeight		-	Receives the height of the image
		\param [in] plSize			-	Receives the size of the buffer
										in pBits.
		\param [in] plScanWidth		-	Receives the scan width of the
										image.

		Any of the parameters can be NULL if they are not needed.
		
		\return Non-zero if valid information is returned.
	
		\see 
	*/
	BOOL GetMetrics( LPBYTE *pBits, LONG *plWidth, LONG *plHeight, LONG *plSize, LONG *plScanWidth );

	//==============================================================
	// GetPixel()
	//==============================================================
	/// Returns the value of the pixel at the specified position
	/**
		\param [in] x	-	Horzontal offset of pixel
		\param [in] y	-	Vertical offset of the pixel
		
		\return RGB value of the pixel at the specified point.
	
		\see 
	*/
	COLORREF GetPixel( long x, long y );

	//==============================================================
	// SetPixel()
	//==============================================================
	/// Sets the value of the pixel at the specified point
	/**
		\param [in] x	-	Horzontal offset of pixel
		\param [in] y	-	Vertical offset of the pixel
		\param [in] rgb -	New RGB color value for pixel
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPixel( long x, long y, COLORREF rgb );

	//==============================================================
	// Aluminize()
	//==============================================================
	/// Creates an Aluminum texture.
	/**
		\param [in] var		-	Random variance
		\param [in] run		-	Over-run
		\param [in] pRect	-	Portion of image to texture.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Aluminize( DWORD var = 5, DWORD run = 40, LPRECT pRect = NULL );

	//==============================================================
	// Texturize()
	//==============================================================
	/// Creates a random texture
	/**
		\param [in] var		-	Random variance
		\param [in] pRect	-	Portion of image to texture
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Texturize( DWORD var, LPRECT pRect = NULL );

	//==============================================================
	// Mask()
	//==============================================================
	/// Creates a mask for the image
	/**
		\param [in] pMask	-	Receives the mask image
		\param [in] pRect	-	Portion of image to mask
		\param [in] pBck	-	Pointer to background image
		\param [in] rgbBck	-	Color to mask
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Mask( CWinDc *pMask, LPRECT pRect, CWinDc *pBck,  COLORREF rgbBck );

	//==============================================================
	// Tile()
	//==============================================================
	/// Tiles the image
	/**
		\param [in] hDC		-	Destination device context
		\param [in] pRect	-	Portion of hDC to tile
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Tile( HDC hDC, LPRECT pRect );

	//==============================================================
	// DeepFill()
	//==============================================================
	/// Creates a random textured fill pattern
	/**
		\param [in] pRect		-	Portion of image to texture
		\param [in] rgbFill		-	Fill color
		\param [in] var			-	Random variance
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DeepFill( LPRECT pRect, COLORREF rgbFill, DWORD var );

	//==============================================================
	// CreateOuterRgn()
	//==============================================================
	/// Creates a region mask for the image
	/**
		\param [in] hRgn	-	Handle to region to update
		\param [in] xoff	-	Horizontal offset into region
		\param [in] yoff	-	Vertical offset into region
		
		\return Handle to created region
	
		\see 
	*/
	HRGN CreateOuterRgn( HRGN hRgn, long xoff, long yoff )
	{	return CreateOuterRgn(	hRgn, GetSafeHdc(), GetRect(), GetTransparentColor(),
							xoff, yoff, GetPosChromaErr(), GetNegChromaErr() ); }

	//==============================================================
	// CreateOuterRgn()
	//==============================================================
	/// Creates a region mask for the image
	/**
		\param [in] hRgn		-	Handle to region to update
		\param [in] hDC			-	Source DC
		\param [in] pRect		-	Portion of image to consider
		\param [in] rgbtrans	-	Transparent color
		\param [in] xoff		-	Horizontal offset into region
		\param [in] yoff		-	Vertical offset into region
		\param [in] cpos		-	Positive chroma keying error
		\param [in] cneg		-	Negative chroma keying error
		
		\return Handle to created region
	
		\see 
	*/
	HRGN CreateOuterRgn( HRGN hRgn, HDC hDC, LPRECT pRect, COLORREF rgbtrans, long xoff, long yoff, long cpos, long cneg );

	//==============================================================
	// MapFloodFill()
	//==============================================================
	/// Maps a flood fill
	/**
		\param [in] map		-	Receives the flood fill map
		\param [in] x		-	Horizontal offset of the map
		\param [in] y		-	Vertical offset of the map
		\param [in] w		-	Width of the map
		\param [in] h		-	Height of the mamp
		\param [in] fc		-	Foreground map color
		\param [in] nc		-	Background map color

		Fills in the byte array map with fc for positions that would be
		filled, and nc for positions that would not be filled.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL MapFloodFill( LPBYTE map, long x, long y, long w, long h, BYTE fc, BYTE nc );

	//==============================================================
	// FloodFill()
	//==============================================================
	/// Performs a flood fill on the image
	/**
		\param [in] x		-	Horizontal offset to start fill
		\param [in] y		-	Vertical offset to start fill
		\param [in] fcol	-	Fill color
		\param [in] ncol	-	No fill color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FloodFill( long x, long y, COLORREF fcol, COLORREF ncol );

	//==============================================================
	// Capture()
	//==============================================================
	/// Captures an image of the specified window or desktop
	/**
		\param [in] hWnd	-	Handle of window to capture, NULL for
								desktop.
		\param [in] pRect	-	Portion of window or desktop to capture.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Capture( HWND hWnd, LPRECT pRect = NULL );

	//==============================================================
	// SetRgn()
	//==============================================================
	/// Sets the region of a window
	/**
		\param [in] hWnd		-	Window handle
		\param [in] hParent		-	Parent window handle
		\param [in] pRect		-	Portion of window to apply region
		\param [in] bAddRgn		-	Non-zero to add to current window region
		\param [in] bOuter		-	Non-zero for outer region
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetRgn(HWND hWnd, HWND hParent = NULL, LPRECT pRect = NULL, BOOL bAddRgn = FALSE, BOOL bOuter = FALSE );

	//==============================================================
	// CreateRgn()
	//==============================================================
	/// Creates a region from the image
	/**
		\param [in] hRgn	-	Region handle
		\param [in] xoff	-	Horizontal offset of region
		\param [in] yoff	-	Vertical offset of region
		
		\return Handle to created region
	
		\see 
	*/
	HRGN CreateRgn( HRGN hRgn, long xoff, long yoff )
	{	return CreateRgn(	hRgn, GetSafeHdc(), GetRect(), GetTransparentColor(),
							xoff, yoff, GetPosChromaErr(), GetNegChromaErr() ); }

	//==============================================================
	// CreateRgn()
	//==============================================================
	/// Creates a region from the image
	/**
		\param [in] hRgn		-	Region handle
		\param [in] hDC			-	Handle to HDC to consider
		\param [in] pRect		-	Portion of hDC
		\param [in] rgbtrans	-	Transparent color
		\param [in] xoff		-	Horizontal offset of region
		\param [in] yoff		-	Vertical offset of region
		\param [in] cpos		-	Positive chroma keying error
		\param [in] cneg		-	Negative chroma keying error
		
		\return Handle to created region
	
		\see 
	*/
	HRGN CreateRgn( HRGN hRgn, HDC hDC, LPRECT pRect, COLORREF rgbtrans, long xoff, long yoff, long cpos, long cneg );

	//==============================================================
	// Capture()
	//==============================================================
	/// Assumes control of the image passed in
	/**
		\param [in] pDc		-	Source image
		
		\return Non-zero on success
	
		\see 
	*/
	BOOL Capture( CWinDc *pDc );

	//==============================================================
	// DrawState()
	//==============================================================
	/// Applies a visual effect to indicate a state
	/**
		\param [in] state	-	State effect to apply
		\param [in] hBrush	-	Optional brush used
		
		\code

		Value (state)		Meaning 

		DSS_NORMAL			No modification. 
		DSS_UNION			Dithers the image. 
		DSS_DISABLED		Embosses the image. 
		DSS_MONO			Draws the image using the brush specified by hBrush. 

		\endcode	

		\return Non-zero if success
	
		\see 
	*/
	BOOL DrawState( UINT state = DSS_DISABLED, HBRUSH hBrush = NULL );

	//==============================================================
	// MonoChrome()
	//==============================================================
	/// Translates the current image into a monochrome representation.
	/**
		\param [in] key		-	Monochrome color key
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL MonoChrome( COLORREF key = MAXDWORD );

	//==============================================================
	// CreateIcon()
	//==============================================================
	/// Creates an icon from the encapsulated DC
	/**
		\param [in] w		-	Width of the new icon
		\param [in] h		-	Height of the new icon
		\param [in] bck		-	Background color
		
		\return Handle to new icon or NULL if failure
	
		\see 
	*/
	HICON CreateIcon( long w, long h, COLORREF bck = RGB( 0, 0, 0 ), HDC hDC = NULL, HBITMAP hUseMask = NULL );

	//==============================================================
	// CaptureScreen()
	//==============================================================
	/// Captures an image from the desktop
	/**
		\param [in] pWindow	-	Name of window to capture.
								-	NULL or "< desktop >"	=	Desktop window
								-	"< active window >"		=	Focused window
		\param [in] pRect	-	Size and offset of area to capture
		\param [in] w		-	Width of captured image
		\param [in] h		-	Height of captured image

		\return Non-zero if success
	
		\see 
	*/
	BOOL CaptureScreen( LPCTSTR pWindow, LPRECT pRect, long w, long h );

	//==============================================================
	// TlDraw()
	//==============================================================
	/// Draws applying a translucent effect
	/**
		\param [out] hDC		-	Handle to destination DC
		\param [in] pRect		-	Destination rectangle
		\param [in] dwTl		-	Translucency value
									-	0		= Opaque
									-	10000	= Fully translucent
									-	5000	= 50% translucent
									-	etc...
		\param [in] bStretch 
		
		\return 
	
		\see 
	*/
	BOOL TlDraw( HDC hDC, LPRECT pRect, DWORD dwTl, BOOL bStretch );

	//==============================================================
	// rDraw()
	//==============================================================
	/// This function consolidates some of the internal functionality
	/**
		\param [out] hDC		-	Handle to destination DC
		\param [in] pRect		-	Destination rectangle
		\param [in] bStretch 	-	Non-zero to stretch image to pRect
		\param [in] dwRop		-	Copy operation type
		
		This function is meant for internal use by the class.  Other
		functions probably provide the functionality you desire.

		\return Non-zero if success
	
		\see Draw(), TlDraw(), BitBlt()
	*/
	BOOL rDraw( HDC hDC, LPRECT pRect, BOOL bStretch = TRUE, DWORD dwRop = SRCCOPY );

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes internal variables
	void Init();

	//==============================================================
	// ReleaseMask()
	//==============================================================
	/// Releases any associated image mask
	void ReleaseMask();


	//==============================================================
	// CreateMask()
	//==============================================================
	/// Gernerates a mask from the encapsulated DC
	/**
		\param [in] rgb		-	Color to mask

		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateMask( COLORREF rgb = MAXDWORD );

	//==============================================================
	// CreateDIBSection()
	//==============================================================
	/// Creates a DC that allows direct image data access
	/**
		\param [in] hDC		-	Handle to reference DC, NULL for screen
		\param [in] w		-	Width of new DC
		\param [in] h		-	Height of new DC
		\param [in] bpp		-	Bits per pixel in the new DC
		\param [out] pBits	-	Receives a pointer to the image buffer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateDIBSection( HDC hDC, long w, long h, long bpp = 24, LPBYTE *pBits = NULL );

	//==============================================================
	// CreateDIBSection()
	//==============================================================
	/// Creates a DC that allows direct image data access
	/**
		\param [in] hDC		-	Handle to reference DC, NULL for screen
		\param [in] pRect	-	Size of the new DC
		\param [in] bpp		-	Bits per pixel in the new DC
		\param [out] pBits	-	Receives a pointer to the image buffer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateDIBSection( HDC hDC, LPRECT pRect, long bpp = 24, LPBYTE *pBits = NULL )
	{	return CreateDIBSection( hDC, PRW( pRect ), PRH( pRect ), bpp, pBits ); }

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a standard Windows Device Context
	/**
		\param [in] hDC		-	Reference DC, NULL for screen
		\param [in] x		-	Horizontal offset of the new DC
		\param [in] y		-	Vertical offset of the new DC
		\param [in] width	-	Width of the new DC
		\param [in] height	-	Height of the new DC
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( HDC hDC, long x, long y, long width, long height );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies from the specified Device Context
	/**
		\param [in] hDC			-	Source Device Context
		\param [in] pRect		-	Area to copy
		\param [in] bStretch	-	If non-zero, the image is stretched 
									to the size of the encapsulated DC.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( HDC hDC, LPRECT pRect, BOOL bStretch = TRUE );

	//==============================================================
	// Draw()
	//==============================================================
	/// Draws the image to the specified DC
	/**
		\param [out] hDC		-	Destination DC
		\param [in] pRect		-	Destination rectangle
		\param [in] bStretch	-	Non-zero to stretch image to pRect
		\param [in] dwRop		-	Copy mode
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Draw( HDC hDC, LPRECT pRect, BOOL bStretch = TRUE, DWORD dwRop = SRCCOPY );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the encapsulated DC and any other associated resources.
	void Destroy();

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a standard Windows Device Context
	/**
		\param [in] hDC			-	Handle to reference DC
		\param [in] pRect		-	Size of the new DC
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( HDC hDC, LPRECT pRect );

	/// Default Constructor	
	CWinDc();

	//==============================================================
	// CWinDc()
	//==============================================================
	/// Constructs the specified DC
	/**
		\param [in] hDC		-	Reference DC, NULL for screen
		\param [in] x		-	Horizontal offset of the new DC
		\param [in] y		-	Vertical offset of the new DC
		\param [in] w		-	Width of the new DC
		\param [in] h		-	Height of the new DC
	*/
	CWinDc( HDC hDC, long x, long y, long w, long h );

	//==============================================================
	// CWinDc()
	//==============================================================
	/// Creates a standard Windows Device Context
	/**
		\param [in] hDC			-	Handle to reference DC
		\param [in] pRect		-	Size of the new DC
	*/
	CWinDc( HDC hDC, LPRECT pRect );

	//==============================================================
	// CWinDc()
	//==============================================================
	/// Attaches to an existing Device Context
	/**
		\param [in] hDC				-	Handle to DC
		\param [in] hOldBmp			-	Original HBITMAP in hDC
		\param [in] bAutoDetach		-	Non-zero to detach without
										releasing upon destruction.
		
	*/
	CWinDc( HDC hDC, HBITMAP hOldBmp, BOOL bAutoDetach = TRUE );

	/// Destructor
	virtual ~CWinDc();

	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches to an existing Device Context
	/**
		\param [in] hDC				-	Handle to DC
		\param [in] hOldBmp			-	Original HBITMAP in hDC
		\param [in] bAutoDetach		-	Non-zero to detach without
										releasing upon destruction.
		
	*/
	void Attach( HDC hDC, HBITMAP hOldBmp, BOOL bAutoDetach = TRUE );

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from encapsulated DC without releasing
	/**
		\param [in] bAll	-	Non-zero to detach from the mask 
								image as well.
		
	*/
	void Detach( BOOL bAll = FALSE ) 
	{	m_hDC = NULL; m_hBmp = NULL; m_hOldBmp = NULL; 
		ZeroMemory( &m_rect, sizeof( m_rect ) );
		m_bAutoDetach = FALSE; m_pBits = NULL;
		if ( bAll ) { m_mask = NULL; }
	}

	//==============================================================
	// GetSafeHdc()
	//==============================================================
	/// Returns the encapsulated Device Context handle
	HDC GetSafeHdc() { return m_hDC; }

	//==============================================================
	// HDC()
	//==============================================================
	/// Returns the encapsulated Device Context handle
	operator HDC() { return GetSafeHdc(); }

	// Rect access
	void GetRect( LPRECT r ) { CopyRect( r, GetRect() ); }
	LPRECT GetRect() { return &m_rect; }
	operator LPRECT() { return GetRect(); };

	//==============================================================
	// GetBitmap()
	//==============================================================
	/// Returns the encapsulated bitmap handle
	HBITMAP GetBitmap() { return m_hBmp; }

	//==============================================================
	// HBITMAP()
	//==============================================================
	/// Returns the encapsulated bitmap handle
	operator HBITMAP() { return GetBitmap(); }

	//==============================================================
	// GetWidth()
	//==============================================================
	/// Returns the width of the device context in pixels
	long GetWidth() { return ( m_rect.right - m_rect.left ); }

	//==============================================================
	// GetHeight()
	//==============================================================
	/// Returns the height of the device context in pixels
	long GetHeight() { return ( m_rect.bottom - m_rect.top ); }

	//==============================================================
	// GetImageSize()
	//==============================================================
	/// Returns the size of the raw image buffer
	///	\warning 24 bits per pixel images only
	long GetImageSize() { return ( GetScanWidth( GetWidth() ) * GetHeight() ); }

	//==============================================================
	// GetScanWidth()
	//==============================================================
	/// Returns the byte size of a single horizontal line of image data
	///	\warning 24 bits per pixel images only
	DWORD GetScanWidth() { return GetScanWidth( GetWidth() ); }

	//==============================================================
	// GetScanWidth()
	//==============================================================
	/// Calculates the byte width for an image of the given width
	/**
		\param [in] w	-	Width of the image in pixels
		
		\warning 24 bits per pixel images only

		\return Width of a single horizontal line in bytes.
	
		\see 
	*/
	static DWORD GetScanWidth( DWORD w );

	//==============================================================
	// CalcImageSize()
	//==============================================================
	/// Returns the size of the raw image buffer
	/**
		\param [in] lWidth	-	Image width
		\param [in] lHeight	-	Image height
		
		\warning 24 bits per pixel images only
	
		\return Size of raw image buffer in bytes
	
		\see 
	*/
	static DWORD CalcImageSize(long lWidth, long lHeight);

	//==============================================================
	// DefaultMotionStruct()
	//==============================================================
	/// Initializes a MOTION structure for use
	/**
		\param [out] pm	-	Pointer to MOTION structure that is 
							initialized

		Call ReleaseMotion() when you are done detecting motion.

		\see ReleaseMotion()
	*/
	static void DefaultMotionStruct( LPMOTION pm );

	//==============================================================
	// DetectMotion()
	//==============================================================
	/// Runs motion detection on the encapsulated image
	/**
		\param [out] pm		-	Pointer to MOTION structure that is
								updated.
		\param [in] hDC		-	Handle to DC containing new image
		\param [in] pRect	-	Area of hDC to analyze.
		
		\return Non-zero if motion is detected
	
		\see 
	*/
	static BOOL DetectMotion( LPMOTION pm, HDC hDC, LPRECT pRect );

	//==============================================================
	// DrawMotion()
	//==============================================================
	/// Draws a representation of any detected motion
	/**
		\param [in] pm		-	Pointer to MOTION structure containing 
								information on currently detected motion
		\param [out] hDC	-	Handle to Device Context that receives 
								the motion representation.
		\param [in] pRect	-	Size of the area in hDC.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawMotion( LPMOTION pm, HDC hDC, LPRECT pRect );

	//==============================================================
	// IsMotion()
	//==============================================================
	/// Returns non-zero if MOTION structure indicates motion detected.
	/**
		\param [in] pm	-	Pointer to MOTION structure data
		
		\return Non-zero if motion is detected.
	
		\see 
	*/
	static BOOL IsMotion( LPMOTION pm ) { return ( pm->dwMotionFrame != 0 ); }

	//==============================================================
	// ReleaseMotion()
	//==============================================================
	/// Frees any allocated resources in a MOTION structure
	/**
		\param [in] pm	-	Pointer to MOTION structure to free.
	
		\see DefaultMotionStruct()
	*/
	static void ReleaseMotion( LPMOTION pm );

	//==============================================================
	// DetectMotion()
	//==============================================================
	/// Runs motion detection on the encapsulated image
	/**
		\param [out] pm		-	Pointer to MOTION structure that is
								updated.
		
		\return Non-zero if motion is detected
	
		\see 
	*/
	BOOL DetectMotion( LPMOTION pm )
	{	return DetectMotion( pm, m_hDC, &m_rect ); }

	//==============================================================
	// DrawMotion()
	//==============================================================
	/// Draws a representation of any detected motion
	/**
		\param [in] pm		-	Pointer to MOTION structure containing 
								information on currently detected motion
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DrawMotion( LPMOTION pm )
	{	return DrawMotion( pm, m_hDC, &m_rect ); }
	
	//==============================================================
	// SetChromaErr()
	//==============================================================
	/// Sets the chromatic error in the current transparency color
	/**
		\param [in] pos		-	Positive chromatic error value
		\param [in] neg		-	Negative chromatic error value

		Use this image to compensate for minor deviations in an
		image from the transparent color.  Such as JPEG artifacts.
	*/
	void SetChromaErr( LONG pos, LONG neg )
	{	if ( m_lCPos != pos || m_lCNeg != neg ) ReleaseMask();
		m_lCPos = pos; m_lCNeg = neg; }

	//==============================================================
	// GetPosChromaErr()
	//==============================================================
	/// Returns the current positive chromatic color error value
	LONG GetPosChromaErr() { return m_lCPos; }

	//==============================================================
	// GetNegChromaErr()
	//==============================================================
	/// Returns the current negative chromatic color error value
	LONG GetNegChromaErr() { return m_lCNeg; }

	//==============================================================
	// SetTransparency()
	//==============================================================
	/// Enables / disables transparency
	/**
		\param [in] b	-	Non-zero to enable transparency, zero to
							disable.
	*/
	void SetTransparency( BOOL b ) { m_bTransparent = b; }

	//==============================================================
	// GetTransparency()
	//==============================================================
	/// Returns non-zero if transparency is currently enabled
	BOOL GetTransparency() { return m_bTransparent; }

	//==============================================================
	// SetTransparentColor()
	//==============================================================
	/// Sets the transparent color
	/**
		\param [in] rgb		-	Transparent color

		This color is not drawn during transparent Draw() operations.
		
		Transparency must be enabled using SetTransparency( true ).
		
		If chroma error values are set, then colors in the range of
		( rgb - cneg ) to ( rgb + cpos ) are not drawn.  Use
		SetChromaErr() to set chroma error values.
		
		\return 
	
		\see 
	*/
	void SetTransparentColor( COLORREF rgb ) 
	{	if ( m_rgbTransparent != rgb ) ReleaseMask(); m_rgbTransparent = rgb; }

	//==============================================================
	// GetTransparentColor()
	//==============================================================
	/// Returns the current transparent color
	COLORREF GetTransparentColor() { return m_rgbTransparent; }
	
	//==============================================================
	// GetBits()
	//==============================================================
	/// Returns a pointer to the image buffer.
	/**
		\note	The image must have been created with 
				CreateDIBSection() for this function to return a
				valid pointer.

		\return Returns a pointer to the image buffer or NULL if the
				Image was not created with CreateDIBSection()				
		
	*/
	LPBYTE GetBits() { return m_pBits; }

	//==============================================================
	// SetAntiAlias()
	//==============================================================
	/// Enables / disables anti-aliasing drawing
	/**
		\param [in] b	-	Non-zero to enable anti-aliasing

		If enabled, the drawing engine will anti-alias 
		<b>transparent</b> edges.
	*/
	void SetAntiAlias( BOOL b ) { m_bAntiAlias = b; }

#ifdef ENABLE_OPENCV

	//==============================================================
	// InitIplHeader()
	//==============================================================
	/// Initializes the Intel IplImage structure
	/**
		\param [in] pImg	-	Pointer to CWinDc object containing image
		\param [out] pIpl	-	IplImage structure to initialize
		
		\return Returns the pointer in pIpl
	
		\see 
	*/
	static IplImage* InitIplHeader( CWinDc *pImg, IplImage *pIpl );

	//==============================================================
	// InitIplHeader()
	//==============================================================
	/// Initializes the Intel IplImage structure
	/**
		\param [out] pIpl	-	IplImage structure to initialize
		
		\return Returns the pointer in pIpl
	
		\see 
	*/
	IplImage* InitIplHeader( IplImage *pIpl )
	{	return InitIplHeader( this, pIpl ); }

	//==============================================================
	// CWinDc::InitIplHeader()
	//==============================================================
	/// Initializes the Intel IplImage structure
	/**
		\param [out] pIpl	-	IplImage structure to initialize
		\param [in] pBuf	-	Pointer to image data
		\param [in] dwMax	-	Number of bytes in pBuf
		\param [in] lWidth	-	Image width
		\param [in] lHeight	-	Image height

		This function currently assumes the image to be 24 bbp.
		
		\return Returns the value in pIpl.
	
		\see 
	*/
	IplImage* InitIplHeader(IplImage *pIpl, LPBYTE pBuf, DWORD dwMax, long lWidth, long lHeight);

#endif

private:

	/// Transparent color
	COLORREF		m_rgbTransparent;

	/// Positive chromatic error in transparent color
	LONG			m_lCPos;

	/// Negative chromatic error in transparent color
	LONG			m_lCNeg;

	/// Non-zero to enable transparent edge anti-aliasing
	BOOL			m_bAntiAlias;
	
	/// Handle of encapsulated DC
	HDC				m_hDC;

	/// Handle of bitmap that is selected into m_hDC
	HBITMAP			m_hBmp;

	/// Original bitmap in m_hDC
	HBITMAP			m_hOldBmp;

	/// Size of the DC in m_hDC
	RECT			m_rect;
	
	/// Set to non-zero to <b>not</b> destroy m_hDC on CWinDc object destruction
	BOOL			m_bAutoDetach;

	/// Non-zero if transparency is enabled
	BOOL			m_bTransparent;

	/// Pointer to image used as drawing mask
	CWinDc			*m_mask;

	/// Creates unique index indicating motion frame
	static DWORD	m_dwMotionFrame;

	/// Pointer to the image buffer returned during CreateDIBSection()
	LPBYTE			m_pBits;

};

#endif // !defined(AFX_WINDC_H__4F8FB0B2_A5C5_11D4_8214_0050DA205C15__INCLUDED_)
