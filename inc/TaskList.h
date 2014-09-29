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
*******************************************************************/
//
// TaskList.h: interface for the CTaskList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKLIST_H__3C592A6E_FF22_48DC_9711_C49182E4EC8C__INCLUDED_)
#define AFX_TASKLIST_H__3C592A6E_FF22_48DC_9711_C49182E4EC8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <psapi.h>
#include <tlhelp32.h>

#include <vdmdbg.h>

#include "Module.h"


class CTask;

/// Holds information about a single task item
/**
	\see CTaskList, CTask
*/
typedef struct tagTASKINFO : LLISTINFO
{
	/// Process id
	DWORD			dwPid;

	/// Process handle
	HANDLE			hProcess;

	/// Task name
	char			name[ CWF_STRSIZE ];

	/// Process filename
	char			fname[ CWF_STRSIZE ];

	/// Full process filename
	char			file[ CWF_STRSIZE ];

	/// Number of modules
	DWORD			dwModules;

	/// List of sub tasks
	CTask			*subtasks;

	// Toolhelp only

	/// Number of references to task
	DWORD			dwReferences;

	/// Number of threads in task
	DWORD			dwThreads;

	/// Parent process ID
	DWORD			dwParentPid;

	/// Default Heap ID
	DWORD			dwDefaultHeapId;

	/// Module ID
	DWORD			dwModuleId;

	/// Process module base address
	void*			pBaseAddr;

	/// Process module entry point address
	void*			pEntryPoint;

	/// Size of module memory image
	DWORD			dwModuleSize;

	/// Total memory used by module
	DWORD			dwTotalMemory;

	/// Module main thread id
	DWORD			dwThreadId;

	/// Module main task id
	DWORD			dwTaskId;

	/// Module processor use
	DWORD			dwProccntUsage;

	/// Global processor use
	DWORD			dwGlblcntUsage;

	/// Number of page faults since startup
	DWORD			dwPageFaultCount;

	/// Maximum number of pages used
	DWORD			dwPeakWorkingSetSize;

	/// Number of pages in use
	DWORD			dwWorkingSetSize;

	/// Maximum page pools used
	DWORD			dwQuotaPeakPagedPoolUsage;

	/// Page pools used
	DWORD			dwQuotaPagedPoolUsage;

	/// Maximum non page pool use
	DWORD			dwQuotaPeakNonPagedPoolUsage;

	/// Non page pool use
	DWORD			dwQuotaNonPagedPoolUsage;

	/// Maximum file use
	DWORD			dwPageFileUsage;

	/// Maximum page file use
	DWORD			dwPeakPageFileUsage;

} TASKINFO, *LPTASKINFO; // end typedef struct

//==================================================================
// CTask
//
/// Holds list of task items
/**
	\see CTaskList
*/
//==================================================================
class CTask : public CLList
{
public:

	/// Constructor
	CTask(){}

	/// Destructor
	virtual ~CTask(){ Destroy(); }

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Return the size of a task item structure
	virtual DWORD GetObjSize() { return sizeof( TASKINFO ); }

	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Do task element cleanup
	/**
		\param [in] node	-	Pointer to TASKINFO structure
	*/
	virtual void DeleteObject( void *node )
	{	LPTASKINFO pti = (LPTASKINFO)node;
		if ( pti == NULL ) return;
		if ( pti->subtasks != NULL )
		{	delete pti->subtasks;
			pti->subtasks = NULL;
		} // end if
		CLList::DeleteObject( node );
	} 

	//==============================================================
	// CreateSubList()
	//==============================================================
	/// Creates sub task list in task list item
	/**
		\param [in] pti		-	Pointer to task item
		
		\return Pointer to new sub list
	
		\see 
	*/
	static CTask* CreateSubList( LPTASKINFO pti )
	{	if ( pti == NULL ) return NULL;
		if ( pti->subtasks != NULL ) return pti->subtasks;
		return ( pti->subtasks = new CTask );
	}
		
};


//==================================================================
// CTaskList
//
/// Enumerates running processes.
/**
	Use this class to retreive information on running processes.	
*/
//==================================================================
class CTaskList
{
public:

	// Modes
	enum { AUTO, PSAPI, TOOLHELP };

