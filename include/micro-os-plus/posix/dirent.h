/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#ifndef POSIX_DIRENT_H_
#define POSIX_DIRENT_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wpedantic"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif

#include_next <dirent.h>

#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

#if !defined(MICRO_OS_PLUS_INTEGER_DIRENT_NAME_MAX)
#define MICRO_OS_PLUS_INTEGER_DIRENT_NAME_MAX (256)
#endif

  // --------------------------------------------------------------------------

  // http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/dirent.h.html
  struct dirent
  {
    // File serial number.
    ino_t d_ino;
    // Filename string of entry.
    char d_name[MICRO_OS_PLUS_INTEGER_DIRENT_NAME_MAX];
  };

  // The content of this structure is not relevant, it is here just to keep
  // POSIX compatibility, in real life the directory class is used
  // and casted to DIR.
  typedef struct
  {
    // Empty.
  } DIR;

  // --------------------------------------------------------------------------

  DIR*
  opendir (const char* dirname);

  struct dirent*
  readdir (DIR* dirp);

#if 0
  int
  readdir_r (DIR* dirp, struct dirent* entry, struct dirent** result);
#endif

  void
  rewinddir (DIR* dirp);

  int
  closedir (DIR* dirp);

  // --------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // defined(_POSIX_VERSION)

// ----------------------------------------------------------------------------

#endif // POSIX_DIRENT_H_

// ----------------------------------------------------------------------------
