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
// NetMgr.h: interface for the CNetMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETMGR_H__339BB6F2_D486_46B2_87CF_E2C5403BE954__INCLUDED_)
#define AFX_NETMGR_H__339BB6F2_D486_46B2_87CF_E2C5403BE954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CNetMgr
//
/// Top level network class
/**
	Provides the network server and manages multiple connected
	clients.  Routes communications and handles message callbacks.	
*/
//==================================================================
class CNetMgr : public CNetCmd  
{

public:	
	
	//==============================================================
	// CNetMgr::RemoveNode()
	//==============================================================
	/// Disconnects and removes the specified network session
	/**
		\param [in] itNode	-	Network session
		
		\return Pointer to previous session
	
		\see 
	*/
	THList< GUID, CNetCom>::iterator CNetMgr::RemoveNode( THList< GUID, CNetCom>::iterator itNode );

	//==============================================================
	// RemoveUpdateNotification()
	//==============================================================
	/// Removes update notifications for the specified class and command
	/**
		\param [in] pguidNode	-	Session requesting removal
		\param [in] pguidClass	-	Class ID of unwanted message
		\param [in] dwCmd		-	Unwanted command ID
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL RemoveUpdateNotification( const GUID *pguidNode, const GUID *pguidClass, DWORD dwCmd );

	//==============================================================
	// CleanupNotifications()
	//==============================================================
	/// Removes invalid sessions from the message map
	/**
		\return Non-zero if success
	*/
	BOOL CleanupNotifications();

	//==============================================================
	// NotifyNetwork()
	//==============================================================
	/// Sends the specified notification message to network
	/**
		\param [in] pguidClass	-	Class ID
		\param [in] dwCmd		-	Command ID
		
		\return Non-zero if success
	
		\see 
	*/
	virtual BOOL NotifyNetwork( const GUID *pguidClass, DWORD dwCmd );

	//==============================================================
	// AddUpdateNotification()
	//==============================================================
	/// Includes the specified session to receive described update messages
	/**
		\param [in] pguidNode	-	Session wanting updates
		\param [in] pguidClass	-	Class ID
		\param [in] dwCmd		-	Command ID
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL AddUpdateNotification( const GUID *pguidNode, const GUID *pguidClass, DWORD dwCmd );
	
	//==============================================================
	// BootNode()
	//==============================================================
	/// Disconnects the specified session
	/**
		\param [in] pNode	-	Pointer to session
	*/
	void BootNode( CNetCom *pNode )
	{	m_lstSession.erase( m_lstSession.find_obj( pNode ) ); }

	//==============================================================
	// BootNode()
	//==============================================================
	/// Disconnects the specified node
	/**
		\param [in] pNode	-	Network ID of session to boot
	*/
	void BootNode( const GUID *pNode )
	{	m_lstSession.erase( *pNode ); }

	//==============================================================
	// CreateNewSession()
	//==============================================================
	/// Creates a new unconnected network session
	/**
		
		\return 
	
		\see 
	*/
	CNetCom* CreateNewSession();

	//==============================================================
	// InitSession()
	//==============================================================
	/// Initializes the specified network session object
	/**
		\param [in] pNc		-	Pointer to network session object
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL InitSession( CNetCom *pNc );

	//==============================================================
	// Cleanup()
	//==============================================================
	/// Removes disconnected sessions from the list
	/**
		\return Non-zero if success
	
		\see 
	*/
	BOOL Cleanup();

	//==============================================================
	// Destroy()
	//==============================================================
	/// Closes all sessions and the network server
	void Destroy();
	
	//==============================================================
	// FindSession()
	//==============================================================
	/// Returns the session object for a given session ID
	/**
		\param [in] guidId	-	Session ID
		
		\return Pointer to session object or NULL if failed
	
		\see 
	*/
	CNetCom* FindSession( const GUID *guidId );

	//==============================================================
	// AddSession()
	//==============================================================
	/// Adds a session with the specified ID to the network
	/**
		\param [in] guidId	-	Session ID
		
		\return Pointer to session object or NULL if failed
	
		\see 
	*/
	CNetCom* AddSession( const GUID *guidId );

	//==============================================================
	// AddSession()
	//==============================================================
	/// Adds a session with the specified ID to the network
	/**
		\param [in] guidId	-	Session ID
		\param [in] pNc		-	Session object to add to list
		
		\return Pointer to session object in pNc or NULL if error
	
		\see 
	*/
	CNetCom* AddSession( const GUID *guidId, CNetCom* pNc );

