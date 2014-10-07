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

#pragma once

#include "..\CxImage\xImage.h"

static void* _cximage_malloc( size_t size )
{
	return malloc( size );
}

static void* _cximage_calloc( size_t num, size_t size )
{
	return calloc( num, size );
}

static void* _cximage_realloc( void *ptr, size_t size )
{
	return realloc( ptr, size );
}

static void _cximage_free( void *ptr )
{
	free( ptr );
}

extern "C"
{	t_cx_malloc cximage_malloc = _cximage_malloc;
	t_cx_calloc cximage_calloc = _cximage_calloc;
	t_cx_realloc cximage_realloc = _cximage_realloc;
	t_cx_free cximage_free = _cximage_free;
};

