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
// GrDC.h: interface for the CGrDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRDC_H__49A61F76_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
#define AFX_GRDC_H__49A61F76_AC0F_11D4_821A_0050DA205C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrPen.h"
#include "GrBrush.h"

//==================================================================
// CGrDC
//
/// Simple wrapper for HDC.  Primarily provides static GDI functions
/**
	This class primarily provides basic GDI drawing functionality
	through its static member functions.	
*/
//==================================================================
class CGrDC  
{
public:

	//==============================================================
	// DottedBox()
	//==============================================================
	/// Draws a dotted box
	/**
		\param [in] hDC			-	DC handle for drawing
		\param [in] pRect		-	Position of the dotted box
		\param [in] rgbPen		-	Pen color
		\param [in] rgbBck		-	Background color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DottedBox( HDC hDC, LPRECT pRect, COLORREF rgbPen, COLORREF rgbBck );

	//==============================================================
	// DottedBox()
	//==============================================================
	/// Draws a dotted box
	/**
		\param [in] pRect		-	Position of the dotted box
		\param [in] rgbPen		-	Pen color
		\param [in] rgbBck		-	Background color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DottedBox( LPRECT pRect, COLORREF rgbPen, COLORREF rgbBck )
	{	return DottedBox( m_hDC, pRect, rgbPen, rgbBck ); }

	//==============================================================
	// FillHatchedPolygon()
	//==============================================================
	/// Fills a polygon with a hatch
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pt		-	Pointer to point array
		\param [in] pts		-	Number of points in pt
		\param [in] bck		-	Background color
		\param [in] lines	-	Hatch line colors
		\param [in] hatch	-	Hatch style
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL FillHatchedPolygon( HDC hDC, LPPOINT pt, DWORD pts, COLORREF bck, COLORREF lines, int hatch );

	//==============================================================
	// FillHatchedPolygon()
	//==============================================================
	/// Fills a polygon with a hatch
	/**
		\param [in] pt		-	Pointer to point array
		\param [in] pts		-	Number of points in pt
		\param [in] bck		-	Background color
		\param [in] lines	-	Hatch line colors
		\param [in] hatch	-	Hatch style
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillHatchedPolygon( LPPOINT pt, DWORD pts, COLORREF bck, COLORREF lines, int hatch )
	{	return FillHatchedPolygon( m_hDC, pt, pts, bck, lines, hatch ); }

	//==============================================================
	// DrawButton()
	//==============================================================
	/// Draws an irregular button shape
	/**
		\param [in] hDC			-	DC handle for drawing
		\param [in] bPressed	-	Pressed state
		\param [in] pt			-	Array of POINT structures
		\param [in] pts			-	Number of points in pt
		\param [in] color		-	Button color
		\param [in] hIcon		-	Button icon
		
		The first half of the points should be the top of the button,
		or the highlighted portion.  The second half the points will
		be the bottom of the button, or the portion of the buttons
		edges in shadow.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawButton( HDC hDC, BOOL bPressed, LPPOINT pt, DWORD pts, COLORREF color, HICON hIcon );

	//==============================================================
	// DrawButton()
	//==============================================================
	/// Draws an irregular button shape
	/**
		\param [in] bPressed	-	Pressed state
		\param [in] pt			-	Array of POINT structures
		\param [in] pts			-	Number of points in pt
		\param [in] color		-	Button color
		\param [in] hIcon		-	Button icon
		
		The first half of the points should be the top of the button,
		or the highlighted portion.  The second half the points will
		be the bottom of the button, or the portion of the buttons
		edges in shadow.

		\return Non-zero if success
	
		\see 
	*/
	BOOL DrawButton( BOOL bPressed, LPPOINT pt, DWORD pts, COLORREF color, HICON hIcon )
	{	return DrawButton( m_hDC, bPressed, pt, pts, color, hIcon ); }

	//==============================================================
	// FillSolidPolygon()
	//==============================================================
	/// Fills a solid polygon
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pt		-	Array of POINT structures
		\param [in] pts		-	Number of structures in pt
		\param [in] rgb		-	Fill color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL FillSolidPolygon( HDC hDC, LPPOINT pt, DWORD pts,  COLORREF rgb );

