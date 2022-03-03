/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#include <micro-os-plus/posix-io/types.h>

// ----------------------------------------------------------------------------

extern "C"
{

  // Many newlib functions call the reentrant versions right away,
  // and these call the _name() implementations. To avoid this, the
  // shortcut is to simply skip the reentrant code (ignore the pointer)
  // and directly call the posix implementation.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wunused-parameter"

  int __attribute__ ((weak)) _close_r (void* ptr, int fildes)
  {
    return __posix_close (fildes);
  }

  int __attribute__ ((weak)) _execve_r (void* ptr, const char* path,
                                        char* const argv[], char* const envp[])
  {
    return __posix_execve (path, argv, envp);
  }

  int __attribute__ ((weak)) _fcntl_r (void* ptr, int fildes, int cmd, int arg)
  {
    return __posix_fcntl (fildes, cmd, arg);
  }

  pid_t __attribute__ ((weak)) _fork_r (void* ptr)
  {
    return __posix_fork ();
  }

  int __attribute__ ((weak)) _fstat_r (void* ptr, int fildes, stat* buf)
  {
    return __posix_fstat (fildes, buf);
  }

  pid_t __attribute__ ((weak)) _getpid_r (void* ptr)
  {
    return __posix_getpid ();
  }

  int __attribute__ ((weak))
  _gettimeofday_r (void* ptr, timeval* ptimeval, void* ptimezone)
  {
    return __posix_gettimeofday (ptimeval, ptimezone);
  }

  int __attribute__ ((weak)) _isatty_r (void* ptr, int fildes)
  {
    return __posix_isatty (fildes);
  }

  int __attribute__ ((weak)) _kill_r (void* ptr, pid_t pid, int sig)
  {
    return __posix_kill (pid, sig);
  }

  int __attribute__ ((weak))
  _link_r (void* ptr, const char* existing, const char* _new)
  {
    return __posix_link (existing, _new);
  }

  off_t __attribute__ ((weak))
  _lseek_r (void* ptr, int fildes, off_t offset, int whence)
  {
    return __posix_lseek (fildes, offset, whence);
  }

  int __attribute__ ((weak))
  _mkdir_r (void* ptr, const char* path, mode_t mode)
  {
    return __posix_mkdir (path, mode);
  }

  int __attribute__ ((weak))
  _open_r (void* ptr, const char* path, int oflag, int mode)
  {
    return __posix_open (path, oflag, mode);
  }

  ssize_t __attribute__ ((weak))
  _read_r (void* ptr, int fildes, void* buf, size_t nbyte)
  {
    return __posix_read (fildes, buf, nbyte);
  }

  int __attribute__ ((weak))
  _rename_r (void* ptr, const char* oldfn, const char* newfn)
  {
    return __posix_rename (oldfn, newfn);
  }

  int __attribute__ ((weak)) _stat_r (void* ptr, const char* path, stat* buf)
  {
    return __posix_stat (path, buf);
  }

  clock_t __attribute__ ((weak)) _times_r (void* ptr, tms* buf)
  {
    return __posix_times (buf);
  }

  int __attribute__ ((weak)) _unlink_r (void* ptr, const char* name)
  {
    return __posix_unlink (name);
  }

  pid_t __attribute__ ((weak)) _wait_r (void* ptr, int* stat_loc)
  {
    return __posix_wait (stat_loc);
  }

  ssize_t __attribute__ ((weak))
  _write_r (void* ptr, int fildes, const void* buf, size_t nbyte)
  {
    return __posix_write (fildes, buf, nbyte);
  }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic pop
}

// ----------------------------------------------------------------------------

#endif // defined(__ARM_EABI__)

// ----------------------------------------------------------------------------
