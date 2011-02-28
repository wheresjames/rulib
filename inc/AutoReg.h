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
// Reg.h: interface for the CAutoReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REG_H__32DBBA44_11A7_11D2_8353_00104B2C9CFA__INCLUDED_)
#define AFX_REG_H__32DBBA44_11A7_11D2_8353_00104B2C9CFA__INCLUDED_

//#include "Instance.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma pack ( push, Global_Vars, 1 )
 
/// Registry entry information
typedef struct tagREGENTRY
{
	DWORD	dwUser;
	char	szComputer[ MAX_PATH ];
	HKEY	hKey;
	char	szKey[ MAX_PATH ];
	char	szName[ MAX_PATH ];
	DWORD	dwType;
	DWORD	dwLength;
	union
	{	void*	pvoidData;				// The Variable containing data

		void*	regBinary;				// REG_BINARY 
		DWORD*	regDword;				// REG_DWORD 
		DWORD*	regDwordLittleEndian;	// REG_DWORD_LITTLE_ENDIAN 
		DWORD*	regDwordBigEndian;		// REG_DWORD_BIG_ENDIAN 
		char*	regExpandSz;			// REG_EXPAND_SZ 
		char*	regLink;				// REG_LINK 
		char*	regMultiSz;				// REG_MULTI_SZ 
		void*	regNone;				// REG_NONE 
		char*	regResourceList;		// REG_RESOURCE_LIST 
		char*	regSz;					// REG_SZ 
	}; // end union

	DWORD	dwDef;
	char	szDef[ MAX_PATH ];
	DWORD	dwDefLength;
	DWORD	dwDefSource;
	union
	{	void*	pvoidDef;				// The Variable containing data

		void*	defBinary;				// REG_BINARY 
		DWORD*	defDword;				// REG_DWORD 
		DWORD*	defDwordLittleEndian;	// REG_DWORD_LITTLE_ENDIAN 
		DWORD*	defDwordBigEndian;		// REG_DWORD_BIG_ENDIAN 
		char*	defExpandSz;			// REG_EXPAND_SZ 
		char*	defLink;				// REG_LINK 
		char*	defMultiSz;				// REG_MULTI_SZ 
		void*	defNone;				// REG_NONE 
		char*	defResourceList;		// REG_RESOURCE_LIST 
		char*	defSz;					// REG_SZ 
	}; // end union

} REGENTRY;
typedef REGENTRY* LPREGENTRY;

//==================================================================
// CAutoRegEntry
//
/// Holds information on a single registry entry.
//==================================================================
class CAutoRegEntry : public REGENTRY
{
public:

	/// Constructor
	CAutoRegEntry() { ZeroMemory( (LPVOID)(REGENTRY*)this, sizeof( REGENTRY ) ); }

	bool operator == ( DWORD index ) { return index == dwUser; }
	bool operator == ( const CAutoRegEntry& x ) { return x.dwUser == dwUser; }
	bool operator == ( const char * x ) { return ( strcmpi( x, szName ) == 0 ); }
	CAutoRegEntry& operator = ( const CAutoRegEntry& x ) 
	{	memcpy( (LPVOID)(REGENTRY*)this, (LPVOID)(REGENTRY*)&x, sizeof( REGENTRY ) );
		return *this; }	

}; // end class

#pragma pack ( pop, Global_Vars )

