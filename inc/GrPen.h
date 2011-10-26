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
// GrPen.h: interface for the CGrPen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPEN_H__49A61F7A_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
#define AFX_GRPEN_H__49A61F7A_AC0F_11D4_821A_0050DA205C15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CGrPen
//
/// Simple wrapper for GDI HPEN object
/**
	Simple wrapper for GDI HPEN object.	
*/
//==================================================================
class CGrPen  
{
public:

	//==============================================================
	// Create()
	//==============================================================
	/// Creates an HPEN object
	/**
		\param [in] nStyle	-	Pen style
		\param [in] nWidth	-	Pen width
		\param [in] rgb		-	Pen color
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( int nStyle, int nWidth, COLORREF rgb );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Destroys the encapsulated HPEN object
	void Destroy();

	/// Default constructor
	CGrPen();

	//==============================================================
	// CGrPen()
	//==============================================================
	/// Wraps an existing HPEN object
	/**
		\param [in] hPen	-	Existing HPEN object
	*/
	CGrPen( HPEN hPen );

	//==============================================================
	// CGrPen()
	//==============================================================
	/// Constructs the specified pen
	/**
		\param [in] nStyle	-	Pen style
		\param [in] nWidth	-	Pen width
		\param [in] rgb		-	Pen color
	*/
	CGrPen( int nStyle, int nWidth, COLORREF rgb );

	/// Destructor
	virtual ~CGrPen();

	//==============================================================
	// HPEN()
	//==============================================================
	/// Returns encapsulated HPEN handle
	operator HPEN() { return m_hPen; }

	//==============================================================
	// GetSafeHpen()
	//==============================================================
	/// Returns encapsulated HPEN handle
	HPEN GetSafeHpen() { return m_hPen; }

	//==============================================================
	// IsObject()
	//==============================================================
	/// Returns non-zero if there is a valid HPEN object
	BOOL IsObject() { return ( m_hPen != NULL ); }

	//==============================================================
	// Attach()
	//==============================================================
	/// Encapsulates an existing HPEN object
	/**
		\param [in] hPen	-	Existing HPEN object
	*/
	void Attach( HPEN hPen ) { Destroy(); m_hPen = hPen; }

	//==============================================================
	// Detach()
	//==============================================================
	/// Detaches from encapsulated HPEN object without releasing it
	void Detach() { m_hPen = NULL; }

private:

	/// Handle to encapsulated HPEN object
	HPEN	m_hPen;

};

#endif // !defined(AFX_GRPEN_H__49A61F7A_AC0F_11D4_821A_0050DA205C15__INCLUDED_)
