#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus)
# Copyright (c) 2021 Liviu Ionescu
#
# This Source Code Form is subject to the terms of the MIT License.
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.18/
# https://cmake.org/cmake/help/v3.18/manual/cmake-packages.7.html#package-configuration-file

if(micro-os-plus-posix-io-included)
  return()
endif()

set(micro-os-plus-posix-io-included TRUE)

message(STATUS "Processing xPack ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}...")

# -----------------------------------------------------------------------------
# Dependencies.

find_package(micro-os-plus-utils-lists REQUIRED)
find_package(micro-os-plus-diag-trace REQUIRED)

# -----------------------------------------------------------------------------
# The current folder.

get_filename_component(xpack_current_folder ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# -----------------------------------------------------------------------------

if(NOT TARGET micro-os-plus-posix-io-static)

  add_library(micro-os-plus-posix-io-static STATIC EXCLUDE_FROM_ALL)

  # ---------------------------------------------------------------------------

  xpack_glob_recurse_cxx(source_files "${xpack_current_folder}/src")
  xpack_display_relative_paths("${source_files}" "${xpack_current_folder}")

  target_sources(
    micro-os-plus-posix-io-static

    PRIVATE
      ${source_files}
  )

  target_include_directories(
    micro-os-plus-posix-io-static

    PUBLIC
      ${xpack_current_folder}/include
  )

  target_link_libraries(
    micro-os-plus-posix-io-static
    
    PUBLIC
      micro-os-plus::utils-lists
      micro-os-plus::diag-trace
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::posix-io ALIAS micro-os-plus-posix-io-static)
  message(STATUS "=> micro-os-plus::posix-io")

endif()

# -----------------------------------------------------------------------------
