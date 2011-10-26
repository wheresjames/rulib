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
// WinImg.h: interface for the CWinImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINIMG_H__0B8AFCBD_9798_4FD4_952B_1C0373D75FCA__INCLUDED_)
#define AFX_WINIMG_H__0B8AFCBD_9798_4FD4_952B_1C0373D75FCA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//==================================================================
// CWinImg
//
/// Provides image manipulation, JPEG, PNG, BMP, WMF, EMF, etc...
/**
	Provides image encoding/decoding for JPEG, PNG, BMP, WMF, EMF,
	etc...  This functionality is primarily achieved by encapsulating
	the CxImage implementation.  You will need to download the
	CxImage library from http://www.xdp.it/cximage.htm.  Many thanks
	to Davide Pizzolato for providing this library free of charge.
	If you get the chance, please stop in and give Davide a million
	dollars.
*/
//==================================================================
class CWinImg  
{
public:

	//==============================================================
	// OpenUserImage()
	//==============================================================
	/// Opens a user specified image
	/**
		\param [in] hOwner			-	Handle to parent window
		\param [in] pTitle			-	Title of the dialog box
		\param [in] pDefaultExt 	-	Default image extension

		\return Non-zero if image opened.
	
		\see 
	*/
	BOOL OpenUserImage( HWND hOwner = NULL, LPCTSTR pTitle = "Open Image", LPCTSTR pDefaultExt = "jpg" );

	//==============================================================
	// SaveUserImage()
	//==============================================================
	/// Save image to user specified file
	/**
		\param [in] hOwner			-	Handle to parent window
		\param [in] pFile			-	Initial file name
		\param [in] pTitle			-	Title of the dialog box
		\param [in] pDefaultExt 	-	Default image extension

		\return Non-zero if image saved
	
		\see 
	*/
	BOOL SaveUserImage( HWND hOwner = NULL, LPCTSTR pFile = NULL, LPCTSTR pTitle = "Open Image", LPCTSTR pDefaultExt = "jpg" );

	//==============================================================
	// GetUserImage()
	//==============================================================
	/// Opens a user image
	/**
		\param [in] pFile			-	Receives the user filename.
		\param [in] hOwner			-	Handle to parent window
		\param [in] bOpen			-	Non-zero for open dialog, zero
										to display the save dialog.
		\param [in] pTitle			-	Title of the dialog box
		\param [in] pDefaultExt 	-	Default image extension

		\return Number of bytes written to pBuf
	
		\see 
	*/
	static BOOL GetUserImage( LPSTR pFile, HWND hOwner = NULL, BOOL bOpen = TRUE, LPCTSTR pTitle = "Open Image", LPCTSTR pDefaultExt = "jpg" );

	//==============================================================
	// GetImageSize()
	//==============================================================
	/// Returns the size, in bytes, of the raw image buffer 
	DWORD GetImageSize();

	//==============================================================
	// Encode()
	//==============================================================
	/// Encodes the specified file to a buffer
	/**
		\param [in] pBuf	-	Pointer to buffer that receives the 
								encoded image
		\param [in] dwSize 	-	Size of the buffer in pBuf
		\param [in] pType	-	File extension specifying type of
								encoding.  "jpg", "png", etc...

		pBuf must be large enough to hold the encoded image.
	  
		\return Number of bytes written to pBuf
	
		\see 
	*/
	DWORD Encode( LPBYTE pBuf, DWORD dwSize, LPCTSTR pType );
	
