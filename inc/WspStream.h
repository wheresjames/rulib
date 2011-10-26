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
// CWspStream.h: interface for the CWspGeneric class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSPSTREAM_H__88AE8C38_4601_4C15_824F_D073F045C0B8__INCLUDED_)
#define AFX_WSPSTREAM_H__88AE8C38_4601_4C15_824F_D073F045C0B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinAsyncSocket.h"

//==================================================================
// CWspStream
//
/// Implements a raw read write protocol for TWinAsyncSocket
/**
	Implements a raw read write protocol for TWinAsyncSocket	
*/
//==================================================================
class CWspStream : public CWinSocketProtocol
{
public:

	/// Maximum amount of data transferred to the tx'er and rx'er
	enum { eMaxTransferBlockSize = 0x4000 };

public:	

	//==============================================================
	// Write()
	//==============================================================
	/// Writes a string to the socket
	/**
		\param [in] pStr	-	String to write
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( LPCTSTR pStr )
	{	return m_tx.Write( pStr ); }
	
	//==============================================================
	// Read()
	//==============================================================
	/// Reads data from the socket
	/**
		\param [in] pData	-	Buffer that receives the data
		\param [in] uSize	-	Size of buffer in pData
		\param [out] puRead	-	Number of bytes read
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Read( void *pData, UINT uSize, UINT *puRead = NULL )
	{	return m_rx.Read( pData, uSize, (LPDWORD)puRead ); }
	
	//==============================================================
	// Write()
	//==============================================================
	/// Writes data to the socket
	/**
		\param [in] pData	-	Data to write
		\param [in] uSize	-	Number of bytes in pData
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Write( void const *pData, UINT uSize )
	{	return m_tx.Write( pData, uSize ); }
	
	/// Default constructor
	CWspStream();

	/// Destructor
	virtual ~CWspStream();

	//==============================================================
	// OnWrite()
	//==============================================================
	/// Called when there is data to be transmitted
	/**
		\param [in] nErr	-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnWrite( int nErr );	

	//==============================================================
	// OnRead()
	//==============================================================
	/// Called when new data arrives
	/**
		\param [in] nErr	-	Error code
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnRead( int nErr );

	//==============================================================
	// OnTx()
	//==============================================================
	/// Call when tx'er is empty and new transmit data is available
	/**
		\return Non-zero if success
	
		\see 
	*/
	BOOL OnTx();

	//==============================================================
	// GetDataReadyHandle()
	//==============================================================
	/// Return a valid handle if you want to receive data tx messages
	/**
		
		\return Waitable event handle
	
		\see 
	*/
	virtual HANDLE GetDataReadyHandle()
	{	return m_tx.GetDataReadyHandle(); }

	//==============================================================
	// OnDataReady()
	//==============================================================
	/// Called when the event handle returned by GetDataReadyHandle() is set
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL OnDataReady() 
	{	return OnTx(); }

	//==============================================================
	// Rx()
	//==============================================================
	/// Returns a pointer to the receiver messenger
	CCircBuf* Rx() { return &m_rx; }

	//==============================================================
	// Tx()
	//==============================================================
	/// Returns a pointer to the transmitter messenger
	CCircBuf* Tx() { return &m_tx; }

private:

	/// Transmit buffer queue
	CCircBuf					m_tx;

	/// Receive buffer queue
	CCircBuf					m_rx;

	/// Non-zero if the tx'er is busy
	BOOL						m_bTxBusy;

	/// Non-zero if the rx'er is busy
	BOOL						m_bRxBusy;

};

#endif // !defined(AFX_WSPGENERIC_H__88AE8C38_4601_4C15_824F_D073F045C0B8__INCLUDED_)
