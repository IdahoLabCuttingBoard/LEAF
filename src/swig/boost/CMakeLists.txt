
include_directories( ${Boost_INCLUDE_DIRS} )

if( CSHARP_FOUND )
    set( TARGET_NAME boost_csharp )
    set( TARGET_VERSION ${LEAF_VERSION} )
    set( TARGET_EXPORT ${CMAKE_PROJECT_NAME}Targets )
    set( TARGET_CATEGORY Swig )
    set( SWIG_LANGUAGE CSHARP )
    set( PRIVATE_HEADERS "" )
    set( PUBLIC_HEADERS "" )
    set( PRIVATE_SOURCES "" )

    set( INTERFACE_FILE module.i )
    set( CMAKE_SWIG_FLAGS -namespace boost -dllimport ${TARGET_NAME} )
    set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp )
    set_source_files_properties( ${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )
    set( SWIG_MODULE_${TARGET_NAME}_EXTRA_DEPS
        std_csharp
        ${SWIG_DIR}/std/module.i )
    swig_add_module( ${TARGET_NAME} csharp ${INTERFACE_FILE} )
    swig_link_libraries( ${TARGET_NAME}
        ${Boost_LIBRARIES} )

    include( ModuleInstall )
endif()
