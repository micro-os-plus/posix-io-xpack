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

#include <micro-os-plus/posix-io/file.h>
#include <micro-os-plus/posix-io/file-system.h>

#include <micro-os-plus/diag/trace.h>

#include <cerrno>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    file::file (file_impl& impl) : io{ impl, type::file }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s()=%p\n", __func__, this);
#endif
    }

    file::~file ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    int
    file::close (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      int ret = io::close ();

      // Note: the constructor is not called here.

      // Link the file object to a list kept by the file system.
      // It will be deallocated at the next open.
      file_system ().add_deferred_file (this);

      return ret;
    }

    int
    file::ftruncate (off_t length)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%u) @%p\n", __func__, length, this);
#endif

      if (length < 0)
        {
          errno = EINVAL;
          return -1;
        }

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_ftruncate (length);
    }

    int
    file::fsync (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s() @%p\n", __func__, this);
#endif

      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_fsync ();
    }

    int
    file::fstatvfs (struct statvfs* buf)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file::%s(%p) @%p\n", __func__, buf, this);
#endif

      errno = 0;

      // Execute the file system code. Might be locked there.
      return file_system ().statvfs (buf);
    }

    // ========================================================================

    file_impl::file_impl (class file_system& fs) : file_system_ (fs)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file_impl::%s()=%p\n", __func__, this);
#endif
    }

    file_impl::~file_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE)
      trace::printf ("file_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    file_impl::do_ftruncate (off_t length)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

    int
    file_impl::do_fsync (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