	//==============================================================
	// Tile()
	//==============================================================
	/// Tiles the image into the specified device context
	/**
		\param [in] hDC		-	Handle to device context
		\param [in] pRect	-	Portion of hDC to tile
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Tile( HDC hDC, LPRECT pRect );

	//==============================================================
	// AlphaDelete()
	//==============================================================
	/// Deletes the alpha layer
	BOOL AlphaDelete();

	//==============================================================
	// HasAlpha()
	//==============================================================
	/// Returns non-zero if the image has an alpha layer
	BOOL HasAlpha();

	//==============================================================
	// ToClipboard()
	//==============================================================
	/// Copies the image to the clipboard
	/**
		\param [in] hWnd	-	Window handle
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ToClipboard( HWND hWnd );

	//==============================================================
	// FromClipboard()
	//==============================================================
	/// Grabs an image from the clipboard
	/**
		\param [in] hWnd	-	Non-zero if success
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FromClipboard( HWND hWnd );

	//==============================================================
	// DecreaseBpp()
	//==============================================================
	/// Decreases the bits per pixel used to represent the image
	/**
		\param [in] bpp					-	Desired bits per pixel
		\param [in] bErrorDiffusion		-	Non-zero to enable error
											diffusion.
		\param [in] ppal				-	Optional palette to use.
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL DecreaseBpp( DWORD bpp, bool bErrorDiffusion, RGBQUAD* ppal = NULL );

	//==============================================================
	// IncreaseBpp()
	//==============================================================
	/// Increases the bits per pixel used to represent the image
	/**
		\param [in] bpp		-	Desired bits per pixel
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL IncreaseBpp( DWORD bpp );

	//==============================================================
	// GetNearestIndex()
	//==============================================================
	/// Gets the nearest value from the palette to the specified color
	/**
		\param [in] rgb		-	Color value
		
		\return	Returns the palette index of the nearest color
	
		\see 
	*/
	BYTE GetNearestIndex( RGBQUAD rgb );

	//==============================================================
	// SetTransIndex()
	//==============================================================
	/// Set the palette index of the transparent color
	/**
		\param [in] index	-	Palette index of transparent color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetTransIndex( long index );

	//==============================================================
	// GetTransIndex()
	//==============================================================
	/// Returns the palette index of the transparent color
	long GetTransIndex();

	//==============================================================
	// SetPixel()
	//==============================================================
	/// Sets the specified pixel color value
	/**
		\param [in] x		-	Horizontal offset of pixel
		\param [in] y		-	Vertical offset of pixel
		\param [in] rgb		-	New color value for pixel
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPixel( long x, long y, RGBQUAD rgb );

	//==============================================================
	// SetPixel()
	//==============================================================
	/// Sets the specified pixel color value
	/**
		\param [in] x		-	Horizontal offset of pixel
		\param [in] y		-	Vertical offset of pixel
		\param [in] rgb		-	New color value for pixel
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPixel( long x, long y, COLORREF rgb )
	{	return SetPixel( x, y, *(LPRGBQUAD)SwapRB( (LPBYTE)&rgb, 1 ) ); }	

	//==============================================================
	// GetPixel()
	//==============================================================
	/// Returns the color of the specified pixel
	/**
		\param [in] x		-	Horizontal offset of pixel
		\param [in] y		-	Vertical offset of pixel
		
		\return Color value of pixel
	
		\see 
	*/
	RGBQUAD GetPixel( long x, long y );

	//==============================================================
	// GetRgbPixel()
	//==============================================================
	/// Returns the color of the specified pixel
	/**
		\param [in] x		-	Horizontal offset of pixel
		\param [in] y		-	Vertical offset of pixel
		
		\return Color value of pixel
	
		\see 
	*/
	COLORREF GetRgbPixel( long x, long y )
	{	RGBQUAD rgbq = GetPixel( x, y );
		return *(LPDWORD)SwapRB( (LPBYTE)&rgbq, 1 );
	}
	
	//==============================================================
	// GetBpp()
	//==============================================================
	/// Returns the number of bits per pixel used to represent the image
	DWORD GetBpp();

	//==============================================================
	// GetTransColor()
	//==============================================================
	/// Returns the RGB value of the current transparent color
	RGBQUAD GetTransColor();

	//==============================================================
	// SetTransColor()
	//==============================================================
	/// Sets the transparent color
	/**
		\param [in] rgb	-	Transparent color value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetTransColor( RGBQUAD rgb );

	//==============================================================
	// IsTransparent()
	//==============================================================
	/// Returns non-zero if transparency is enabled
	BOOL IsTransparent();

	//==============================================================
	// Resample()
	//==============================================================
	/// Resamples the image to the new size
	/**
		\param [in] pRect	-	New size for image
		\param [in] fast	-	Resampling algorithm to use ( 0, 1 )
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Resample( LPRECT pRect, int fast = 1 );

	//==============================================================
	// Copy()
	//==============================================================
	/// Copies the specified image
	/**
		\param [in] pImg	-	Source image to copy
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( CWinImg *pImg );

	//==============================================================
	// operator =()
	//==============================================================
	/// Copies the specified image
	/**
		\param [in] img	-	Source image to copy
		
		\return Non-zero if success
	
		\see 
	*/
    CWinImg& operator = ( const CWinImg &img )
    {   Copy( (CWinImg*)&img ); return *this; }