	//==============================================================
	// FillSolidPolygon()
	//==============================================================
	/// Fills a solid polygon
	/**
		\param [in] pt		-	Array of POINT structures
		\param [in] pts		-	Number of structures in pt
		\param [in] rgb		-	Fill color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillSolidPolygon( LPPOINT pt, DWORD pts, COLORREF rgb )
	{	return FillSolidPolygon( m_hDC, pt, pts, rgb ); }
	
	//==============================================================
	// Box3d()
	//==============================================================
	/// Draws a 3D box
	/**
		\param [in] hDC			-	DC handle for drawing
		\param [in] pRect		-	Size and position of the box
		\param [in] size		-	Thickness of the box edges
		\param [in] lt			-	Highlighted color of box
		\param [in] rb			-	Shadowed color of box
		\param [in] bInvert		-	Inverts the highlighted and shadowed colors.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Box3d( HDC hDC, LPRECT pRect, DWORD size, COLORREF lt, COLORREF rb, BOOL bInvert = FALSE );

	//==============================================================
	// Box3d()
	//==============================================================
	/// Draws a 3D box
	/**
		\param [in] pRect		-	Size and position of the box
		\param [in] size		-	Thickness of the box edges
		\param [in] lt			-	Highlighted color of box
		\param [in] rb			-	Shadowed color of box
		\param [in] bInvert		-	Inverts the highlighted and shadowed colors.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Box3d( LPRECT pRect, DWORD size, COLORREF lt, COLORREF rb, BOOL bInvert = FALSE )
	{	return Box3d( m_hDC, pRect, size, lt, rb, bInvert ); }
	
	//==============================================================
	// RoundedBox3d()
	//==============================================================
	/// Draws a 3D rectangle with rounded edges.
	/**
		\param [in] hDC			-	DC handle for drawing
		\param [in] pRect		-	Size and position of rectangle
		\param [in] size		-	Thickness of the rectangle edges.
		\param [in] lt			-	Hightlighted color of box
		\param [in] rb			-	Shadowed color of box
		\param [in] bInvert		-	Inverts the highlighted and shadowed colors.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL RoundedBox3d( HDC hDC, LPRECT pRect, DWORD size, COLORREF lt, COLORREF rb, BOOL bInvert = FALSE );

	//==============================================================
	// RoundedBox3d()
	//==============================================================
	/// Draws a 3D rectangle with rounded edges.
	/**
		\param [in] pRect		-	Size and position of rectangle
		\param [in] size		-	Thickness of the rectangle edges.
		\param [in] lt			-	Hightlighted color of box
		\param [in] rb			-	Shadowed color of box
		\param [in] bInvert		-	Inverts the highlighted and shadowed colors.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RoundedBox3d( LPRECT pRect, DWORD size, COLORREF lt, COLORREF rb, BOOL bInvert = FALSE )
	{	return RoundedBox3d( m_hDC, pRect, size, lt, rb, bInvert ); }
	
	//==============================================================
	// VertGradientFill()
	//==============================================================
	/// Draws a vertical gradient.
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pRect	-	Size and position of the box
		\param [in] top		-	Color at the top of the gradient	
		\param [in] bottom	-	Color at the bottom of the gradient

		This function colors a rectangular area, smoothly transitioning
		from the top color to the bottom color.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL VertGradientFill( HDC hDC, LPRECT pRect, COLORREF top, COLORREF bottom );

	//==============================================================
	// VertGradientFill()
	//==============================================================
	/// Draws a vertical gradient.
	/**
		\param [in] pRect	-	Size and position of the box
		\param [in] top		-	Color at the top of the gradient	
		\param [in] bottom	-	Color at the bottom of the gradient

		This function colors a rectangular area, smoothly transitioning
		vertically from the top color to the bottom color.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL VertGradientFill( LPRECT pRect, COLORREF top, COLORREF bottom )
	{	return VertGradientFill( m_hDC, pRect, top, bottom ); }

	//==============================================================
	// HorzGradientFill()
	//==============================================================
	/// Draws a horizontal gradient.
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pRect	-	Size and position of the box
		\param [in] left	-	Color at the left of the gradient	
		\param [in] right	-	Color at the right of the gradient

		This function colors a rectangular area, smoothly transitioning
		horizontally from the left color to the right color.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL HorzGradientFill( HDC hDC, LPRECT pRect, COLORREF left, COLORREF right );

	//==============================================================
	// HorzGradientFill()
	//==============================================================
	/// Draws a horizontal gradient.
	/**
		\param [in] pRect	-	Size and position of the box
		\param [in] left	-	Color at the left of the gradient	
		\param [in] right	-	Color at the right of the gradient

		This function colors a rectangular area, smoothly transitioning
		horizontally from the left color to the right color.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL HorzGradientFill( LPRECT pRect, COLORREF left, COLORREF right )
	{	return HorzGradientFill( m_hDC, pRect, left, right ); }

	//==============================================================
	// GradientFill()
	//==============================================================
	/// Draws a gradient that transitions in four directions
	/**
		\param [in] hDC				-	DC handle for drawing
		\param [in] pRect			-	Position and size of the gradient
		\param [in] lefttop			-	Color at the top-left of gradient
		\param [in] righttop		-	Color at the top-right of the gradient
		\param [in] leftbottom		-	Color at the bottom-left of the gradient
		\param [in] rightbottom		-	Color at the bottom-right of the gradient
		
		This function colors a rectangular area, smoothly transitioning
		horizontally from the left color to the right color and vertically
		from the top color to the bottom color.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL GradientFill( HDC hDC, LPRECT pRect, COLORREF lefttop, COLORREF righttop, COLORREF leftbottom, COLORREF rightbottom );

	//==============================================================
	// GradientFill()
	//==============================================================
	/// Draws a gradient that transitions in four directions
	/**
		\param [in] pRect			-	Position and size of the gradient
		\param [in] lefttop			-	Color at the top-left of gradient
		\param [in] righttop		-	Color at the top-right of the gradient
		\param [in] leftbottom		-	Color at the bottom-left of the gradient
		\param [in] rightbottom		-	Color at the bottom-right of the gradient
		
		This function colors a rectangular area, smoothly transitioning
		horizontally from the left color to the right color and vertically
		from the top color to the bottom color.

		\return Non-zero if success
	
		\see 
	*/
	BOOL GradientFill( LPRECT pRect, COLORREF lefttop, COLORREF righttop, COLORREF leftbottom, COLORREF rightbottom )
	{	return GradientFill( m_hDC, pRect, lefttop, righttop, leftbottom, rightbottom ); }
	
