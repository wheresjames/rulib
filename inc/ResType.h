// ResType.h: interface for the CResType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTYPE_H__430CC840_D201_4EF9_A2FA_8292E40D2101__INCLUDED_)
#define AFX_RESTYPE_H__430CC840_D201_4EF9_A2FA_8292E40D2101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HList.h"

typedef struct tagRESTYPEINFO : LLISTINFO
{

} RESTYPEINFO, *LPRESTYPEINFO; // end typedef struct


#pragma pack( push )
#pragma pack( 2 )

typedef struct
{
	BYTE	bWidth;			// Width, in pixels, of the image
	BYTE	bHeight;		// Height, in pixels, of the image
	BYTE	bColorCount;	// Number of colors in image (0 if >=8bpp)
	BYTE	bReserved;		// Reserved
	WORD	wPlanes;		// Color Planes
	WORD	wBitCount;		// Bits per pixel
	DWORD	dwBytesInRes;	// how many bytes in this resource?
	WORD	nID;			// the ID

} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct 
{
	WORD				idReserved; 	// Reserved (must be 0)
	WORD				idType; 		// Resource type (1 for icons)
	WORD				idCount;		// How many images?
	GRPICONDIRENTRY 	idEntries[1];	// The entries for each image
} GRPICONDIR, *LPGRPICONDIR;

#pragma pack( pop )


class CResType : public CHList  
{
public:

	void FreeMod();
	BOOL Enum( LPCTSTR pFile, LPCTSTR pType, DWORD max = MAXDWORD );
	LPRESTYPEINFO FindRes( LPCTSTR pRes );
	HICON LoadIcon( LPRESTYPEINFO prti, DWORD i = 0, LPDWORD count = NULL );
	BOOL Add( LPCTSTR pName );
	BOOL OnEnum( HMODULE hModule, LPCTSTR pType, LPCTSTR pName );
	BOOL Enum( HMODULE hMod, LPCTSTR pType, DWORD max = MAXDWORD );
	CResType();
	virtual ~CResType();

	virtual DWORD GetObjSize() { return sizeof( RESTYPEINFO ); }
	virtual void Destroy();

	LPCTSTR GetType() { return GetType( m_dwType, m_szType ); }

	static LPCTSTR GetType( DWORD dwType, LPCTSTR pType );
	static BOOL LoadResource( LPCTSTR pResource, LPBYTE pPtr, LPDWORD pdwSize, LPCTSTR pType, HMODULE hModule );


private:

	static BOOL CALLBACK EnumResNameProc(	HMODULE hModule, LPCTSTR lpszType, 
											LPTSTR lpszName, LPARAM lParam );

	BOOL		m_bUnload;
	HMODULE		m_hModule;
	char		m_szType[ CWF_STRSIZE ];
	DWORD		m_dwType;
	DWORD		m_dwMaxEnum;

};

#endif // !defined(AFX_RESTYPE_H__430CC840_D201_4EF9_A2FA_8292E40D2101__INCLUDED_)
