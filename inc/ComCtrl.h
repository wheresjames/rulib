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
// ComCtrl.h: interface for the CComCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMCTRL_H__BA15C2F5_38AD_42E4_8BBD_274AF4B45989__INCLUDED_)
#define AFX_COMCTRL_H__BA15C2F5_38AD_42E4_8BBD_274AF4B45989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CommCtrl.h>

#define TreeView_SetItemImage( tree, it, i, si )				\
{																\
	TVITEM	item;												\
	ZeroMemory( &item, sizeof( item ) );						\
	item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;	\
	item.hItem = it;											\
	item.iImage = i;											\
	item.iSelectedImage = si;									\
	TreeView_SetItem( tree, &item );							\
}

#define TreeView_GetItemImage( tree, it, i, si )				\
{																\
	TVITEM	item;												\
	ZeroMemory( &item, sizeof( item ) );						\
	item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;	\
	item.hItem = it;											\
	TreeView_GetItem( tree, &item );							\
	i = item.iImage;											\
	si = item.iSelectedImage;									\
}
/*
#define TreeView_GetItemState( tree, it, state )	\
{													\
	TVITEM	item;									\
	ZeroMemory( &item, sizeof( item ) );			\
	item.mask = TVIF_STATE | TVIF_HANDLE;			\
	item.hItem = it;								\
	TreeView_GetItem( tree, &item );				\
	state = item.state;								\
}
*/
#define TreeView_GetItemData( tree, it, data )	\
{												\
	TVITEM	item;								\
	ZeroMemory( &item, sizeof( item ) );		\
	item.mask = TVIF_PARAM | TVIF_HANDLE;		\
	item.hItem = it;							\
	TreeView_GetItem( tree, &item );			\
	data = item.lParam;							\
}

#define TreeView_GetItemText( tree, it, buf, size )	\
{													\
	TVITEM	item;									\
	ZeroMemory( &item, sizeof( item ) );			\
	item.mask = TVIF_TEXT | TVIF_HANDLE;			\
	item.hItem = it;								\
	item.pszText = buf;								\
	item.cchTextMax = size;							\
	TreeView_GetItem( tree, &item );				\
}


#define LVM_SETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 54)
#define ListCtrl_SetExtendedListViewStyle(hwndLV, dw)\
		(DWORD)SNDMSG((hwndLV), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dw)

#define LVM_GETEXTENDEDLISTVIEWSTYLE (LVM_FIRST + 55)
#define ListCtrl_GetExtendedListViewStyle(hwndLV)\
		(DWORD)SNDMSG((hwndLV), LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0)

#define MFC_ID2HWND( id ) ( GetDlgItem( id ) ? ( GetDlgItem( id )->GetSafeHwnd() ) : NULL )


//==================================================================
// CComCtrl
//
/// Generic Windows control helper functions
/**
	This class provides a set of static functions to supplement the
	functionality of the standard windows controls.	 It is not
	necessary to create an instance of this class as it only contains
	static functions.
*/
//==================================================================
class CComCtrl  
{
public:

	/// Default Constructor
	CComCtrl();

	/// Destructor
	virtual ~CComCtrl();

public:


	//==============================================================
	// PosWindow()
	//==============================================================
	/// Sets the position of the specified winodw
	/**
		\param [in] hWnd		-	Window handle
		\param [in] pRect		-	New position for window
		\param [in] bShow		-	Set to non-zero to make the window visible.
		\param [in] bRepaint	-	Set to non-zero to repaint the window.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL PosWindow( HWND hWnd, LPRECT pRect, BOOL bShow = TRUE, BOOL bRepaint = TRUE );

	//==============================================================
	// ListGetItemData()
	//==============================================================
	/// Gets the item data from a list control item
	/**
		\param [in] hWnd	-	Handle to list control
		\param [in] item	-	Item index
		
		\return Item data
	
		\see 
	*/
	static DWORD ListGetItemData( HWND hWnd, int item );

