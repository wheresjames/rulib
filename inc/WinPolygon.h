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
// WinPolygon.h: interface for the CWinPolygon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPOLYGON_H__7B5CA763_398F_49AD_B6EB_7E126CBE9DC9__INCLUDED_)
#define AFX_WINPOLYGON_H__7B5CA763_398F_49AD_B6EB_7E126CBE9DC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// Holds information defining a line
/**
	\see CWinPolygon
*/
typedef struct tagLINE
{	POINT		a;
	POINT		b;
} LINE, *LPLINE; // end typedef struct

//==================================================================
// CWinPolygon
//
/// Encapsulates a polygon
/**
	This structure encapsulates a polylgon and provides a few 
	nifty functions for manipulating them.  The polygon is stored
	simply as a list of points.	
*/
//==================================================================
class CWinPolygon  
{

public:


	//==============================================================
	// Distance()
	//==============================================================
	/// Calculates the minimum distance between polygon points
	/**
		\param [in] pWp - CWinPolygon object
		
		This function calculates the minimum distance between to
		points in the polygons.

		\warning	This function currently makes only a rough 
					estimation of distance.

		\return The distance between the closest points.
	
		\see 
	*/
	long Distance( CWinPolygon *pWp );

	//==============================================================
	// GetIBeam()
	//==============================================================
	/// Returns points representing the vertical height of the polygon.
	/**
		\param [out] ptTop		-	The top point of the line
		\param [out] ptBottom	-	Bottom point of the line
		\param [in] bInvY		-	Non-zero to flip the line upside
									down.
		
		This function basically creates a line that cuts the polygon
		in half vertcally.  The line is as long as the polygon is high,
		and is located roughly at the horizontal center.

		I know this is a strange function, but I needed it OK?

		\return Non-zero if success
	
		\see 
	*/
	BOOL GetIBeam( LPPOINT ptTop, LPPOINT ptBottom, BOOL bInvY = FALSE );

	//==============================================================
	// Scale()
	//==============================================================
	/// Scales the polygon points according to the ratio of specified rectangles
	/**
		\param [in] pSrc		-	Source rect 
		\param [in] pDst		-	Destination rect

		Calculates the vertical and horizontal difference and scales the
		polygon accordingly.  Great for scaling a polygon for overlaying
		on one image to the same image of a different size.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Scale( LPRECT pSrc, LPRECT pDst );

	//==============================================================
	// GetBoundingRect()
	//==============================================================
	/// Returns the bounding rectangle
	/**
		\param [in] pRect		-	Receives bounding rect

		\return Non-zero if success
	
		\see 
	*/
	BOOL GetBoundingRect( LPRECT pRect );

	//==============================================================
	// Intersect()
	//==============================================================
	/// Collision tests with another polygon
	/**
		\param [in] pPts	-	Points in polygon for collision test
		\param [in] dwPts	-	Number of points in pPts
  		
		Unfortunately, this is not a very optimized function.  But
		it works well.

		\return Non-zero if polygons intersect
	
		\see 
	*/
	BOOL Intersect( LPPOINT pPts, DWORD dwPts );

	//==============================================================
	// Intersect()
	//==============================================================
	/// Collision tests with another polygon
	/**
		\param [in] pWp		-	CWinPolygon object to collision test
		
		\return Non-zero if polygons intersect
	
		\see 
	*/
	BOOL Intersect( CWinPolygon *pWp )
	{	return Intersect( pWp->GetPoints(), pWp->GetNumPoints() ); }

	//==============================================================
	// rScanSort()
	//==============================================================
	/// Performs a recursive quick sort on the polygons
	/**
		\param [in] pPts	-	Points to sort
		\param [in] left	-	Left index
		\param [in] right	-	Right index

		Call ScanSort().  Don't call this function directly
	*/
	void rScanSort( LPPOINT pPts, DWORD left, DWORD right );

