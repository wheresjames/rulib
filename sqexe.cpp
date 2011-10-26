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
// SqExe.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include <ShlObj.h>
#include <Shellapi.h>

#if defined( ENABLE_SQUIRREL )

#ifdef DEBUG_NEW
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#endif

// Disable NULL reference warning
#pragma warning( disable : 327 )

#define     SQEXE_FN_INIT       _T( "_init" )
#define     SQEXE_FN_END        _T( "_end" )
#define     SQEXE_THIS          _T( "_parent" )
#define     SQEXE_SELF          _T( "_self" )

void CSqExe::Destroy() 
{
    if ( !m_bLoaded ) 
        return;

    Execute( SQEXE_FN_END );

    m_bLoaded = FALSE;

    m_bFile = FALSE;

    m_script.Reset();

    m_vm.Shutdown();

    m_sErr = "";
    m_sOutput = "";
}

BOOL CSqExe::Init()
{
    Destroy();

    try
    {
        m_vm.Init();

        // Set compiler error handler
        sq_setcompilererrorhandler( m_vm.GetVMHandle(), &CSqExe::SqCompilerErrorHandler );

        // Set print function
        sq_setprintfunc( m_vm.GetVMHandle(), CSqExe::SqPrint );

        // Set run time error handler
//        sq_newclosure( m_vm.GetVMHandle(), &CSqExe::SqErrorHandler, 0 );

        // Set auxiliary error handler
    	sq_newclosure( m_vm.GetVMHandle(), &CSqExe::SqAuxErrorHandler, 0 );

        sq_seterrorhandler( m_vm.GetVMHandle() );

        // Define our base class
        SqPlus::SQClassDef< CSqExeExport > ( m_vm, _T( "CSqExeExport" ) )
                                            .func( &CSqExeExport::alert, "alert" )
                                            .func( &CSqExeExport::import, "import" )
                                            .func( &CSqExeExport::log, "log" )
                                            .func( &CSqExeExport::sleep, "sleep" )
                                          ;

        // Set base class pointer
        BindRootVariable( (CSqExeExport*)this, SQEXE_THIS );
        BindRootVariable( (CSqExeExport*)this, SQEXE_SELF );

        // Allow derived class to register it's stuff
        OnRegisterVariables();

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    m_bLoaded = TRUE;

    return TRUE;
}

BOOL CSqExe::Start()
{
    if ( !IsScript() )
        return FALSE;

    try
    {
        // Initialize the script
        m_vm.RunScript( m_script ); 

        // Execute init function
        Execute( SQEXE_FN_INIT );

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    return TRUE;
}

BOOL CSqExe::Load( LPCTSTR pScript, BOOL bFile, BOOL bRelative, BOOL bStart )
{
    if ( !pScript || !*pScript ) 
        return FALSE;

    if ( !Init() )
        return FALSE;

    try
    {
        char szFull[ CWF_STRSIZE ] = "";
        if ( bRelative )
        {   CWinFile::GetExePath( szFull, sizeof( szFull ) );
            CWinFile::BuildPath( szFull, szFull, pScript );
            pScript = szFull;
        } // end if

        // Load the script
	    m_script = bFile ? m_vm.CompileScript( pScript ) 
	 				     : m_vm.CompileBuffer( pScript );

        if ( bStart )
        {
            // Initialize the script
            m_vm.RunScript( m_script ); 

            // Execute init function
            Execute( SQEXE_FN_INIT );

        } // end if

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    // Save script source information
    m_bFile = bFile;
    m_sScript = pScript;

    return TRUE;
}

BOOL CSqExe::Run( LPCTSTR pScript )
{
    if ( !pScript || !*pScript ) 
        return FALSE;

    if ( !IsScript() )
        return FALSE;

    try
    {
        m_vm.RunScript( m_vm.CompileBuffer( pScript ) );

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    return TRUE;
}

BOOL CSqExe::Execute( LPCTSTR pFunction )
{
    if ( !IsScript() )
        return FALSE;

    if ( !pFunction )
        return FALSE;;

    try
    {
        SqPlus::SquirrelFunction< void > f( m_vm, m_vm.GetRootTable(), pFunction );            

        if ( f.func.IsNull() )
        {   m_sErr = "Function not found : "; m_sErr += pFunction;
            return FALSE;
        } // end if

        f();

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    return TRUE;
}

BOOL CSqExe::Execute( SquirrelObject &soFunction )
{
    if ( !IsScript() )
        return FALSE;

    if ( NULL == &soFunction )
        return FALSE;;

    try
    {
        SqPlus::SquirrelFunction< void > f( m_vm, m_vm.GetRootTable(), soFunction );            

        if ( f.func.IsNull() )
        {   m_sErr = "Bad function object";
            return FALSE;
        } // end if

        f();

    } // end try

    catch( SScriptErrorInfo &e )
    {
        TCHAR szErr[ CWF_STRSIZE ] = _T( "" );

        // Format
        _sntprintf( szErr, sizeof( szErr ), "%s(%lu)\r\n   %s", e.sSource.c_str(), e.uLine, e.sDesc.c_str() );
        szErr[ sizeof( szErr ) - 1 ] = 0;

        // Save string
        m_sErr = szErr;
        
        return FALSE;

    } // end catch

    catch( SquirrelError &e )
    {
	    m_sErr = e.desc;

        return FALSE;

    } // end catch

    return TRUE;
}

void CSqExe::SqPrint( HSQUIRRELVM v, const SQChar* s, ... )
{
    StackHandler sa( v );
    SquirrelObject root( v, sa.GetObjectHandle( 1 ) );

    // Retrieve our class pointer
    SquirrelObject ptr = root.GetValue( SQEXE_SELF );
    CSqExe *pSqExe = (CSqExe*)(CSqExeExport*)ptr.GetInstanceUP( SqPlus::ClassType< CSqExeExport >::type() );
    if ( !pSqExe ) return;

    va_list vl;
    va_start( vl, s );

    SQChar str[ CWF_STRSIZE ] = "";
    SQChar *pBuf = str;
    int nSize = sizeof( str );

    // Until we get it right
    if ( 0 > vsnprintf( pBuf, nSize, s, vl ) && nSize < ( 10 * 1024 * 1024 ))
        nSize <<= 2, pBuf = new SQChar[ nSize ];

    // Ensure NULL terminated
    pBuf[ nSize - 1 ] = 0;

    // Save the output string
    pSqExe->m_sOutput += pBuf;

    // Lose dynamic buffer if needed
    if ( nSize != sizeof( str ) && pBuf )
        delete [] pBuf;

    va_end( vl );
}

SQInteger CSqExe::SqErrorHandler( HSQUIRRELVM v )
{
    if( 0 >= sq_gettop( v ) ) 
        return 0;

    const SQChar *sErr = 0;
    if( SQ_SUCCEEDED( sq_getstring( v, -1, &sErr ) ) ) 
        throw SScriptErrorInfo( sErr, "", 0, 0 );

    else
        throw SScriptErrorInfo( "Unknown error", "", 0, 0 );

    return 0;
}

void CSqExe::SqCompilerErrorHandler( HSQUIRRELVM v, const SQChar *sErr, const SQChar *sSource, SQInteger line, SQInteger column )
{
    throw SScriptErrorInfo( sErr, sSource, line, column );
}

SQInteger CSqExe::SqAuxErrorHandler( HSQUIRRELVM v )
{
    if( 0 >= sq_gettop( v ) ) 
        return 0;

    std::string sErr;
    const SQChar *pErr = 0;
    if( SQ_SUCCEEDED( sq_getstring( v, -1, &pErr ) ) ) 
        sErr = pErr;
    else
        sErr = "Unknown Error";

    // Get stack trace
    SQStackInfos si;
	if ( SQ_SUCCEEDED( sq_stackinfos( v, 1, &si ) ) )
    {   std::string sMsg;
        if ( si.funcname ) 
        {   sMsg += si.funcname; 
            sMsg += "() : "; 
        } // end if
        sMsg += sErr;
        throw SScriptErrorInfo( sMsg.c_str(), si.source ? si.source : "", si.line, 0 );
    } // end if

    else 
        throw SScriptErrorInfo( sErr.c_str(), "", 0, 0 );

    return 0;
}

//------------------------------------------------------------------
// >>> Squirrel exports
//
// import these functions by calling _parent.import( <class name> );
//
// example 
//
// _parent.import( "CWinFile" );
//
// Other functions
//
// // Displays a message box
// _parent.alert( "Hello" );
//
// // Makes entry into the log file
// _parent.log( "Error accessing file" );
//
//------------------------------------------------------------------
int CSqExe::OnImport( const std::string &sClass )
{
    if ( sClass == "CWinTime" )

        SqPlus::SQClassDef< CWinTime > ( m_vm, _T( "CWinTime" ) )
                . func( &CWinTime::GetLocalTime, "GetLocalTime" )
                . func( &CWinTime::GetSystemTime, "GetSystemTime" )
                . func( &CWinTime::GetUnixTime, "GetUnixTime" )
                . func( &CWinTime::SetUnixTime, "SetUnixTime" )
                . func( &CWinTime::GetNetTime, "GetNetTime" )
                . func( &CWinTime::SetNetTime, "SetNetTime" )
                . func( &CWinTime::GetDosTime, "GetDosTime" )
                . func( &CWinTime::SetDosTime, "SetDosTime" )
                . func( &CWinTime::sGetString, "GetString" )
                . func( &CWinTime::sParseString, "ParseString" )
                . func( &CWinTime::GetHour, "GetHour" )
                . func( &CWinTime::SetHour, "SetHour" )
                . func( &CWinTime::GetMinute, "GetMinute" )
                . func( &CWinTime::SetMinute, "SetMinute" )
                . func( &CWinTime::GetSecond, "GetSecond" )
                . func( &CWinTime::SetSecond, "SetSecond" )
                . func( &CWinTime::GetMilliSeconds, "GetMilliSeconds" )
                . func( &CWinTime::SetMilliSeconds, "SetMilliSeconds" )
                . func( &CWinTime::GetDay, "GetDay" )
                . func( &CWinTime::SetDay, "SetDay" )
                . func( &CWinTime::GetMonth, "GetMonth" )
                . func( &CWinTime::SetMonth, "SetMonth" )
                . func( &CWinTime::GetYear, "GetYear" )
                . func( &CWinTime::SetYear, "SetYear" )
            ;

    else if ( sClass == "CFindFiles" )

        SqPlus::SQClassDef< CFindFiles > ( m_vm, _T( "CFindFiles" ) )
                . func( &CFindFiles::sFindFirst, "FindFirst" )
                . func( &CFindFiles::FindNext, "FindNext" )
                . func( &CFindFiles::GetPath, "GetPath" )
                . func( &CFindFiles::GetFullPath, "GetFullPath" )
                . func( &CFindFiles::GetFileName, "GetFileName" )
                . func( &CFindFiles::sIsDirectory, "IsDirectory" )
            ;

    else if ( sClass == "CWinFile" )

        SqPlus::SQClassDef< CWinFile > ( m_vm, _T( "CWinFile" ) )
                . staticFunc( &CWinFile::sDelete, "Delete" )
                . staticFunc( &CWinFile::sDeleteDirectory, "DeleteDirectory" )
                . staticFunc( &CWinFile::sCreateDirectory, "CreateDirectory" )
                . staticFunc( &CWinFile::sBuildPath, "BuildPath" )                    
                . staticFunc( &CWinFile::sGetContents, "GetContents" )
                . staticFunc( &CWinFile::sPutContents, "PutContents" )
                . staticFunc( &CWinFile::sGetOpenFileName, "GetOpenFileName" )
                . staticFunc( &CWinFile::sGetSaveFileName, "GetSaveFileName" )
                . staticFunc( &CWinFile::sGetModulePath, "GetModulePath" )
                . staticFunc( &CWinFile::sGetFileNameFromPath, "GetFileNameFromPath" )
                . staticFunc( &CWinFile::sGetPathFromFileName, "GetPathFromFileName" )
                . staticFunc( &CWinFile::DoesExist, "DoesExist" )

				. enumInt( (int)CSIDL_USER_SYSTEM,				_T( "CSIDL_USER_SYSTEM" ) )
				. enumInt( (int)CSIDL_USER_WINDOWS,				_T( "CSIDL_USER_WINDOWS" ) )
				. enumInt( (int)CSIDL_USER_TEMP,				_T( "CSIDL_USER_TEMP" ) )
				. enumInt( (int)CSIDL_USER_CURRENT,				_T( "CSIDL_USER_CURRENT" ) )
				. enumInt( (int)CSIDL_USER_DEFDRIVE,			_T( "CSIDL_USER_DEFDRIVE" ) )
				. enumInt( (int)CSIDL_ALTSTARTUP,				_T( "CSIDL_ALTSTARTUP" ) )
				. enumInt( (int)CSIDL_APPDATA,					_T( "CSIDL_APPDATA" ) )
				. enumInt( (int)CSIDL_BITBUCKET,				_T( "CSIDL_BITBUCKET" ) )
				. enumInt( (int)CSIDL_COMMON_ALTSTARTUP,		_T( "CSIDL_COMMON_ALTSTARTUP" ) )
				. enumInt( (int)CSIDL_COMMON_DESKTOPDIRECTORY,  _T( "CSIDL_COMMON_DESKTOPDIRECTORY" ) )
				. enumInt( (int)CSIDL_COMMON_FAVORITES,			_T( "CSIDL_COMMON_FAVORITES" ) )
				. enumInt( (int)CSIDL_COMMON_PROGRAMS,			_T( "CSIDL_COMMON_PROGRAMS" ) )
				. enumInt( (int)CSIDL_COMMON_STARTMENU,			_T( "CSIDL_COMMON_STARTMENU" ) )
				. enumInt( (int)CSIDL_COMMON_STARTUP,			_T( "CSIDL_COMMON_STARTUP" ) )
				. enumInt( (int)CSIDL_CONTROLS,					_T( "CSIDL_CONTROLS" ) )
				. enumInt( (int)CSIDL_COOKIES,					_T( "CSIDL_COOKIES" ) )
				. enumInt( (int)CSIDL_DESKTOP,					_T( "CSIDL_DESKTOP" ) )
				. enumInt( (int)CSIDL_DESKTOPDIRECTORY,			_T( "CSIDL_DESKTOPDIRECTORY" ) )
				. enumInt( (int)CSIDL_DRIVES,					_T( "CSIDL_DRIVES" ) )
				. enumInt( (int)CSIDL_FAVORITES,				_T( "CSIDL_FAVORITES" ) )
				. enumInt( (int)CSIDL_FONTS,					_T( "CSIDL_FONTS" ) )
				. enumInt( (int)CSIDL_HISTORY,					_T( "CSIDL_HISTORY" ) )
				. enumInt( (int)CSIDL_INTERNET,					_T( "CSIDL_INTERNET" ) )
				. enumInt( (int)CSIDL_INTERNET_CACHE,			_T( "CSIDL_INTERNET_CACHE" ) )
				. enumInt( (int)CSIDL_NETHOOD,					_T( "CSIDL_NETHOOD" ) )
				. enumInt( (int)CSIDL_NETWORK,					_T( "CSIDL_NETWORK" ) )
				. enumInt( (int)CSIDL_PERSONAL,					_T( "CSIDL_PERSONAL" ) )
				. enumInt( (int)CSIDL_PRINTERS,					_T( "CSIDL_PRINTERS" ) )
				. enumInt( (int)CSIDL_PRINTHOOD,				_T( "CSIDL_PRINTHOOD" ) )
				. enumInt( (int)CSIDL_PROGRAMS,					_T( "CSIDL_PROGRAMS" ) )
				. enumInt( (int)CSIDL_RECENT,					_T( "CSIDL_RECENT" ) )
				. enumInt( (int)CSIDL_SENDTO,					_T( "CSIDL_SENDTO" ) )
				. enumInt( (int)CSIDL_STARTMENU,				_T( "CSIDL_STARTMENU" ) )
				. enumInt( (int)CSIDL_STARTUP,					_T( "CSIDL_STARTUP" ) )
				. enumInt( (int)CSIDL_TEMPLATES,				_T( "CSIDL_TEMPLATES" ) )
                . staticFunc( &CWinFile::sGetSpecialFolder, "GetSpecialFolder" )
            ;

    else if ( sClass == "CReg" )

        SqPlus::SQClassDef< CReg > ( m_vm, _T( "CReg" ) )
                . func( &CReg::sGet, "Get" )
                . func( &CReg::sSet, "Set" )
                . func( &CReg::sIsSet, "IsSet" )
            ;

    else if ( sClass == "CWinImg" )

        SqPlus::SQClassDef< CWinImg > ( m_vm, _T( "CWinImg" ) )
                . func( &CWinImg::Create24bpp, "Create" )
                . func( &CWinImg::Load, "Load" )
                . func( &CWinImg::Save, "Save" )
                . func( &CWinImg::IsValid, "IsValid" )
                . func( &CWinImg::GetWidth, "GetWidth" )
                . func( &CWinImg::GetHeight, "GetHeight" )
                . func( &CWinImg::FillSolid, "FillSolid" )
                . func( &CWinImg::Aluminize, "Aluminize" )
                . func( &CWinImg::Texturize, "Texturize" )
//                . func( &CWinImg::Resample, "Resample" )
            ;

    else if ( sClass == "CNetFile" )
        
        SqPlus::SQClassDef< CNetFile > ( m_vm, _T( "CNetFile" ) )
            . func( &CNetFile::sqDownload,          "Download" )
            . func( &CNetFile::sqUpload,            "Upload" )
            . func( &CNetFile::Close,               "Close" )
            . func( &CNetFile::GetTransferStatus,   "GetTransferStatus" )
            . func( &CNetFile::GetTransferError,    "GetTransferError" )
            . func( &CNetFile::GetDataRead,         "GetDataRead" )
            . func( &CNetFile::GetDataWritten,      "GetDataWritten" )
            . func( &CNetFile::SetUsernamePassword, "SetUsernamePassword" )
            . func( &CNetFile::EnableCache,         "EnableCache" )
            . func( &CNetFile::IsWorking,           "IsWorking" )
            . func( &CNetFile::GetTotalSize,        "GetTotalSize" )
            . func( &CNetFile::sqSetData,           "SetData" )
            . func( &CNetFile::sqGetData,           "GetData" )
        ;

    else if ( sClass == "CWin32" )
        
        SqPlus::SQClassDef< CWin32 > ( m_vm, _T( "CWin32" ) )
            . staticFunc( &CWin32::sMD5_guid,          "Md5Guid" )
            . staticFunc( &CWin32::sMD5_base64,        "Md5Base64" )
        ;

    else 
        return 0;

    return 1;
}

#endif // defined( ENABLE_SQUIRREL )

