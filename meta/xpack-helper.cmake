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

function(target_sources_micro_os_plus_posix_io target)

  get_filename_component(PARENT_DIR ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_sources(
    ${target}

    PRIVATE
    ${PARENT_DIR}/src/newlib/c-newlib-reent.cpp
    ${PARENT_DIR}/src/block-device.cpp
    ${PARENT_DIR}/src/block-device-partition.cpp
    ${PARENT_DIR}/src/char-device.cpp
    ${PARENT_DIR}/src/c-syscalls-posix.cpp
    ${PARENT_DIR}/src/device.cpp
    ${PARENT_DIR}/src/directory.cpp
    ${PARENT_DIR}/src/file.cpp
    ${PARENT_DIR}/src/file-descriptors-manager.cpp
    ${PARENT_DIR}/src/file-system.cpp
    ${PARENT_DIR}/src/io.cpp
    ${PARENT_DIR}/src/net-stack.cpp
    ${PARENT_DIR}/src/socket.cpp
    ${PARENT_DIR}/src/tty.cpp
  )
endfunction()

function(target_include_directories_micro_os_plus_posix_io target)

  get_filename_component(PARENT_DIR ${CMAKE_CURRENT_FUNCTION_LIST_DIR} DIRECTORY)

  target_include_directories(
    ${target}

    PUBLIC
      ${PARENT_DIR}/include
  )
endfunction()

# -----------------------------------------------------------------------------
