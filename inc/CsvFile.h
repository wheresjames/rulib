/*******************************************************************
// Copyright (c) 2002, Robert Umbehant ( www.wheresjames.com )
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
// CsvFile.h: interface for the CCsvFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSVFILE_H__D7EB8F67_510C_11D5_82D5_FFFFFF000000__INCLUDED_)
#define AFX_CSVFILE_H__D7EB8F67_510C_11D5_82D5_FFFFFF000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Row handle
typedef LPBYTE	CSV_HROW;
typedef DWORD	CSV_HCOL;

//==================================================================
// CCsvFile
//
/// Class for reading comma separated value files
/**
	  This class provides simple functionality for working with
	  CSV (Comma Separated Value) files.
*/
//==================================================================
class CCsvFile  
{
public:

	//==============================================================
	// GetElementPtr()
	//==============================================================
	/// Gets a pointer to element string
	/**
		\param [in] hRow	-	Handle to row
		\param [in] pCol	-	Pointer to column name
		\param [in] pDef	-	Default value for item
		
		\return Value string or pDef if failure
	
		\see 
	*/
	LPCTSTR GetElementPtr( CSV_HROW hRow, LPCTSTR pCol, LPCTSTR pDef = "" );

	//==============================================================
	// AddColumn()
	//==============================================================
	/// Adds a column to the CSV file
	/**
		\param [in] pDescription	-	Description of column
		\param [in] insert			-	Index of column, MAXDWORD to append.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddColumn( LPCTSTR pDescription, DWORD insert = MAXDWORD );

	//==============================================================
	// GetHeaderString()
	//==============================================================
	/// Returns the line that was determined to be the header strings
	/**
		\param [out] buf	-	Pointer to header data
		\param [in] size	-	Size of the buffer in buf
		
		\return Length of string in buf
	
		\see 
	*/
	DWORD GetHeaderString( LPSTR buf, DWORD size );

	//==============================================================
	// GetRowString()
	//==============================================================
	/// Copies the string associated with a particular row
	/**
		\param [in] row		-	Row index
		\param [out] pRow	-	Pointer that receives row data
		\param [in] size	-	Size of buffer in pRow
		
		\return Length of data in pRow
	
		\see 
	*/
	DWORD GetRowString( DWORD row, LPSTR pRow, DWORD size );
	
	//==============================================================
	// Copy()
	//==============================================================
	/// Copies a string of specified size
	/**
		\param [out] buf	-	Buffer pointer
		\param [in,out] ptr	-	Location in buffer to copy string
		\param [in] size	-	Size of buffer in buf
		\param [in] str		-	String to copy into buffer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Copy( LPVOID buf, LPDWORD ptr, DWORD size, LPCTSTR str );
	
	//==============================================================
	// SaveToMem()
	//==============================================================
	/// Writes the CSV file to a memory buffer
	/**
		\param [out] buf	-	Pointer to a memory buffer.
		\param [in] size	-	Size of the buffer in buf
		
		\return Size of data written to buf.
	
		\see 
	*/
	DWORD SaveToMem( LPBYTE buf, DWORD size );

	//==============================================================
	// SaveFile()
	//==============================================================
	/// Saves the CSV file to disk
	/**
		\param [in] pFile	-	Disk filename.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL SaveFile( LPCTSTR pFile );

	//==============================================================
	// LoadColumns()
	//==============================================================
	/// Loads the column names from string
	/**
		\param [in] pLine	-	String containing column names
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL LoadColumns( LPCTSTR pLine );
	
	//==============================================================
	// AddLine()
	//==============================================================
	/// Adds a line of data to the CSV file
	/**
		\param [in] pLine	-	Pointer to string containing single line
								of CSV data.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL AddLine( LPCTSTR pLine );
	
	//==============================================================
	// AllocateRows()
	//==============================================================
	/// Allocates the specified number of rows
	/**
		\param [in] size	-	Number of rows to allocate.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL AllocateRows( DWORD size );

	//==============================================================
	// LoadFromMem()
	//==============================================================
	/// Loads a CSV file from memory.
	/**
		\param [in] buf		-	Buffer containing CSV data.
		\param [in] size	-	Size of the buffer in buf
		\param [in] bAdd	-	Set to non-zero to add data to current file.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL LoadFromMem( LPBYTE buf, DWORD size, BOOL bAdd = FALSE );

	//==============================================================
	// LoadFile()
	//==============================================================
	///	Loads CSV file from disk file.
	/**
		\param [in] pFile	-	Pointer to filename
		\param [in] bAdd	-	Set to non-zero to add data to current file.
		
		\return 
	
		\see 
	*/
	BOOL LoadFile( LPCTSTR pFile, BOOL bAdd = FALSE );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all resources associated with CSV
	void Destroy();

	//==============================================================
	// AllocateColumns()
	//==============================================================
	/// Allocates specified number of columns.
	/**
		\param [in] columns		-	Number of columns to allocate.
		
		\return Non-zero if success.
	
		\see 
	*/
	BOOL AllocateColumns( DWORD columns );

	/// Constructor
	CCsvFile();

