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

#include <cerrno>
#include <micro-os-plus/posix/sys/socket.h>
#include <micro-os-plus/posix-io/net-stack.h>

#include <micro-os-plus/posix-io/socket.h>

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

    socket::socket (socket_impl& impl, class net_stack& ns)
        : io{ impl, type::socket }, //
          net_stack_ (&ns)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket::%s()=@%p\n", __func__, this);
#endif
    }

    socket::~socket ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket::%s() @%p\n", __func__, this);
#endif

      net_stack_ = nullptr;
    }

    // ------------------------------------------------------------------------

    class socket*
    socket::accept (sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      class socket* new_socket = impl ().do_accept (address, address_len);
      if (new_socket == nullptr)
        {
          return nullptr;
        }
      new_socket->alloc_file_descriptor ();
      return new_socket;
    }

    int
    socket::bind (const sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_bind (address, address_len);
    }

    int
    socket::connect (const sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_connect (address, address_len);
    }

    int
    socket::getpeername (sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getpeername (address, address_len);
    }

    int
    socket::getsockname (sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getsockname (address, address_len);
    }

    int
    socket::getsockopt (int level, int option_name, void* option_value,
                        socklen_t* option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getsockopt (level, option_name, option_value,
                                    option_len);
    }

    int
    socket::listen (int backlog)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_listen (backlog);
    }

    ssize_t
    socket::recv (void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recv (buffer, length, flags);
    }

    ssize_t
    socket::recvfrom (void* buffer, size_t length, int flags,
                      sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recvfrom (buffer, length, flags, address, address_len);
    }

    ssize_t
    socket::recvmsg (msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recvmsg (message, flags);
    }

    ssize_t
    socket::send (const void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_send (buffer, length, flags);
    }

    ssize_t
    socket::sendmsg (const msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sendmsg (message, flags);
    }

    ssize_t
    socket::sendto (const void* message, size_t length, int flags,
                    const sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sendto (message, length, flags, dest_addr, dest_len);
    }

    int
    socket::setsockopt (int level, int option_name, const void* option_value,
                        socklen_t option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_setsockopt (level, option_name, option_value,
                                    option_len);
    }

    int
    socket::shutdown (int how)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_shutdown (how);
    }

    int
    socket::sockatmark (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sockatmark ();
    }
    // ========================================================================

    socket_impl::socket_impl (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket_impl::%s()=%p\n", __func__, this);
#endif
    }

    socket_impl::~socket_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket_impl::%s() @%p\n", __func__, this);
#endif
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