	//==============================================================
	// ScanSort()
	//==============================================================
	/// Performs a recursive quick sort on the polygons
	/**
		Reasonably fast sorting algorithm.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ScanSort();

	//==============================================================
	// Inflate()
	//==============================================================
	/// Inflates the polygon by the specified amount
	/**
		\param [in] x	-	Horizontal inflate value
		\param [in] y	-	Vertical inflate value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Inflate( long x, long y );

	//==============================================================
	// SimpleArea()
	//==============================================================
	/// Calculates a rough estimate of the polygon area.
	/**
		
		\return value representing the rough polygon area.
	
		\see 
	*/
	float SimpleArea();

	//==============================================================
	// ConvexHull()
	//==============================================================
	/// Calculates a convex hull of the polygon
	/**
		After calling this function, the polygon will contain a 
		convex hull of the original polygon.  This calculates an
		exact convex hull.

		\warning	Requires sorting, call ScanSort() before this function
					unless you know the points are sorted.

		Copyright notice for this function:

		Based on "Andrew's Monotone Chain algorithm"
		Copyright 2001, softSurfer (www.softsurfer.com)
		This code may be freely used and modified for any purpose
		providing that this copyright notice is included with it.
		SoftSurfer makes no warranty for this code, and cannot be held
		liable for any real or imagined damage resulting from its use.
		Users of this code must verify correctness for their application.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ConvexHull();

	//==============================================================
	// ApxConvexHull()
	//==============================================================
	/// Calculates an approximate convex hull 
	/** 
		After this call, this class will contain the approximate
		convex hull of the original points.  

		Similar to the BFP Approximate Hull Algorithm
		( Bentley-Faust-Preparata, 1982 )
		but slightly faster

		\warning	This does not calculate an <b>exact</b> convex hull.
					For a precise convex hull, call ConvexHull()

		\return Non-zero if success
	
		\see 
	*/
	BOOL ApxConvexHull();

	//==============================================================
	// FromMap()
	//==============================================================
	/// Creates a polygon from a map
	/**
		\param [in] map -	pointer to map values
		\param [in] x	-	Horizontal offset for the extraction
		\param [in] y	-	Vertical offset for the extraction
		\param [in] w	-	Width of the image area for the extraction
		\param [in] h	-	Height of the image area for the extraction
		\param [in] th	-	Value threshold
		\param [in] buf	-	Pointer to buffer that holds map walk data.
							You can speed up multiple calls to this
							function by allocating this ahead of time,
							then passing it into this function.
		
		This function can create a polygon by 'filling' around a
		bitmap image.  Great for selcting parts of an image.

		\return Non-zero if success
	
		\see 
	*/
	BOOL FromMap( LPDWORD map, long x, long y, long w, long h, UINT th, LPBYTE buf = NULL );

	/// Default constructor
	CWinPolygon() 
	{ m_dwPts = 0; }

	//==============================================================
	// CWinPolygon()
	//==============================================================
	/// Constructs a polygon object from a list of points
	/**
		\param [in] pPts	-	Pointer to POINT structure array
		\param [in] dwPts	-	Number of points in pPts
	*/
	CWinPolygon( LPPOINT pPts, DWORD dwPts )
	{	m_dwPts = 0; SetPoints( pPts, dwPts ); }

	//==============================================================
	// CWinPolygon()
	//==============================================================
	/// Constructs a polygon object by copying another CWinPolygon object
	/**
		\param [in] wp	-	Pointer to CWinPolygon object
	*/
	CWinPolygon( CWinPolygon &wp )
	{	m_dwPts = 0; SetPoints( wp, wp ); }

	/// Destructor
	virtual ~CWinPolygon();

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the polygon resources
	void Destroy() { m_dwPts = 0; m_pts.destroy(); }