	//==============================================================
	// ListGetItemByData()
	//==============================================================
	/// Finds a list control item by matching its item data
	/**
		\param [in] hWnd	-	Handle to list control
		\param [in] data	-	item data value to find
		
		\return Index of list control or -1 if failure
	
		\see 
	*/
	static int ListGetItemByData( HWND hWnd, DWORD data );

	//==============================================================
	// OnScroll()
	//==============================================================
	/// Implements a standard response to an OnHScroll or OnVScroll message
	/**
		\param [in] hCtrl		-	Handle to control
		\param [in] nSBCode		-	Scroll code
		\param [in] bRedraw		-	Set to non zero to redraw
		\param [in] lLine		-	Size of a single scroll line
		
		\return New scroll position
	
		\see 
	*/
	static long OnScroll( HWND hCtrl, UINT nSBCode, BOOL bRedraw = TRUE, LONG lLine = 10 );

	//==============================================================
	// EnableCtrl()
	//==============================================================
	/// Enables specified control
	/**
		\param [in] hWnd		-	Handle of parent window
		\param [in] uCtrl		-	Control ID
		\param [in] bEnable		-	Non-zero to enable, zero to disable control
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL EnableCtrl( HWND hWnd, UINT uCtrl, BOOL bEnable );

	//==============================================================
	// ListSelectSingleItem()
	//==============================================================
	/// Selects a single item in a list control
	/**
		\param [in] hList	-	Handle to list control
		\param [in] item	-	Index of the list item to select
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ListSelectSingleItem( HWND hList, int item );

	//==============================================================
	// ListFindItemByText()
	//==============================================================
	/// Finds a list item by matching the specified text.
	/**
		\param [in] hList	-	Handle to list control
		\param [in] nSub	-	Sub item index of text to search
		\param [in] pText	-	Text to search for.
		
		\return Index of matching item or -1 if not found
	
		\see 
	*/
	static int ListFindItemByText( HWND hList, int nSub, LPCTSTR pText );

	//==============================================================
	// ListGetItemImage()
	//==============================================================
	/// Returns the items image index
	/**
		\param [in] hWnd	-	Handle to list control
		\param [in] item	-	Item index
		
		\return Index of the item image or -1 if failure
	
		\see 
	*/
	static int ListGetItemImage(HWND hWnd, int item);

	//==============================================================
	// ListSetItemImage()
	//==============================================================
	/// Sets the items image
	/**
		\param [in] hWnd	-	Handle to list control
		\param [in] item	-	Item index
		\param [in] iImage	-	New image index
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL ListSetItemImage(HWND hWnd, int item, int iImage);


	//==============================================================
	// TreeSelectByPath()
	//==============================================================
	/// Selects a tree item using path
	/**
		\param [in] hTree	-	Handle to tree control

		\param [in] pPath	-	Path to tree item
		
		pPath should be set to a standard file path such as
		<b>"my/path/to/tree/item"</b> or <b>"\\This\\Is really\\My Path"</b>

		\return Handle to tree item that was selected or NULL if failed.
	
		\see 
	*/
	static HTREEITEM TreeSelectByPath( HWND hTree, LPCTSTR pPath );

