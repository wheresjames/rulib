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
// DataPacket.h: interface for the CDataPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPACKET_H__A522F9EF_C095_40D5_982C_F5A4F849EA33__INCLUDED_)
#define AFX_DATAPACKET_H__A522F9EF_C095_40D5_982C_F5A4F849EA33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DPF1_ENCRYPTED		0x00000001
#define DPF1_COMPRESSED		0x00000002

#define DPID_HEADER			0xa17ca55a

//==================================================================
// CDataPacket
//
/// Provides basic serial packetizing class.
/**
	Use this class to provide serial communication packetizing.
	Useful for sending data over any serial link such as Ethernet.
	
*/
//==================================================================
class CDataPacket : public CCircBuf
{

public:

	/// This structure defines a serial packet header.
	struct SPacketHeader
	{
		/// Unique ID identifying the start of packet
		DWORD			dwId;

		/// The total length of the packet
		DWORD			dwLength;

		/// Type specifier for the packet
		DWORD			dwType;

		/// Number of data blocks in this packet
		DWORD			dwDataBlocks;

	}; typedef SPacketHeader* LPSPacketHeader;

	/// This structure defines a data chunk
	struct SDataHeader
	{
		/// The total length of the packet
		DWORD			dwLength;

		/// Unique ID identifying the type of data
		DWORD			dwType;

	}; typedef SDataHeader* LPSDataHeader; 

	/// Data check sum
	struct SCheckSum
	{
		/// Crc of the data
		BYTE			md5[ 16 ];

	}; typedef SCheckSum* LPSCheckSum;

public:

	//==============================================================
	// VerifyPacketData()
	//==============================================================
	/// Call this function to verify the packet data
	/**
		\param [in] dwBlock		-	Block index
		\param [in] dwType		-	Block type
		\param [in] pGuid		-	Block ID
		
		\return Non-zero if verified
	
		\see 
	*/
	BOOL VerifyPacketData( DWORD dwBlock, DWORD dwType, const GUID *pGuid );

	//==============================================================
	// GetPacketDataHash()
	//==============================================================
	/// Returns packet data hash
	/**
		\param [in] pGuid		-	Buffer to hold hash
		\param [in] dwBlock		-	Block index we're looking for
		\param [in] dwType		-	Block type
		
		\return Non-zero if hash was retrieved.
	
		\see 
	*/
	BOOL GetPacketDataHash( GUID *pGuid, DWORD dwBlock, DWORD dwType );

	//==============================================================
	// ReadPacketData()
	//==============================================================
	/// This function is used to retrieve the packet data
	/**
		\param [in] dwBlock		-	Index of block
		\param [in] dwType		-	Block type
		\param [in] pBuf		-	Pointer to buffer that receives the data.
		\param [in] dwMax		-	Size of buffer in pBuf
		\param [in] pdwRead		-	Number of bytes written to pBuf
		\param [in] lOffset		-	Offset from block start pointer
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadPacketData( DWORD dwBlock, DWORD dwType, LPVOID pBuf, DWORD dwMax, LPDWORD pdwRead = NULL, long lOffset = 0 );

	//==============================================================
	// ReadPacketString()
	//==============================================================
	/// Reads a string from the packet data.  Ensures no buffer overflow and NULL terminates.
	/**
		\param [in] dwBlock		-	Block index
		\param [in] dwType		-	Block type
		\param [in] pStr		-	Buffer that receives the string
		\param [in] dwMax		-	Size of the buffer in pStr
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL ReadPacketString( DWORD dwBlock, DWORD dwType, LPSTR pStr, DWORD dwMax )
	{	DWORD dwLen = 0;
		BOOL bRes = ReadPacketData( dwBlock, dwType, pStr, dwMax, &dwLen );
		if ( dwLen >= dwMax ) dwLen = dwMax - 1; pStr[ dwLen ] = 0; 
		return bRes;
	}

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases resources and readys the object for reuse
	virtual void Destroy();

	//==============================================================
	// SkipPacket()
	//==============================================================
	/// Skips the first complete and valid packet in the buffer
	/**		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL SkipPacket();

	//==============================================================
	// VerifyPacket()
	//==============================================================
	/// Verifys a packet at current buffer position
	/**
		
		Searches for the packet ID, then verifys the length and checksum
		If this function returns TRUE, a valid packet awaits in the buffer.

		\return Non-zero if a valid packet is in the buffer.
	
		\see 
	*/
	virtual BOOL VerifyPacket();
	
	//==============================================================
	// FindPacket()
	//==============================================================
	/// Searches the buffer for the packet ID.  Does not mean it is a valid packet.
	/**
		\param [in] pPh				-	Receives packet header data
		\param [in] pdwAvailable	-	Number of valid bytes left in buffer
		
		\return Non-zero if valid packet header is found.
	
		\see 
	*/
	virtual BOOL FindPacket(LPSPacketHeader pPh, LPDWORD pdwAvailable);

	//==============================================================
	// ReadPacket()
	//==============================================================
	/// Adds the specified data to the buffer and checks for a valid packet
	/**
		\param [in] pBuf	-	Buffer containing new raw data to read
		\param [in] dwSize	-	Size of the data in pBuf
		
		\return Non-zero if success.
	
		\see 
	*/
	virtual BOOL ReadPacket( LPVOID pBuf, DWORD dwSize );
	