	//==============================================================
	// Create()
	//==============================================================
	/// Creates an image of the specified size and color depth
	/**
		\param [in] w	-	Width of new image
		\param [in] h	-	Height of new image
		\param [in] bpp	-	Color depth of new image in bits per pixel
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( long w, long h, long bpp = 24 );

	//==============================================================
	// Create()
	//==============================================================
	/// Creates an image of the specified size
	/**
		\param [in] w	-	Width of new image
		\param [in] h	-	Height of new image
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create24bpp( long w, long h )
    {   return Create( w, h, 24 ); }

	//==============================================================
	// GetEffWidth()
	//==============================================================
	/// Returns the scan width of a row of image data
	DWORD GetEffWidth();

	//==============================================================
	// GetBits()
	//==============================================================
	/// Returns a pointer to the raw image buffer
	LPBYTE GetBits();

	//==============================================================
	// LoadFromResource()
	//==============================================================
	/// Loads an image from a resource
	/**
		\param [in] pResource	-	Resource name
		\param [in] pResType	-	Resource type
		\param [in] pImgType	-	Image type, "jpg", "png", etc...
		\param [in] hModule		-	Handle to module containing the
									image resource
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL LoadFromResource( LPCTSTR pResource, LPCTSTR pResType, LPCTSTR pImgType, HMODULE hModule = NULL );

	//==============================================================
	// Histogram()
	//==============================================================
	/// Generates a historgram for the current image
	/**
		\param [out] red			-	Receives the red histogram
		\param [out] green			-	Receives the green histogram
		\param [out] blue			-	Receives the blue histogram
		\param [out] gray			-	Receives the gray scale histogram
		\param [in] colorspace		-	Colorspace 
		
		\return Length of histogram
	
		\see 
	*/
	long Histogram( long *red, long *green = 0, long *blue = 0, long *gray = 0, long colorspace = 0  );

	//==============================================================
	// Dilate()
	//==============================================================
	/// Dialates the image
	/**
		\param [in] Ksize	-	Dialation weight
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Dilate( long Ksize = 2 );

	//==============================================================
	// Erode()
	//==============================================================
	/// Erodes the image
	/**
		\param [in] Ksize	-	Erosion weight
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Erode( long Ksize = 2 );

	//==============================================================
	// Light()
	//==============================================================
	/// Lightens the image
	/**
		\param [in] level		-	Lighten level
		\param [in] contrast	-	Contrast
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Light( long level, long contrast = 100 );

	//==============================================================
	// Colorize()
	//==============================================================
	/// Colorizes the image
	/**
		\param [in] hue		-	Hue
		\param [in] sat		-	Saturation
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Colorize( BYTE hue, BYTE sat );

	//==============================================================
	// Threshold()
	//==============================================================
	/// Thresholds the image
	/**
		\param [in] level	-	Threshold level

		Converts the image to a monochrome image, converting all values
		above the threshold value to white.  All values below the 
		threshold are converted to black.
	  
		\return Non-zero if success
	
		\see 
	*/
	BOOL Threshold( BYTE level );

	//==============================================================
	// ShiftRGB()
	//==============================================================
	/// Changes the RGB component levels
	/**
		\param [in] r	-	Red component level
		\param [in] g	-	Green component level
		\param [in] b	-	Blue component level
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ShiftRGB( long r, long g, long b );

	//==============================================================
	// Gamma()
	//==============================================================
	/// Gamma correction effect
	/**
		\param [in] gamma	-	Gamma correction value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Gamma( float gamma );

	//==============================================================
	// Median()
	//==============================================================
	/// Median image effect
	/**
		\param [in] Ksize	-	Median image weight
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Median( long Ksize = 3 );

	//==============================================================
	// Noise()
	//==============================================================
	/// Adds noise to the image
	/**
		\param [in] level	-	Noise level
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Noise( long level );

	//==============================================================
	// Crop()
	//==============================================================
	/// Crops an area of the image
	/**
		\param [in] left	-	Horizontal offset of upper left corner
		\param [in] top		-	Vertical offset of upper left corner
		\param [in] right	-	Horizontal offset of lower right corner
		\param [in] bottom	-	Vertical offset of lower right corner
		\param [in] pDst	-	CWinDc object that receives the cropped
								image.  NULL to replace current image.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Crop( long left, long top, long right, long bottom, CWinImg *pDst = NULL );

	//==============================================================
	// Dither()
	//==============================================================
	/// Dithers the image
	/**
		\param [in] method	-	Dither method ( 0, 1 )
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Dither( long method = 0 );

	//==============================================================
	// Resample()
	//==============================================================
	/// Resamples the image
	/**
		\param [in] newx	-	New horizontal image size
		\param [in] newy	-	New vertical image size
		\param [in] fast	-	Resampling algorithm to use ( 0, 1 )
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Resample( long newx, long newy, long fast = 1 );

	//==============================================================
	// Rotate()
	//==============================================================
	/// Rotates the image by the specified ammount
	/**
		\param [in] angle	-	Rotation angle in degrees
		\param [in] pDst	-	CWinDc object that receives rotated
								image.  NULL to replace current image.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Rotate( float angle, CWinImg *pDst = NULL );

	//==============================================================
	// Negative()
	//==============================================================
	/// Generates negative for image
	/**
		\return Non-zero if success
	*/
	BOOL Negative();

