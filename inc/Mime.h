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
// Mime.h: interface for the CMime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIME_H__6A44310C_6E73_468A_899A_6B85DE14C240__INCLUDED_)
#define AFX_MIME_H__6A44310C_6E73_468A_899A_6B85DE14C240__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define MIME_STRSIZE	1024
#if( MIME_STRSIZE < MAX_PATH )
#error MIME_STRSIZE is too small!
#endif

#define MIMEENCODERVERHI	1
#define MIMEENCODERVERLO	2
#define MIMEENCODERVERSTR	"1.2"	

#define MBF1_ATTACHMENT		0x00000001
#define MBF1_DECODED		0x00000002
#define MBF1_EXTRA			0x00000004

#define MBEN_BASE64			0x00000001
#define MBEN_QP				0x00000002

/// Contains information on a single MIME data block
/**
	\see CMime
*/
typedef struct tagMIMEBLOCK
{
	/// Size of this structure
	DWORD		size;

	/// MIME variables
	CVar		var;

	/// Block type
	DWORD		type;

	/// Block flags
	DWORD		f1;

	/// Data encoding type
	DWORD		encode;

	/// Type name
	TCHAR		ctype[ MIME_STRSIZE ];

	/// Filename
	TCHAR		fname[ MIME_STRSIZE ];							   

	/// Boundry string
	TCHAR		boundry[ MIME_STRSIZE ];

	/// CID
	TCHAR		cid[ MIME_STRSIZE ];
														   
	/// Number of bytes in pdata
	DWORD		dsize;

	/// Pointer to block data
	LPVOID		pdata;

	/// Pointer to next block
	tagMIMEBLOCK	*pNext;

	/// Pointer to previous block
	tagMIMEBLOCK	*pPrev;

} MIMEBLOCK; // end typedef struct
typedef MIMEBLOCK* LPMIMEBLOCK;