	//==============================================================
	// GetMinimumPacketOverhead()
	//==============================================================
	/// Returns the minimum packet size
	DWORD GetMinimumPacketOverhead()
	{	return ( sizeof( SPacketHeader ) + sizeof( SCheckSum ) ); }

	//==============================================================
	// WritePacket()
	//==============================================================
	/// Writes a complete packet to the buffer containing a single data block
	/**
		\param [in] dwPacketType	-	Packet type id
		\param [in] dwDataType		-	Data type id
		\param [in] pData			-	Pointer to data
		\param [in] dwData			-	Number of bytes in pData
		
		Writes packet data to buffer.

		\return Non-zero if success
	
		\see 
	*/
	BOOL WritePacket( DWORD dwPacketType, DWORD dwDataType, LPVOID pData, DWORD dwData );

	//==============================================================
	// WriteMultiPacket()
	//==============================================================
	/// Writes multiple packet buffers
	/**
		\param [in] dwPacketType	-	Packet type
		\param [in] dwBuffers		-	Number of buffers
		
		Writes multiple buffers add 
		( DWORD dwDataType, LPBYTE pBuf, DWORD dwSize, ... ) for each buffer

		\return 
	
		\see 
	*/
	BOOL WriteMultiPacket( DWORD dwPacketType, DWORD dwBuffers = 0, ... )
	{	return vWriteMultiPacket( dwPacketType, dwBuffers, ( (LPVOID*)&dwBuffers ) + 1 ); }

	//==============================================================
	// vWriteMultiPacket()
	//==============================================================
	/// Writes multiple packet buffers
	/**
		\param [in] dwPacketType	-	Packet type
		\param [in] dwBuffers		-	Number of buffers
		\param [in] pArgs			-	Pointer to argument pointer list
		
		pArgs[] = { DWORD *dwDataType, LPBYTE *pBuf, DWORD *dwSize, ... }

		\return Non-zero if success
	
		\see 
	*/
	BOOL vWriteMultiPacket( DWORD dwPacketType, DWORD dwBuffers = 0, LPVOID *pArgs = NULL );

	//==============================================================
	// EndPacket()
	//==============================================================
	/// Call to conclude a packet being written to the buffer
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EndPacket();

	//==============================================================
	// AddPacketData()
	//==============================================================
	/// Creates a data block and writes it into the buffer
	/**
		\param [in] dwType	-	Packet data type
		\param [in] pData	-	Buffer containing packet data
		\param [in] dwSize	-	Number of bytes in pData
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddPacketData( DWORD dwType, LPVOID pData, DWORD dwSize );

	//==============================================================
	// WritePacketData()
	//==============================================================
	/// Write raw data into packet and update checksum
	/**
		\param [in] pData		-	Pointer to packet data
		\param [in] dwSize		-	Number of bytes in pData
		\param [in] dwEncode	-	Optional encoding
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL WritePacketData( LPVOID pData, DWORD dwSize, DWORD dwEncode = 0 );

	//==============================================================
	// InitPacket()
	//==============================================================
	/// Initializes the packet header and writes it to the buffer
	/**
		\param [in] dwType				-	Packet type
		\param [in] dwDataBlocks		-	Number of data blocks
		\param [in] dwTotalDataSize		-	Total size of packet data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL InitPacket( DWORD dwType, DWORD dwDataBlocks, DWORD dwTotalDataSize );

	//==============================================================
	// OnInspectWrite()
	//==============================================================
	/// Over-ride to implement write block inspection
	/**
		\param [in] dwBlock		-	Block index
		\param [in] pBuf		-	Pointer to write buffer
		\param [in] dwSize		-	Size of data in pBuf
		
		\return Non-zero if success.
	
		\see 
	*/
	virtual BOOL OnInspectWrite( DWORD dwBlock, LPBYTE pBuf, DWORD dwSize );

	/// Constructor
	CDataPacket();

	/// Constructor
	CDataPacket( DWORD dwSize );

	/// Destructor
	virtual ~CDataPacket();

	//==============================================================
	// SetEncoding()
	//==============================================================
	/// Call this function to enable packet data encoding
	/**
		\param [in] dw	-	Current encoding value
	*/
	void SetEncoding( DWORD dw ) { m_dwEncode = dw; } 

	//==============================================================
	// GetEncoding()
	//==============================================================
	/// Returns the type of packet data encoding
	DWORD GetEncoding() { return m_dwEncode; }

private:

	/// Set to true when a valid packet has beenf found
	BOOL			m_bValidPacket;

	/// Set to true when building a packet
	BOOL			m_bInPacket;
	
	/// Packet header data
	SPacketHeader	m_ph;

	/// Running MD5 context
	MD5_CTX			m_md5Context;

	/// Current encode value
	DWORD			m_dwEncode;

};

#endif // !defined(AFX_DATAPACKET_H__A522F9EF_C095_40D5_982C_F5A4F849EA33__INCLUDED_)