	//==============================================================
	// GrayScale()
	//==============================================================
	/// Converts image to gray scale
	/**
		\return Non-zero if success
	*/
	BOOL GrayScale();

	//==============================================================
	// Flip()
	//==============================================================
	/// Flips the current image vertically
	/**
		\return Non-zero if success
	*/
	BOOL Flip();

	//==============================================================
	// Mirror()
	//==============================================================
	/// Mirrors image horizontally
	/**
		\return Non-zero if success
	*/
	BOOL Mirror();

	//==============================================================
	// CreateIcon()
	//==============================================================
	/// Creates an icon from the image
	/**
		\param [in] w	-	New icon width
		\param [in] h	-	New icon height
		\param [in] bck	-	Transparent color used to generate mask.
		\param [in] hDC	-	Reference device context, NULL to use
							the desktop.
		
		\return Handle to new icon or NULL if failure
	
		\see 
	*/
	HICON CreateIcon( long w, long h, COLORREF bck = RGB( 255, 255, 255 ), HDC hDC = NULL );

	//==============================================================
	// SetQuality()
	//==============================================================
	/// Sets the quality level for the JPEG encoder
	/**
		\param [in] q	-	JPEG quality level
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetQuality( DWORD q );

	//==============================================================
	// CreateFromHBITMAP()
	//==============================================================
	/// Creates an image from an HBITMAP handle
	/**
		\param [in] hBmp	-	HBITMAP handle
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL CreateFromHBITMAP( HBITMAP hBmp );

	//==============================================================
	// IsValid()
	//==============================================================
	/// Returns non-zero if there is a valid image
	BOOL IsValid();

	//==============================================================
	// GetHeight()
	//==============================================================
	/// Returns the image height
	long GetHeight();

	//==============================================================
	// GetWidth()
	//==============================================================
	/// Returns the image width
	long GetWidth();

	//==============================================================
	// Draw()
	//==============================================================
	/// Draws the image to the specified device context
	/**
		\param [in] hDC			-	Handle to device context
		\param [in] pRect		-	Destination rectangle for image
		\param [in] bStretch	-	Non-zero to stretch to pRect
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Draw( HDC hDC, LPRECT pRect, BOOL bStretch = TRUE);

	//==============================================================
	// GetLastError()
	//==============================================================
	/// Returns a string pointer describing the last error
	LPCTSTR GetLastError();

	//==============================================================
	// Decode()
	//==============================================================
	/// Decodes an image from a memory buffer
	/**
		\param [in] buf		-	Pointer to buffer containing the 
								compressed image.
		\param [in] size	-	Number of bytes in buf.
		\param [in] pType	-	Image type, "jpg", "png", etc...
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Decode( LPBYTE buf, DWORD size, LPCTSTR pType = NULL );

	//==============================================================
	// ReleaseEncodeMemory()
	//==============================================================
	/// Releases temporary memory used by CxImage when encoding an image to memory
	void ReleaseEncodeMemory();

	// Raw JPEG encoder
	//==============================================================
	// InitEncodeJpg()
	//==============================================================
	/// 
	/**
		\param [in] lWidth		-	Image width
		\param [in] lHeight		-	Image height
		\param [in] lQuality	-	JPEG encoding quality
		
		This function is called automatically by EncodeJpg(), so
		it is not necessary to call this function directly.

		\return Non-zero if success
	
		\see EncodeJpg(), ReleaseEncodeJpg()
	*/
	BOOL InitEncodeJpg( long lWidth, long lHeight, long lQuality );

