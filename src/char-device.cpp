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

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#include <micro-os-plus/posix-io/char-device.h>
#include <micro-os-plus/posix-io/device-registry.h>

#include <cstring>
#include <cassert>
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

    char_device::char_device (char_device_impl& impl, const char* name)
        : device{ impl, type::char_device, name }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device::%s(\"%s\")=@%p\n", __func__, name_, this);
#endif

      device_registry<device>::link (this);
    }

    char_device::~char_device ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device::%s() @%p %s\n", __func__, this, name_);
#endif

      registry_links_.unlink ();

      name_ = nullptr;
    }

    // ========================================================================

    char_device_impl::char_device_impl (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device_impl::%s()=@%p\n", __func__, this);
#endif
    }

    char_device_impl::~char_device_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device_impl::%s() @%p\n", __func__, this);
#endif
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    off_t
    char_device_impl::do_lseek (off_t offset, int whence)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

    void
    char_device_impl::do_sync (void)
    {
      errno = ENOSYS; // Not implemented
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
