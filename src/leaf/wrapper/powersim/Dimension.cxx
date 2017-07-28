/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


// --- LEAF Includes --- //
#include "Dimension.h"

namespace leaf
{
namespace wrapper
{
namespace powersim
{

////////////////////////////////////////////////////////////////////////////////
Dimension::Dimension(
    ATL::CComPtr< ::IDimension > const& iD )
    :
    m_dimension( iD ),
    m_elemCnt( 0 )
{
    if( !m_dimension ) return;

    long numElmnts;
    HRESULT hr = m_dimension->get_ElementCount( &numElmnts );
    m_elemCnt = ( numElmnts < 0 ) ? 0 : numElmnts;
}
////////////////////////////////////////////////////////////////////////////////
Dimension::~Dimension()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
unsigned int const& Dimension::GetElementCount() const
{
    return m_elemCnt;
}
////////////////////////////////////////////////////////////////////////////////

} //end powersim
} //end wrapper
} //end leaf