//==================================================================
// CMime
//
/// Encapsulates the MIME format
/**
	Reads / Writes files and memory buffers with the MIME data
	format.  Allows simple creation of E-mail and News group messages.
	Supports multiple data types and attachements.

    This example creates a simple e-mail message with an attachment.
	
	\code

	// Create object
	CMime mime;

	// Create the message
	mime.Create( "to@nwhere.com", "from@nowhere.com", "Subject Line" );

	// Attach a text file from a memory buffer
	TCHAR *pFileText = "This is an empty file.";
	mime.AddFile( "empty_file.txt", (LPBYTE)pFileText, strlen( pFileText ) );

	TCHAR *pMsg = "This is the message body";
	mime.AddPlainText( pMsg, strlen( pMsg ) );

	// Save e-mail message
	mime.Save( "Test.eml" );

	\endcode


	Here's an example of how to extract the HTML portion from a
	MIME document and link any embedded cid parts.

  \code

static BOOL GetTextBlocks(CMime *pMime, LPMIMEBLOCK *plaintext, LPMIMEBLOCK *htmltext, LPMIMEBLOCK *multipart)
{
	LPMIMEBLOCK pmb = NULL;

	*plaintext = NULL;
	*htmltext = NULL;
	*multipart = NULL;

	// Find text blocks
	while ( ( NULL == *htmltext || NULL == *plaintext ) &&
			NULL != ( pmb = pMime->GetNext( pmb ) ) )
	{
		if ( 	( pmb->f1 & MBF1_ATTACHMENT ) == 0 &&
				!strnicmp( pmb->ctype, "multipart/alternative", strlen( "multipart/alternative" ) ) )
			*multipart = pmb;

		else if ( 	( pmb->f1 & MBF1_ATTACHMENT ) == 0 &&
					pmb->dsize > 2 && *plaintext == NULL &&
					!strnicmp( pmb->ctype, "text/plain", strlen( "text/plain" ) ) )
					*plaintext = pmb;

		else if ( 	( pmb->f1 & MBF1_ATTACHMENT ) == 0 &&
					pmb->dsize > 2 && *htmltext == NULL &&
					!strnicmp( pmb->ctype, "text/html", strlen( "text/html" ) ) )
			*htmltext = pmb;

		else if (	( pmb->f1 & MBF1_ATTACHMENT ) == 0 && pmb->ctype[ 0 ] == 0 &&
					pmb->dsize > 2 && *plaintext == NULL ) 
			*plaintext = pmb;
	} // end while

	return TRUE;
}

static BOOL GetTextBlocks(CMime *pMime, CMime *pMulti, LPMIMEBLOCK *plaintext, LPMIMEBLOCK *htmltext)
{
	LPMIMEBLOCK multipart = NULL;

	// Get text blocks
	GetTextBlocks( pMime, plaintext, htmltext, &multipart );

	// Get blocks from sub block(s)
	BOOL quit = FALSE;
	LPMIMEBLOCK oldmulti = NULL;
	while ( !quit &&
			( *plaintext == NULL || *htmltext == NULL ) && 
			multipart != NULL && multipart != oldmulti )
	{
		// One shot
		oldmulti = multipart;

		if ( pMulti->LoadFromMemory( (LPBYTE)multipart->pdata, multipart->dsize ) )
		{
			LPMIMEBLOCK		pt = NULL, ht = NULL;
			GetTextBlocks( pMulti, &pt, &ht, &multipart );

			// Set pointers
			if ( pt != NULL && *plaintext == NULL ) quit = TRUE, *plaintext = pt;

			if ( ht != NULL && *htmltext == NULL ) quit = TRUE, *htmltext = ht;

		} // end if

	} // end while

	return TRUE;
}

	CMime mime;

	// File containing cid parts
	mime.Load( "Test_001.eml" );

	CMime		multi;
	LPMIMEBLOCK plaintext = NULL;
	LPMIMEBLOCK htmltext = NULL;

	// Get text components
	GetTextBlocks( &mime, &multi, &plaintext, &htmltext );
	
	// Extract attachments
	::CreateDirectory( "./attachments", NULL );
	mime.SaveAttachments( "./attachments" );

	// Extract extra files
	::CreateDirectory( "./extras", NULL );
	mime.SaveExtras( "./extras" );

	// Replace the cid's with actual file links
	mime.ReplaceCid( htmltext, "./extras" );

	// We need to process any multi-part as well
	// Is there a multipart?
	if ( multi.Size() )
	{	multi.SaveAttachments( "./attachments" );
		multi.SaveExtras( "./extras" );
		multi.ReplaceCid( htmltext, "./extras" );
	} // end if
  
	// Extract the html document if any
	if ( htmltext )
	{
		CWinFile f;
		if ( f.OpenNew( "Test_001.htm" ) )
			f.Write( htmltext->pdata , htmltext->dsize );

	} // end if

  \endcode
	

*/
//==================================================================
class CMime  
{

public:

	//==============================================================
	// ReplaceCid()
	//==============================================================
	/// Replaces the cid tags in the mime block with the specified path.
	/**
		\param [in] pmb		-	Pointer to MIME block
		\param [in] pPath	-	Replacement data path
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReplaceCid( LPMIMEBLOCK pmb, LPCTSTR pPath = NULL );

	//==============================================================
	// ReplaceCid()
	//==============================================================
	/// Replaces the cid tags in the mime block with the specified path.
	/**			   
		\param [in] pCid		-	CID string
		\param [in] pReplace	-	Replacement string
		\param [in] buf			-	Buffer containing replacement text
		\param [in] size		-	Number of bytes in buf
		\param [in] dest		-	Receives the processed data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReplaceCid( LPCTSTR pCid, LPCTSTR pReplace, LPCTSTR buf, DWORD size, LPSTR dest );

	//==============================================================
	// SaveExtras()
	//==============================================================
	/// Saves attached data not marked as an attachment to specified folder
	/**
		\param [in] pDir	-	Output folder
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveExtras( LPCTSTR pDir );

	//==============================================================
	// DecodeBlock()
	//==============================================================
	/// Decodes the data in a MIME block
	/**
		\param [in] pmb		-	MIME block information
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DecodeBlock( LPMIMEBLOCK pmb );

	//==============================================================
	// ahtoud()
	//==============================================================
	/// Converts an ASCII hex string to binary
	/**
		\param [in] pBuffer		-	Buffer containing ASCII hex string
		\param [in] ucBytes		-	Number of bytes in pBuffer to convert
		
		\return Number value
	
		\see 
	*/
	DWORD ahtoud( LPCTSTR pBuffer, BYTE ucBytes );

