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

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#include <micro-os-plus/posix-io/directory.h>
#include <micro-os-plus/posix-io/file-system.h>

#include <micro-os-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <string.h>

// ----------------------------------------------------------------------------

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    directory::directory (directory_impl& impl) : impl_ (impl)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s()=%p\n", __func__, this);
#endif
    }

    directory::~directory ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    dirent*
    directory::read (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);

      // POSIX requires not to change errno when end of directory is
      // encountered. However, in this implementation, errno is
      // always cleared when entering system calls.
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_read ();
    }

    void
    directory::rewind (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);

      // POSIX does not mention what to do with errno.
      errno = 0;

      // Execute the implementation specific code.
      impl ().do_rewind ();
    }

    int
    directory::close (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory::%s() @%p\n", __func__, this);
#endif

      // assert(file_system_ != nullptr);
      errno = 0;

      // Execute the implementation specific code.
      int ret = impl ().do_close ();
      if (ret != 0)
        {
          trace::printf ("directory::%s() @%p do_close() returned %d\n",
                         __func__, this, ret);
        }

      // The file object will be deallocated at the next open.
      file_system ().add_deferred_directory (this);

      return ret;
    }

    // ========================================================================

    directory_impl::directory_impl (class file_system& fs) : file_system_ (fs)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_impl::%s()=%p\n", __func__, this);
#endif
      memset (&dir_entry_, 0, sizeof (dirent));
    }

    directory_impl::~directory_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ========================================================================

  } // namespace posix
} // namespace micro_os_plus

// ----------------------------------------------------------------------------