	//==============================================================
	// FillSolidRect()
	//==============================================================
	/// Fills a rectangle with a solid color
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pRect	-	Position and size of area to fill
		\param [in] rgb		-	Color to fill area
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL FillSolidRect( HDC hDC, LPRECT pRect, COLORREF rgb );

	//==============================================================
	// FillSolidRect()
	//==============================================================
	/// Fills a rectangle with a solid color
	/**
		\param [in] pRect	-	Position and size of area to fill
		\param [in] rgb		-	Color to fill area
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillSolidRect( LPRECT pRect, COLORREF rgb )
	{	return FillSolidRect( m_hDC, pRect, rgb ); }

	//==============================================================
	// FillSolidEllipse()
	//==============================================================
	/// Fills an ellipse with a solid color
	/**
		\param [in] hDC		-	DC handle for drawing
		\param [in] pRect	-	Position and size of ellipse area
		\param [in] pen		-	Color of border
		\param [in] bck		-	Fill color
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL FillSolidEllipse(HDC hDC, LPRECT pRect, COLORREF pen, COLORREF bck);

	//==============================================================
	// FillSolidEllipse()
	//==============================================================
	/// Fills an ellipse with a solid color
	/**
		\param [in] pRect	-	Position and size of ellipse area
		\param [in] pen		-	Color of border
		\param [in] bck		-	Fill color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillSolidEllipse( LPRECT pRect, COLORREF pen, COLORREF bck )
	{	return FillSolidEllipse( m_hDC, pRect, pen, bck ); }

	//==============================================================
	// CGrDC::CreateIcon()
	//==============================================================
	/// Creates an icon with the specified data
	/**
		\param [in] w				-	Width of the new icon
		\param [in] h				-	Height of the new icon
		\param [in] lSrcW			-	Width of the source images
		\param [in] lSrcH			-	Height of the source images
		\param [in] bck				-	Background color
		\param [in] hDC				-	Handle to DC containing image
		\param [in] hIconBmp		-	HBITMAP handle to icon image
		\param [in] hIconMask		-	HBITMAP handle to icon mask
		
		\return HICON handle to icon
	
		\see 
	*/
	static HICON CreateIcon(long w, long h, long lSrcW, long lSrcH, COLORREF bck, HDC hDC, HBITMAP hIconBmp, HBITMAP hIconMask);
	
	//==============================================================
	// PtInPolygon()
	//==============================================================
	/// Checks to see if the specified point is within the polygon
	/**
		\param [in] pt		-	Point to check
		\param [in] ppt		-	Array of POINT structures defining the polygon
		\param [in] pts		-	Number of structures in ppt
		
		\return Non-zero if the point is in the polygon.
	
		\see 
	*/
	static BOOL PtInPolygon( LPPOINT pt, LPPOINT ppt, DWORD pts );

	//==============================================================
	// GetAngle()
	//==============================================================
	/// Returns the angle between the specified points
	/**
		\param [in] x1	-	Horizontal offset of first point
		\param [in] y1	-	Vertical offset of first point
		\param [in] x2	-	Horizontal offset of second point
		\param [in] y2	-	Vertical offset of second point
		
		\return Angle between the points
	
		\see 
	*/
	static double GetAngle( long x1, long y1, long x2, long y2 );

	//==============================================================
	// GetAngle()
	//==============================================================
	/// Returns the angle between the specified points
	/**
		\param [in] x1	-	Horizontal offset of first point
		\param [in] y1	-	Vertical offset of first point
		\param [in] x2	-	Horizontal offset of second point
		\param [in] y2	-	Vertical offset of second point
		
		\return Angle between the points
	
		\see 
	*/
	static double GetAngle( double x1, double y1, double x2, double y2 );

