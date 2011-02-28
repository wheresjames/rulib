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
// MovableObj.h: interface for the CMovableObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVABLEOBJ_H__4FA021CB_04CA_4179_9A2A_F376AC3462D7__INCLUDED_)
#define AFX_MOVABLEOBJ_H__4FA021CB_04CA_4179_9A2A_F376AC3462D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MOCUR_LEFT			0x00000001
#define MOCUR_RIGHT			0x00000002
#define MOCUR_TOP			0x00000004
#define MOCUR_BOTTOM		0x00000008
#define MOCUR_MOVE			0x00000010
#define MOCUR_ROTATE1		0x00000020
#define MOCUR_ROTATE2		0x00000040
#define MOCUR_ROTATE3		0x00000080
#define MOCUR_ROTATE4		0x00000100

#define MOOBJ_MAXANGLES		8

//==================================================================
// CMovableObj
//
/// Assists in building graphical editors by managing object movement.
/**
	This class assists in graphical editor building by managing user
	interactions with elements.  Simple interactions such as movement,
	rectangular resizing, and rotation are supported.	
*/
//==================================================================
class CMovableObj  
{
public:

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources
	void Destroy();
	

	//==============================================================
	// Move()
	//==============================================================
	/// Moves objects if needed
	/**
		\param [in] pPt		-	Cursor position
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Move( LPPOINT pPt );

	//==============================================================
	// OnMouseMove()
	//==============================================================
	/// Call in response to a WM_MOUSEMOVE message
	/**
		\param [in] uFlags	-	Mouse flags
		\param [in] pPt		-	Cursor position
		
		\return Non-zero if redraw is required
	
		\see 
	*/
	virtual BOOL OnMouseMove( UINT uFlags, LPPOINT pPt );

	//==============================================================
	// OnLButtonDown()
	//==============================================================
	/// Call in response to a WM_LBUTTONDOWN message
	/**
		\param [in] uFlags	-	Mouse flags
		\param [in] pPt		-	Cursor position
		
		\return Non-zero if redraw is required
	
		\see 
	*/
	virtual BOOL OnLButtonDown( UINT uFlags, LPPOINT pPt );

	//==============================================================
	// OnLButtonUp()
	//==============================================================
	/// Call in response to a WM_LBUTTONUP message
	/**
		\param [in] uFlags	-	Mouse flags
		\param [in] pPt		-	Cursor position
		
		\return Non-zero if redraw is required
	
		\see 
	*/
	virtual BOOL OnLButtonUp( UINT uFlags, LPPOINT pPt );

	//==============================================================
	// SetCursor()
	//==============================================================
	/// Sets the needed mouse cursor
	/**
		\param [in] pt	-	Cursor position

		Call this from within mouse move and click events to set the
		appropriate mouse cursor.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetCursor( LPPOINT pt );

	//==============================================================
	// GetCursor()
	//==============================================================
	/// Returns the appropriate mouse cursor for a user action
	/**
		\param [in] dwCursor	-	User interaction code
		
		\return Handle to cursor
	
		\see 
	*/
	virtual HCURSOR GetCursor( DWORD dwCursor );

	//==============================================================
	// GetCursorFlags()
	//==============================================================
	/// Returns the user interaction code for a particular cursor position
	/**
		\param [in] pPt		-	Cursor position
		
		\return User interaction code / Cursor flags
	
		\see 
	*/
	DWORD GetCursorFlags( LPPOINT pPt );

	//==============================================================
	// Draw()
	//==============================================================
	/// Draws a select box around the active area
	/**
		\param [in] hDC		-	Handle to DC that receives the image
		\param [in] pRect	-	Size of the area in hDC
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Draw( HDC hDC, LPRECT pRect );

	/// Default constructor
	CMovableObj( LPRECT pRect = NULL );
	
	/// Destructor
	virtual ~CMovableObj();
		
	//==============================================================
	// GetRect()
	//==============================================================
	/// Returns the current size and position
	LPRECT GetRect() { return &m_rect; }

	//==============================================================
	// operator LPRECT()
	//==============================================================
	/// Returns the current size and position
	operator LPRECT() { return &m_rect; }

	//==============================================================
	// SetAngle()
	//==============================================================
	/// Sets the object rotation angle for the specified index
	/**
		\param [in] i	-	Angle index
		\param [in] v	-	Angle value
		
		\return Value in v
	
		\see 
	*/
	double SetAngle( DWORD i, double v ) 
	{	if ( i < MOOBJ_MAXANGLES ) m_dAngle[ i ] = v; return v; }

	//==============================================================
	// SetDistance()
	//==============================================================
	/// Sets the distance for resolution calculations
	/**
		\param [in] v	-	Distance value
	*/
	void SetDistance( double v ) { m_dDistance = v; }

