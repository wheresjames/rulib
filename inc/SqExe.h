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
// SqExe.h: interface for the CXpTheme class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SQEXE_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_)
#define SQEXE_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Is squirrel script enabled?
#if defined( ENABLE_SQUIRREL )

namespace SqPlus
{
	inline void Push(HSQUIRRELVM v,const std::string& value)
	{	sq_pushstring(v,value.c_str(),value.length()); }
	inline bool Match(TypeWrapper<const std::string&>, HSQUIRRELVM v, int idx)
	{	return sq_gettype(v,idx) == OT_STRING; }
	inline std::string Get(TypeWrapper<const std::string&>,HSQUIRRELVM v,int idx)
	{	const SQChar * s = 0;
		SQInteger sz = sq_getsize(v,idx);
		SQPLUS_CHECK_GET(sq_getstring(v,idx,&s));
		return std::string(s,(std::string::size_type)sz);
	}
};

//==================================================================
// CSqExe
//
/// Thin wrapper around the SqPlus API
/**
    Provides a quick and easy wrapper around the SqPlus/Squirrel API.
*/
//==================================================================
class CSqExeExport 
{                        
public:

    // Simple alert function
    int alert( const std::string &sMsg )
    {   return ::MessageBox( NULL, sMsg.c_str(), _T( "Squirrel Script" ), MB_OK ); }

    int import( const std::string &sClass )
    {   return OnImport( sClass ); }

    virtual int OnImport( const std::string &sClass ) { return 0; }

    void log( LPCTSTR pLog )
    {
        if ( !pLog || !*pLog )
            return;

        if ( m_bFile ) 
            _vLOG( _ES_WARNING, ERROR_GEN_FAILURE, "SQ : %s : %s", m_sScript.c_str(), pLog ); 

        else
            _vLOG( _ES_WARNING, ERROR_GEN_FAILURE, "SQ : %s", pLog ); 
    }

    void sleep( UINT uMilliseconds )
    {   ::Sleep( uMilliseconds ); }


protected:

    /// Non-zero if loaded from file
    BOOL                m_bFile;

    /// Either a file name or actual script
    std::string         m_sScript;
    
};

// Declare class functions
DECLARE_INSTANCE_TYPE( CSqExeExport );
DECLARE_INSTANCE_TYPE( CWinTime );
DECLARE_INSTANCE_TYPE( CFindFiles );
DECLARE_INSTANCE_TYPE( CWinFile );
DECLARE_INSTANCE_TYPE( CReg );
DECLARE_INSTANCE_TYPE( CWinImg );
DECLARE_INSTANCE_TYPE( CWin32 );

/// Simplistic interface to the squirrel library
class CSqExe : public CSqExeExport
{
public:

    /// Contains information about a script error
    struct SScriptErrorInfo
    {
        /// Constructor
        SScriptErrorInfo( std::string x_sDesc, std::string x_sSource, UINT x_uLine, UINT x_uCol )
        {   sDesc = x_sDesc; sSource = x_sSource; uLine = x_uLine; uCol = x_uCol; }

        /// Description
        std::string     sDesc;

        /// Source file
        std::string     sSource;

        /// Line number
        UINT            uLine;

        /// Column number
        UINT            uCol;
    };

public:

    /// Defualt constructor
    CSqExe() : m_vm( SquirrelVM::StdLib_All ), m_script( m_vm.GetVMHandle() ) 
    {
        m_bLoaded = FALSE;
        m_bFile = FALSE;
    }

    /// Destructor
    virtual ~CSqExe() { Destroy(); }

    /// Destroys the virtual machine and prepares the class for re-use
    void Destroy();

    /// Over-ride to perform special registration
    virtual void OnRegisterVariables() {}

    /// Initializes the virtual machine
    /**
        You don't need to call this directly, it is called by Load().
    */
    BOOL Init();

    /// Loads a script and prepares it for execution
    /**
        \param [in] pScript     -   The script or a file name.
        \param [in] bFile       -   If non-zero, pScript contains a file name.

    */
    BOOL Load( LPCTSTR pScript, BOOL bFile, BOOL bRelative = FALSE, BOOL bStart = TRUE );

    /// Runs the script an executes the initialization function
    /**
        You can just set the bStart flag in Load() to TRUE to avoid
        calling this function if you have no special registrations.
    */
    BOOL Start();

    /// Executes a specific function within the script
    /**
        \param [in] pFunction   -   Name of the function to execute
    */
    BOOL Execute( LPCTSTR pFunction );

    /// Executes a specific function within the script
    /**
        \param [in] soFunction   -   Name of the function to execute
    */
    BOOL Execute( SquirrelObject &soFunction );

    /// Executes a specific function within the script
    /**
        \param [in] pFunction   -   Name of the function to execute
    */
    template< typename T_P1 >
        BOOL Execute( LPCTSTR pFunction, T_P1 &p1 )
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

                f( p1 );

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

    /// Executes a specific function within the script
    /**
        \param [in] pFunction   -   Name of the function to execute
    */
    template< typename T_P1, typename T_P2 >
        BOOL Execute( LPCTSTR pFunction, T_P1 &p1, T_P2 &p2 )
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

                f( p1, p2 );

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

    /// Executes a specific function within the script
    /**
        \param [in] pFunction   -   Name of the function to execute
    */
    template< typename T_P1, typename T_P2, typename T_P3 >
        BOOL Execute( LPCTSTR pFunction, T_P1 &p1, T_P2 &p2, T_P3 &p3 )
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

                f( p1, p2, p3 );

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