	//==============================================================
	// InflatePolygon()
	//==============================================================
	/// Scales the size of the polygon by the specified ammount
	/**	
		\param [in] pt		-	Array of POINT structures defining the polygon
		\param [in] pts		-	Number of structures in pt
		\param [in] mag		-	Scaling factor.  
								-	1  = No scaling
								-	.5 = half size
								-	2  = double size
								-	etc...		
		\return 
	
		\see 
	*/
	static BOOL InflatePolygon( LPPOINT pt, DWORD pts, double mag );
	
	//==============================================================
	// GetPolygonRect()
	//==============================================================
	/// Returns the smallest rectangle enclosing all specified polygon
	/**
		\param [in] pRect	-	Receives the enclosing rectangle
		\param [in] pt		-	Array of POINT structures defining the polygon
		\param [in] pts		-	Number of structures in pt
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetPolygonRect( LPRECT pRect, LPPOINT pt, DWORD pts );

	//==============================================================
	// InflatePolygon()
	//==============================================================
	/// Linearly inflates the size of a polygon
	/**
		\param [in] pt	-	Array of POINT structures defining the polygon
		\param [in] pts	-	Number of structues in pt
		\param [in] x	-	Amount to add to x value of polygon points
		\param [in] y	-	Amount to add to y value of polygon points

		This function calculates the center point of the specified polygon.
		Subtracts x from points to the left of the center point.
		Adds x to points to the right of the center point.
		Subtracts y from points to the top of the center point.
		Adds y to points to the bottom of the center point.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL InflatePolygon( LPPOINT pt, DWORD pts, long x, long y );

	//==============================================================
	// AspectCorrect()
	//==============================================================
	/// Corrects the aspect ratio of a rectangle using the specified sizes
	/**
		\param [in] r1	-	Rectangle whose aspect ratio is corrected
		\param [in] w	-	Reference width
		\param [in] h	-	Reference height
		
		The width of r1 is scaled so the aspect ratio matches w x h

		\return Returns the pointer in r1
	
		\see 
	*/
	static LPRECT AspectCorrect( LPRECT r1, long w, long h );

	//==============================================================
	// AspectCorrect()
	//==============================================================
	/// Corrects the aspect ratio of a rectangle using the specified sizes
	/**
		\param [in] r1	-	Rectangle whose aspect ratio is corrected
		\param [in] r2	-	Reference aspect ratio
		
		The width of r1 is scaled so the aspect ratio matches w x h of r2

		\return The pointer in r1
	
		\see 
	*/
	static LPRECT AspectCorrect( LPRECT r1, LPRECT r2 );

	//==============================================================
	// Center()
	//==============================================================
	/// Centers one rectangle within another
	/**
		\param [in] pRect	-	Rectangle to center
		\param [in] pRef	-	Reference rectangle
		\param [in] x		-	Non-zero to center horizontally
		\param [in] y		-	Non-zero to center vertically
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Center( LPRECT pRect, LPRECT pRef, BOOL x = TRUE, BOOL y = TRUE );	

	//==============================================================
	// ScaleColor()
	//==============================================================
	/// Scales between two colors
	/**
		\param [in] c1		-	First color
		\param [in] c2		-	Second color
		\param [in] scale	-	Blending weight of each color.
								Range is 0 to 1000

		This function should probably have been called Blend().  It
		blends two colors together using scale as a weight.

		If scale is 0, returns c1
		If scale is 1000 returns c2
		If scale is 500 returns a 50% blend of c1 and c2
		etc...
		
		\return 
	
		\see 
	*/
	static COLORREF ScaleColor( COLORREF c1, COLORREF c2, long scale );

	//==============================================================
	// ScaleColor()
	//==============================================================
	/// Creates a shade of the specified color
	/**
		\param [in] col		-	Color
		\param [in] scale	-	Shade weight.  Range is -255, 255.
		
		This function should have been called Shade().  It creates 
		a lighter or darker shade of a color depending on the weight.

		If scale is -100, returns a color 100 points darker than col
		If scale is 100, returns a color 100 points lighter than col
		If scale is 0, returns col

		This function always returns the same color in a different shade.

		\return 
	
		\see 
	*/
	static COLORREF ScaleColor( COLORREF col, long scale );

	//==============================================================
	// Shade()
	//==============================================================
	/// Shades the specified area of the video
	/**
		\param [in] pBuf			-	Pointer to the video buffer
		\param [in] lWidth			-	Width of the video buffer
		\param [in] lHeight			-	Height of the video buffer
		\param [in] pRect			-	Area to shade
		\param [in] rgbShade		-	Color to shade the area.  
										Color component values should 
										be less than 127.

		Makes a nice background for drawing text on top of video.		
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Shade(LPBYTE pBuf, long lWidth, long lHeight, LPRECT pRect, COLORREF rgbShade);

	//==============================================================
	// Restore()
	//==============================================================
	/// Restores default objects to encapsulated DC
	void Restore();

	//==============================================================
	// SelectObject()
	//==============================================================
	/// Selects a font in the the encapsulated DC
	/**
		\param [in] hFont	-	Font to select
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SelectObject( HFONT hFont );

	//==============================================================
	// SelectObject()
	//==============================================================
	/// Selects a brush in to the encapsulated DC
	/**
		\param [in] hBrush	-	Brush to select
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SelectObject( HBRUSH hBrush );

	//==============================================================
	// SelectObject()
	//==============================================================
	/// Selects a pen into the encapsulated DC
	/**
		\param [in] hPen	-	Pen to select
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SelectObject( HPEN hPen );

	//==============================================================
	// GetRect()
	//==============================================================
	/// Gets the size of the encapsulated DC
	/**
		\param [in] pRect	-	Receives the size of the DC
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetRect( LPRECT pRect );

	//==============================================================
	// ReleaseDC()
	//==============================================================
	/// Releases the encapsulated DC
	/**
		\param [in] hWnd	-	Window that owns the DC
		
		\return 
	
		\see 
	*/
	BOOL ReleaseDC( HWND hWnd );

