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
#include <leaf/wrapper/daycent/out/Definitions.h>
#include <leaf/wrapper/daycent/out/File.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace out
{

///
class OutFiles : public boost::noncopyable
{
public:
    ///
    void Exit();

    ///
    void Init(
        std::string const& dirRoot,
        unsigned int const& fileNames = DEFAULT | DC_SIP | YEAR_SUMMARY );

    ///
    static OutFiles* instance(
        bool const& erase = false );

    ///
    void Read();

    ///
    void Set(
        unsigned int const& fileNames );

protected:
    ///
    OutFiles();

    ///
    ~OutFiles();

private:
    ///
    unsigned int m_fileNames;

    ///
    std::string m_header;

    ///
    boost::filesystem::path m_dirRoot;

    ///
    boost::filesystem::path m_outFilesInPath;

    ///
    Files m_files;

};

} //end out
} //end daycent
} //end wrapper
} //end leaf