	//==============================================================
	// Allocate()
	//==============================================================
	/// Allocates the specified number of points in the polygon
	/**
		\param [in] sz	-	Number of points to allocate
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Allocate( DWORD sz ) 
	{	if ( !m_pts.allocate( sz ) ) return FALSE; m_pts.Zero(); return TRUE; }

	// Points
	//==============================================================
	// GetPoints()
	//==============================================================
	/// Returns a pointer to the polygon POINT structure array
	LPPOINT GetPoints() { return m_pts; }

	//==============================================================
	// GetNumPoints()
	//==============================================================
	/// Returns the total number of points in the polygon
	DWORD GetNumPoints() { return m_dwPts; }

	//==============================================================
	// GetNumEdges()
	//==============================================================
	/// Returns the number of edges the polygon has
	DWORD GetNumEdges() { DWORD pts = GetNumPoints(); if ( pts ) return pts - 1; return 0; }

	//==============================================================
	// GetEdge()
	//==============================================================
	/// Returns a line struture pointer for the specified edge
	/**
		\param [in] i	-	Edge index
		
		This function returns an offset into the point array of the
		specified edge.  It does not allocate any new memory

		\return Pointer to the specified edge
	
		\see 
	*/
	LPLINE GetEdge( DWORD i ) { return (LPLINE)&m_pts[ i ]; }

	//==============================================================
	// PtInPolygon()
	//==============================================================
	/// Tests to see if the point lies within the polygon
	/**
		\param [in] ppt		-	Point to test

		This function works well and is reasonably fast.
		
		\return Non-zero if point is within the polygon
	
		\see 
	*/
	BOOL PtInPolygon( LPPOINT ppt ) { return PtInPolygon( GetPoints(), GetNumPoints(), ppt ); }

	//==============================================================
	// PtInPolygon()
	//==============================================================
	/// Tests to see if the point lies within the polygon
	/**
		\param [in] pptList		-	Polygon point list
		\param [in] dwListSize	-	Number of points in pptList
		\param [in] ppt			-	Point to test
		
		This function works well and is reasonably fast.
		
		\return Non-zero if point is within the polygon
	
		\see 
	*/
	static BOOL PtInPolygon( LPPOINT pptList, DWORD dwListSize, LPPOINT ppt );

	//==============================================================
	// CombinePoint()
	//==============================================================
	/// Adds a point into the current polygon by calculating the closest edge
	/**
		\param [in] ppt		-	Point to add
		
		Magic function

		\warning I haven't implemented this function yet!

		The idea is just to calculate the closest edge to the polygon and
		conjoin the point into the polygon at that edge.

		\return Non-zero if success
	
		\see 
	*/
	BOOL CombinePoint( LPPOINT ppt ) { return CombinePoint( GetPoints(), GetNumPoints(), ppt ); }

	//==============================================================
	// CombinePoint()
	//==============================================================
	/// Adds a point into the current polygon by calculating the closest edge
	/**
		\param [in] pptList			-	Pointer to POINT structure array
		\param [in] dwListSize		-	Number of points in pptList
		\param [in] ppt				-	Point to add
		
		Magic function

		\warning I haven't implemented this function yet!

		The idea is just to calculate the closest edge to the polygon and
		conjoin the point into the polygon at that edge.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL CombinePoint( LPPOINT pptList, DWORD dwListSize, LPPOINT ppt );

	//==============================================================
	// operator += ()
	//==============================================================
	/// Appends point to the end of the polygon
	/**
		\param [in] ppt		-	Point to add
		
		\return Reference to this object
	
		\see 
	*/
	CWinPolygon& operator += ( LPPOINT ppt ) { AddPoint( ppt ); return *this; }

	//==============================================================
	// AddPoint()
	//==============================================================
	/// Appends point to the end of the polygon
	/**
		\param [in] ppt		-	Point to add
		
		\return Non-zero if added
	
		\see 
	*/
	BOOL AddPoint( LPPOINT ppt ) { return AddPoint( ppt->x, ppt->y ); }