    /// Executes a specific function within the script
    /**
        \param [in] soFunction   -   Function to execute
    */
    template< typename T_P1 >
        BOOL Execute( SquirrelObject &soFunction, T_P1 &p1 )
        {
            if ( !IsScript() )
                return FALSE;

            if ( NULL == &soFunction )
                return FALSE;;

            try
            {
                SqPlus::SquirrelFunction< void > f( m_vm, m_vm.GetRootTable(), soFunction ); 

                if ( f.func.IsNull() )
                {   m_sErr = "Invalid function object";
                    return FALSE;
                } // end if

                f( p1 );

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

    /// Executes a specific function within the script
    /**
        \param [in] soFunction   -   Function to execute
    */
    template< typename T_P1, typename T_P2 >
        BOOL Execute( SquirrelObject &soFunction, T_P1 &p1, T_P2 &p2 )
        {
            if ( !IsScript() )
                return FALSE;

            if ( NULL == &soFunction )
                return FALSE;;

            try
            {
                SqPlus::SquirrelFunction< void > f( m_vm, m_vm.GetRootTable(), soFunction ); 

                if ( f.func.IsNull() )
                {   m_sErr = "Invalid function object";
                    return FALSE;
                } // end if

                f( p1, p2 );

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

    /// Executes a specific function within the script
    /**
        \param [in] soFunction   -   Function to execute
    */
    template< typename T_P1, typename T_P2, typename T_P3 >
        BOOL Execute( SquirrelObject &soFunction, T_P1 &p1, T_P2 &p2, T_P3 &p3 )
        {
            if ( !IsScript() )
                return FALSE;

            if ( NULL == &soFunction )
                return FALSE;;

            try
            {
                SqPlus::SquirrelFunction< void > f( m_vm, m_vm.GetRootTable(), soFunction ); 

                if ( f.func.IsNull() )
                {   m_sErr = "Invalid function object";
                    return FALSE;
                } // end if

                f( p1, p2, p3 );

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

    /// Executes a specific function within the script
    /**
        \param [in] soFunction   -   Function to execute
    */
    template< typename T_P1 >
        BOOL ExecuteCtx( SquirrelObject &soRoot, SquirrelObject &soFunction, T_P1 &p1 )
        {
            if ( !IsScript() )
                return FALSE;

            if ( NULL == &soFunction )
                return FALSE;;

            try
            {
                SqPlus::SquirrelFunction< void > f( m_vm, soRoot, soFunction ); 

                if ( f.func.IsNull() )
                {   m_sErr = "Invalid function object";
                    return FALSE;
                } // end if

                f( p1 );

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

    /// Registers library classes
    virtual int OnImport( const std::string &sClass );

    /// Intercepts print statements and puts the data in m_sOutput
    static void SqPrint( HSQUIRRELVM v, const SQChar* s, ... );

    /// Intercepts errors
    static SQInteger CSqExe::SqErrorHandler( HSQUIRRELVM v );

    /// Handles a script compiler error
    static void CSqExe::SqCompilerErrorHandler( HSQUIRRELVM v, const SQChar *sErr, const SQChar *sSource, SQInteger line, SQInteger column );

    /// Auxiliary error handler
    static SQInteger CSqExe::SqAuxErrorHandler( HSQUIRRELVM v );

    /// Executes the specified buffer
    BOOL Run( LPCTSTR pScript );

    /// Returns non-zero if squirrel has been initialized
    BOOL IsScript() { return m_bLoaded; }

    /// Returns the last error
    std::string& GetLastError() { return m_sErr; }

    /// Returns the script output
    std::string& GetOutput() { return m_sOutput; }

    /// Returns a reference the to the virtual machine
    SquirrelVM& GetVM() { return m_vm; }

    /// Returns a reference to the virtual machine
    operator SquirrelVM&() { return m_vm; }

    /// Returns a referenced to the compiled script object
    SquirrelObject& GetScriptObj() { return m_script; }

    /// Returns the scripts root table
    const SquirrelObject& GetRootTable() { return m_vm.GetRootTable(); }

    /// Binds variables to the root table
    template< typename T >
        void BindRootVariable( T *pVar, LPCTSTR pName, LPCTSTR pImport = NULL, SqPlus::VarAccessType access = SqPlus::VAR_ACCESS_READ_WRITE )
        {   if ( pImport && *pImport ) import( pImport );
            SquirrelObject root = m_vm.GetRootTable();
            SqPlus::BindVariable( m_vm, root, pVar, pName, access ); 
        }

    template< typename T >
        void BindRootFunction( T pFunction, LPCTSTR pName )
        {   SqPlus::RegisterGlobal( m_vm, pFunction, pName );
        }

    template< typename T_CLASS, typename T_FUNCTION >
        void BindRootMemberFunction( T_CLASS pClass, T_FUNCTION pFunction, LPCTSTR pName )
        {   SqPlus::RegisterGlobal( m_vm, pClass, pFunction, pName );
        }

    /// Adds a log message on behalf of Squirrel
    static void SqLog( const SQChar *pLog );

private:

    /// Non-zero if squirrel has been initialized
    BOOL                m_bLoaded;
    
    /// Squirrel virtual machine
    SquirrelVM          m_vm;

    /// Squirrel script
    SquirrelObject      m_script;

    /// Last squirrel error
    std::string         m_sErr;

    /// Script output
    std::string         m_sOutput;
};

#endif // defined( ENABLE_SQUIRREL )

#endif // !defined(SQEXE_H__DBB25F77_FB59_4672_8D95_E01888CDC388__INCLUDED_)
