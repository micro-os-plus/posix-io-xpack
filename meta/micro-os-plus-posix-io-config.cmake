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

message(STATUS "Including micro-os-plus-posix-io...")

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

  target_sources(
    micro-os-plus-posix-io-static

    PRIVATE
      ${xpack_current_folder}/src/newlib/c-newlib-reent.cpp
      ${xpack_current_folder}/src/block-device.cpp
      ${xpack_current_folder}/src/block-device-partition.cpp
      ${xpack_current_folder}/src/char-device.cpp
      ${xpack_current_folder}/src/c-syscalls-posix.cpp
      ${xpack_current_folder}/src/device.cpp
      ${xpack_current_folder}/src/directory.cpp
      ${xpack_current_folder}/src/file.cpp
      ${xpack_current_folder}/src/file-descriptors-manager.cpp
      ${xpack_current_folder}/src/file-system.cpp
      ${xpack_current_folder}/src/io.cpp
      ${xpack_current_folder}/src/net-stack.cpp
      ${xpack_current_folder}/src/socket.cpp
      ${xpack_current_folder}/src/tty.cpp
  )

  target_include_directories(
    micro-os-plus-posix-io-static

    PUBLIC
      ${xpack_current_folder}/include
  )

  target_link_libraries(
    micro-os-plus-posix-io-static
    
    PUBLIC
      micro-os-plus::utils-lists-static
      micro-os-plus::diag-trace-static
  )

  # ---------------------------------------------------------------------------
  # Aliases.

  add_library(micro-os-plus::posix-io-static ALIAS micro-os-plus-posix-io-static)
  message(STATUS "micro-os-plus::posix-io-static")

endif()

# -----------------------------------------------------------------------------