//==================================================================
// CAutoReg
//
/// Provides registry read write capability.
/**
	The primary purpose of this class is to read values on remote computers
	using the Windows Registry API.  If you only want registry values from 
	the local machine, use CReg instead.
*/
//==================================================================
class CAutoReg  
{
	/// For handling linked list
	template<class T> class list
	{

	public:

		list ( ) { last = next = NULL; }
		list ( const T& x ) { val = x; last = next = NULL; }

		~list() { RELEASE( next ); }

		list& operator = ( const T& x ) { val = x; return *this; }
		list& operator = ( const list &x ) { val = x.val; return *this; }

		operator T() { return val; }

	// Once again, ( darn it Microsoft )
	// This suppresses the compiler warning when this
	// template is used with a common data type
	// **** There is a bug in MSVC++ 5.0 that may cause it to choke on
	//		these pragmas.  If this happens you'll have to comment them 
	//		and live with the warnings
	#pragma warning( disable : 4284 )
		T* operator ->() { return &val; } 
	#pragma warning( default : 4284 )
		// These are supposed to be equal according to Bjarne
		T* operator *() { return &val; }
		T& operator &() { return val; }

		T& operator [] ( const unsigned long x ) 
		{	if ( next ) return next->index( x ); else return val; }

		// Adds an entry to the end of the list
		list& operator += ( const T& x ) 
		{	if ( last ) last = last->add( x ); else last = add( x ); return *this; }

		// Inserts an entry to the begining of the list
		// Sorry, couldn't think of a good operator
		bool insert( const T& x )
		{ list *n = new list( x ); if ( n ) { n->next = next; next = n; return true; } return false; }

		// Remove Specific Entry
		list& operator -= ( const T& x ) { remove( x ); return *this; }

		list& operator -- ( ) // Prefix: Remove The First Entry
		{	if ( next && next->next ) { list *n = next; next = n->next; n->next = NULL; RELEASE( n ); }
		else { RELEASE( next ); last = NULL; } return *this; }

		list& operator -- ( int ) // Postfix: Remove The Last Entry
		{	if ( next && next->next ) { (*next)--; } else RELEASE( next ); last = NULL; return *this; }

		// removes the specified number of entrys from the *start* of the list
		list& operator <<= ( unsigned long x ) { while ( x-- && next ) --(*this); return *this; }
		// removes the specified number of entrys from the *end* of the list
		list& operator >>= ( unsigned long x ) { while ( x-- && next ) (*this)--; return *this; }

		bool operator == ( const T& x ) // Is Equal (Is In List)
		{	if ( val==x ) return true; else if ( next ) return ( (*next)==x ); else return false; }
		// Is Not Equal (Is Not In List)
		bool operator != ( const T& x ) {	return !( (*this)==x ); }

		// Don't call this repeatedly since it requires a list search to get the size
		// We could save it locally in this class, but that would require another variable 
		// and we already have 3
		unsigned long size() { if ( next ) return next->size()+1L; return 0L; }

		void empty() { if ( next ) next->empty(); RELEASE( next ); last = NULL; }

	private:
		T& index ( const unsigned long x )
		{ if ( x && next ) return next->index( x - 1 ); return val; }
		
		list* add( const T& x ) 
		{	if ( next ) return next->add( x ); else return (next = new list( x )); } 

		void remove( const T& x ) 
		{	if ( next ) { if ( (*next) == x ) {	if ( next->next ) --(*next); 
												else RELEASE( next ); }
						  else next->remove( x ); } }

		list	*next, *last;
		T		val;

	}; // end template 


public:

	/// Constructor
	CAutoReg();

	/// Destructor
	virtual ~CAutoReg();

public:

	//==============================================================
	// GetEntry()
	//==============================================================
	/// Returns a single registry entry by name
	/**
		\param [in] pName	-	The registry value name.

		\return Returns a pointer to the REGENTRY structure if found.  Otherwise returns NULL.
	*/
	LPREGENTRY GetEntry( LPCTSTR pName );

	// Static functions

	//==============================================================
	// Default()
	//==============================================================
	/// Sets default values in a registry item
	/**
		\param [in] pReg	-	Pointer to REGENTRY structure.

		\return Returns non-zero if success.
	*/
	static BOOL Default( LPREGENTRY pReg );

	//==============================================================
	// SetDefault()
	//==============================================================
	/// Sets default values in a registry item
	/**
		\param [in] pReg		-	Pointer to REGENTRY structure.
		\param [in] dwDef		-	Default DWORD value.
		\param [in] pvoidDef	-	Default value buffer.
		\param [in] dwDefLength	-	Size of buffer in pvoidDef.

		\return Returns non-zero if success.
	*/
	static BOOL SetDefault( LPREGENTRY pReg, DWORD dwDef = 0, 
							void* pvoidDef = NULL, DWORD dwDefLength = 0 );