	/// Default contstructor
	CNetMgr();

	/// Destructor
	virtual ~CNetMgr();

	//==============================================================
	// OnNetMsg()
	//==============================================================
	/// Relays network messages to appropriate dispatch objects
	/**
		\param [in] wParam	-	Message WPARAM
		\param [in] lParam	-	Message LPARAM
		\param [in] pGuid	-	Class ID
		
		\return Message specific return code
	
		\see 
	*/
	long OnNetMsg( WPARAM wParam, LPARAM lParam, const GUID *pGuid );

	//==============================================================
	// NetMsg()
	//==============================================================
	/// Relays network messages to appropriate dispatch objects
	/**
		\param [in] pData	-	User data, usually class pointer.
		\param [in] wParam	-	Message WPARAM
		\param [in] lParam	-	Message LPARAM
		\param [in] pGuid	-	Class ID
		
		\return Message specific return code
	*/
	/// 
	static long NetMsg( LPVOID pData, WPARAM wParam, LPARAM lParam, const GUID *pGuid );

	//==============================================================
	// CreateSessionObject()
	//==============================================================
	/// Create a new CNetCom derived session object
	/**
		\return Pointer to new session object or NULL if failure
	
		\see 
	*/
	virtual CNetCom* CreateSessionObject() = 0;
	
	//==============================================================
	// OnAuthenticate()
	//==============================================================
	/// Over-ride to supply custom authentication
	/**
		\param [in] pNc		-	Network session to authenticate
		
		Over-ride this function and provide custom login functionality.
		Query the session object for attributes and return non-zero
		to allow the connection.  Zero will cause an 'Access Denied'
		message to be sent to the session, then it is immediately
		disconnected.

		\return Return non-zero to allow the session to connect
	
		\see 
	*/
	virtual BOOL OnAuthenticate( CNetCom* pNc ) { return TRUE; }

	//==============================================================
	// GetNode()
	//==============================================================
	/// Returns a pointer to the specified session.
	/**
		\param [in] pGuid	-	Session ID
		
		\return Pointer to network session or NULL if not found
	
		\see 
	*/
	virtual CNetCom* GetNode( const GUID *pGuid );

	//==============================================================
	// FindByRxName()
	//==============================================================
	/// Finds the session by its remote session name.
	/**
		\param [in] pName	-	Name of the remote session object.
		
		\return Pointer to network session or NULL if not found
	
		\see 
	*/
	virtual CNetCom* FindByRxName( LPCTSTR pName );

	//==============================================================
	// FindByTxName()
	//==============================================================
	/// Finds the session by its local session name.
	/**
		\param [in] pName	-	Name of the local session object.
		
		\return Pointer to network session or NULL if not found
	
		\see 
	*/
	virtual CNetCom* FindByTxName( LPCTSTR pName );

	//==============================================================
	// GetNumConnections()
	//==============================================================
	/// Returns the number of connected sessions
	DWORD GetNumConnections() 
	{	Cleanup(); return m_lstSession.Size(); }

	//==============================================================
	// SetGlobalDescription()
	//==============================================================
	/// Sets the description of this network object.
	/**
		\param [in] pDescription	-	Our session description.

		This is the description other networks will receive upon
		connecting to our machine.
	*/
	void SetGlobalDescription( LPCTSTR pDescription )
	{	m_sGlobalDescription = pDescription; }

	//==============================================================
	// GetGlobalDescription()
	//==============================================================
	/// Returns our network description string.
	/**
		
		\return 
	
		\see 
	*/
	LPCTSTR GetGlobalDescription() { return m_sGlobalDescription; }

	//==============================================================
	// GetSessionList()
	//==============================================================
	/// Returns a pointer to the network session list
	THList< GUID, CNetCom >* GetSessionList() { return &m_lstSession; }

	//==============================================================
	// CTlLock*()
	//==============================================================
	/// Returns a pointer to the thread lock
	operator CTlLock*() { return m_lstSession.GetLock(); }

private:

	/// Contains our global network description
	CStr								m_sGlobalDescription;

	/// List of sessions
	THList< GUID, CNetCom >				m_lstSession;

	/// List of nodes wanting updates (nested hashes for speed)
	THList< GUID, THList< DWORD, THList< GUID, CTimeout > > >	m_lstUpdates;

};

#endif // !defined(AFX_NETMGR_H__339BB6F2_D486_46B2_87CF_E2C5403BE954__INCLUDED_)