	//==============================================================
	// TreeFindItemByPath()
	//==============================================================
	/// Finds a tree item using path
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] pPath	-	Path to item
		\param [in] hRoot	-	Root item path is relative to
		\param [in] bDisk	-	Set to true if path contains a drive letter
		
		\return Handle to tree item or NULL
	
		\see 
	*/
	static HTREEITEM TreeFindItemByPath( HWND hTree, LPCTSTR pPath, HTREEITEM hRoot = NULL, BOOL bDisk = FALSE );

	//==============================================================
	// TreeFindItemByName()
	//==============================================================
	/// Searches tree items for item with specified name
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] pName	-	Item text to search for
		\param [in] hRoot	-	Root item
		
		\return Handle to tree item or NULL if not found.
	
		\see 
	*/
	static HTREEITEM TreeFindItemByName( HWND hTree, LPCTSTR pName, HTREEITEM hRoot = NULL );

	//==============================================================
	// TreeInsertPath()
	//==============================================================
	/// Inserts an item into a tree control using a path
	/**
		\param [in] hTree		-	Handle to tree control
		\param [in] hItem		-	Handle to parent item
		\param [in] pPath		-	Path for item
		\param [in] dwFolder	-	Folder icon offset
		\param [in] dwIcon		-	Item icon offset
		
		\return Handle to tree item or NULL if failure.
	
		\see 
	*/
	static HTREEITEM TreeInsertPath( HWND hTree, HTREEITEM hItem, LPCTSTR pPath, DWORD dwFolder, DWORD dwIcon );

	//==============================================================
	// TreeRefreshDirItem()
	//==============================================================
	/// Refreshes a tree that contains a list of disk folders
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Root item to refresh
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeRefreshDirItem( HWND hTree, HTREEITEM hItem );

	//==============================================================
	// TreeFillSystem()
	//==============================================================
	/// Fills in a tree with system file structure.
	/**
		\param [in] hTree	-	Handle of tree control
		\param [in] hRoot	-	Root item to add system file structure.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeFillSystem( HWND hTree, HTREEITEM hRoot = NULL );

	//==============================================================
	// TreeSetItemImage()
	//==============================================================
	/// 
	/**
		\param [in] hTree		-	Handle to tree control
		\param [in] hItem		-	Handle to item
		\param [in] dwImage		-	New image index
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeSetItemImage( HWND hTree, HTREEITEM hItem, DWORD dwImage );

	//==============================================================
	// TreeFillDirectory()
	//==============================================================
	/// Fills in a tree with the specified directory
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Handle to root item to add directory
		\param [in] pPath	-	Disk path to directory
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeFillDirectory( HWND hTree, HTREEITEM hItem, LPCTSTR pPath );

	//==============================================================
	// TreeValidateDirItem()
	//==============================================================
	/// Validates the folder items in a tree
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Root item to validate

		Adds new folders and removes deleted folders from tree.
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeValidateDirItem( HWND hTree, HTREEITEM hItem );

	//==============================================================
	// TreeGetItemText()
	//==============================================================
	/// 
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Handle to tree control item
		\param [out] pText	-	Buffer that receives the item text.
		\param [in] dwSize	-	Size of the buffer in pText
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeGetItemText( HWND hTree, HTREEITEM hItem, LPSTR pText, DWORD dwSize );

	//==============================================================
	// TreeGetItemPath()
	//==============================================================
	/// Gets the full path for the specified tree item
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Handle to tree item
		\param [in] pPath	-	Buffer that receives the path
		\param [in] dwDepth	-	Tracks the depth of the current item
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL TreeGetItemPath( HWND hTree, HTREEITEM hItem, LPSTR pPath, DWORD dwDepth = 0 );

	//==============================================================
	// TreeGetFileIcon()
	//==============================================================
	/// Returns the system icon for the specified file
	/**
		\param [in] pFile	-	File path
		\param [in] hWnd	-	Tree control
		\param [in] bSmall	-	Set to non-zero to retreive the small icon
		\param [in] bLarge	-	Set to non-zero to retreive the large icon
		
		\return Index into the image list of the added icon
	
		\see 
	*/
	static DWORD TreeGetFileIcon( LPCTSTR pFile, HWND hWnd = NULL, BOOL bSmall = TRUE, BOOL bLarge = FALSE );

	//==============================================================
	// TreeGetItemData()
	//==============================================================
	/// Retreives the item data for a specified tree item
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Handle to tree item
		
		\return The DWORD user data
	
		\see 
	*/
	static DWORD TreeGetItemData( HWND hTree, HTREEITEM hItem );

	//==============================================================
	// TreeInsertItem()
	//==============================================================
	/// Inserts an arbitrary item into a tree control
	/**
		\param [in] hTree			-	Handle to tree control
		\param [in] hParent			-	Parent tree item
		\param [in] pText			-	Item text
		\param [in] dwImage			-	Image list index
		\param [in] dwUser			-	User value to set
		\param [in] bExpand			-	Set to true if item should start expanded.
		\param [in] hInsertAfter	-	Tree item to insert after
		\param [in] uMask			-	Mask indicating valid arguments.
		
		\return Tree item handle or NULL if error.
	
		\see 
	*/
	static HTREEITEM TreeInsertItem( HWND hTree, HTREEITEM hParent, LPCTSTR pText, DWORD dwImage = 0, DWORD dwUser = 0, BOOL bExpand = FALSE, HTREEITEM hInsertAfter = TVI_LAST, UINT uMask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM );

	//==============================================================
	// TreeGetItemImage()
	//==============================================================
	/// Returns the image list index for the specified tree item
	/**
		\param [in] hTree	-	Handle to tree control
		\param [in] hItem	-	Handle to tree item
		
		\return Image list index of item image.
	
		\see 
	*/
	static int TreeGetItemImage( HWND hTree, HTREEITEM hItem );

	//==============================================================
	// ListSetColumns()
	//==============================================================
	/// Sets the columns for a specified list control
	/**
		\param [in] hList	-	Handle to list control
		\param [in] dwCols	-	Number of columns

		Use:
		\code
		ListSetColumns( hList, 3, "First", "Second", "Third" );
		\endcode
		
		\return Non-zero if success
	
		\see 
	*/
	static BOOL _cdecl ListSetColumns( HWND hList, DWORD dwCols, ... );

	//==============================================================
	// ListGetFullWidth()
	//==============================================================
	/// Width of list control without scroll bar
	/**
		\param [in] hList	-	Handle of a list control
		
		\return Width of the list control without scroll bar
	
		\see 
	*/
	static long ListGetFullWidth( HWND hList );

	//==============================================================
	// ListGetFileIcon()
	//==============================================================
	/// Gets the system icon associated with the specified file
	/**
		\param [in] pFile		-	Filename
		\param [in] hWnd		-	Handle to list control
		\param [in] bSmall		-	Set to non-zero to retreive small icon
		\param [in] bLarge		-	Set to non-zero to retreive large icon
		
		\return Image list index of specified file icon.
	
		\see 
	*/
	static DWORD ListGetFileIcon( LPCTSTR pFile, HWND hWnd = NULL, BOOL bSmall = TRUE, BOOL bLarge = TRUE );

	//==============================================================
	// ListGetFirstSelectedItem()
	//==============================================================
	/// Returns the first selected item in a list control
	/**
		\param [in] hListCtrl	-	Handle of list control
		
		\return Index of the first selected item in a list control
	
		\see 
	*/
	static int ListGetFirstSelectedItem(HWND hListCtrl);

	//==============================================================
	// ComboSelectItemByData()
	//==============================================================
	/// Selects a combo box item by its data
	/**
		\param [in] hWnd	-	Handle to a combo box
		\param [in] data	-	Value of data to search for
		
		\return Index of the item selected or -1 if error
	
		\see 
	*/
	static int ComboSelectItemByData( HWND hWnd, DWORD data );

	//==============================================================
	// ComboGetItemByData()
	//==============================================================
	/// Finds an item in a combo box by user data value
	/**
		\param [in] hWnd	-	Handle to a combo box
		\param [in] data	-	Value of data to search for
		
		\return Index of item with matching data or -1 if not found.
	
		\see 
	*/
	static int ComboGetItemByData( HWND hWnd, DWORD data );

};

#endif // !defined(AFX_COMCTRL_H__BA15C2F5_38AD_42E4_8BBD_274AF4B45989__INCLUDED_)
