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
// WinDropTarget.h: interface for the CWinDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __ATLBASE_H__

/// Simple replacement for CComPtr in atlbase.h
template < typename T >
	class CComPtr
{
public:

	/// Default constructor
	CComPtr()
	{
		p = NULL;
	}

	/// Copy constructor
	CComPtr( const CComPtr< T > &r )
	{
		p = r.p;
		if ( p )
			p->AddRef();
	}

	/// Copy constructor
	CComPtr( T *t )
	{
		p = t;
		if ( p )
			p->AddRef();
	}

	/// Destructor
	~CComPtr()
	{	Release();
	}

	/// Releases the com pointer
	void Release()
	{
		if ( p )
			p->Release();
		p = NULL;
	}

	/// Attach to an existing pointer
	void Attach( T* t )
	{
		Release();
		p = t;
	}

	/// Detach from encapsulated pointer
	T* Detach()
	{
		T* t = p;
		p = NULL;
		return t;
	}

	/// Create object instance
	HRESULT CoCreateInstance( REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL, REFIID riid = __uuidof( T ), LPVOID *ppv = NULL )
	{	Release();
		HRESULT hr = ::CoCreateInstance( rclsid, pUnkOuter, dwClsContext, riid, (void**)&p);
		if ( ppv )
			*ppv = p;
		return hr;
	}

	// operators
	operator bool() { return NULL != p; }
	T** operator &() { return &p; }
	T* operator ->() { return p; }
	operator T&() { return *p; }
	operator T*() { return p; }
	CComPtr< T >& operator = ( const CComPtr< T > &r ) { p = r.p; if ( p ) p->AddRef(); return *this; }
	CComPtr< T >& operator = ( T *t ) { p = t; if ( p ) p->AddRef(); return *this; }

public:

	// Must be the same as the real CComPtr
	T *p;

};

/// Simple replacement for CComBSTR in atlbase.h
class CComBSTR
{
public:

	/// Default constructor
	CComBSTR()
	{
		bstr = NULL;
	}

	/// From multi byte string
	CComBSTR( int sz )
	{
		bstr = ::SysAllocStringLen( NULL, sz );
	}

	/// From multi byte string
	CComBSTR( LPCSTR pStr )
	{
		bstr = NULL;
		fromMb( pStr );
	}

	/// From multi byte string
	CComBSTR( BSTR pStr )
	{
		bstr = NULL;
		fromWc( pStr );
	}

	~CComBSTR()
	{
		Free();
	}

	/// Free the string
	void Free()
	{
		::SysFreeString( bstr );
	}

	CComBSTR& fromWc( BSTR pStr )
	{
		Free();

		if ( pStr )
			bstr = ::SysAllocStringLen( pStr, wcslen( pStr ) );

		return *this;
	}
	
	CComBSTR& fromMb( LPCSTR pStr )
	{
		Free();

		if ( pStr )
		{
			// Convert to wide char
			int nSz = MultiByteToWideChar( CP_ACP, 0, pStr, -1, 0, 0 );
			if ( 0 < nSz )
			{
				// Create string
				bstr = ::SysAllocStringLen( NULL, nSz );
				if ( MultiByteToWideChar( CP_ACP, 0, pStr, -1, bstr, nSz ) != nSz )
					Free();

			} // end if

		} // end if

		return *this;
	}

	operator BSTR() { ASSERT( bstr ); return bstr; }

private:

	BSTR bstr;

};

#endif