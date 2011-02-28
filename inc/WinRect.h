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
// WinRect.h: interface for the CWinRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_)
#define AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CWinRect
//
/// Encapsulates a rectangle object
/**
	Simple wrapper.  I should really add a lot more to this class.	
*/
//==================================================================
class CWinRect
{
public:

	//==================================================================
	// SPanelLayout
	//
	/// Contains information on a panel layout
	/**
		Sometimes it is needed to divide a rectangle into smaller 
		'panels', such as when tiling multiple images into an area.
		This class holds information on a layout of multiple rectangular
		areas.
	*/
	//==================================================================
	struct SPanelLayout
	{
		/// Number of panels in the layout
		long			lNumPanels;

		/// Aspect ratio of a single panel
		double			dPanelAspectRatio;

		/// Number of panel columns
		long			lxPanels; 

		/// Number of panel rows
		long			lyPanels;

		/// Width of a single panel
		long			lPanelWidth;

		/// Height of a single panel
		long			lPanelHeight;

		/// Extra horizontal space in the layout
		long			lxMargin;

		/// Extra vertical space in the layout
		long			lyMargin;
	};

public:

	/// Constructor
	CWinRect();
	
	/// Destructor
	virtual ~CWinRect();

public:

	//==============================================================
	// CombineOverlapping()
	//==============================================================
	/// Combines an overlapping rectangle list
	/**
		\param [in] pRectList		-	Array of RECT structures
		\param [in] dwRectListSize	-	Rectangles in pRectList
		\param [in] xThreshold		-	Horizontal overlap threshold
		\param [in] yThreshold		-	Vertical overlap threshold
		\param [in] max				-	Maximum number to combine
		\param [in] pdwFree			-	Next free rectangle

		This function does not defrag the rectangle list.
		Free rectangles are marked by setting all their values to zero.
	  
		\return Number of rectangles consolidated
	
		\see 
	*/
	static DWORD CombineOverlapping( LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold, LONG yThreshold, DWORD max = MAXDWORD, LPDWORD pdwFree = NULL );

	//==============================================================
	// IsAdjacentRect()
	//==============================================================
	/// Returns non-zero if the rectangles are 'close' as defined by the threshold
	/**
		\param [in] pRect1		-	First rectangle
		\param [in] pRect2		-	Second rectangle
		\param [in] xThreshold	-	Horizontal threshold
		\param [in] yThreshold	-	Vertical threshold
		
		\return Non-zero if rectangles are 'close' as defined by threshold
	
		\see 
	*/
	static BOOL IsAdjacentRect( LPRECT pRect1, LPRECT pRect2, LONG xThreshold = 0, LONG yThreshold = 0 );

	//==============================================================
	// AddRectToList()
	//==============================================================
	/// Adds a rectangle to a list
	/**
		\param [in] pRect			-	Rectangle to add
		\param [in] pRectList		-	List of rectangles
		\param [in] dwRectListSize	-	Number of rectangles in pRectList
		\param [in] xThreshold		-	Horizontal threshold
		\param [in] yThreshold		-	Vertical threshold

		This function adds a rectangle to the list by scanning for a free
		slot marked by all the rectangle values being set to zero.  It
		also checks each rectangle in the list for one that is 'close'
		as defined by the threshold values.  If a 'close' rectangle is
		found, then this rectangle is added to the 'close' rectangle by
		creating a unioned rectangle.  That is, the smallest rectangle
		that completely encompases both the new rectangle and the 'close'
		rectangle.
		
		\return Non-zero if rectangle was added
	
		\see 
	*/
	static BOOL AddRectToList( LPRECT pRect, LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold = 0, LONG yThreshold = 0 );

	//==============================================================
	// LPRECT()
	//==============================================================
	/// Returns a pointer to the encapsulated rectangle
	operator LPRECT() { return &m_rect; }

	//==============================================================
	// RECT()
	//==============================================================
	/// Returns a reference to the encapsulated rectangle
	operator RECT&() { return m_rect; }

	//==============================================================
	// Width()
	//==============================================================
	/// Returns the width of the rectangle
	LONG Width() { return Width( &m_rect ); }

	//==============================================================
	// Width()
	//==============================================================
	/// Returns the width of the rectangle
	static LONG Width( LPRECT pRect )
	{	return ( pRect->right - pRect->left ); }

	//==============================================================
	// Height()
	//==============================================================
	/// Returns the height of the rectangle
	LONG Height() { return Height( &m_rect ); }

	//==============================================================
	// Height()
	//==============================================================
	/// Returns the height of the rectangle
	static LONG Height( LPRECT pRect )
	{	return ( pRect->bottom - pRect->top ); }

	//==============================================================
	// CalculateIdealLayout()
	//==============================================================
	/// Calculates the ideal rectangle to contain a number of rectangles.
	/**
		\param [in] pSl			-	SPanelLayout structure that receives
									information about the layout.
		\param [in] pRect		-	Actual size of a single panel.
		\param [in] lNumPanels	-	Number of panels desired.
		
		This function returns an minimal bounding box containing the
		specified number of panels at the specified size.  pSl contains
		information about the layout such as the number of rows and
		columns.

		\return Non-zero if successful
	
		\see 
	*/
	static BOOL CalculateIdealLayout( SPanelLayout *pSl, LPRECT pRect, long lNumPanels );

	//==============================================================
	// CalculateLayout()
	//==============================================================
	/// Divides the specified rect into ideal panels for the specified aspect ratio
	/**
		\param [in] pSl					-	SPanelLayout structure that receives
											information about the layout.
		\param [in] pRect				-	Size of the bounding rectangle
		\param [in] lNumPanels			-	Number of panels desired.
		\param [in] dPanelAspectRatio 	-	Aspect ratio of individual 
											panels.  height / width
		
		This will divide the specified rectangle into optimal number of rows
		and columns to best achieve the specified aspect ratio and number of panels.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL CalculateLayout( SPanelLayout *pSl, LPRECT pRect, long lNumPanels, double dPanelAspectRatio = (double)3 / (double)4 );
	
	//==============================================================
	// GetPanelPosition()
	//==============================================================
	/// Gets the position of a particular panel within a layout.
	/**
		\param [in] pSl		-	SPanelLayout structure describing the layout.
		\param [in] lPanel 	-	Zero based panel index
		\param [in] pRect	-	Recieves the panel position.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetPanelPosition( SPanelLayout *pSl, long lPanel, LPRECT pRect );

	//==============================================================
	// HitTest()
	//==============================================================
	/// Returns the panel in which a specified point lies.
	/**
		\param [in] pSl		-	SPanelLayout structure describing the layout.
		\param [in] pPt		-	Point to test.
		
		Checks the point against the specified layout to determine where
		the point lies.

		\return Zero based panel index.
	
		\see 
	*/
	static long HitTest( SPanelLayout *pSl, LPPOINT pPt );


private:

	/// Encapsulated rectangle object
	RECT			m_rect;

};

#endif // !defined(AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_)
