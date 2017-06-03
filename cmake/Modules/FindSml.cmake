# Copyright Louis Dionne 2013-2017
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
#
# This CMake module finds the Hana library. It sets the following CMake
# variables:
#
# Hana_FOUND
#   Set to 1 when Hana is found, set to 0 otherwise.
#
# Hana_INCLUDE_DIRS
#   If Hana is found, this is a list containing Hana's include/ directory,
#   along with that of any dependency of Hana. Since Hana has no external
#   dependencies as a library, this will always contain only Hana's include/
#   directory. This is provided for consistency with normal CMake Find-modules.
#   If Hana is not found, this is not set.
#
#
# The following variables can be used to customize the behavior of the module:
#
# Hana_ROOT
#   The path to Hana's installation prefix. If this is specified, this prefix
#   will be searched first while looking for Hana. If Hana is not found in
#   this prefix, the usual system directories are searched.
#
# Hana_INSTALL_PREFIX
#   If Hana can't be found and this is set to something, Hana will be downloaded
#   and installed to that directory. This can be very useful for installing
#   Hana as a local dependency of the current project. A suggestion is to set
#   `Hana_INSTALL_PREFIX` to `project_root/ext/hana`, and to Git-ignore the
#   whole `ext/` subdirectory, to avoid tracking external dependencies in
#   source control. Note that when `Hana_INSTALL_PREFIX` is specified, the
#   version of Hana must be provided explicitly to `find_package`.
#
# Hana_ENABLE_TESTS
#   If Hana is installed as an external project and this is set to true, the
#   tests will be run whenever Hana is updated. By default, this is set to
#   false because the tests tend to be quite long.


##############################################################################
# Sanitize input variables
##############################################################################
if (DEFINED Sml_INSTALL_PREFIX AND NOT DEFINED Sml_FIND_VERSION)
    message(FATAL_ERROR
        "The version of Sml must be specified when cloning locally. "
        "Otherwise, we wouldn't know which version to clone.")
endif()


##############################################################################
# First, try to find the library locally (using Sml_ROOT as a hint, if provided)
##############################################################################
find_path(Sml_INCLUDE_DIR
    NAMES boost/sml.hpp
    HINTS "${Sml_ROOT}/include"
    DOC "Include directory for the Sml library"
)

# Extract Sml's version from <boost/sml/version.hpp>, and set
# Sml_VERSION_XXX variables accordingly.
if (EXISTS "${Sml_INCLUDE_DIR}")
    foreach(level MAJOR MINOR PATCH)
        file(STRINGS
            ${Sml_INCLUDE_DIR}/boost/sml/version.hpp
            Sml_VERSION_${level}
            REGEX "#define BOOST_SML_${level}_VERSION"
        )
        string(REGEX MATCH "([0-9]+)" Sml_VERSION_${level} "${Sml_VERSION_${level}}")
    endforeach()

    set(Sml_VERSION_STRING "${Sml_VERSION_MAJOR}.${Sml_VERSION_MINOR}.${Sml_VERSION_PATCH}")
endif()

# Temporarily override the quietness level to avoid producing a "not-found"
# message when we're going to install anyway.
if (Sml_INSTALL_PREFIX)
    set(_sml_quiet_level ${Sml_FIND_QUIETLY})
    set(Sml_FIND_QUIETLY 1)
endif()
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sml
    REQUIRED_VARS Sml_INCLUDE_DIR
    VERSION_VAR Sml_VERSION_STRING
)
if (Sml_INSTALL_PREFIX)
    set(Sml_FIND_QUIETLY ${_sml_quiet_level})
endif()

if (Sml_FOUND)
    set(Sml_INCLUDE_DIRS "${Sml_INCLUDE_DIR}")
    return()
endif()


##############################################################################
# Otherwise, if Sml_INSTALL_PREFIX was specified, try to install the library.
##############################################################################
if (NOT Sml_FOUND AND DEFINED Sml_INSTALL_PREFIX)
    if (DEFINED Sml_ENABLE_TESTS)
        set(_sml_test_cmd ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/sml --target check)
    endif()

    include(ExternalProject)
    ExternalProject_Add(Sml EXCLUDE_FROM_ALL 1
        SOURCE_DIR      ${Sml_INSTALL_PREFIX}
        PREFIX          ${CMAKE_CURRENT_BINARY_DIR}/sml
        BINARY_DIR      ${CMAKE_CURRENT_BINARY_DIR}/sml
        DOWNLOAD_DIR    ${CMAKE_CURRENT_BINARY_DIR}/sml/_download
        STAMP_DIR       ${CMAKE_CURRENT_BINARY_DIR}/sml/_stamps
        TMP_DIR         ${CMAKE_CURRENT_BINARY_DIR}/sml/_tmp
        # Download step
        DOWNLOAD_NAME "sml-v${Sml_FIND_VERSION}.tar.gz"
        URL "https://github.com/boost-experimental/sml/tarball/v${Sml_FIND_VERSION}"
        TIMEOUT 20

        # Configure step
        # CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}

        # No build step because Sml is header-only
        BUILD_COMMAND ""

        # No install step
        INSTALL_COMMAND ""

        # Test step
        TEST_COMMAND ${_sml_test_cmd}
    )

    if ("${Sml_FIND_VERSION}" MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)")
        set(Sml_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(Sml_VERSION_MINOR ${CMAKE_MATCH_2})
        set(Sml_VERSION_PATCH ${CMAKE_MATCH_3})
        set(Sml_VERSION_STRING "${Sml_VERSION_MAJOR}.${Sml_VERSION_MINOR}.${Sml_VERSION_PATCH}")
    endif()

    set(Sml_INCLUDE_DIR "${Sml_INSTALL_PREFIX}/include")
    find_package_handle_standard_args(Sml
        REQUIRED_VARS Sml_INCLUDE_DIR
        VERSION_VAR Sml_VERSION_STRING
    )

    if (Sml_FOUND)
        set(Sml_INCLUDE_DIRS "${Sml_INCLUDE_DIR}")
    endif()
endif()
