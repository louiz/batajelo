# - Find catch
# Find the catch cryptographic library
#
# This module defines the following variables:
#   CATCH_FOUND  -  True if library and include directory are found
# If set to TRUE, the following are also defined:
#   CATCH_INCLUDE_DIRS  -  The directory where to find the header file
#   CATCH_LIBRARIES  -  Where to find the library file
#
# For conveniance, these variables are also set. They have the same values
# than the variables above.  The user can thus choose his/her prefered way
# to write them.
#   CATCH_LIBRARY
#   CATCH_INCLUDE_DIR
#
# This file is in the public domain

find_path(CATCH_INCLUDE_DIRS NAMES catch.hpp
  PATH_SUFFIXES catch-1.11
  DOC "The catch include directory")

# Use some standard module to handle the QUIETLY and REQUIRED arguments, and
# set CATCH_FOUND to TRUE if these two variables are set.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CATCH REQUIRED_VARS CATCH_INCLUDE_DIRS)

if(CATCH_FOUND)
  set(CATCH_INCLUDE_DIR ${CATCH_INCLUDE_DIRS})
endif()

mark_as_advanced(CATCH_INCLUDE_DIRS)
