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

message(STATUS "Including micro-os-plus-posix-io...")

# -----------------------------------------------------------------------------

function(target_sources_micro_os_plus_posix_io target)

  get_filename_component(xpack_root_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_sources(
    ${target}

    PRIVATE
      ${xpack_root_folder}/src/newlib/c-newlib-reent.cpp
      ${xpack_root_folder}/src/block-device.cpp
      ${xpack_root_folder}/src/block-device-partition.cpp
      ${xpack_root_folder}/src/char-device.cpp
      ${xpack_root_folder}/src/c-syscalls-posix.cpp
      ${xpack_root_folder}/src/device.cpp
      ${xpack_root_folder}/src/directory.cpp
      ${xpack_root_folder}/src/file.cpp
      ${xpack_root_folder}/src/file-descriptors-manager.cpp
      ${xpack_root_folder}/src/file-system.cpp
      ${xpack_root_folder}/src/io.cpp
      ${xpack_root_folder}/src/net-stack.cpp
      ${xpack_root_folder}/src/socket.cpp
      ${xpack_root_folder}/src/tty.cpp
  )

endfunction()

# -----------------------------------------------------------------------------

function(target_include_directories_micro_os_plus_posix_io target)

  get_filename_component(xpack_root_folder ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_include_directories(
    ${target}

    PUBLIC
      ${xpack_root_folder}/include
  )

endfunction()

# -----------------------------------------------------------------------------