	//==============================================================
	// GetDC()
	//==============================================================
	///	Gets the DC for a window
	/**
		\param [in] hWnd	-	Window handle
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetDC( HWND hWnd );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Restores objects and releases the encapsulated DC
	void Destroy();

	/// Constructor
	CGrDC();

	//==============================================================
	// CGrDC()
	//==============================================================
	/// Constructs an object encapsulating the specified DC
	/**
		\param [in] hDC -	Handle of DC
	*/
	CGrDC( HDC hDC );

	//==============================================================
	// CGrDC()
	//==============================================================
	/// Constructs object encapsulating specified DC
	/**
		\param [in] hDC			-	Handle to DC
		\param [in] bAutoDelete -	Non-zero if DC should be released
									on object destruction.
	*/
	CGrDC( HDC hDC, BOOL bAutoDelete );

	// Destructor
	virtual ~CGrDC();

	//==============================================================
	// HDC()
	//==============================================================
	/// Returns a pointer to the encapsulated Device Context
	operator HDC() { return m_hDC; }

	//==============================================================
	// GetSafeHdc()
	//==============================================================
	/// Returns a pointer to the encapsulated Device Context
	HDC GetSafeHdc() { return m_hDC; }

	//==============================================================
	// IsDC()
	//==============================================================
	/// Returns non-zero if there is a valid encapsulated Device Context
	BOOL IsDC() { return ( m_hDC != NULL ); }

	//==============================================================
	// Attach()
	//==============================================================
	/// Encapsulates an existing Device Context
	/**
		\param [in] hDC		-	Existing DC handle
	*/
	void Attach( HDC hDC ) { Destroy(); m_hDC = hDC; }

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from the encapsulated Device Context with out releasing it
	void Detach() { Restore(); m_hDC = NULL; }

	//==============================================================
	// MoveTo()
	//==============================================================
	/// Moves the GDI drawing caret to the specified position
	/**
		\param [in] x	-	Horizontal offset
		\param [in] y	-	Vertical offset
		\param [in] pt	-	Optional variable to receive previous position.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL MoveTo( long x, long y, LPPOINT pt = NULL )
	{	return ::MoveToEx( m_hDC, x, y, pt ); }

	//==============================================================
	// LineTo()
	//==============================================================
	/// Draws a line from the current GDI caret positino to the specified point
	/**
		\param [in] x	-	Horizontal endpoint of line
		\param [in] y	-	Vertical endpoint of line
		
		\return	Non-zero if success
	
		\see 
	*/
	BOOL LineTo( long x, long y )
	{	return ::LineTo( m_hDC, x, y ); }

	//==============================================================
	// PutPixel()
	//==============================================================
	/// Plots a single pixel of the specified color
	/**
		\param [in] x		-	Horizontal offset of pixel
		\param [in] y		-	Vertical offset of pixel
		\param [in] rgb		-	Color of pixel
		
		This is not a particularly fast pixel plotting function and
		should be used wisely.

		\return Non-zero if success
	
		\see 
	*/
	BOOL PutPixel( long x, long y, COLORREF rgb )
	{	return ::SetPixel( m_hDC, x, y, rgb ); }

	//==============================================================
	// AutoDelete()
	//==============================================================
	/// Sets the Auto delete state
	/**
		\param [in] b	-	Non-zero if encapsulated DC should be deleted
							on object destruction.
	*/
	void AutoDelete( BOOL b ) { m_bAutoDelete = b; }

private:

	/// Handle to encapsulated Device Context
	HDC			m_hDC;

	/// Handle to originally selected pen
	HPEN		m_hOldPen;

	/// Handle to originally selected brush
	HBRUSH		m_hOldBrush;

	/// Handle to originally selected font
	HFONT		m_hOldFont;	

	/// Non-zero if m_hDC should be deleted upon object destruction.
	BOOL		m_bAutoDelete;
public:

