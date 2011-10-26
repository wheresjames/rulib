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
// Params.h: interface for the CParams class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMS_H__7DE81FFF_5009_4AB1_9C0A_153D5EFCF311__INCLUDED_)
#define AFX_PARAMS_H__7DE81FFF_5009_4AB1_9C0A_153D5EFCF311__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define PTYPE_VOID			0
#define PTYPE_DWORD			1		
#define PTYPE_STR			2
#define PTYPE_BIN			3
#define PTYPE_LONG			4
#define PTYPE_ULONG			5
#define PTYPE_INT			6
#define PTYPE_UINT			7
#define PTYPE_CHAR			8
#define PTYPE_UCHAR			9
#define PTYPE_BOOL			10
#define PTYPE_FLOAT			11
#define PTYPE_DOUBLE		12
#define PTYPE_LDOUBLE		13

#define PFLAG_MIN			0x00000001
#define PFLAG_MAX			0x00000002
#define PFLAG_DEF			0x00000004
#define PFLAG_STEP			0x00000008


typedef struct tagPARAM
{
	DWORD		flags;
	BYTE		type;
	DWORD		size;
	void*		data;
	TCHAR		name[ 128 ];
	double		min;
	TCHAR		pmin[ 128 ];
	double		max;
	TCHAR		pmax[ 128 ];
	double		def;
	TCHAR		pdef[ 128 ];
	double		step;
	TCHAR		pstep[ 128 ];

} PARAM; // end typedef struct
typedef PARAM* LPPARAM;

typedef struct tagPARAMINFO : LLISTINFO
{
	DWORD		function;
	char		pdesc[ 1024 ];

	DWORD		n;
	LPPARAM		param;

	DWORD		size;
	LPBYTE		pdata;

} PARAMINFO; // end typedef struct
typedef PARAMINFO* LPPARAMINFO;
typedef LPPARAMINFO HPARAM;


//==================================================================
// CParams
//
/// Encapsulates functionality for saving and restoring function parameters
/**
	This class provides functionality for saving and restoring
	function parameters.  It enables the specification of parameter
	names, type, and range, and enforces these parameters.

*/
//==================================================================
class CParams : public CLList
{
public:

	//==============================================================
	// SetParam()
	//==============================================================
	/// Saves parameter data into data structure
	/**
		\param [in] pParam	-	Parameter information structure
		\param [in] ptr		-	Parameter data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetParam( LPPARAM pParam, void *ptr );

	//==============================================================
	// GetParam()
	//==============================================================
	/// Reads parameter data from data structure
	/**
		\param [in] pParam	-	Parameter information structure
		\param [out] ptr	-	Parameter data
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL GetParam( LPPARAM pParam, void *ptr );

	//==============================================================
	// SetValue()
	//==============================================================
	/// Saves parameter data
	/**
		\param [in] hParam	-	Handle to parameter
		\param [in] i		-	Parameter index
		\param [in] v		-	New parameter value
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL SetValue( HPARAM hParam, DWORD i, double v );

	//==============================================================
	// GetValue()
	//==============================================================
	/// Returns parameter data
	/**
		\param [in] hParam	-	Handle to parameter
		\param [in] i		-	Parameter index
		\param [in] dDef	-	Value to return on error
		
		\return Parameter value or dDef if error
	
		\see 
	*/
	double GetValue( HPARAM hParam, DWORD i, double dDef = 0 );
	
	//==============================================================
	// GetParamType()
	//==============================================================
	/// Converts a type string into type code
	/**
		\param [in] pType	-	Type string
		
		\return Type code for string
	
		\see 
	*/
	BYTE GetParamType( LPCTSTR pType );
	
	//==============================================================
	// Convert()
	//==============================================================
	/// Converts one parameter type to another
	/**
		\param [in] type	-	Conversion type
		\param [in] from	-	Source value
		
		\return Converted type
	
		\see 
	*/
	long double Convert( BYTE type, long double from );
	
	//==============================================================
	// GetToken()
	//==============================================================
	/// Returns a token from the specified buffer
	/**
		\param [out] pToken		-	Receives token
		\param [in] pStr		-	Source string
		\param [in] bSpaceOk	-	Non-zero to include spaces
		
		\return Offset of the end of the returned token in pStr
	
		\see 
	*/
	DWORD GetToken( LPSTR pToken, LPCTSTR pStr, BOOL bSpaceOk = FALSE );
	
