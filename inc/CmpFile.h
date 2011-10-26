/*******************************************************************
// Copyright (c) 2003, Robert Umbehant
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
// CmpFile.h: interface for the CCmpFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMPFILE_H__2A30EFB9_8ABE_4503_9E0E_8524A94B603E__INCLUDED_)
#define AFX_CMPFILE_H__2A30EFB9_8ABE_4503_9E0E_8524A94B603E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define CMPTYPE_VOID		0
#define CMPTYPE_HEADER		1
#define CMPTYPE_BIN			2
#define CMPTYPE_FILE		3
#define CMPTYPE_DIR			4
#define CMPTYPE_REG			5
#define CMPTYPE_WEB			6
#define CMPTYPE_MANIFEST	7

#define CMPTYPE_MASK		0xff

typedef struct tagCMPINFO : LLISTINFO
{
	CCfgFile	*cfg;

	LONGLONG	type;
	
	LONGLONG	user;
	TCHAR		name[ 1024 ];
	
	// Read data
	LONGLONG	foff;
	LONGLONG	blocks;
	LONGLONG	size;

	// Write data
	HKEY		hkey;
	TCHAR		str[ 1024 ];
	TCHAR		mask[ 1024 ];
	DWORD		dsize;

	LPVOID		pdata;
	LPVOID		copy;

} CMPINFO; // end typedef struct
typedef CMPINFO* LPCMPINFO;

typedef struct tagCMPCHUNKINFO
{
	DWORD		id[ 4 ];
	DWORD		sz;
	LONGLONG	size;
	LONGLONG	type;
	LONGLONG	user;
	LONGLONG	blocks;

} CMPCHUNKINFO; // end typedef struct
typedef CMPCHUNKINFO* LPCMPCHUNKINFO;

#define CMPBFLAG_ZLIB			0x00000001
#define CMPBFLAG_INFO			0x00000002
#define CMPBFLAG_MANIFEST		0x00000004

typedef struct tagCMPBLOCKINFO
{
	DWORD		id[ 4 ];
	DWORD		sz;
	DWORD		csize;
	DWORD		usize;
	DWORD		index;
	DWORD		flags;
	LONGLONG	offset;

} CMPBLOCKINFO; // end typedef struct
typedef CMPBLOCKINFO* LPCMPBLOCKINFO;

typedef struct tagCMPINDEX
{
	DWORD		md5[ 4 ];
	LONGLONG	manifest;

} CMPINDEX, *LPCMPINDEX; // end typedef struct

// Load return values
#define CMPRET_BADCHUNK		( MAXDWORD )
#define CMPRET_SKIPCHUNK	( MAXDWORD - 1 )

// Just for demo
typedef BOOL (*cmp_InitDownload)();
typedef BOOL (*cmp_EndDownload)();
typedef BOOL (*cmp_Download)( LPCTSTR pLink, LPBYTE *buf, LPDWORD size, DWORD index, DWORD total );

class CCmpFile : public CWinFile, public CHList
{

public:
	BOOL LoadManifest( LPCTSTR pFile = NULL, CCfgFile *pManifest = NULL );
	BOOL VerifyMD5();
	BOOL SaveManifest();
	BOOL NormalizeLink( LPSTR dst, LPCTSTR src );
	BOOL GetLookup( LPCTSTR pLink, LPDWORD poffset );
	BOOL AddLookup( LPCTSTR pLink, LONGLONG offset );
	BOOL Get( LPCTSTR pName, TMem< BYTE > &mem );
	BOOL InitLookup();
	DWORD SaveLinks( LPCMPINFO pCi );
	BOOL AddLinks( CHList *pLinks, LPCTSTR pName = NULL, DWORD dwUser = 0 );
	DWORD ExtractFile( LPCTSTR pName, DWORD dwUser, LPVOID buf, DWORD size );
	DWORD ExtractFile( LPCTSTR pName, DWORD dwUser, LPCTSTR pFile );
	LPCMPINFO FindFile( LPCTSTR pFile, DWORD dwUser = 0, LPSTR pLink = NULL, LPCMPINFO pLast = NULL );
	BOOL Remove( LPCTSTR pName, DWORD dwUser );
	LPCMPINFO FindCmp( LPCTSTR pName, DWORD dwUser = 0, LPCMPINFO pLast = NULL );
	BOOL GetNextBlock( LPCMPBLOCKINFO pcbi );
	BOOL FindBlock( LPCMPINFO pci, LPCMPBLOCKINFO pcbi, DWORD block = 0 );
	
	BOOL ExtractInfo( LPCMPBLOCKINFO pcbi, LPCTSTR pFile );
	BOOL ExtractInfo( LPCMPBLOCKINFO pcbi, LPVOID buf, DWORD size );
	
	BOOL Extract( LPCMPINFO pci, LPCTSTR pName, LPCTSTR pFile );
	DWORD Extract( LPCMPINFO pci, LPCTSTR pName, LPVOID buf, DWORD size );

	virtual BOOL InitObject( void *node );
	virtual DWORD GetObjSize() { return sizeof( CMPINFO ); }
	virtual void DeleteObject( void *node );

	BOOL SaveCfg( CCfgFile *pCfg );
	BOOL AddFileInfo( LPCMPINFO pCi, LPCTSTR pPath = NULL, LPDWORD index = NULL, HGROUP hGroup = NULL );
	DWORD SaveBin( LPVOID buf, DWORD size, DWORD index = 0 );
	DWORD SaveReg( HKEY hKey, LPCTSTR pKey );
	DWORD SaveDir( LPCMPINFO pCi );
	DWORD SaveFile( LPCTSTR pFile, DWORD index = 0 );
	BOOL SaveChunk( LPCMPINFO pCi );
	BOOL Store( LPCTSTR pFile );
	LPCMPINFO AddReg( LPCTSTR pReg, LPCTSTR pName = NULL, DWORD dwUser = 0 );
	LPCMPINFO AddDir( LPCTSTR pDir, LPCTSTR pMask, LPCTSTR pName = NULL, DWORD dwUser = 0 );
	LPCMPINFO AddFile( LPCTSTR pFile, LPCTSTR pName = NULL, DWORD dwUser = 0 );
	LPCMPINFO AddBin( LPVOID buf, DWORD size, LPCTSTR pName = NULL, DWORD dwUser = 0, BOOL bCopy = TRUE );
	LONGLONG LoadChunk( BOOL bHeader = FALSE );
	void Destroy();
	virtual DWORD Load( LPCTSTR pFile, BOOL bMerge = FALSE );
	CCmpFile();
	virtual ~CCmpFile();

	CCfgFile* GetHeader() { return &m_header; }

	void SetDownloadInterface( cmp_InitDownload fnIF, cmp_Download fnD, cmp_EndDownload fnED )
	{	m_fnInitDownload = fnIF; m_fnDownload = fnD; m_fnEndDownload = fnED; }

	DWORD Get( LPCTSTR pName, DWORD dwUser, TMem< BYTE > &mem )
	{	DWORD dwSize = ExtractFile( pName, dwUser, NULL, 0 );
		if ( mem.allocate( dwSize ) && ExtractFile( pName, dwUser, mem.ptr(), mem.size() ) )
			return dwSize;
		mem.destroy(); return 0;
	}

private:

	DWORD				m_dwBadChunks;
	DWORD				m_dwBadBlocks;
	LONGLONG			m_llLastGoodChunk;
	DWORD				m_dwChunks;
	CCfgFile			m_header;
	CCfgFile			m_manifest;
	DWORD				m_dwIndex;

	cmp_InitDownload	m_fnInitDownload;
	cmp_Download		m_fnDownload;
	cmp_EndDownload		m_fnEndDownload;

	CHList				m_lookup;

	CMPINDEX			m_index;
	
};

#endif // !defined(AFX_CMPFILE_H__2A30EFB9_8ABE_4503_9E0E_8524A94B603E__INCLUDED_)
