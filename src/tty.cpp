/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#include <micro-os-plus/posix-io/tty.h>

#include <micro-os-plus/diag/trace.h>

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

    tty::tty (tty_impl& impl, const char* name) : char_device{ impl, name }
    {
      type_ |= type::tty;
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty::%s(\"%s\")=@%p\n", __func__, name_, this);
#endif
    }

    tty::~tty () noexcept
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty::%s() @%p %s\n", __func__, this, name_);
#endif
    }

    // ------------------------------------------------------------------------

    inline int
    tty::tcsendbreak (int duration)
    {
      return impl ().do_tcsendbreak (duration);
    }

    inline int
    tty::tcgetattr (termios* ptio)
    {
      return impl ().do_tcgetattr (ptio);
    }

    inline int
    tty::tcsetattr (int options, const termios* ptio)
    {
      return impl ().do_tcsetattr (options, ptio);
    }

    inline int
    tty::tcflush (int queue_selector)
    {
      return impl ().do_tcflush (queue_selector);
    }

    inline int
    tty::tcdrain (void)
    {
      return impl ().do_tcdrain ();
    }

    // ========================================================================

    tty_impl::tty_impl (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_impl::%s()=@%p\n", __func__, this);
#endif
    }

    tty_impl::~tty_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_impl::%s() @%p\n", __func__, this);
#endif
    }

    int
    tty_impl::do_isatty (void)
    {
      return 1; // Yes!
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