	//==============================================================
	// ExFill()
	//==============================================================
	/// Fills the excluded region of two rectangles
	/**
		\param [in] hDc			-	Handle to DC for drawing
		\param [in] pInside		-	Area to exclude
		\param [in] pOutside	-	Area to fill
		\param [in] rgb			-	Color of fill
		
		This function fills the area that is within pOutside but not
		within pInside.  This is usually used when displaying an aspect
		corrected image within a defined rectangle.  It's faster than
		filling the entire background.

		For the below instances, this function fills the area marked
		with '*'.

		\code
		 pOutside
		----------------------
		|********************|
		|****-----------*****|
		|****| pInside |*****|
		|****|         |*****|
		|****|         |*****|
		|****|         |*****|
		|****-----------*****|
		|********************|
		----------------------
		\endcode
  
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ExFill( HDC hDc, LPRECT pInside, LPRECT pOutside, COLORREF rgb );
	//==============================================================
	// IconToCursor()
	//==============================================================
	/// Converts an icon into a cursor
	/**
		\param [in] hIcon		-	Icon to convert
		\param [in] lxHotSpot	-	Horizontal hot spot for cursor
		\param [in] lyHotSpot	-	Vertical hot spot for cursor
		
		\warning You must release this cursor when done by calling ::DestroyCursor()

		\return Handle to newly created cursor
	
		\see 
	*/
	static HCURSOR IconToCursor( HICON hIcon, long lxHotSpot, long lyHotSpot );

	//==============================================================
	// Ellipse()
	//==============================================================
	/// Draws a circle using a center point and radius
	/**
		\param [in] hDC			-	Handle to DC for drawing
		\param [in] sz			-	Size of the border
		\param [in] rgb			-	Color of ellipse
		\param [in] pptCenter	-	Center point
		\param [in] lRadius		-	Radius
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Ellipse( HDC hDC, long sz, COLORREF rgb, LPPOINT pptCenter, long lRadius );
	
	//==============================================================
	// ScalePoints()
	//==============================================================
	/// Scales the specified points
	/**
		\param [in] psrcPts		-	Array of source POINT structures
		\param [in] pdstPts		-	Array of destination POINT structures
		\param [in] dwPts		-	Number of points
		\param [in] pSrc		-	Source rect 
		\param [in] pDst		-	Destination rect
		
		Usefull for scaling points into another bounding rectangle.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL ScalePoints(LPPOINT psrcPts, LPPOINT pdstPts, DWORD dwPts, LPRECT pSrc, LPRECT pDst);

	//==============================================================
	// DrawSelectionPolygon()
	//==============================================================
	/// Draws a translucent polygon
	/**
		\param [in] hDC					-	DC handle for drawing
		\param [in] pPts				-	Array of POINT structures defining the polygon
		\param [in] dwPts				-	Number of structures in pPts
		\param [in] rgbBck				-	Color of the background
		\param [in] rgbHatch			-	Color of the hatch
		\param [in] dwTranslucency		-	Translucency 0 = Solid, 10000 = Invisible.
		
		Useful for highlighting an irregularly shaped area

		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawSelectionPolygon( HDC hDC, LPPOINT pPts, DWORD dwPts, COLORREF rgbBck, COLORREF rgbHatch, DWORD dwTranslucency = 5000);
	
	//==============================================================
	// DrawPolygonOutline()
	//==============================================================
	/// Draws the outline of a polygon
	/**
		\param [in] hDC		-	Handle to DC for drawing
		\param [in] pRect	-	Size of DC in hDC
		\param [in] ppi		-	POINT array defining the polygon
		\param [in] pts		-	number of points in ppi
		\param [in] rgb		-	Color of outline
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawPolygonOutline( HDC hDC, LPRECT pRect, LPPOINT ppi, DWORD pts, COLORREF rgb );
	
	//==============================================================
	// DrawSelectionBox()
	//==============================================================
	/// Draws a translucent rectangle
	/**
		\param [in] hDC					-	DC handle for drawing
		\param [in] pRect				-	Rectangle defining selection area
		\param [in] rgbBck				-	Color of the background
		\param [in] rgbHatch			-	Color of the hatch
		\param [in] dwTranslucency		-	Translucency 0 = Solid, 10000 = Invisible.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawSelectionBox( HDC hDC, LPRECT pRect, COLORREF rgbBck, COLORREF rgbHatch, DWORD dwTranslucency = 5000 );

	//==============================================================
	// FillHatchedRect()
	//==============================================================
	/// Fills in a rectangle with a hatch pattern
	/**
		\param [in] hDC		-	Handle to DC for drawing
		\param [in] pRect	-	Rectangle area
		\param [in] bck		-	Background color
		\param [in] lines	-	Color of hatch lines
		\param [in] hatch	-	Type of hatch
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL FillHatchedRect( HDC hDC, LPRECT pRect, COLORREF bck, COLORREF lines, int hatch );

	//==============================================================
	// FillHatchedRect()
	//==============================================================
	/// Fills in a rectangle with a hatch pattern
	/**
		\param [in] pRect	-	Rectangle area
		\param [in] bck		-	Background color
		\param [in] lines	-	Color of hatch lines
		\param [in] hatch	-	Type of hatch
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL FillHatchedRect( LPRECT pRect, COLORREF bck, COLORREF lines, int hatch )
	{	return FillHatchedRect( m_hDC, pRect, bck, lines, hatch ); }
		
	//==============================================================
	// GetAngleDif()
	//==============================================================
	/// Returns the minimum difference between two angles
	/**
		\param [in] a1	-	First angle in degrees
		\param [in] a2	-	Second angle in degrees
		
		\return Minimum difference between the two angles
	
		\see 
	*/
	static float GetAngleDif( float a1, float a2 );

