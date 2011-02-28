// md5.cpp - modified by Wei Dai from Colin Plumb's public domain md5.c
// any modifications are placed in the public domain
// Further modifications by Robert Umbehant placed in the public domain

#ifndef CRYPTO_MD5_H
#define CRYPTO_MD5_H

//==================================================================
// CMD5
//
/// Calculates an MD5 Hash
/**
	Calculates an MD5 hash.  Also includes a few support functions.	
*/
//==================================================================
class CMD5
{
public:

	//==============================================================
	// Random()
	//==============================================================
	/// Creates a random hash
	/**
		\param [in] buf		-	Seed buffer
		\param [in] size	-	Number of bytes in buf
	*/
	void Random( LPVOID buf = NULL, DWORD size = 0 );

	//==============================================================
	// HashToString()
	//==============================================================
	/// Convers the hash to a string
	/**
		\param [out] str	-	Receives the string
		\param [in] size	-	Size of the buffer in str
		
		\return Non-zero if success
	
		\see StringToHash()
	*/
	BOOL HashToString( LPSTR str, DWORD size )
	{	return HashToString( (LPBYTE)m_hash, 128, str, size ); }

	//==============================================================
	// StringToHash()
	//==============================================================
	/// Converts a string to a hash
	/**
		\param [in] str		-	String representation of hash
		
		\return Non-zero if success
	
		\see HashToString()
	*/
	BOOL StringToHash( LPCTSTR str )
	{	return StringToHash( (LPBYTE)m_hash, 128, str ); }

	//==============================================================
	// CalcOffsetMask()
	//==============================================================
	/// Calculates offset mask for bit
	/**
		\param [in] bit		-	Bit index
		\param [out] offset	-	Byte offset
		\param [out] mask	-	Bit mask

		To check say the 125th bit in a buffer

		\code
			
		BYTE buf[ 100 ];

		BYTE  ucMask;
		DWORD dwOffset;

		CalcOffsetMask( 125, &dwOffset, &ucMask );

		// Check the 125 bit
		if ( buf[ dwOffset ] & ucMask )
			; // bit is set
		else ; // bit is not set

		\endcode

	*/
	static void CalcOffsetMask( DWORD bit, LPDWORD offset, LPBYTE mask );

	//==============================================================
	// GetBit()
	//==============================================================
	/// Gets the value of the specified bit
	/**
		\param [in] buf		-	Pointer to bit buffer
		\param [in] bit		-	Bit offset
				
		\return Value of specified bit, 0 or 1
	
		\see 
	*/
	static DWORD GetBit( LPBYTE buf, DWORD bit );

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns the value of the specified bit range
	/**
		\param [in] buf			-	Bit buffer
		\param [in] bitoff		-	bit offset
		\param [in] bits		-	Number of bits
		
		\return Value of specfied bit range
	
		\see 
	*/
	static DWORD GetValue( LPBYTE buf, DWORD bitoff, DWORD bits );

	//==============================================================
	// HashToString()
	//==============================================================
	/// Converts a hash to a string
	/**
		\param [in] hash		-	Hash buffer
		\param [in] hashbits 	-	Number of bits in the hash
		\param [out] str		-	Receives the hash string
		\param [in] size		-	Size of the buffer in str
		
		\return Non-zero on success
	
		\see 
	*/
	static BOOL HashToString( LPBYTE hash, DWORD hashbits, LPSTR str, DWORD size );

	//==============================================================
	// SetBit()
	//==============================================================
	/// Sets the specified bit
	/**
		\param [out] buf	-	Buffer containing bit
		\param [in] bit		-	Bit index
		\param [in] set		-	Bit state, zero or non-zero
		
		\return 
	
		\see 
	*/
	static void SetBit( LPBYTE buf, DWORD bit, DWORD set );

	//==============================================================
	// SetValue()
	//==============================================================
	/// Sets the value of a bit sequence
	/**
		\param [in] buf		-	Buffer containing bits
		\param [in] bitoff	-	Bit sequence offset
		\param [in] bits	-	Number of bits in sequence
		\param [in] value	-	Value of bit sequences

		Since value is a DWORD, a maximum of 32 bits can be set at
		once.
		
		\return Contents of value
	
		\see 
	*/
	static DWORD SetValue( LPBYTE buf, DWORD bitoff, DWORD bits, DWORD value );

