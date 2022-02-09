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

#ifndef MICRO_OS_PLUS_POSIX_IO_NET_STACK_H_
#define MICRO_OS_PLUS_POSIX_IO_NET_STACK_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/socket.h>
#include <micro-os-plus/utils/lists.h>

#include <micro-os-plus/diag/trace.h>

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#if __GNUC__ >= 10
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class socket;
    class net_interface;

    class net_stack_impl;

    /**
     * @ingroup micro-os-plus-posix-io-function
     * @{
     */

    // ------------------------------------------------------------------------
    // ----- Non-io, global file system functions -----
    class socket*
    socket (int domain, int type, int protocol);

    /**
     * @}
     */

    // ------------------------------------------------------------------------
    /**
     * @brief Network stack class.
     * @headerfile net-stack.h <micro-os-plus/posix-io/net-stack.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class net_stack
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      net_stack (net_stack_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_stack (const net_stack&) = delete;
      net_stack (net_stack&&) = delete;
      net_stack&
      operator= (const net_stack&)
          = delete;
      net_stack&
      operator= (net_stack&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~net_stack ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Static Member Functions
       * @{
       */

    public:
      virtual class socket*
      socket (int domain, int type, int protocol);

      const char*
      name (void) const;

      void
      add_deferred_socket (class socket* sock);

      using deferred_sockets_list_t
          = utils::intrusive_list<class socket, utils::double_list_links,
                                  &socket::deferred_links_>;

      deferred_sockets_list_t&
      deferred_sockets_list (void);

      // ----------------------------------------------------------------------

      template <typename T>
      T*
      allocate_socket (void);

      template <typename T, typename L>
      T*
      allocate_socket (L& locker);

      // ----------------------------------------------------------------------
      // Support functions.

      net_interface&
      interface (void) const;

      net_stack_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      const char* name_ = nullptr;

      net_stack_impl& impl_;

      deferred_sockets_list_t deferred_sockets_list_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    public:
      /**
       * @cond ignore
       */

      // Intrusive node used to link this net stack to the
      // net manager list.
      // Must be public. The constructor clears the pointers.
      utils::double_list_links net_manager_links_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      // Statics.
      using net_list
          = utils::intrusive_list<net_stack, utils::double_list_links,
                                  &net_stack::net_manager_links_>;
      static net_list net_list__;

      /**
       * @endcond
       */
    };

    // ========================================================================

    class net_stack_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      net_stack_impl (net_interface& interface);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_stack_impl (const net_stack_impl&) = delete;
      net_stack_impl (net_stack_impl&&) = delete;
      net_stack_impl&
      operator= (const net_stack_impl&)
          = delete;
      net_stack_impl&
      operator= (net_stack_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~net_stack_impl ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      virtual class socket*
      do_socket (int domain, int type, int protocol)
          = 0;

      // ----------------------------------------------------------------------
      // Support functions.

      net_interface&
      interface (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      net_interface& interface_;

      /**
       * @endcond
       */
    };

    // ========================================================================

    template <typename T>
    class net_stack_implementable : public net_stack
    {
      // ----------------------------------------------------------------------

    public:
      using value_type = T;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      template <typename... Args>
      net_stack_implementable (const char* name, net_interface& interface,
                               Args&&... arguments);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_stack_implementable (const net_stack_implementable&) = delete;
      net_stack_implementable (net_stack_implementable&&) = delete;
      net_stack_implementable&
      operator= (const net_stack_implementable&)
          = delete;
      net_stack_implementable&
      operator= (net_stack_implementable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~net_stack_implementable ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // Support functions.

      value_type&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      value_type impl_instance_;

      /**
       * @endcond
       */
    };

    // ========================================================================

    template <typename T, typename L>
    class net_stack_lockable : public net_stack
    {
      // ----------------------------------------------------------------------

    public:
      using value_type = T;
      using lockable_type = L;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      template <typename... Args>
      net_stack_lockable (const char* name, net_interface& interface,
                          lockable_type& locker, Args&&... arguments);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_stack_lockable (const net_stack_lockable&) = delete;
      net_stack_lockable (net_stack_lockable&&) = delete;
      net_stack_lockable&
      operator= (const net_stack_lockable&)
          = delete;
      net_stack_lockable&
      operator= (net_stack_lockable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~net_stack_lockable ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // TBD

      // ----------------------------------------------------------------------
      // Support functions.

      value_type&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      value_type impl_instance_;

      /**
       * @endcond
       */
    };

    // ========================================================================
  } // namespace posix
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    inline const char*
    net_stack::name (void) const
    {
      return name_;
    }

    inline net_stack_impl&
    net_stack::impl (void) const
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"
      return impl_;
#pragma GCC diagnostic pop
    }

    inline void
    net_stack::add_deferred_socket (class socket* sock)
    {
      deferred_sockets_list_.link (*sock);
    }

    inline net_stack::deferred_sockets_list_t&
    net_stack::deferred_sockets_list (void)
    {
      return deferred_sockets_list_;
    }

    template <typename T>
    T*
    net_stack::allocate_socket (void)
    {
      using socket_type = T;

      socket_type* sock;

      if (deferred_sockets_list_.empty ())
        {
          sock = new socket_type (*this);
        }
      else
        {
          sock = static_cast<socket_type*> (
              deferred_sockets_list_.unlink_head ());

          // Call the constructor before reusing the object,
          sock->~socket_type ();

          // Placement new, run only the constructor.
          new (sock) socket_type (*this);

          // Deallocate all remaining elements in the list.
          while (!deferred_sockets_list_.empty ())
            {
              socket_type* s = static_cast<socket_type*> (
                  deferred_sockets_list_.unlink_head ());

              // Call the destructor and the deallocator.
              delete s;
            }
        }
      return sock;
    }

    template <typename T, typename L>
    T*
    net_stack::allocate_socket (L& locker)
    {
      using socket_type = T;

      socket_type* sock;

      if (deferred_sockets_list_.empty ())
        {
          sock = new socket_type (*this, locker);
        }
      else
        {
          sock = static_cast<socket_type*> (
              deferred_sockets_list_.unlink_head ());

          // Call the constructor before reusing the object,
          sock->~socket_type ();

          // Placement new, run only the constructor.
          new (sock) socket_type (*this, locker);

          // Deallocate all remaining elements in the list.
          while (!deferred_sockets_list_.empty ())
            {
              socket_type* s = static_cast<socket_type*> (
                  deferred_sockets_list_.unlink_head ());

              // Call the destructor and the deallocator.
              delete s;
            }
        }
      return sock;
    }

    // ========================================================================

    inline net_interface&
    net_stack_impl::interface (void) const
    {
      return interface_;
    }

    // ========================================================================

    template <typename T>
    template <typename... Args>
    net_stack_implementable<T>::net_stack_implementable (
        const char* name, net_interface& interface, Args&&... arguments)
        : net_stack{ impl_instance_, name }, //
          impl_instance_{ interface, std::forward<Args> (arguments)... }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack_implementable::%s(\"%s\")=@%p\n", __func__,
                     name_, this);
#endif
    }

    template <typename T>
    net_stack_implementable<T>::~net_stack_implementable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack_implementable::%s() @%p %s\n", __func__, this,
                     name_);
#endif
    }

    template <typename T>
    typename net_stack_implementable<T>::value_type&
    net_stack_implementable<T>::impl (void) const
    {
      return static_cast<value_type&> (impl_);
    }

    // ========================================================================

    template <typename T, typename L>
    template <typename... Args>
    net_stack_lockable<T, L>::net_stack_lockable (const char* name,
                                                  net_interface& interface,
                                                  lockable_type& locker,
                                                  Args&&... arguments)
        : net_stack{ impl_instance_, name }, //
          impl_instance_{ interface, locker,
                          std::forward<Args> (arguments)... }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack_lockable::%s()=%p\n", __func__, this);
#endif
    }

    template <typename T, typename L>
    net_stack_lockable<T, L>::~net_stack_lockable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack_lockable::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    template <typename T, typename L>
    typename net_stack_lockable<T, L>::value_type&
    net_stack_lockable<T, L>::impl (void) const
    {
      return static_cast<value_type&> (impl_);
    }

    // ========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_NET_STACK_H_

// ----------------------------------------------------------------------------