	//==============================================================
	// EncodeJpg()
	//==============================================================
	/// Encodes the specified image buffer as a JPEG in memory
	/**
		\param [in] pSrc		-	Uncompressed source image
		\param [in] lWidth		-	Width of image
		\param [in] lHeight		-	Height of image
		\param [in] pDst		-	Receives JPEG image
		\param [in] dwDst		-	Size of buffer in pDst
		\param [in] lQuality	-	JPEG image quality

		I couldn't figure out a way to use CxImage to encode a buffer
		in memory without a memcpy and didn't want to have to depend
		on a modified library.  So this function does not use CxImage
		to encode.  
  
		The image in pSrc must be 24-bit BGR encoded.  Scan lines must
		be DWORD aligned, but that's typical.

		The buffer in pDst must be large enough to hold the image or 
		the function fails returning zero.  So it is not possible to 
		figure out the required buffer size before calling.  I would 
		just make the buffer at least as large as the uncompressed 
		image.

		This function also reuses the same JPEG compression structures
		if subsequent calls are made with the same size image
		
		\return Number of bytes written to pDst, or zero if failure.
	
		\see InitEncodeJpg(), ReleaseEncodeJpg()
	*/
	DWORD EncodeJpg(LPBYTE pSrc, long lWidth, long lHeight, LPBYTE pDst, DWORD dwDst, long lQuality );


	//==============================================================
	// ReleaseEncodeJpg()
	//==============================================================
	/// Releases memory allocated by InitEncodeJpg()
	/**
		This function will be called when this CWinDc object is
		destroyed, so it is not necessary to call this function.
		
		\see InitEncodeJpg(), EncodeJpg()
	*/
	void ReleaseEncodeJpg();

	// Memory pointer returned by Encode() is
	// good until object is destroyed or this 
	// function is called again
	BOOL Encode( LPBYTE *buf, LPDWORD size, LPCTSTR pType = NULL );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves encoded image to disk
	/**
		\param [in] pFile	-	Filename
		\param [in] pType	-	Image type "jpg", "png", etc...
								If pType is NULL, the image type is
								determined by examining the file 
								extension in pFile.

		
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL Save( LPCTSTR pFile, LPCTSTR pType = NULL );

	//==============================================================
	// GetFileType()
	//==============================================================
	/// Examines the file extension and returns the image type code
	/**
		\param [in] pFile	-	Filename to examine
		
		\return Image type code
	
		\see 
	*/
	static DWORD GetFileType( LPCTSTR pFile );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads the specified file from disk
	/**
		\param [in] pFile	-	Filename
		\param [in] pType	-	Image type "jpg", "png", etc...
								If pType is NULL, the image type is
								determined by examining the file 
								extension in pFile.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Load( LPCTSTR pFile, LPCTSTR pType = NULL );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the image and all associated resources
	void Destroy();

	/// Default constructor
	CWinImg();

    /// Copy constructor
    CWinImg( const CWinImg &img );

	/// Destructor
	virtual ~CWinImg();
	
	//==============================================================
	// GetFileName()
	//==============================================================
	/// Returns the current image filename if available
	LPCTSTR GetFileName() 
	{	if ( m_filename.ptr() != NULL ) return m_filename.ptr();
		return ""; 
	}

	//==============================================================
	// SetTransparency()
	//==============================================================
	/// Enables / disables transparency
	/**
		\param [in] b	-	Non-zero to enable transparency.  Zero
							disables transparency.
	*/
	void SetTransparency( BOOL b ) { m_bTransparent = b; }

	//==============================================================
	// GetTransparency()
	//==============================================================
	/// Returns non-zero if transparency is enabled
	BOOL GetTransparency() { return m_bTransparent; }

	//==============================================================
	// SetTransparentColor()
	//==============================================================
	/// Sets the transparent color
	/**
		\param [in] rgb		-	Transparent color
	*/
	void SetTransparentColor( COLORREF rgb ) { m_rgbTransparent = rgb; }

	//==============================================================
	// GetTransparentColor()
	//==============================================================
	/// Returns the transparent color value
	COLORREF GetTransparentColor() { return m_rgbTransparent; }

	//==============================================================
	// LPRECT()
	//==============================================================
	/// Returns the size and position of the image
	operator LPRECT() { return GetRect(); }

	//==============================================================
	// GetRect()
	//==============================================================
	/// Returns the size and position of the image
	LPRECT GetRect()
	{	SetRect( &m_rect, 0, 0, GetWidth(), GetHeight() ); return &m_rect; }
	