	//==============================================================
	// DecodeQP()
	//==============================================================
	/// Decodes 'Quoted Printable' encoding
	/**
		\param [in] pSrc		-	'Quoted Printable' encode string
		\param [in] dwSrc		-	Number of bytes in pSrc
		\param [in] pDst		-	Receives the decoded data
		\param [in,out] pdwDst	-	Receives the number of bytes decoded.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL DecodeQP( LPCTSTR pSrc, DWORD dwSrc, LPSTR pDst, LPDWORD pdwDst );

	//==============================================================
	// SaveAttachments()
	//==============================================================
	/// Saves attachments to specified folder
	/**
		\param [in] pDir	-	Folder
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveAttachments( LPCTSTR pDir );

	//==============================================================
	// SaveBase64()
	//==============================================================
	/// Decodes Base64 encoded data and saves it to a file
	/**
		\param [in] pmb		-	Pointer to MIME block structure
		\param [in] pFile	-	Destination filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveBase64( LPMIMEBLOCK pmb, LPCTSTR pFile );

	//==============================================================
	// SaveToFile()
	//==============================================================
	/// Saves the specified MIME block to a file, decodes as needed
	/**
		\param [in] pmb		-	Pointer to MIME block structure
		\param [in] pFile	-	Destination filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SaveToFile( LPMIMEBLOCK pmb, LPCTSTR pFile );

	//==============================================================
	// ReadBlock()
	//==============================================================
	/// Reads MIME data from buffer into list
	/**
		\param [in] buf		-	Buffer containing MIME data
		\param [in] size	-	Number of bytes in buf
		\param [out] bsize	-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadBlock( LPBYTE buf, DWORD size, LPDWORD bsize );

	//==============================================================
	// ReadHeader()
	//==============================================================
	/// Reads MIME header data
	/**
		\param [in] buf		-	Buffer containing MIME header data
		\param [in] size	-	Number of bytes in buf
		\param [in] hsize	-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadHeader( LPBYTE buf, DWORD size, LPDWORD hsize );

	//==============================================================
	// LoadFromMemory()
	//==============================================================
	/// Loads a MIME file from a memory buffer
	/**
		\param [in] buf		-	Buffer pointer
		\param [in] size	-	Size of buffer in buf
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL LoadFromMemory( LPBYTE buf, DWORD size );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads a MIME file from disk file
	/**
		\param [in] pFile	-	Filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Load( LPCTSTR pFile );

	//==============================================================
	// RemoveBlock()
	//==============================================================
	/// Removes the specified MIME block structure from the list
	/**
		\param [in] node	-	Pointer to MIME block structure
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemoveBlock( LPMIMEBLOCK node );

	//==============================================================
	// AddBlock()
	//==============================================================
	/// Adds a MIME block to the list
	/**
		\param [in] node	-	Pointer to a MIME block
		
		\return MIME block pointer in node
	
		\see 
	*/
	LPMIMEBLOCK AddBlock( LPMIMEBLOCK node );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases all MIME blocks and other resources
	void Destroy();

	/// Default constructor
	CMime();

	/// Destructor
	virtual ~CMime();

	//==============================================================
	// GetNumBlocks()
	//==============================================================
	/// Gets the total number of MIME blocks
	DWORD GetNumBlocks() { return Size(); }

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the size of the internal MIME block list
	DWORD Size() { return m_dwSize; }

	//==============================================================
	// GetNumAttachments()
	//==============================================================
	/// Returns the total number of attachements
	DWORD GetNumAttachments() { return m_dwAttachments; }

	//==============================================================
	// GetNext()
	//==============================================================
	/// Returns a pointer to the next MIME block
	/**
		\param [in] ptr		-	Pointer to MIME block structure

		If ptr is NULL, returns the head pointer
		
		\return 
	
		\see 
	*/
	LPMIMEBLOCK GetNext( LPMIMEBLOCK ptr )
	{	if ( ptr != NULL ) return ptr->pNext; return m_pHead; }

	//==============================================================
	// GetHeader()
	//==============================================================
	/// Returns a pointer to the MIME header information block
	LPMIMEBLOCK GetHeader() { return m_header; }

	//==============================================================
	// GetNumExtras()
	//==============================================================
	/// Returns the number of attachements, not marked as attachments
	DWORD GetNumExtras() { return m_dwExtras; }