	//==============================================================
	// RotateIcon()
	//==============================================================
	/// Rotates the specified icon
	/**
		\param [in] hIcon	-	Source icon
		\param [in] angle	-	Angle to rotate in degrees
		
		\warning You must release this icon when done by calling ::DestroyIcon()

		\return HICON handle to newly created rotated icon
	
		\see 
	*/
	static HICON RotateIcon( HICON hIcon, float angle );

	//==============================================================
	// DrawGrid()
	//==============================================================
	/// Draws a grid into the specified rectangle
	/**
		\param [in] hDC			-	Handle to DC for drawing
		\param [in] pRect		-	Rectangular area
		\param [in] rgb			-	Color of the grid
		\param [in] cx			-	Horizontal spacing between grid lines
		\param [in] cy			-	Vertical spacing between grid lines
		\param [in] ox			-	Horizontal offset of grid lines
		\param [in] oy			-	Vertical offset of grid lines
		\param [in] pentype		-	GDI pen type
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawGrid( HDC hDC, LPRECT pRect, COLORREF rgb, long cx, long cy, long ox = 0, long oy = 0, int pentype = PS_SOLID );
	
	//==============================================================
	// GetIconRect()
	//==============================================================
	/// Gets the size of an icon
	/**
		\param [in] hIcon	-	Valid icon handle
		\param [in] pRect	-	Receives the size of the icon
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetIconRect( HICON hIcon, LPRECT pRect );
	
	//==============================================================
	// CreateDisabledIcon()
	//==============================================================
	/// Creates a disabled icon
	/**
		\param [in] hIcon	-	Source icon
		\param [in] bck		-	Shading color
		
		\warning You must release this icon when done by calling ::DestroyIcon()

		\return Handle to newly created disabled icon
	
		\see 
	*/
	static HICON CreateDisabledIcon( HICON hIcon, COLORREF bck = RGB( 0, 0, 0 ) );

	//==============================================================
	// CreateMonoChromeIcon()
	//==============================================================
	/// Creates a monochrome version of an icon
	/**
		\param [in] hIcon	-	Handle to source icon
		\param [in] key		-	Foreground color
		\param [in] bck		-	Background color
		
		\warning You must release this icon when done by calling ::DestroyIcon()

		\return Handle to newly created mono-chrome icon
	
		\see 
	*/
	static HICON CreateMonoChromeIcon( HICON hIcon, COLORREF key = MAXDWORD, COLORREF bck = RGB( 0, 0, 0 ) );

	//==============================================================
	// LoadIcon()
	//==============================================================
	/// Loads icon resource
	/**
		\param [in] uIcon	-	UINT resource id
		\param [in] cx		-	Desired horizontal size of the icon
		\param [in] cy		-	Desired vertical size of the icon
		
		\warning You must release this icon when done by calling ::DestroyIcon()

		\return Handle to newly created icon
	
		\see 
	*/
	static HICON LoadIcon( UINT uIcon, long cx = 0, long cy = 0 )
	{	return (HICON)LoadImage(	GetModuleHandle( NULL ), MAKEINTRESOURCE( uIcon ),
									IMAGE_ICON, cx, cy, 0 );									
	}

	//==============================================================
	// LoadIcon()
	//==============================================================
	/// Loads icon resource
	/**
		\param [in] hInstance	-	Module instance handle containing the icon
		\param [in] uIcon		-	UINT resource id
		\param [in] cx			-	Desired horizontal size of the icon
		\param [in] cy			-	Desired vertical size of the icon
		
		\warning You must release this icon when done by calling ::DestroyIcon()

		\return Handle to newly created icon
	
		\see 
	*/
	static HICON LoadIcon( HINSTANCE hInstance, UINT uIcon, long cx = 0, long cy = 0 )
	{	return (HICON)LoadImage(	hInstance, MAKEINTRESOURCE( uIcon ),
									IMAGE_ICON, cx, cy, 0 );
	}