	//==============================================================
	// AddPoints()
	//==============================================================
	/// Appends points to the end of the polygon point list
	/**
		\param [in] ppt		-	List of points
		\param [in] dwPts	-	Number of points in ppt
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddPoints( LPPOINT ppt, DWORD dwPts )
	{	BOOL bRet = TRUE; 
		for ( DWORD i = 0; i < dwPts; i++ ) 
			if ( !AddPoint( &ppt[ i ] ) ) bRet = FALSE;
		return bRet;
	}

	//==============================================================
	// AddPoint()
	//==============================================================
	/// Appends point to the end of the polygon
	/**
		\param [in] x	-	Horizontal offset of point
		\param [in] y	-	Vertical offset of point
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddPoint( long x, long y )
	{	if ( !m_pts.grow( m_dwPts + 1 ) ) return FALSE;
		m_pts[ m_dwPts ].x = x; m_pts[ m_dwPts ].y = y; m_dwPts++;
		return TRUE;
	}

	//==============================================================
	// AddPolygon()
	//==============================================================
	/// Add The points from the specified polygon
	/**
		\param [in] pWp		-	Polygon containing points to add
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddPolygon( CWinPolygon *pWp )
	{	return AddPoints( pWp->GetPoints(), pWp->GetNumPoints() ); }

	//==============================================================
	// RemovePoint()
	//==============================================================
	/// Removes the specified point from the polygon
	/**
		\param [in] dwPt	-	Index of point to remove
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemovePoint( DWORD dwPt )
	{	if ( !m_dwPts || dwPt >= m_dwPts ) return FALSE;
		for ( DWORD i = dwPt + 1; i < m_dwPts; i++ )
			m_pts[ i - 1 ] = m_pts[ i ];
		m_dwPts--;
		return TRUE;
	}

	//==============================================================
	// RemovePoint()
	//==============================================================
	/// Removes the specified point from the polygon
	/**
		\param [in] x	-	Horizontal offset of point
		\param [in] y	-	Vertical offset of point

		Searches the polygon for a point matching the specified x, y
		position and removes the first occurence.
	  
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemovePoint( long x, long y )
	{	for ( DWORD i = 0; i < m_dwPts; i++ )
			if ( m_pts[ i ].x == x && m_pts[ i ].y == y )
				return RemovePoint( i );
		return FALSE;
	}

	//==============================================================
	// operator DWORD()
	//==============================================================
	/// Returns the number of points in the polygon
	operator DWORD() { return m_dwPts; }

	//==============================================================
	// operator LPPOINT()
	//==============================================================
	/// Returns a pointer to the polygon POINT structure array
	operator LPPOINT() { return m_pts; }


	//==============================================================
	// operator []()
	//==============================================================
	/// Returns a reference to the specified point
	POINT& operator []( DWORD i ) {	return m_pts[ i ]; }

	//==============================================================
	// isLeft()
	//==============================================================
	/// Returns greater than zero if the specified triangle is left-handed
	/**
		\param [in] P0	-	First point in triangle
		\param [in] P1	-	Second point in triangle
		\param [in] P2	-	Third point in triangle				 		

		\return greater than zero if left handed triangle is specified
	
		\see 
	*/
	inline long isLeft( POINT P0, POINT P1, POINT P2 )
	{	return ( P1.x - P0.x ) * ( P2.y - P0.y ) - ( P2.x - P0.x ) * ( P1.y - P0.y ); }

	//==============================================================
	// ComparePoint()
	//==============================================================
	/// For sorting points
	/**
		\param [in] p1	-	First point
		\param [in] p2	-	Second point

		-1	if p1 is before p2
		0 	if points are the same
		1	if p2 is before p1
		
		\return 
	
		\see 
	*/
	inline long ComparePoint( POINT p1, POINT p2 )
	{	if ( p1.x > p2.x ) return 1; 
		else if ( p1.x == p2.x ) 
		{ if ( p1.y > p2.y ) return 1; else if ( p1.y == p2.y ) return 0; }
		return -1;
	}		

	//==============================================================
	// CalculateCenter()
	//==============================================================
	/// Calculates the center point of the polygon
	/**
		\return Center point of polygon
	
		\see 
	*/
	POINT CalculateCenter();