	//==============================================================
	// GetParam()
	//==============================================================
	/// Converts a parameter declaration into its components
	/**
		\param [in] pList	-	Parameter declaration
		\param [in] pType	-	Parameter type
		\param [in] pSize	-	Parameter size
		\param [in] pName	-	Parameter name
		\param [in] pMin	-	Minimum value
		\param [in] pMax	-	Maximum value
		\param [in] pDef	-	Default value
		\param [in] pStep	-	Increment step

		Param forms ( all params optional except type )

		-	{type}[ size ] {name} <[min],[max]>, ...
		-	{type}[ size ] {name} <[max]>, ...
		-	{type}[ size ] {name}, ...
		-	{type}[ size ], ...
		-	{type}, ...

		Example param list:
		\code
			"char String[ 256 ]"
			"float Angle <0,359.99>"
			"uchar Hue <0,255,128>, uchar Saturation<0,255,128>"
			"long Level <0,255,80>, long Contrast<0,255,128>"
			"long New X <1,%width,%width>, long New Y<1,%height,%height>"
		\endcode
		
		\return 
	
		\see 
	*/
	DWORD GetParam( LPCTSTR pList, LPSTR pType, LPSTR pSize, LPSTR pName, LPSTR pMin, LPSTR pMax, LPSTR pDef, LPSTR pStep );
	
	//==============================================================
	// Load()
	//==============================================================
	/// Loads parameters from a configuration file
	/**
		\param [in] pCfg	-	Filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Load( CCfgFile *pCfg );
	
	//==============================================================
	// Save()
	//==============================================================
	/// Saves parameter information to a configuration file
	/**
		\param [in] pCfg	-	Filename
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Save( CCfgFile *pCfg );
	
	//==============================================================
	// GetParamSize()
	//==============================================================
	/// Returns the parameter size in bytes
	/**
		\param [in] type	-	Parameter type
		\param [in] size	-	Parameter array size
		
		\return Parameter size in bytes
	
		\see 
	*/
	DWORD GetParamSize( BYTE type, DWORD size );
	
	//==============================================================
	// Add()
	//==============================================================
	/// Adds a function to the list
	/**
		\param [in] function	-	Function ID
		\param [in] pParams		-	Function parameter definition
		
		\return Handle to parameters if success
	
		\see 
	*/
	HPARAM Add( DWORD function, LPCTSTR pParams );
	
	//==============================================================
	// DeleteObject()
	//==============================================================
	/// Provides custom destruction of parameter structures
	/**
		\param [in] node	-	Pointer to parameter structure
	*/
	virtual void DeleteObject( void *node );
	
	//==============================================================
	// Init()
	//==============================================================
	/// Initializes this class for use
	/**
		\param [in] bDestroy	-	Non-zero to destroy previous
									parameter list
	*/
	void Init( BOOL bDestroy = TRUE );
	
	//==============================================================
	// Destroy()
	//==============================================================
	/// Releases parameter list
	void Destroy();
	
	//==============================================================
	// GetObjSize()
	//==============================================================
	/// Returns the size of the parameter item information structure
	virtual DWORD GetObjSize() { return sizeof( PARAMINFO ); }

	/// Default constructor
	CParams();

	/// Destructor
	virtual ~CParams();

