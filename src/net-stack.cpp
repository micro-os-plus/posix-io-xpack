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

#include <micro-os-plus/posix-io/net-stack.h>
#include <micro-os-plus/posix-io/net-interface.h>

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
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

    net_stack::net_list net_stack::net_list__;

#pragma GCC diagnostic pop

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    class socket*
    socket (int domain, int type, int protocol)
    {
      errno = 0;

      // TODO: implement a way to identify the net stack.
      net_stack* ns = nullptr;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
      class socket* sock = ns->socket (domain, type, protocol);
#pragma GCC diagnostic pop
      if (sock == nullptr)
        {
          return nullptr;
        }
      sock->alloc_file_descriptor ();
      return sock;
    }

#pragma GCC diagnostic pop

    // ========================================================================

    net_stack::net_stack (net_stack_impl& impl, const char* name)
        : name_ (name), //
          impl_ (impl)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
      deferred_sockets_list_.clear ();
    }

    net_stack::~net_stack ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack::%s(\"%s\") %p\n", __func__, name_, this);
#endif
    }

    class socket*
    net_stack::socket (int domain, int type, int protocol)
    {
      errno = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
      return impl ().do_socket (domain, type, protocol);
#pragma GCC diagnostic pop
    }

    // ========================================================================

    net_stack_impl::net_stack_impl (net_interface& interface)
        : interface_ (interface)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("net_stack_impl::%s()=%p\n", __func__, this);
#endif
    }

    net_stack_impl::~net_stack_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("net_stack_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