	/// 2D range
	typedef struct tagRANGE
	{	POINT min, max;
	} RANGE, *LPRANGE;


	//==============================================================
	// operator =()
	//==============================================================
	/// Copies another CWinPolygon object
	CWinPolygon& operator = ( CWinPolygon &wp )
	{	SetPoints( &wp ); return *this;
	}

	//==============================================================
	// SetPoints()
	//==============================================================
	/// Sets the value of the polygon points
	/**
		\param [in] pPts	-	Pointer to POINT array
		\param [in] dwPts	-	Number of points in pPts
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPoints( LPPOINT pPts, DWORD dwPts )
	{	if ( !m_pts.allocate( dwPts ) ) return FALSE;
		m_pts.copy( pPts, dwPts ); m_dwPts = dwPts;
		return TRUE;
	}

	//==============================================================
	// SetPoints()
	//==============================================================
	/// Sets the value of the polygon points
	/**
		\param [in] pWp		-	Polygon containing point values
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetPoints( CWinPolygon *pWp )
	{	return SetPoints( pWp->GetPoints(), pWp->GetNumPoints() ); }

		
	//==============================================================
	// Offset()
	//==============================================================
	/// Offsets the polygon by the specified distance
	/**
		\param [in] ppt		-	Point specifying offset distance
	*/
	void Offset( LPPOINT ppt ) { Offset( ppt->x, ppt->y ); }

	//==============================================================
	// Offset()
	//==============================================================
	/// Offsets the polygon by the specified distance
	/**
		\param [in] x	-	Horizontal offset distance
		\param [in] y	-	Vertical offset distance
	*/
	void Offset( long x, long y )
	{	for ( DWORD i = 0; i < m_dwPts; i++ )
			m_pts[ i ].x += x, m_pts[ i ].y += y;
	}

	//==============================================================
	// ConvexArea()
	//==============================================================
	/// Calculates the area of a convex polygon
	/**

		The polygon in the object must be convex or this function
		returns an undefined value!
		
		\return Area
	
		\see 
	*/
	float ConvexArea();

	//==============================================================
	// TriangleArea()
	//==============================================================
	/// Calculates the area of a triangle given the lengths of it's sides
	/**
		\param [in] a	-	First side length
		\param [in] b	-	Second side length
		\param [in] c	-	Third side length
		
		\return Area of the triangle
	
		\see 
	*/
	static float TriangleArea( float a, float b, float c )
	{	float s = ( a + b + c ) / (float)2;
		return sqrt( s * ( s - a ) * ( s - b ) * ( s - c ) );
	}

	//==============================================================
	// TriangleArea()
	//==============================================================
	/// Calculates the area of the triangle defined by three points.
	/**
		\param [in] x1	-	Horizontal offset first point
		\param [in] y1	-	Vertical offset first point
		\param [in] x2	-	Horizontal offset second point
		\param [in] y2	-	Vertical offset second point
		\param [in] x3	-	Horizontal offset third point
		\param [in] y3	-	Vertical offset third point
		
		\return Area of the specified triangle
	
		\see 
	*/
	static float TriangleArea( float x1, float y1, float x2, float y2, float x3, float y3 )
	{
		float ax = x1 - x2;
		float ay = y1 - y2;
		float al = sqrt( ( ax * ax ) + ( ay * ay ) );

		float bx = x2 - x3;
		float by = y2 - y3;
		float bl = sqrt( ( bx * bx ) + ( by * by ) );

		float cx = x3 - x1;
		float cy = y3 - y1;
		float cl = sqrt( ( cx * cx ) + ( cy * cy ) );

		return TriangleArea( al, bl, cl );
	}


private:

	/// Contains the polygon point array
	TMem< POINT >				m_pts;

	/// Number of points in the polygon
	DWORD						m_dwPts;

};

#endif // !defined(AFX_WINPOLYGON_H__7B5CA763_398F_49AD_B6EB_7E126CBE9DC9__INCLUDED_)