	//==============================================================
	// SetPriority()
	//==============================================================
	/// Sets the priority of the MIME message
	/**
		\param [in] p	-	Priority level
	*/
	void SetPriority( DWORD p ) { m_dwPriority = p; }

	//==============================================================
	// GetPriority()
	//==============================================================
	/// Returns the priority level of the MIME message
	DWORD GetPriority() { return m_dwPriority; }

public:

	//==============================================================
	// ZeroMimeBlock()
	//==============================================================
	/// Initializes a MIME block data structure
	/**
		\param [in] pMb		-	MIME block structure to initialize
	*/
	void ZeroMimeBlock( LPMIMEBLOCK pMb );

	//==============================================================
	// AddToHeader()
	//==============================================================
	/// Sets a value in the MIME header
	/**
		\param [in] pParam	-	Parameter name to set
		\param [in] pValue	-	Parameter value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddToHeader( LPCTSTR pParam, LPCTSTR pValue );

	//==============================================================
	// Save()
	//==============================================================
	/// Saves a MIME file to disk
	/**
		\param [in] pFile	-	Filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Save( LPCTSTR pFile );

	//==============================================================
	// AddPlainText()
	//==============================================================
	/// Adds a plain text MIME block
	/**
		\param [in] pText	-	Plain text
		\param [in] dwSize	-	Number of bytes in pText
		
		\return Pointer to new MIME block data structure
	
		\see 
	*/
	LPMIMEBLOCK AddPlainText( LPCTSTR pText, DWORD dwSize = 0 );

	//==============================================================
	// AddFile()
	//==============================================================
	/// Adds file attachment MIME block
	/**
		\param [in] pFile	-	Filename
		\param [in] buf		-	Buffer containing data
		\param [in] size	-	Size of data in buf
		\param [in] flags	-	MIME block flags
		
		\return Pointer to new MIME block data structure
	
		\see 
	*/
	LPMIMEBLOCK AddFile( LPCTSTR pFile, LPBYTE buf, DWORD size, DWORD flags = MBF1_ATTACHMENT );

	//==============================================================
	// AddFile()
	//==============================================================
	/// Adds file attachment MIME block
	/**
		\param [in] pFile	-	File containing data
		\param [in] pName	-	Filename to give MIME block
		\param [in] flags	-	MIME block flags
		
		\return Pointer to new MIME block data structure
	
		\see 
	*/
	LPMIMEBLOCK AddFile( LPCTSTR pFile, LPCTSTR pName = NULL, DWORD flags = MBF1_ATTACHMENT );

	//==============================================================
	// Create()
	//==============================================================
	/// Creates an E-mail style MIME file
	/**
		\param [in] pTo			-	Destination e-mail list
		\param [in] pFrom		-	Source e-mail list
		\param [in] pSubject	-	Subject line
		
		\return Pointer to MIME block or NULL if failure
	
		\see 
	*/
	LPMIMEBLOCK Create( LPCTSTR pTo = NULL, LPCTSTR pFrom = NULL, LPCTSTR pSubject = NULL );

	//==============================================================
	// SaveBlock()
	//==============================================================
	/// Saves a MIME block to a memory buffer
	/**
		\param [in] pmb		-	Pointer to MIME block structure
		\param [out] buf	-	Receives the MIME block data
		\param [in] size	-	Size of buffer in buf
		  
		Call with buf set to NULL to return required buffer size.

		\return Number of bytes written to buf
	
		\see 
	*/
	DWORD SaveBlock( LPMIMEBLOCK pmb, LPBYTE buf, DWORD size );

	//==============================================================
	// SaveToMem()
	//==============================================================
	/// Saves a MIME file to a memory buffer
	/**
		\param [out] buf	-	Receives MIME file data
		\param [in] size	-	Size of buffer in buf
		
		Call with buf set to NULL to return required buffer size.

		\return Number of bytes written to buf
	
		\see 
	*/
	DWORD SaveToMem( LPBYTE buf, DWORD size );

