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
// WinRect.cpp: implementation of the CWinRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinRect::CWinRect()
{_STT();
}

CWinRect::~CWinRect()
{_STT();
}

BOOL CWinRect::AddRectToList(LPRECT pRect, LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold, LONG yThreshold )
{_STT();
	// Add if possible
	RECT dst;
	DWORD i = 0, blank = MAXDWORD;
	for ( i = 0; i < dwRectListSize; i++ )
	{
		// If valid area
		if ( RW( pRectList[ i ] ) != 0 && RH( pRectList[ i ] ) != 0 )
		{	if ( IsAdjacentRect( &pRectList[ i ], pRect, xThreshold, yThreshold ) )
				if ( UnionRect( &dst, &pRectList[ i ], pRect ) )
				{	CopyRect( &pRectList[ i ], &dst ); return TRUE; }
		} // end if

		// Save blank location
		else if ( blank == MAXDWORD ) 
			blank = i;

	} // end for

	// Any more blank slots?
	if ( blank == MAXDWORD ) 
	{
		// Attempt to create an empty slot
		if ( !CombineOverlapping( pRectList, dwRectListSize, xThreshold, yThreshold, 1, &blank ) )
			return FALSE;

	} // end if

	// Add to slot
	CopyRect( &pRectList[ blank ], pRect );

	return TRUE;
}

BOOL CWinRect::IsAdjacentRect(LPRECT pRect1, LPRECT pRect2, LONG xThreshold, LONG yThreshold)
{_STT();
	// Sanity check
	if ( pRect1 == NULL || pRect2 == NULL ) 
		return FALSE;

	// Rect to the left?
	if ( pRect1->right < ( pRect2->left - xThreshold ) )
		return FALSE;

	// Rect to the right?
	if ( pRect1->left > ( pRect2->right + xThreshold ) )
		return FALSE;

	// Rect above
	if ( pRect1->bottom < ( pRect2->top - yThreshold ) )
		return FALSE;

	// Rect below
	if ( pRect1->top > ( pRect2->bottom + yThreshold ) )
		return FALSE;

	return TRUE;
}

DWORD CWinRect::CombineOverlapping(LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold, LONG yThreshold, DWORD max, LPDWORD pdwFree)
{_STT();
	// Combine overlapping rects
	BOOL dwCombined = 0;
	for ( DWORD i = 0; i < dwRectListSize; i++ )
		for ( DWORD c = i + 1; c < dwRectListSize; c++ )
		{	RECT dst;
			if ( 	( RW( pRectList[ i ] ) != 0 && RH( pRectList[ i ] ) != 0 ) &&
					( RW( pRectList[ c ] ) != 0 && RH( pRectList[ c ] ) != 0 ) &&
					IsAdjacentRect( &pRectList[ i ], &pRectList[ c ] ) )
				if ( UnionRect( &dst, &pRectList[ i ], &pRectList[ c ] ) )
				{	dwCombined++;
					
					// Copy the new rect
					CopyRect( &pRectList[ i ], &dst ); 
					
					// Lose the old rect
					ZeroMemory( &pRectList[ c ], sizeof( pRectList[ c ] ) ); 

					// Save pointer to blank if needed
					if ( dwCombined == 1 && pdwFree != NULL ) *pdwFree = c;					

					// Punt if user doesn't want any more
					if ( !max ) return dwCombined;

					i = 0; c = 1; max--;

				} // end if
		} // end for

	return dwCombined;
}