	//==============================================================
	// ReadReg()
	//==============================================================
	/// Reads the registry value
	/**
		\param [in] pName		-	The name of the registry key to read.
		\param [out] pReg		-	Pointer to the registry entry structure.
		\param [in] pComputer	-	The computer name to read from or NULL for local machine.

		\return Returns non-zero if success.
	*/
	static BOOL ReadReg( LPCTSTR pName,  LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// WriteReg()
	//==============================================================
	/// Reads the registry value
	/**
		\param [in] pName		-	The name of the registry key to write.
		\param [in] pReg		-	Pointer to the registry entry structure.
		\param [in] pComputer	-	The computer name to write to or NULL for local machine.

		\return Returns non-zero if success.
	*/
	static BOOL WriteReg( LPCTSTR pName,  LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// TestAccess()
	//==============================================================
	/// Returns information about the allowed access to the specified key
	/**
		\param [in] pName		-	The name of the registry key to test.
		\param [in] pReg		-	Pointer to the registry entry structure.
		\param [in] pComputer	-	The computer name to write to or NULL for local machine.

		\return Retuns a value indicating the access rights available for the registry key.
	*/
	static DWORD TestAccess( LPCTSTR pName, LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// AccessString()
	//==============================================================
	/// Returns a human readable string describing access level.
	/**
		\param [in] access	-	Access level.

		\return Returns a pointer to a string describing the specified access level.
	*/
	static char* AccessString( DWORD access );

	//==============================================================
	// CreateEntry()
	//==============================================================
	/// Creates the specified registry key
	/**
		\param [in] pReg		-	Pointer to the registry entry structure.
		\param [in] hKey		-	Handle to the registry key.
		\param [in] pKey		-	Name of the registry key.
		\param [in] dwType		-	The type of value.
		\param [in] pName		-	The name of the value.
		\param [in] pvoidData	-	The data for the key.
		\param [in] dwLength	-	Length of the buffer in pvoidData.
		\param [in] dwDef		-	Default DWORD value for the key.
		\param [in] pvoidDef	-	Default buffer value for the key.
		\param [in] dwDefLength	-	Length of the buffer in pvoidDef.
		\param [in] dwUser		-	User value.
		\param [in] pComputer	-	The address of the remote computer.

		\return Returns non-zero if success.
	*/
	static BOOL CreateEntry(	
		LPREGENTRY pReg, HKEY hKey, LPCTSTR pKey, DWORD dwType,
		LPCTSTR pName, void* pvoidData, DWORD dwLength,
		DWORD dwDef = 0, void* pvoidDef = NULL, DWORD dwDefLength = 0,
		DWORD dwUser = 0, LPCTSTR pComputer = NULL );

	//==============================================================
	// DeleteEntry()
	//==============================================================
	/// Deletes the specified entry from the registry
	/**
		\param [in] pName		-	The name of the registry key to write.
		\param [in] pReg		-	Pointer to the registry entry structure.
		\param [in] pComputer	-	The computer name to write to or NULL for local machine.

		\return Returns non-zero if success.
	*/
	static BOOL DeleteEntry ( LPCTSTR pName, LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// DeleteSubKeys()
	//==============================================================
	/// Deletes specified keys sub keys.
	/**
		\param [in] hRoot	-	The root of the key to delete.
		\param [in] pKey	-	The name of the key to delete.

		\return Returns non-zero if success.
	*/
	static BOOL DeleteSubKeys ( HKEY hRoot, LPCTSTR pKey );

	//==============================================================
	// DeleteSubKeys()
	//==============================================================
	/// Deletes specified keys sub keys.
	/**
		\param [in] pName		-	The name of the registry key to write.
		\param [in] pReg		-	Pointer to the registry entry structure.
		\param [in] pComputer	-	The computer name to write to or NULL for local machine.

		\return Returns non-zero if success.
	*/
	static BOOL DeleteSubKeys ( LPCTSTR pName, LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// GetSubKey()
	//==============================================================
	/// Returns information from the specified sub key
	/**
		\param [in] index		-	Sub key index.
		\param [in] pDest		-	Destination reg entry structure.
		\param [in] pName		-	Name of the sub key
		\param [in] pReg		-	Registry entry information.
		\param [in] pComputer	-	The computer name to write to or NULL for local machine.

		\return Returns non-zero if success.
	*/
	static BOOL GetSubKey( DWORD index, LPREGENTRY pDest, LPCTSTR pName, LPREGENTRY pReg, LPCTSTR pComputer );

	//==============================================================
	// TestAccess()
	//==============================================================
	/// Returns information about the allowed access to the specified key
	/**
		\param [in] pName		-	The name of the registry key to test.
		\param [in] pReg		-	Pointer to the registry entry structure.

		\return Retuns a value indicating the access rights available for the registry key.
	*/
	DWORD TestAccess( LPCTSTR pName = NULL, LPREGENTRY pReg = NULL );

	//==============================================================
	// Restore()
	//==============================================================
	/// Restores a registry key from backup
	/**
		\param [in] pName		-	Filename containing registry information.
		\param [in] pdwNumRead	-	Number of registry entries restored.

		\return Returns non-zero if success.
	*/
	virtual BOOL Restore( LPCTSTR pName = NULL, DWORD *pdwNumRead = NULL );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves registry information to a file
	/**
		\param [in] pName		-	Filename.

		\return Returns non-zero if success.
	*/
	virtual BOOL Save( LPCTSTR pName = NULL );

	//==============================================================
	// DefaultAll()
	//==============================================================
	/// Sets default values.
	void DefaultAll();

	//==============================================================
	// Get()
	//==============================================================
	/// Returns information about a specified value
	/**
		\param [out] pRE	-	Registry entry information.
		\param [in] dwIndex	-	The index of the registry entry.

		\return Returns non-zero if success.
	*/
	BOOL Get( LPREGENTRY pRE, DWORD dwIndex );

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the total number of registry entry items.
	DWORD Size();

	//==============================================================
	// RemoveAll()
	//==============================================================
	/// Deletes all registry entry items.
	void RemoveAll();

	//==============================================================
	// AddEntry()
	//==============================================================
	/// Adds an entry to the list of items
	/**
		\param [in] hKey		-	Handle to the registry key.
		\param [in] pKey		-	Name of the registry key.
		\param [in] dwType		-	The type of value.
		\param [in] pName		-	The name of the value.
		\param [in] pvoidData	-	The data for the key.
		\param [in] dwLength	-	Length of the buffer in pvoidData.
		\param [in] dwDef		-	Default DWORD value for the key.
		\param [in] pvoidDef	-	Default buffer value for the key.
		\param [in] dwDefLength	-	Length of the buffer in pvoidDef.
		\param [in] dwUser		-	User value.
		\param [in] pComputer	-	The address of the remote computer.

		\return Returns non-zero if success.
	*/
	BOOL AddEntry(	HKEY hKey, LPCTSTR pKey, DWORD dwType,
					LPCTSTR pName, void* pvoidData, DWORD dwLength,
					DWORD dwDef = 0, void* pvoidDef = NULL, DWORD dwDefLength = 0,
					DWORD dwUser = 0, LPCTSTR pComputer = NULL );

	//==============================================================
	// SetComputer()
	//==============================================================
	/// Sets the default computer to connect to.
	/**
		\param [in] pComputer	-	Network id of the default computer.  
	*/
	void SetComputer( LPCTSTR pComputer ) 
	{ 	if ( pComputer == NULL ) m_szComputer[ 0 ] = 0x0;
		else strcpy( m_szComputer, pComputer ); }

	//==============================================================
	// GetComputer()
	//==============================================================
	/// Returns the name of the default computer used for registry transactions
	LPCTSTR GetComputer() { return m_szComputer; }

private:

	/// List of registry entries
	list< CAutoRegEntry >	m_Entrys;

	/// Name of default computer used for registry transactions
	char m_szComputer[ MAX_PATH ];

};

#endif // !defined(AFX_REG_H__32DBBA44_11A7_11D2_8353_00104B2C9CFA__INCLUDED_)