	//==============================================================
	// GetEncBuffer()
	//==============================================================
	/// Returns a pointer to the internal encoded image buffer
	/**
		This buffer is only valid after a call to Encode()

		\return Pointer to the internal encoded image buffer
	
		\see 
	*/
	LPBYTE GetEncBuffer() { return m_pMem; }

	//==============================================================
	// GetEncBufferSize()
	//==============================================================
	/// Returns the size of the internal encoded image buffer
	/**
		This buffer is only valid after a call to Encode()

		\return Size of the internal encoded image buffer
	
		\see 
	*/
	long GetEncBufferSize() { return m_lMem; }


	//==============================================================
	// CreateHBITMAP()
	//==============================================================
	/// Creates HBITMAP handle from the current image
	/**
		You must pass the handle to DeleteObject() when it is no 
		longer needed.

		\return Handle to bitmap or NULL if error.
	
		\see 
	*/
	HBITMAP CreateHBITMAP( HDC hDC = NULL );

	//==============================================================
	// operator HBITMAP()
	//==============================================================
	/// Creates HBITMAP handle from the current image
	/**
		You must pass the handle to DeleteObject() when it is no 
		longer needed.

		\return Handle to bitmap or NULL if error.
	
		\see 
	*/
	operator HBITMAP() { return CreateHBITMAP(); }

	//==============================================================
	// RGBtoBGR()
	//==============================================================
	/// Swaps the specified buffer values from RGB to BGR or vice versa
	/**
		\param [in] pBuf	-	Input color values
		\param [in] uLen	-	Length of color values

		\return The value in pBuf
	*/
	LPBYTE SwapRB( LPBYTE pBuf, UINT uLen )
	{	for ( UINT i = 0; i < uLen; i += 3 )
			SWAP_INT( pBuf[ i ], pBuf[ i + 2 ] );
		return pBuf;
	}

#ifdef ENABLE_OPENCV

	//==============================================================
	// InitIplHeader()
	//==============================================================
	/// Initializes the Intel IplImage structure from image
	/**
		\param [in] pImg	-	Pointer to CWinImg object
		\param [out] pIpl	-	Pointer to IplImage structure
		
		Note, this function does not copy the image!  It only fills
		in pIpl with appropriate values.  When the CWinImg object is
		destroyed, the IplImage structure is no longer valid.

		\return The pointer in pIpl
	
		\see 
	*/
	static IplImage* InitIplHeader( CWinImg *pImg, IplImage *pIpl );
	
	//==============================================================
	// InitIplHeader()
	//==============================================================
	/// Initializes the Intel IplImage structure from image
	/**
		\param [out] pIpl	-	Pointer to IplImage structure
		
		Note, this function does not copy the image!  It only fills
		in pIpl with appropriate values.  When the CWinImg object is
		destroyed, the IplImage structure is no longer valid.

		\return The pointer in pIpl
	
		\see 
	*/
	IplImage* InitIplHeader( IplImage *pIpl )
	{	return InitIplHeader( this, pIpl ); }

#endif

	//==============================================================
	// Aluminize()
	//==============================================================
	/// Creates an Aluminum texture.
	/**
		\param [in] var		-	Random variance
		\param [in] run		-	Over-run
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Aluminize( DWORD var = 5, DWORD run = 40 );

	//==============================================================
	// Texturize()
	//==============================================================
	/// Creates a random texture
	/**
		\param [in] var		-	Random variance
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Texturize( DWORD var );

	//==============================================================
	// FillSolid()
	//==============================================================
	/// Fills the image with a solid color
	/**
		\param [in] rgb		-	Color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillSolid( COLORREF rgb );

private:

	/// Non-zero if transparency is enabled
	BOOL			m_bTransparent;

	/// Transparent color
	COLORREF		m_rgbTransparent;

private:

	/// Pointer to CxImage object
	void			*m_pimg;

	/// Pointer to JPEG encoder state structure
	void			*m_pEncoderState;

	/// Pointer to encoder memory created by CxImage
	LPBYTE			m_pMem;

	/// Size of the buffer in m_pMem
	long			m_lMem;

	/// The filename of the current image if available
	CStr			m_filename;

	/// Size and position of the current image
	RECT			m_rect;

};

#endif // !defined(AFX_WINIMG_H__0B8AFCBD_9798_4FD4_952B_1C0373D75FCA__INCLUDED_)
