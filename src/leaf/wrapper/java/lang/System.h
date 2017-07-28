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


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/java/io/PrintStream.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

///STATIC INTERFACE
class LEAF_JAVA_EXPORTS System : public boost::noncopyable
{
public:
    ///
    static StringPtr GetProperty(
        String const& key );

    ///
    static void SetOut(
        io::PrintStream const& out );

    ///
    static void SetErr(
        io::PrintStream const& err );

    ///
    static StringPtr SetProperty(
        String const& key,
        String const& value );

    ///
    static jclass const& GetJclass();

protected:
    ///
    ~System();

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end lang
} //end java
} //end wrapper
} //end leaf