	//==============================================================
	// SetParams()
	//==============================================================
	/// Sets function parameter values
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] ...		-	Function value list

		... values must match that described in hParam function
		parameter declarations.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL _cdecl SetParams( HPARAM hParam, ... );
	
	//==============================================================
	// GetParams()
	//==============================================================
	/// Gets function parameter values
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] ...		-	Function value list

		... values must match that described in hParam function
		parameter declarations.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL _cdecl GetParams( HPARAM hParam, ... );

	//==============================================================
	// Load()
	//==============================================================
	/// Loads function parameter definitions from a memory buffer
	/**
		\param [in] buf		-	Memory buffer
		\param [in] size	-	Number of bytes in buf
		\param [in] bMerge	-	Non-zero to merge into current list
		
		\return Number of bytes read or zero if failure
	
		\see 
	*/
	virtual DWORD Load( LPBYTE buf, DWORD size, BOOL bMerge = FALSE );
	
	//==============================================================
	// Save()
	//==============================================================
	/// Saves function parameter definitions into a memory buffer
	/**
		\param [out] buf	-	Memory buffer
		\param [in] size	-	Size of buffer in buf
		
		\return Number of bytes written to buf or zero if failure
	
		\see 
	*/
	virtual DWORD Save( LPBYTE buf, DWORD size );
	
	//==============================================================
	// Load()
	//==============================================================
	/// Loads function parameter definitions from a disk file
	/**
		\param [in] pFile	-	Filename
		\param [in] bMerge	-	Non-zero to merge into current list
		
		\return Number of bytes read or zero if failure
	
		\see 
	*/
	virtual DWORD Load( LPCTSTR pFile, BOOL bMerge = FALSE );
	
	//==============================================================
	// Load()
	//==============================================================
	/// Saves function parameter definitions into a disk file
	/**
		\param [in] pFile	-	Filename
		
		\return Number of bytes written or zero if failure
	
		\see 
	*/
	virtual DWORD Save( LPCTSTR pFile );

	//==============================================================
	// CParams::GetName()
	//==============================================================
	/// Returns a pointer to the name of the parameter
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] i		-	Index of parameter
		
		\return Pointer to parameter name
	
		\see 
	*/
	LPCTSTR CParams::GetName(HPARAM hParam, DWORD i)
	{	if ( hParam == NULL || i >= hParam->n ) return "";
		return hParam->param[ i ].name;
	}

	//==============================================================
	// CParams::GetMin()
	//==============================================================
	/// Returns the minimum value of a parameter
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] i		-	Index of parameter
		
		\return Parameter minimum value
	
		\see 
	*/
	double CParams::GetMin(HPARAM hParam, DWORD i)
	{	if ( hParam == NULL || i >= hParam->n ) return (double)0;
		if ( hParam->param[ i ].pmin[ 0 ] == '%' )
			return Replace( &hParam->param[ i ].pmin[ 1 ] );
		return hParam->param[ i ].min;
	}

	//==============================================================
	// CParams::GetMax()
	//==============================================================
	/// Returns the maximum value of a parameter
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] i		-	Index of parameter
		
		\return Parameter maximum value
	
		\see 
	*/
	double CParams::GetMax(HPARAM hParam, DWORD i)
	{	if ( hParam == NULL || i >= hParam->n ) return (double)0;
		if ( hParam->param[ i ].pmax[ 0 ] == '%' )
			return Replace( &hParam->param[ i ].pmax[ 1 ] );
		return hParam->param[ i ].max;
	}

	//==============================================================
	// CParams::GetDef()
	//==============================================================
	/// Returns the default value of a parameter
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] i		-	Index of parameter
		
		\return Parameter default value
	
		\see 
	*/
	double CParams::GetDef(HPARAM hParam, DWORD i)
	{	if ( hParam == NULL || i >= hParam->n ) return (double)0;
		if ( hParam->param[ i ].pdef[ 0 ] == '%' )
			return Replace( &hParam->param[ i ].pdef[ 1 ] );
		return hParam->param[ i ].def;
	}

	//==============================================================
	// CParams::GetStep()
	//==============================================================
	/// Returns the step value of a parameter
	/**
		\param [in] hParam	-	Handle to parameter list
		\param [in] i		-	Index of parameter

		This is the resolution of the parameter, or how much you should
		add to move from one value to the subsequent value.  For instance,
		a step value of 2 would indicate the parameter can only validly
		be set to even numbers.

		\note	There should be added a corrisponding value offset, but
				I didn't need it in the implementation I was working on.
				It would make this feature more general case however.
	  
		\return Parameter step value
	
		\see 
	*/
	double CParams::GetStep(HPARAM hParam, DWORD i)
	{	if ( hParam == NULL || i >= hParam->n ) return (double)0;
		if ( hParam->param[ i ].pstep[ 0 ] == '%' )
			return Replace( &hParam->param[ i ].pstep[ 1 ] );
		return hParam->param[ i ].step;
	}

	CVar& GetVar() { return m_var; }

protected:

	//==============================================================
	// Replace()
	//==============================================================
	/// Returns the value of the specified variable
	/**
		\param [in] pVar	-	Name of variable from the list
		\param [in] dDef	-	Default return value
		
		\return Value of named variable or dDef if error
	
		\see 
	*/
	double Replace( LPCTSTR pVar, double dDef = 0 )
	{	double v = dDef;
		m_var.GetVar( pVar, &v );
		return v;
	} // end double

	// Replace variables
	CVar		m_var;

};

#endif // !defined(AFX_PARAMS_H__7DE81FFF_5009_4AB1_9C0A_153D5EFCF311__INCLUDED_)
