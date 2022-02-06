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

#ifndef MICRO_OS_PLUS_POSIX_IO_NET_INTERFACE_H_
#define MICRO_OS_PLUS_POSIX_IO_NET_INTERFACE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_HAS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_HAS_CONFIG_H

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class net_interface_impl;

    // ========================================================================

    /**
     * @brief Network interface class.
     * @headerfile net-interface.h <micro-os-plus/posix-io/net-interface.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class net_interface
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      net_interface (net_interface_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_interface (const net_interface&) = delete;
      net_interface (net_interface&&) = delete;
      net_interface&
      operator= (const net_interface&)
          = delete;
      net_interface&
      operator= (net_interface&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~net_interface ();

      /**
       * @}
       */

      // TODO: add content
    };

  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_NET_INTERFACE_H_

// ----------------------------------------------------------------------------
