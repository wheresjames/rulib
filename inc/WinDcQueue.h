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
// WinDcQueue.h: interface for the CWinDcQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDCQUEUE_H__43DF5FF9_E1BF_47A0_A713_5B0185C87F34__INCLUDED_)
#define AFX_WINDCQUEUE_H__43DF5FF9_E1BF_47A0_A713_5B0185C87F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CWinDcQueue
//
/// Provides a thread safe frame queue
/**
	This class provides a thread save frame queue using CWinDc 
	class objects to buffer individual frames.  This can be useful
	for passing generated frames from one thread to another thread
	for use.	
*/
//==================================================================
class CWinDcQueue  
{

public:

	//==================================================================
	// CFrame
	//
	/// Class encapsulates the data for a single frame
	//==================================================================
	class CFrame
	{
	public:

		/// Constructor
		CFrame() {}

		/// Destructor
		~CFrame() {}

		//==============================================================
		// CTlLock*()
		//==============================================================
		/// Returns a pointer to the thread lock for this frame
		operator CTlLock*() { return &m_lock; }

		//==============================================================
		// CWinDc*()
		//==============================================================
		/// Returns a pointer to the CWinDc object containing the image data
		operator CWinDc*() { return &m_dc; }
		
	private:

		/// Thread lock
		CTlLock			m_lock;

		/// Image data
		CWinDc			m_dc;
		
	public:

		/// Frame start time
		LONGLONG			m_llStart;

		/// Frame stop time
		LONGLONG			m_llStop;
	};


public:

	//==============================================================
	// GetNextQueuedFrame()
	//==============================================================
	/// Returns a pointe to the next queued frame
	CFrame* GetNextQueuedFrame();

	//==============================================================
	// AdvanceTailPtr()
	//==============================================================
	/// Adds a frame to the queue
	CWinDcQueue::CFrame* AdvanceTailPtr();

	//==============================================================
	// AdvanceHeadPtr()
	//==============================================================
	/// Removes a frame from the queue
	CWinDcQueue::CFrame* AdvanceHeadPtr();

	//==============================================================
	// GetOpenFrame()
	//==============================================================
	/// Returns a pointer to the next open frame
	CWinDcQueue::CFrame* GetOpenFrame();

	//==============================================================
	// GetOpenFrame()
	//==============================================================
	/// Waits for a frame to be added to the queue
	/**
		\param [in] dwTimeout	-	Maximum time in milli-seconds to
									wait for a new frame.
		
		\return Non-zero if a frame appears in the queue before the
				timeout expires.
	
		\see 
	*/
	BOOL WaitFrame( DWORD dwTimeout = 30 );

	//==============================================================
	// Destroy()
	//==============================================================
	/// Destroys the frame queue
	void Destroy();

	//==============================================================
	// Create()
	//==============================================================
	/// Creates the specified frame queue and initializes it for use
	/**
		\param [in] dwSize		-	Number of frames in the queue
		\param [in] hDC			-	Handle to the Device Context
		\param [in] lWidth		-	Width of a single image
		\param [in] lHeight		-	Height of a single image
		\param [in] bDibSection	-	Non-zero if each frame should be
									a DIB section.  Set to non-zero if
									you need access to the raw image 
									buffer.
		
		\return Non-zero if success
	
		\see 
	*/
	BOOL Create( DWORD dwSize, HDC hDC, long lWidth, long lHeight, BOOL bDibSection = FALSE );

	/// Constructor
	CWinDcQueue();

	/// Destructor
	virtual ~CWinDcQueue();

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the total number of image frames
	DWORD Size() { return m_frames.Size(); }

	//==============================================================
	// GetFramesQueued()
	//==============================================================
	/// Returns the number of frames in the queue waiting processing
	DWORD GetFramesQueued() { return m_dwFrames; }

	//==============================================================
	// GetWidth()
	//==============================================================
	/// Returns the width of a single video frame
	long GetWidth() { return m_lWidth; }

	//==============================================================
	// GetHeight()
	//==============================================================
	/// Returns the height of a single video frame
	long GetHeight() { return m_lHeight; }

	//==============================================================
	// SetAvgFrameDelay()
	//==============================================================
	/// Sets the average delay between frame additions
	void SetAvgFrameDelay( LONGLONG llDelay ) { m_llAvgFrameDelay = llDelay; }

	//==============================================================
	// GetAvgFrameDelay()
	//==============================================================
	/// Returns the previously set average delay between frame additions
	LONGLONG GetAvgFrameDelay() { return m_llAvgFrameDelay; }

	//==============================================================
	// CTlLock*()
	//==============================================================
	/// Returns a pointer to the frame queue thread lock
	operator CTlLock*() { return &m_lock; }

private:

	/// Thread lock
	CTlLock						m_lock;

	/// List of image frames
	TLList< CFrame >			m_frames;

	/// Width of a single image
	long						m_lWidth;

	/// Height of a single image
	long						m_lHeight;

	/// Number of frames in the queue
	DWORD						m_dwFrames;

	/// Average delay between frame additions.  This value is set not calculated.
	LONGLONG					m_llAvgFrameDelay;

	/// The first frame in the queue
	TLList< CFrame >::iterator	m_head;

	/// The frame after the last frame in the queue
	TLList< CFrame >::iterator	m_tail;

	/// Event is set when a new frame is added to the queue
	HANDLE						m_hFrameReady;

};

#endif // !defined(AFX_WINDCQUEUE_H__43DF5FF9_E1BF_47A0_A713_5B0185C87F34__INCLUDED_)