	//==============================================================
	// GetToolhelpInfo()
	//==============================================================
	/// Retreives task information using toolhelp
	/**
		\param [in] dwPid	-	Process ID
		\param [in] pti		-	Receives task information
		\param [in] bSubs	-	Non-zero to retreive information on sub tasks
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetToolhelpInfo( DWORD dwPid, LPTASKINFO pti, BOOL bSubs );

	//==============================================================
	// EnumProcToolhelp()
	//==============================================================
	/// Begins task enumeration using Toolhelp
	/**
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EnumProcToolhelp();

	//==============================================================
	// GetTaskInfo()
	//==============================================================
	/// Retreives task information
	/**
		\param [in] dwPid	-	Process ID
		\param [in] pti		-	Receives task information
		\param [in] bSubs	-	Non-zero to enumerate sub tasks
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetTaskInfo( DWORD dwPid, LPTASKINFO pti, BOOL bSubs );

	//==============================================================
	// GetPsapiInfo()
	//==============================================================
	/// Retreives task info using PSAPI
	/**
		\param [in] dwPid	-	Process ID
		\param [in] pti		-	Receives task information
		\param [in] bSubs	-	Non-zero to enumerate sub tasks
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetPsapiInfo( DWORD dwPid, LPTASKINFO pti, BOOL bSubs );

	//==============================================================
	// EnumProcPsapi()
	//==============================================================
	/// Enumerates running tasks using Psapi
	/**		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EnumProcPsapi();

	//==============================================================
	// EnumProcesses()
	//==============================================================
	/// Enumerates tasks
	/**
		\param [in] mode		-	Specify what API should be used 
		\param [in] bVdmdbg		-	Enable 16-bit process helper
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EnumProcesses( DWORD mode = AUTO, BOOL bVdmdbg = TRUE );

	//==============================================================
	// EnumModules()
	//==============================================================
	/// Enumerates sub items in specified task
	/**
		\param [in] pti		-	Task item information structure
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL EnumModules( LPTASKINFO pti )
	{	if ( pti == NULL ) return FALSE; return GetTaskInfo( pti->dwPid, pti, TRUE ); }

	//==============================================================
	// FindTask()
	//==============================================================
	/// Finds a task by specified information
	/**
		\param [in] pFile	-	Filename of task to find
		\param [in] pName	-	Name of task to find

		Wild cards are permited, such as "Vx*.dll"
		
		\return Pointer to task info structure if found, otherwise NULL
	
		\see 
	*/
	LPTASKINFO FindTask(LPCTSTR pFile, LPCTSTR pName);

	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Return size of TASKINFO structure
	virtual DWORD GetObjSize() { return sizeof( TASKINFO ); }

	/// Constructor
	CTaskList();

	/// Destructor
	virtual ~CTaskList();

	//==============================================================
	// Init()
	//==============================================================
	/// Initialize structure
	/**
		\param [in] mode		-	Specify what API should be used 
		\param [in] bVdmdbg		-	Enable 16-bit process helper
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Init( DWORD mode, BOOL bVdmdbg );

	//==============================================================
	// GetTasks()
	//==============================================================
	/// Returns pointer to task list
	CTask* GetTasks() { return &m_tasks; }

	//==============================================================
	// GetNumTasks()
	//==============================================================
	/// Returns total number of running tasks at time of capture
	DWORD GetNumTasks() { return m_tasks.Size(); }

	//==============================================================
	// GetPreferredBase()
	//==============================================================
	/// Gets the preferred base for a module
	/**
		\param [in] dwPid		-	Process ID
		\param [in] pModule		-	Address in module
		
		\return Pointer to preferred base address
	
		\see 
	*/
	static void* GetPreferredBase( DWORD dwPid, void *pModule );

	//==============================================================
	// Enum16Proc()
	//==============================================================
	/// Callback for 16-bit process enumeration
	/**
		\param [in] dwThreadId		-	Thread ID
		\param [in] hMod16			-	Module Handle
		\param [in] hTask16			-	Task handle
		\param [in] pszModName		-	Module name
		\param [in] pszFileName		-	Module filename
		\param [in] lpUserDefined	-	User defined data value
		
		\return Non-zero to continue enumeration
	
		\see 
	*/
	static BOOL CALLBACK Enum16Proc( DWORD dwThreadId, WORD hMod16, WORD hTask16, LPSTR pszModName, LPSTR pszFileName, LPARAM lpUserDefined );

public:

	//==============================================================
	// FindByPid()
	//==============================================================
	/// Searches for task information based on process ID
	/**
		\param [in] hModule		-	Handle to module
		
		\return Pointer to task information structure if found, otherwise NULL
	
		\see 
	*/
	LPTASKINFO FindByPid( HMODULE hModule );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases the task list information
	void Destroy();

	//==============================================================
	// IsPsapi()
	//==============================================================
	/// Returns non-zero if PSAPI library is loaded
	BOOL IsPsapi() { return m_psapi.IsLoaded(); }

	//==============================================================
	// IsToolhelp()
	//==============================================================
	/// Returns non-zero if Toolhelp API is loaded
	BOOL IsToolhelp() { return m_th.IsLoaded(); }

	//==============================================================
	// IsVdmdbg()
	//==============================================================
	/// Returns non-zero if 16-bit process helper API is loaded
	BOOL IsVdmdbg() { return m_vdmdbg.IsLoaded(); }

	//==============================================================
	// GetLastError()
	//==============================================================
	/// Returns a pointer to a string describing the last error
	LPCTSTR GetLastError() { return m_szErr; }

private:

	/// Task list
	CTask			m_tasks;

	/// PSAPI module library
	CModule			m_psapi;

	/// Toolhelp library
	CModule			m_th;

	/// 16 bit helper library
	CModule			m_vdmdbg;

	/// Buffer containing last error description
	char			m_szErr[ 256 ];

};

#endif // !defined(AFX_TASKLIST_H__3C592A6E_FF22_48DC_9711_C49182E4EC8C__INCLUDED_)