	/// Destructor
	virtual ~CCsvFile();

	//==============================================================
	// GetNumColumns()
	//==============================================================
	/// Returns the number of columns
	DWORD GetNumColumns() { return m_col; }

	//==============================================================
	// GetNumRows()
	//==============================================================
	/// Returns the number of rows
	DWORD GetNumRows() { return m_rowptr; }

	//==============================================================
	// GetTablePtr()
	//==============================================================
	/// Returns a pointer to the table data
	LPBYTE* GetTablePtr() { return m_pTable; }
	
	//==============================================================
	// IsTable()
	//==============================================================
	/// Returns non-zero if there is a valid data table
	BOOL IsTable() { return ( m_col != 0 ); }

	//==============================================================
	// AppendNewRow()
	//==============================================================
	/// Appends a new row to the current CSV file
	CSV_HROW AppendNewRow();

	//==============================================================
	// GetRow()
	//==============================================================
	/// Returns a handle to the specified row
	/**
		\param [in] i	-	Index of row
		
		\return Handle to row
	
		\see 
	*/
	CSV_HROW GetRow( DWORD i )
	{	if ( i >= m_rowptr ) return NULL; return m_pTable[ i ]; }

	//==============================================================
	// GetRowElement()
	//==============================================================
	/// Returns a pointer to the specified element
	/**
		\param [in] hrow	-	Handle to row
		\param [in] i		-	Index of row element
		
		\return 
	
		\see 
	*/
	LPBYTE GetRowElement( CSV_HROW hrow, DWORD i )
	{	if ( i >= m_col ) return NULL; return &hrow[ i * m_dwRecordSize ]; }

	//==============================================================
	// GetColDesc()
	//==============================================================
	/// Retreives a pointer to the column description string
	/**
		\param [in] i	- Column index
		
		\return Pointer to string naming column.
	
		\see 
	*/
	LPCTSTR GetColDesc( DWORD i )
	{	if ( i >= m_col ) return NULL; return (LPCTSTR)&m_pDesc[ i * m_dwRecordSize ]; }

	//==============================================================
	// FindCol()
	//==============================================================
	/// Returns the index of the named column
	/**
		\param [in] desc	-	Name of the column to find.
		
		\return Index of the specified column or MAXDWORD if not found.
	
		\see 
	*/
	DWORD FindCol( LPCTSTR desc )
	{	for ( DWORD i = 0; i < m_col; i++ ) 
		if ( !strcmpi( GetColDesc( i ), desc ) ) return i;
		return MAXDWORD;
	}

	//==============================================================
	// SetRecordSize()
	//==============================================================
	/// Sets the maximum size of a single CSV item
	/**
		\param [in] rs	-	Maximum text string length
	*/
	void SetRecordSize( DWORD rs ) { m_dwRecordSize = rs; }

	//==============================================================
	// GetRecordSize()
	//==============================================================
	/// Returns the maximum size of a single CSV item
	DWORD GetRecordSize() { return m_dwRecordSize; }

	//==============================================================
	// SetSep()
	//==============================================================
	/// Sets the separator character for the CSV file
	/**
		\param [in] sep		-	Row item separator.  Usually ','
	*/
	void SetSep( BYTE sep ) { m_ucSep = sep; }

	//==============================================================
	// GetSep()
	//==============================================================
	/// Returns the current separator character for the CSV file
	BYTE GetSep() { return m_ucSep; }

	//==============================================================
	// SetRowBlockSize()
	//==============================================================
	/// Sets the row block size
	/**
		\param [in] rbs		-	Row block size
	*/
	void SetRowBlockSize( DWORD rbs ) { m_dwRowBlockSize = rbs; }

	//==============================================================
	// GetRowBlockSize()
	//==============================================================
	/// Returns the current row blocks size
	DWORD GetRowBlockSize() { return m_dwRowBlockSize; }

	//==============================================================
	// SetSkip()
	//==============================================================
	/// Sets the number of lines to skip at the start of the CSV file
	/**
		\param [in] s	-	Number of lines to skip.

		There is many times data that needs to be ignored at the start
		of a CSV file.  Use this function to set the number of lines
		that should be ignored.
	*/
	void SetSkip( DWORD s ) { m_dwSkip = s; }

private:

	/// CSV row item separator
	BYTE		m_ucSep;

	/// Maximum CSV item string length
	DWORD		m_dwRecordSize;

	/// Row block size
	DWORD		m_dwRowBlockSize;

	/// Number of columns
	DWORD		m_col;

	/// Number of rows
	DWORD		m_row;

	/// Current row pointer
	DWORD		m_rowptr;

	/// Pointer to data table
	LPBYTE		*m_pTable;

	/// Pointer to item descriptions
	LPBYTE		m_pDesc;

	/// Pointer to item types
	LPBYTE		m_pTypes;

	/// Number of errors while reading file
	DWORD		m_dwErrors;

	/// Number of lines to skip while reading CSV file
	DWORD		m_dwSkip;

};

#endif // !defined(AFX_CSVFILE_H__D7EB8F67_510C_11D5_82D5_FFFFFF000000__INCLUDED_)
