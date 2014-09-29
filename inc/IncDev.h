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
// IncDev.h: interface for the TIncDev template.
//
//////////////////////////////////////////////////////////////////////

/// Incremental Division
template<class T> class TIncDev
{

public:

	TIncDev()
	{	m_num = 0; m_scale = 1; m_target = 1; m_overflow = 0; }

	TIncDev( T n )
	{	m_num = n; m_scale = 1; m_target = 1; m_overflow = 0;}

	TIncDev( T n, T scale, T target )
	{	m_num = n; m_scale = scale; m_target = target; m_overflow = 0;}

	void SetScale( T scale, T target )
	{	m_scale = scale; m_target = target; }
	
	void Reset() { m_overflow = 0; }
	
	T operator =( T n ) { m_num = n; m_overflow = 0; }
	TIncDev& operator =( TIncDev &o ) 
	{	m_num = o.m_num; m_scale = o.m_scale; 
		m_target = o.m_target; m_overflow = o.m_overflow; 
	}

	T operator ++( int ) { T temp = m_num; ++(*this); return temp; }
	T operator ++() 
	{	m_overflow += m_target;
		while ( m_overflow > m_scale )
			m_overflow -= m_scale, m_num++;
		return m_num;
	}

	int operator += ( int s )
	{	while ( s-- ) ++(*this); return s; }

	operator T() { return m_num; }

private:

	T		m_num;

	T		m_scale;
	T		m_target;
	T		m_overflow;

};