	//==============================================================
	// GetAngle()
	//==============================================================
	/// Returns the specified angle value
	/**
		\param [in] i	-	Angle index
		
		\return Angle value for index i
	
		\see 
	*/
	double GetAngle( DWORD i ) 
	{	if ( i < MOOBJ_MAXANGLES ) return m_dAngle[ i ]; return 0; }

	//==============================================================
	// GetDistance()
	//==============================================================
	/// Returns the distance value used in resolution calculations
	double GetDistance() { return m_dDistance; }

	//==============================================================
	// GetX()
	//==============================================================
	/// Returns object horizontal offset
	long GetX() { return m_rect.left + ( RW( m_rect ) >> 1 ); }

	//==============================================================
	// GetY()
	//==============================================================
	/// Returns object vertical offset
	long GetY() { return m_rect.top + ( RH( m_rect ) >> 1 ); }

	//==============================================================
	// SetRect()
	//==============================================================
	/// Sets the object size and position
	/**
		\param [in] pRect	-	Object size and position
	*/
	void SetRect( LPRECT pRect ) { CopyRect( &m_rect, pRect ); }

	//==============================================================
	// GetRect()
	//==============================================================
	/// Returns the objects size and position
	/**
		\param [out] pRect	-	Receives object size and position
	*/
	void GetRect( LPRECT pRect ) { CopyRect( pRect, &m_rect ); }

	//==============================================================
	// GetWidth()
	//==============================================================
	/// Returns the object width
	long GetWidth() { return RW( m_rect ); }

	//==============================================================
	// GetHeight()
	//==============================================================
	/// Returns the object height
	long GetHeight() { return RH( m_rect ); }

	//==============================================================
	// SetOffset()
	//==============================================================
	/// Sets the objects horizontal and vertical offset
	/**
		\param [in] x	-	Horizontal offset
		\param [in] y	-	Vertical offset
	*/
	void SetOffset( long x, long y ) { m_lXOff = x; m_lYOff = y; }

	//==============================================================
	// Left()
	//==============================================================
	/// Returns the horizontal offset of the objects left side
	long Left() { return m_rect.left; }

	//==============================================================
	// Top()
	//==============================================================
	/// Returns the horizontal offset of the objects top side
	long Top() { return m_rect.top; }

	//==============================================================
	// Right()
	//==============================================================
	/// Returns the horizontal offset of the objects right side
	long Right() { return m_rect.right; }

	//==============================================================
	// Bottom()
	//==============================================================
	/// Returns the horizontal offset of the objects bottom side
	long Bottom() { return m_rect.bottom; }

	//==============================================================
	// OnDraw()
	//==============================================================
	/// Called when the object needs redrawing
	/**
		\param [in] hDc		-	Handle to DC for drawing
		\param [in] pRect	-	Size of hDC

		If you derive from this class, you can provide custom drawing
		here.
		
		\return Non-zero if drawing is done
	
		\see 
	*/
	virtual BOOL OnDraw( HDC hDc, LPRECT pRect )
	{	return FALSE; }

	//==============================================================
	// Select()
	//==============================================================
	/// Set the select state
	/**
		\param [in] bSelect		-	Selected state
	*/
	void Select( BOOL bSelect ) { m_bSelected = bSelect; }

	//==============================================================
	// IsSelected()
	//==============================================================
	/// Returns non-zero if object is selected
	BOOL IsSelected() { return m_bSelected; }

	//==============================================================
	// PtInRect()
	//==============================================================
	/// Returns non-zero if the specified point is within the object bounds
	/**
		\param [in] pt	-	Point to test
		
		\return Non-zero if the point is within the object bounds
	
		\see 
	*/
	BOOL PtInRect( LPPOINT pt ) { return ( pt && ::PtInRect( &m_rect, *pt ) ); }

private:

	/// Size and position of object
	RECT		m_rect;

	/// Object angles
	double		m_dAngle[ MOOBJ_MAXANGLES ];

	/// Object distance
	double		m_dDistance;

	/// Object horizontal click margin
	long		m_lXMargin;

	/// Object vertical click margin
	long		m_lYMargin;

	/// Object minimum horizontal size
	long		m_lXMin;

	/// Object minimum vertical size
	long		m_lYMin;

	/// Object horizontal offset
	long		m_lXOff;

	/// Object vertical offset
	long		m_lYOff;

	/// Non-zero if object is selected
	BOOL		m_bSelected;

	/// Non-zero if mouse button is down
	BOOL		m_bButtonDown;

	/// Point of last mouse click
	POINT		m_ptClick;

	/// Mouse flags, user interaction indication
	DWORD		m_dwClickFlags;

};

#endif // !defined(AFX_MOVABLEOBJ_H__4FA021CB_04CA_4179_9A2A_F376AC3462D7__INCLUDED_)