	//==============================================================
	// StringToHash()
	//==============================================================
	/// Converts a string into a hash
	/**
		\param [in] hash		-	Hash data
		\param [in] hashbits	-	Number of bits in the hash
		\param [out] str		-	Receives the string representation
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL StringToHash( LPBYTE hash, DWORD hashbits, LPCTSTR str );

	//==============================================================
	// TransformFile()
	//==============================================================
	/// Performs MD5 hash on a file
	/**
		\param [in] pFile	-	File to hash
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL TransformFile( LPCTSTR pFile );

	//==============================================================
	// Transform()
	//==============================================================
	/// Updates the MD5 hash with the specified data
	/**
		\param [in] buf		-	Pointer to data
		\param [in] size	-	Number of bytes in buf

		If you intend to make multiple calls to this function, the 
		first calls must be 64 bytes in length.  The last call may
		be less than 64 bytes.

		No more calls to this functions after a block size of less
		than 64 bytes.
	  
		\return Non-zero if success
	
		\see 
	*/
	BOOL Transform( LPVOID buf, DWORD size ) 
	{	return Transform( (LPBYTE)buf, size ); }

	//==============================================================
	// Transform()
	//==============================================================
	/// Updates the MD5 hash with the specified data
	/**
		\param [in] buf		-	Pointer to data
		\param [in] size	-	Number of bytes in buf

		If you intend to make multiple calls to this function, the 
		first calls must be 64 bytes in length.  The last call may
		be less than 64 bytes.

		No more calls to this functions after a block size of less
		than 64 bytes.
	  
		\return Non-zero if success
	
		\see 
	*/
	BOOL Transform( LPBYTE buf, DWORD size );

	//==============================================================
	// Transform()
	//==============================================================
	/// Updates the MD5 hash with the specified string
	/**
		\param [in] pString		-	String to hash
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Transform( LPCTSTR pString );

	//==============================================================
	// End()
	//==============================================================
	/// Completes the hash
	/**
		\return Non-zero if success
	
		\see 
	*/
	BOOL End();

	//==============================================================
	// ByteReverse()
	//==============================================================
	/// Reverses the bytes in buf
	/**
		\param [in,out] buf		-	Buffer pointer
		\param [in] longs		-	Number of bytes to reverse
		
		\return 
	
		\see 
	*/
	void ByteReverse( LPBYTE buf, DWORD longs );
	
	/// MD5 digest
	DWORD	m_digest[ 4 ];

	/// MD5 hash
	DWORD	m_hash[ 4 ];

	/// MD5 input data buffer
	BYTE	m_buf[ 64 ];

	/// Bytes in m_buf
	DWORD	m_total;

	/// Default Constructor
	CMD5(){Init();}

	
	//==============================================================
	// Transform()
	//==============================================================
	/// Updates hash with data
	/**
		\param [in,out] hash	-	Hash
		\param [in] data		-	64 bytes of data
		
		\return 
	
		\see 
	*/
	static void Transform(DWORD *hash, const DWORD *data);

	//==============================================================
	// GetDigest()
	//==============================================================
	/// Returns a pointer to the digest buffer
	LPBYTE  GetDigest() { return (LPBYTE)m_digest; }

	//==============================================================
	// GetHash()
	//==============================================================
	/// Returns a pointer to the hash buffer
	LPBYTE	GetHash() { return (LPBYTE)m_hash; }

	//==============================================================
	// GetHashLen()
	//==============================================================
	/// Returns the length of the hash, currently 16 bytes
	DWORD	GetHashLen() { return sizeof( m_hash ); }

	//==============================================================
	// Init()
	//==============================================================
	/// Initializes the hash variables
	void Init();

protected:

	//==============================================================
	// vTransform()
	//==============================================================
	/// Updates hash with 64 bytes of input data
	/**
		\param [in] data	-	Buffer containing 64 bytes of data
	*/
	void vTransform(const DWORD *data) { Transform( m_hash, data ); }
};

#endif
