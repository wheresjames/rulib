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
// GrBrush.h: interface for the CGrBrush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRBRUSH_H__49A61F7B_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
#define AFX_GRBRUSH_H__49A61F7B_AC0F_11D4_821A_0050DA205C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CGrBrush
//
/// Wraps the Windows GDI HBRUSH object
/**
	Simple wrapper class for the windows GDI HBRUSH object.	
*/
//==================================================================
class CGrBrush  
{
public:

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a bitmap brush from an HBITMAP object
	/**
		\param [in] hBmp	-	Valid HBITMAP
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( HBITMAP hBmp );
	
	//==============================================================
	// Create()
	//==============================================================
	/// Creates the specified style and color brush
	/**
		\param [in] nStyle	-	Brush style
		\param [in] rgb 
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( int nStyle, COLORREF rgb );

	//==============================================================
	// Create()
	//==============================================================
	/// Creates a solid brush of the specified color
	/**
		\param [in] rgb		-	Brush color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( COLORREF rgb );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the brush
	void Destroy();

	/// Default constructor
	CGrBrush();

	//==============================================================
	// CGrBrush()
	//==============================================================
	/// Wraps an existing brush
	/**
		\param [in] hBrush	-	Existing HBRUSH object
	*/
	CGrBrush( HBRUSH hBrush );

	//==============================================================
	// CGrBrush()
	//==============================================================
	/// Creates the specified style and color brush
	/**
		\param [in] nStyle	-	Brush style
		\param [in] rgb 
	*/
	CGrBrush( int nStyle, COLORREF rgb );

	//==============================================================
	// CGrBrush()
	//==============================================================
	/// Creates a solid brush of the specified color
	/**
		\param [in] rgb		-	Brush color
	*/
	CGrBrush( COLORREF rgb );

	//==============================================================
	// CGrBrush()
	//==============================================================
	/// Creates a bitmap brush from an HBITMAP object
	/**
		\param [in] hBmp	-	Valid HBITMAP
	*/
	CGrBrush( HBITMAP hBmp );

	/// Destructor
	virtual ~CGrBrush();

	//==============================================================
	// HBRUSH()
	//==============================================================
	/// Returns the HBRUSH
	/**		
		\return HBRUSH
	*/
	operator HBRUSH() { return m_hBrush; }

	//==============================================================
	// GetSafeHbrush()
	//==============================================================
	/// Returns the HBRUSH
	/**
		\return HBRUSH
	*/
	HBRUSH GetSafeHbrush() { return m_hBrush; }

	//==============================================================
	// IsObject()
	//==============================================================
	/// Returns non-zero if there is a valid HBRUSH handle
	BOOL IsObject() { return ( m_hBrush != NULL ); }

	//==============================================================
	// Attach()
	//==============================================================
	/// Attaches to an existing HBRUSH object
	/**
		\param [in] hBrush	-	Existing HBRUSH object
	*/
	void Attach( HBRUSH hBrush ) { Destroy(); m_hBrush = hBrush; }

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from an HBRUSH without releasing it
	/**
		
		\return 
	
		\see 
	*/
	HBRUSH Detach() { HBRUSH hBrush = m_hBrush; m_hBrush = NULL; return hBrush; }

private:

	/// The Windows GDI HBRUSH object handle
	HBRUSH	m_hBrush;

};

#endif // !defined(AFX_GRBRUSH_H__49A61F7B_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