	//==============================================================
	// GetContentExtension()
	//==============================================================
	/// Returns the content extension for MIME string
	/**
		\param [in] pMime	-	MIME string
		\param [out] pExt	-	Receives content extension
		
		If called with pMime == "image/jpeg", then "jpg" is returned 
		in pExt.

		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetContentExtension( LPCTSTR pMime, LPSTR pExt );

	//==============================================================
	// GetContentType()
	//==============================================================
	/// Returns the MIME content type for a filename
	/**
		\param [in] pFile	-	Filename
		\param [out] pType	-	MIME type
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GetContentType( LPCTSTR pFile, LPSTR pType );

	//==============================================================
	// IsValidEmailCharacter()
	//==============================================================
	/// Returns non-zero if valid e-mail character
	/**
		\param [in] ch	-	Character to test
		
		\return Non-zero if ch is a valid e-mail character
	
		\see 
	*/
	static BOOL IsValidEmailCharacter( char ch );

	//==============================================================
	// GetEmailComponents()
	//==============================================================
	/// Breaks e-mail strings into base components
	/**
		\param [in] pEmail		-	E-Mail string or E-mail list string
		\param [out] pName		-	Name
		\param [out] pUser		-	Username
		\param [out] pDomain	-	Domain
		\param [out] pdwNext	-	Offset to next e-mail in list

		pEmail can contain a single e-mail address string or a list of
		strings separated by ';' character.

		Examples:
		\code

			"Bob Smith<bsmith@wheresjames.com>"
			"\"Jane Smith\"<jsmith@wheresjames.com>;\"Tim Jones\"<tjones@wheresjames.com>"

		\endcode Non-zero if success
	  
		\return 
	
		\see 
	*/
	static BOOL GetEmailComponents( LPCTSTR pEmail, LPSTR pName, LPSTR pUser, LPSTR pDomain, LPDWORD pdwNext = NULL );

	//==============================================================
	// BuildEmail()
	//==============================================================
	/// Builds an e-mail address string from its components
	/**
		\param [out] pEmail		-	Receives e-mail string
		\param [in] pName		-	Name
		\param [in] pUser		-	Username
		\param [in] pDomain		-	Domain
		\param [in] pQuote		-	Quote character
		\param [in] pStart		-	E-mail address quote start
		\param [in] pEnd		-	E-mail address end quote
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL BuildEmail( LPSTR pEmail, LPCTSTR pName, LPCTSTR pUser, LPCTSTR pDomain, LPCTSTR pQuote = "\"", LPCTSTR pStart = "<", LPCTSTR pEnd = ">" );

	//==============================================================
	// VerifyEmailList()
	//==============================================================
	/// Builds an e-mail address string from its components
	/**
		\param [out] pDst		-	Receives verified e-mail list string
		\param [in] pSrc		-	Source e-mail list string
		\param [in] pSep		-	E-mail address separator
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL VerifyEmailList( LPSTR pDst, LPCTSTR pSrc, LPCTSTR pSep = "; " );

	//==============================================================
	// GetNextEmail()
	//==============================================================
	/// Retrieves information about the next e-mail in a list
	/**
		\param [out] pName		-	Name
		\param [out] pAddress	-	E-mail address
		\param [in] pList		-	Pointer to e-mail list
		\param [in] dwOffset	-	Starting offset into pList
		\param [in] pStart		-	E-mail start quote string
		\param [in] pEnd		-	E-mail end quote string
		
		\return Offset of next e-mail
	
		\see 
	*/
	static DWORD GetNextEmail( LPSTR pName, LPSTR pAddress, LPCTSTR pList, DWORD dwOffset = 0, LPCTSTR pStart = "<", LPCTSTR pEnd = ">" );

	//==============================================================
	// GenBoundry()
	//==============================================================
	/// Creates a string suitable to be used as a MIME block boundry
	/**
		\param [out] pBoundry	-	Receives the boundry string
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL GenBoundry( LPSTR pBoundry );

private:

	/// MIME header block
	LPMIMEBLOCK		m_header;

	/// Number of blocks in MIME file
	DWORD			m_dwSize;

	/// Pointer to first MIME block
	LPMIMEBLOCK		m_pHead;

	/// Pointer to last MIME block
	LPMIMEBLOCK		m_pTail;

	/// Number of attachements
	DWORD			m_dwAttachments;

	/// Number of attachments not marked as attachments
	DWORD			m_dwExtras;

	/// MIME file priority
	DWORD			m_dwPriority;

	/// Counter used to generate Boundry strings
	/// \note This helped out during debugging...
	static DWORD	m_dwCounter;
};

#endif // !defined(AFX_MIME_H__6A44310C_6E73_468A_899A_6B85DE14C240__INCLUDED_)
