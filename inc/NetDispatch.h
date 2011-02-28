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
// NetDispatch.h: interface for the CNetDispatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETDISPATCH_H__FD04530B_4CDE_4853_B782_2CC8946723A0__INCLUDED_)
#define AFX_NETDISPATCH_H__FD04530B_4CDE_4853_B782_2CC8946723A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CNetDispatch
//
///	Provides network callback capability.
/**
	Inherit from this class to provide network accessable 
	functionality.	
*/
//==================================================================
class CNetDispatch  
{
public:
	
	//==============================================================
	// SetNetwork()
	//==============================================================
	/// Call to set the network object this class should use
	/**
		\param [in] pNc			-	Pointer to a valid CNetCmd object
		\param [in] pguidClass	-	The class ID for messages this
									class intends to receive.
	*/
	virtual void SetNetwork(CNetCmd *pNc, const GUID *pguidClass);

	//==============================================================
	// GetNetwork()
	//==============================================================
	/// Returns a pointer to the attached network object
	CMessageList* GetNetwork() { return m_pNc; }

	//==============================================================
	// Destroy()
	//==============================================================
	/// Unhooks from the network and releases all resources
	void Destroy();

	/// Default constructor
	CNetDispatch();

	//==============================================================
	// CNetDispatch()
	//==============================================================
	/// Constructs the object and hooks the specified network object
	/**
		\param [in] pNc		-	CNetCmd network object to hook
	*/
	CNetDispatch( CNetCmd *pNc );

	/// Destructor
	virtual ~CNetDispatch();
	
	//==============================================================
	// NetMsg()
	//==============================================================
	/// Callback function that handles network notifications
	/**
		\param [in] pData	-	CNetCom class pointer
		\param [in] wParam	-	Message WPARAM value
		\param [in] lParam	-	Message LPARAM value
		
		\return Message return value
	
		\see 
	*/
	static long NetMsg( LPVOID pData, WPARAM wParam, LPARAM lParam );

	//==============================================================
	// OnNetMsg()
	//==============================================================
	/// Resolved callback function that handles network notifications
	/**
		\param [in] wParam	-	Message WPARAM value
		\param [in] lParam	-	Message LPARAM value
		
		\return Message return value
	
		\see 
	*/
	long OnNetMsg( WPARAM wParam, LPARAM lParam );

	//==============================================================
	// OnMsg()
	//==============================================================
	/// Over-ride to supply custom message handling
	/**
		\param [in] dwFunction	-	Function ID
		\param [in] pNc			-	CNetCom object

		Process the command in dwFunction.  Call pNc->GetParams(),
		pNc->ReadPacketData(), pNc->ReadPacketString(), etc... to
		retreive message information.

		Use pNc->Msg() to send replies.
		
		\return Non-zero if message is processed
	
		\see 
	*/
	virtual BOOL OnMsg( DWORD dwFunction, CNetCom *pNc ) = 0;

	//==============================================================
	// IsNetwork()
	//==============================================================
	/// Returns non-zero if there is an attached CNetCmd object
	BOOL IsNetwork() { return m_pNc != NULL; }

	//==============================================================
	// Net()
	//==============================================================
	/// Returns a pointer to the attached CNetCmd object
	CNetCmd* Net() { return m_pNc; }

private:

	/// Pointer to attached CNetCmd object
	CNetCmd				*m_pNc;

	/// Class ID for which we are receiving messages
	GUID				m_guidClass;
};

#endif // !defined(AFX_NETDISPATCH_H__FD04530B_4CDE_4853_B782_2CC8946723A0__INCLUDED_)