BOOL CWinRect::CalculateIdealLayout( SPanelLayout *pSl, LPRECT pRect, long lNumPanels )
{_STT();

	// Sanity check
	if ( !pSl || !pRect || 0 >= lNumPanels || 
		 PRW( pRect ) <= 0 || PRH( pRect ) <= 0 ) 
		return FALSE;

	// Calculate aspect ratio
	double dAspectRatio = (double)PRH( pRect ) / (double)PRW( pRect );

	RECT rect;
	SetRect( &rect, 0, 0, 10000, 10000 );
	CGrDC::AspectCorrect( &rect, pRect );

	// Get a layout that works
	CalculateLayout( pSl, &rect, lNumPanels, dAspectRatio );	

	// Now adjust to a size we would want
	pSl->lPanelWidth = PRW( pRect );
	pSl->lPanelHeight = PRH( pRect );

	// In the ideal case, there is no margin
	pSl->lxMargin = 0;
	pSl->lyMargin = 0;

	// Set minimal bounding rectangle
	SetRect( pRect, 0, 0,	pSl->lxPanels * pSl->lPanelWidth, 
							pSl->lyPanels * pSl->lPanelHeight );

	return TRUE;
}
BOOL CWinRect::CalculateLayout( SPanelLayout *pSl, LPRECT pRect, long lNumPanels, double dPanelAspectRatio )
{_STT();

	// Sanity check
	if ( !pSl || !pRect || 0 >= lNumPanels ) return FALSE;

	// Ensure valid aspect ratio
	if ( dPanelAspectRatio == 0 ) dPanelAspectRatio = (double)3 / (double)4;

	// Ensure valid number of panels
	pSl->lNumPanels = lNumPanels;
	pSl->dPanelAspectRatio = dPanelAspectRatio;

	// Initialize
	pSl->lxPanels = 1; 
	pSl->lyPanels = 1;

	// Get width / height
	long w = PRW( pRect );
	long h = PRH( pRect );
	if ( w == 0 || h == 0 ) return FALSE;

	// Ideal ratio (4:3)
	double delta = w * h;

	for ( long rows = 1; rows <= pSl->lNumPanels; rows++ )
	{
		long cols = pSl->lNumPanels / rows;
		if ( cols < 1 ) cols = 1;

		// Verify this is enough rows
		while ( ( cols * rows ) < pSl->lNumPanels ) cols++;

		// Track the closest ratio
		double cr = ( (double)h / (double)rows ) / ( (double)w / (double)cols );
		double dl = ( cr > dPanelAspectRatio ) ? cr - dPanelAspectRatio : dPanelAspectRatio - cr;
		if ( dl < delta ) { delta = dl; pSl->lxPanels = cols; pSl->lyPanels = rows; }

	} // end for

	// Ensure proper panel size
	while ( pSl->lyPanels && pSl->lxPanels && ( pSl->lyPanels - 1 ) * pSl->lxPanels >= pSl->lNumPanels ) pSl->lyPanels--;
	while ( pSl->lyPanels && pSl->lxPanels && ( pSl->lxPanels - 1 ) * pSl->lyPanels >= pSl->lNumPanels ) pSl->lxPanels--;

	// Must be larger than one
	if ( pSl->lxPanels < 1 ) pSl->lxPanels = 1;
	if ( pSl->lyPanels < 1 ) pSl->lyPanels = 1;

	// Get panel metrics
	pSl->lPanelWidth = w / pSl->lxPanels;
	pSl->lPanelHeight = h / pSl->lyPanels;

	// Calculate margin
	pSl->lxMargin = ( w - ( pSl->lPanelWidth * pSl->lxPanels ) ) / 2;
	pSl->lyMargin = ( h - ( pSl->lPanelHeight * pSl->lyPanels ) ) / 2;

	return TRUE;
}

BOOL CWinRect::GetPanelPosition( SPanelLayout *pSl, long lView, LPRECT pRect )
{_STT();

	// Sanity checks
	if ( !pSl || !pRect || lView < 0 || lView >= pSl->lNumPanels )
		return FALSE;
			  
	// Must be larger than one
	if ( pSl->lxPanels < 1 ) pSl->lxPanels = 1;
	if ( pSl->lyPanels < 1 ) pSl->lyPanels = 1;

	try
	{
		// Calculate box position and size
		long x = pSl->lxMargin + ( ( lView % pSl->lxPanels ) * pSl->lPanelWidth );
		long y = pSl->lyMargin + ( ( lView / pSl->lxPanels ) * pSl->lPanelHeight );

		// Set the position
		SetRect( pRect, x, y, x + pSl->lPanelWidth, y + pSl->lPanelHeight );

	}
	catch( ... ) { ASSERT( 0 ); return FALSE;}

	return TRUE;
}

long CWinRect::HitTest( SPanelLayout *pSl, LPPOINT pPt )
{_STT();

	// Sanity checks
	if ( !pSl || !pPt ) return -1;

	try
	{
		// Verify that the click lies within our arena
		if ( pPt->x < pSl->lxMargin || pPt->y < pSl->lyMargin ||
			 pPt->x > ( pSl->lxMargin + ( pSl->lxPanels * pSl->lPanelWidth ) ) ||
			 pPt->y > ( pSl->lyMargin + ( pSl->lyPanels * pSl->lPanelHeight ) ) )
			return -1;

		// Calculate the view this point falls in
		long i = ( ( ( pPt->y - pSl->lyMargin ) / pSl->lPanelHeight ) * pSl->lxPanels ) +
				 ( ( pPt->x - pSl->lxMargin ) / pSl->lPanelWidth );

		// Is our calculation valid?
		if ( i < 0 || i >= pSl->lNumPanels ) return i;

		return i;

	} // end try
	catch( ... ) { ASSERT( 0 ); }

	return -1;
}
