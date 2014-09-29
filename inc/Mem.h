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
// Mem.h: interface for the CMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEM_H__11C20F70_E8D8_4A83_89F9_51E5776956F5__INCLUDED_)
#define AFX_MEM_H__11C20F70_E8D8_4A83_89F9_51E5776956F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMem  
{
public:

	
	CMem();
	virtual ~CMem();

	static LPSTR NewStr(DWORD size);
	static LPSTR NewStr(LPCTSTR pStr);
	static void DeleteStr(LPSTR *pBuf);

	static void* New( DWORD size );
	static void* New(DWORD size, BYTE init );

};

#endif // !defined(AFX_MEM_H__11C20F70_E8D8_4A83_89F9_51E5776956F5__INCLUDED_)
