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


///This code is ripped directly from:
///vrkit = https://realityforge.vrsource.org/trac/vrkit

#if defined( WIN32 ) || defined( WIN64 )
#include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#if !defined( WIN32 ) && !defined( WIN64 )
#include <dlfcn.h>
#endif

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/version.hpp>

namespace fs = boost::filesystem;

#if defined( WIN32 ) || defined( WIN64 )

///Windows DLL entry point function. This ensures that the environment
///variable \c LEAF_BASE_DIR is set as soon as this DLL is attached to the
///process. If it is not set, then it sets it based on an assumption about the
///structure of a leaf installation. More specifically, an assumption is made
///that this DLL lives in the \c lib subdirectory of the leaf installation.
///Therefore, the root of the leaf installation is the parent of the
///directory containing this DLL.
BOOL __stdcall DllMain( HINSTANCE module, DWORD reason, LPVOID reserved )
{
    switch( reason )
    {
    case DLL_PROCESS_ATTACH:
    {
        char* env_dir( NULL );
#if defined(_MSC_VER) && _MSC_VER >= 1400
        size_t len;
        _dupenv_s( &env_dir, &len, "LEAF_BASE_DIR" );
#else
        env_dir = std::getenv( "LEAF_BASE_DIR" );
#endif

        try
        {
            fs::path base_dir;

            // If LEAF_BASE_DIR is not set, look up the path to this DLL
            // and use it to provide a default setting for that environment
            // variable.
            if( NULL == env_dir )
            {
                char tmppath[1024];
                std::memset( tmppath, 0, sizeof( tmppath ) );
                GetModuleFileName( module, tmppath, sizeof( tmppath ) );

                const fs::path dll_path( tmppath, fs::native );
                //Check for the standard install directory structure first and then
                //test for a deployed env.
                base_dir = dll_path.parent_path().parent_path();
                if(  !fs::exists( base_dir / "share" / "leaf" ) )
                {
                    base_dir = dll_path.parent_path();
                }
                
#if defined(LEAF_DEBUG) && ! defined(_DEBUG)
                // The debug DLL linked against the release runtime is in
                // <base_dir>\lib\debug.
                base_dir = base_dir.branch_path();
#endif

#if (BOOST_VERSION >= 104600) && (BOOST_FILESYSTEM_VERSION == 3)
                const std::string base_dir_str =
                    base_dir.string();
#else
                const std::string base_dir_str =
                    base_dir.native_directory_string();
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
                _putenv_s( "LEAF_BASE_DIR", base_dir_str.c_str() );
                std::cout << "LEAF_BASE_DIR="
                          << base_dir_str << std::endl;
#else
                std::ostringstream env_stream;
                env_stream << "LEAF_BASE_DIR=" << base_dir_str;
                putenv( env_stream.str().c_str() );
                std::cout << "LEAF_BASE_DIR="
                          << env_stream.str() << std::endl;
#endif
            }
            else
            {
                base_dir = fs::path( env_dir, fs::native );
#if defined(_MSC_VER) && _MSC_VER >= 1400
                std::free( env_dir );
                env_dir = NULL;
#endif
            }

#if defined(_MSC_VER) && _MSC_VER >= 1400
            _dupenv_s( &env_dir, &len, "LEAF_DATA_DIR" );
#else
            env_dir = std::getenv( "LEAF_DATA_DIR" );
#endif

            // If LEAF_DATA_DIR is not set, set a default relative to
            // base_dir.
            if( NULL == env_dir )
            {
                fs::path data_dir( base_dir / "share" / "leaf" );
#if (BOOST_VERSION >= 104600) && (BOOST_FILESYSTEM_VERSION == 3)
                const std::string data_dir_str =
                    data_dir.string();
#else
                const std::string data_dir_str =
                    data_dir.native_directory_string();
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
                _putenv_s( "LEAF_DATA_DIR", data_dir_str.c_str() );
                std::cout << "LEAF_DATA_DIR="
                          << data_dir_str << std::endl;
#else
                std::ostringstream env_stream;
                env_stream << "LEAF_DATA_DIR=" << data_dir_str;
                putenv( env_stream.str().c_str() );
                std::cout << "LEAF_DATA_DIR="
                          << env_stream.str() << std::endl;
#endif
            }
#if defined(_MSC_VER) && _MSC_VER >= 1400
            else
            {
                std::free( env_dir );
                env_dir = NULL;
            }
#endif
        }
        catch( fs::filesystem_error& ex )
        {
            std::cerr << "Automatic assignment of VP environment "
                      << "variables failed:\n" << ex.what() << std::endl;

#if defined(_MSC_VER) && _MSC_VER >= 1400
            if( NULL != env_dir )
            {
                std::free( env_dir );
            }
#endif
        }
    }
    break;
    default:
        break;
    }

    return TRUE;
}
#else

