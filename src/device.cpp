/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#include <micro-os-plus/posix-io/device.h>
#include <micro-os-plus/posix/sys/ioctl.h>

#include <cstring>
#include <cassert>
#include <cerrno>
#include <cstdarg>
#include <unistd.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    device::device (device_impl& impl, type t, const char* name)
        : io{ impl, t }, //
          name_ (name)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
    }

    device::~device ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      registry_links_.unlink ();

      name_ = nullptr;
    }

    // ------------------------------------------------------------------------

    int
    device::open (const char* path, int oflag, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list arguments;
      va_start (arguments, oflag);
      int ret = vopen (path, oflag, arguments);
      va_end (arguments);

      return ret;
    }

    int
    device::vopen (const char* path, int oflag, std::va_list arguments)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\") @%p\n", __func__, path ? path : "",
                     this);
#endif

      errno = 0;

      int ret = 0;
      if (impl ().open_count_ == 0)
        {
          // If so, use the implementation to open the device.
          ret = impl ().do_vopen (path, oflag, arguments);
          if (ret < 0)
            {
              // Open failed.
              return -1;
            }

          auto iop = alloc_file_descriptor ();
          if (iop == nullptr)
            {
              return -1;
            }
        }
      ++(impl ().open_count_);
      ret = file_descriptor ();
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(\"%s\")=%p fd=%d\n", __func__,
                     path ? path : "", this, ret);
#endif

      return ret;
    }

    int
    device::close (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      errno = 0;

      int ret = 0;
      if (impl ().open_count_ == 1)
        {
          ret = io::close ();
        }

      if (impl ().open_count_ > 0)
        {
          // Must be after close(), to keep do_is_open() true.
          --(impl ().open_count_);
        }

      return ret;
    }

    int
    device::ioctl (int request, ...)
    {
      // Forward to the variadic version of the function.
      std::va_list arguments;
      va_start (arguments, request);
      int ret = vioctl (request, arguments);
      va_end (arguments);

      return ret;
    }

    int
    device::vioctl (int request, std::va_list arguments)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s(%d) @%p\n", __func__, request, this);
#endif

      if (impl ().open_count_ == 0)
        {
          errno = EBADF; // Not opened.
          return -1;
        }

      errno = 0;

      return impl ().do_vioctl (request, arguments);
    }

    void
    device::sync (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device::%s() @%p\n", __func__, this);
#endif

      if (impl ().open_count_ == 0)
        {
          errno = EBADF; // Not opened.
          return;
        }

      impl ().do_sync ();
    }

    // ------------------------------------------------------------------------

    bool
    device::match_name (const char* name) const
    {
      assert (name != nullptr);
      assert (name_ != nullptr);

      return (std::strcmp (name, name_) == 0);
    }

    // ========================================================================

    device_impl::device_impl (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device_impl::%s()=%p\n", __func__, this);
#endif
    }

    device_impl::~device_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DEVICE)
      trace::printf ("device_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    bool
    device_impl::do_is_opened (void)
    {
      return (open_count_ > 0);
    }

    // ========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