	//==============================================================
	// DrawIcon()
	//==============================================================
	/// Draws the specified icon into a Device Context
	/**
		\param [in] hDC			-	Handle to DC for drawing
		\param [in] pRect		-	Position and size to draw icon
		\param [in] hIcon		-	Handle to icon
		\param [in] x			-	If pRect is NULL, the horizontal position of icon
		\param [in] y			-	If pRect is NULL, the vertical position of the icon
		\param [in] bDisabled	-	Non-zero to draw disabled version of icon
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL DrawIcon( HDC hDC, LPRECT pRect, HICON hIcon, long x = 0, long y = 0, BOOL bDisabled = FALSE );

	//==============================================================
	// Line()
	//==============================================================
	/// Draws an arbitrary line
	/**
		\param [in] hDC			-	Handle to DC for drawing
		\param [in] rgb			-	Color of line to draw
		\param [in] w			-	Line width
		\param [in] x1			-	Horzontal offset of line start
		\param [in] y1			-	Vertical offset of line start
		\param [in] x2			-	Horzontal offset of line end
		\param [in] y2			-	Vertical offset of line end
		\param [in] nStyle		-	Line style
		\param [in] rgbBrush	-	Color of background
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL Line( HDC hDC, COLORREF rgb, long w, long x1, long y1, long x2, long y2, int nStyle = PS_SOLID, COLORREF rgbBrush = MAXDWORD );
	
	//==============================================================
	// ahtodw()
	//==============================================================
	/// Converts an ASCII hex string into a DWORD value
	/**
		\param [in] pBuffer		-	Buffer containing ASCII hex string
		\param [in] dwBytes		-	Number of byes in pBuffer to convert
		
		\return DWORD value of converted string
	
		\see 
	*/
	static DWORD ahtodw( LPCTSTR pBuffer, DWORD dwBytes );

	//==============================================================
	// GrayScale()
	//==============================================================
	/// Returns grayscale representation of color
	/**
		\param [in] rgb		-	Color to convert to grayscale
		
		\code

Fast Appx		- Gray scale = ( Green + Green + Red + Blue ) >> 2
SMPTE-C         - Gray scale = Y = ( .298912 * Red + .586611 * Green + .114478 * Blue )
Rec 601-1       - Gray scale = Y = ( 299 * Red + 587 * Green + 114 * Blue ) / 1000
Rec 709         - Gray scale = Y = ( 213 * Red + 715 * Green + 72 * Blue ) / 1000
ITU standard    - Gray scale = Y = ( 222 * Red + 707 * Green + 71 * Blue ) / 1000
Microsoft       - Gray scale = Green |or| Gray scale = ( Red + Green + Blue ) / 3

+-----------------------------+----------------+
|Scheme                       |Luminosity level|
+-----------------------------+----------------+
|Gray=Green                   |        1       |
|Gray=ITU (D65)               |        2       |
|Gray=Rec 709 (D65)           |        3       |
|Gray=Rec 601-1 (C illuminant)|        4       |
|Gray=(Red+Green+Blue)/3      |        5       |
+-----------------------------+----------------+

		\endcode

		Currently using ITU standard

		\return Grayscale representation of color value in rgb
	
		\see 
	*/
	static COLORREF GrayScale( COLORREF rgb )
	{	return (BYTE)( (	( (long)GetRValue( rgb ) * 222 ) + 
							( (long)GetGValue( rgb ) * 707 ) + 
							( (long)GetBValue( rgb ) * 71 ) ) / 1000 );
	}

	//==============================================================
	// IsLightColor()
	//==============================================================
	/// Returns non-zero if the specified color is a 'light' color
	/**
		\param [in] rgb		-	Color to analyze
		
		\return Non-zero if light color
	
		\see 
	*/
	static BOOL IsLightColor( COLORREF rgb );

	//==============================================================
	// dwtohstr()
	//==============================================================
	/// Converts a DWORD value into an ASCII hex string
	/**
		\param [in] num		-	DWORD value of number to convert
		\param [in] buf		-	Buffer that receives ASCII string
		\param [in] index	-	Offset into buffer to start copy
		\param [in] pad		-	Minimum number of characters to copy
		\param [in] max		-	Maximum number of characters to copy
		
		\return 
	
		\see 
	*/
	static DWORD dwtohstr( DWORD num, LPSTR buf, DWORD index, DWORD pad = 2, DWORD max = 2 );

	//==============================================================
	// RgbToHtml()
	//==============================================================
	/// Converts an RGB color value into the HTML representation
	/**
		\param [in] rgb			-	RGB value, such as RGB( 255, 0, 255 ) 
		\param [in] pHtml		-	HTML value, such as \#FF00FF
		
		\return 
	
		\see HtmlToRgb()
	*/
	static LPCTSTR RgbToHtml( COLORREF rgb, LPSTR pHtml );

	//==============================================================
	// HtmlToRgb()
	//==============================================================
	/// 
	/**
		\param [in] pHtml		-	HTML value, such as \#FF00FF
		
		\return RGB value such as RGB( 255, 0, 255 )
	
		\see RgbToHtml()
	*/
	static COLORREF HtmlToRgb( LPCTSTR pHtml );
};

#endif // !defined(AFX_GRDC_H__49A61F76_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