///Non-Windows shared library constructor. This ensures that the environment
///variable \c LEAF_BASE_DIR is set as soon as this shared library is loaded.
///If it is not set, then it sets it based on an assumption about the
///structure of a leaf installation. More specifically, an assumption is made
///that this shared library lives in the \c lib subdirectory of the leaf
///installation. Therefore, the root of the leaf installation is the parent
///of the directory containing this shared library.
extern "C" void __attribute( ( constructor ) ) leafLibraryInit()
{
    fs::path base_dir;
    const char* env_dir = std::getenv( "LEAF_BASE_DIR" );

    //#if (defined(__linux__) || defined(__linux) || defined(__APPLE__)) && defined(__x86_64__)
    //    const std::string bit_suffix("64");
    //#else
    const std::string bit_suffix( "" );
    //#endif

    const fs::path lib_subdir( std::string( "lib" ) + bit_suffix );

    // If LEAF_BASE_DIR is not set, look up the path to this shared library
    // and use it to provide a default setting for that environment variable.
    if( NULL == env_dir )
    {
        Dl_info info;
        info.dli_fname = 0;
        const int result =
#if defined(__GNUC__) && __GNUC_MAJOR__ < 4
            dladdr( ( void* ) &leafLibraryInit, &info );
#else
            dladdr( reinterpret_cast<void*>( &leafLibraryInit ), &info );
#endif

        // NOTE: dladdr(3) really does return a non-zero value on success.
        if( 0 != result )
        {
            try
            {
                fs::path lib_file( info.dli_fname );
                lib_file = fs::system_complete( lib_file );

                // Get the directory containing this shared library.
                const fs::path lib_path = lib_file.branch_path();

                // Start the search for the root of the leaf installation in the
                // parent of the directory containing this shared library.
                base_dir = lib_path.branch_path();

                bool found( false );
                while( ! found && ! base_dir.empty() )
                {
                    try
                    {
                        // Use the lib(64) subdirectory to figure out when we have
                        // found the root of the leaf installation tree.
                        if( ! fs::exists( base_dir / lib_subdir ) )
                        {
                            base_dir = base_dir.branch_path();
                        }
                        else
                        {
                            found = true;
                        }
                    }
                    catch( fs::filesystem_error& )
                    {
                        base_dir = base_dir.branch_path();
                    }
                }

                if( found )
                {
                    setenv( "LEAF_BASE_DIR",
                            base_dir.string().c_str(), 1 );
                    std::cout << "LEAF_BASE_DIR="
                              << base_dir.string() << std::endl;
                }
            }
            catch( fs::filesystem_error& ex )
            {
                std::cerr << "Automatic assignment of LEAF_BASE_DIR failed:\n"
                          << ex.what() << std::endl;
            }
        }
    }
    else
    {
        try
        {
            base_dir = fs::path( env_dir );
        }
        catch( fs::filesystem_error& ex )
        {
            std::cerr << "Invalid path set in LEAF_BASE_DIR environment "
                      << "variable:\n" << ex.what() << std::endl;
        }
    }

    // If base_dir were empty, this would result in data_dir and plugin_dir
    // being relative to the current working directory.
    if( ! base_dir.empty() )
    {
        // Construct the values for LEAF_DATA_DIR.

        const fs::path data_dir =
            base_dir / "share" / "leaf";

        // We use the overwrite value of 0 as a way around testing whether the
        // environment variable is already set.
#if (BOOST_VERSION >= 104600) && (BOOST_FILESYSTEM_VERSION == 3)
        setenv( "LEAF_DATA_DIR",
                data_dir.string().c_str(), 0 );
        std::cout << "LEAF_DATA_DIR="
                  << data_dir.string() << std::endl;
#else
        setenv( "LEAF_DATA_DIR",
                data_dir.native_directory_string().c_str(), 0 );
        std::cout << "LEAF_DATA_DIR="
                  << data_dir.native_directory_string() << std::endl;
#endif
    }
}

#endif //defined( WIN32 ) || defined( WIN64 )
