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

#ifndef MICRO_OS_PLUS_POSIX_IO_BLOCK_H_
#define MICRO_OS_PLUS_POSIX_IO_BLOCK_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_HAS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_HAS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/io.h>
#include <micro-os-plus/utils/lists.h>
#include <mutex>

// ----------------------------------------------------------------------------

#if !defined(MICRO_OS_PLUS_STRING_POSIX_DEVICE_PREFIX)
#define MICRO_OS_PLUS_STRING_POSIX_DEVICE_PREFIX "/dev/"
#endif

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class device_impl;

    // ========================================================================

    /**
     * @brief Base device class.
     * @headerfile device.h <micro-os-plus/posix-io/device.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class device : public io
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      device (device_impl& impl, type t, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      device (const device&) = delete;
      device (device&&) = delete;
      device&
      operator= (const device&)
          = delete;
      device&
      operator= (device&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~device () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      /**
       *
       * @param path May be nullptr.
       * @param oflag Flags.
       * @return A non-negative integer, termed a file descriptor, or -1
       * on failure.
       */
      int
      open (const char* path = nullptr, int oflag = 0, ...);

      /**
       *
       * @param path May be nullptr.
       * @param oflag Flags.
       * @param arguments Variable list of arguments.
       * @return A non-negative integer, termed a file descriptor, or -1
       * on failure.
       */
      int
      vopen (const char* path, int oflag, std::va_list arguments);

      virtual int
      close (void) override;

      int
      ioctl (int request, ...);

      virtual int
      vioctl (int request, std::va_list arguments);

      virtual void
      sync (void);

      // ----------------------------------------------------------------------

      virtual bool
      match_name (const char* name) const;

      const char*
      name (void) const;

      static const char*
      device_prefix (void);

      // ----------------------------------------------------------------------
      // Support functions.

      device_impl&
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

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    public:
      /**
       * @cond ignore
       */

      // Intrusive node used to link this device to the registry list.
      // Must be public.
      utils::double_list_links registry_links_;

      /**
       * @endcond
       */
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class device_impl : public io_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class device;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      device_impl (void);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_impl (const device_impl&) = delete;
      device_impl (device_impl&&) = delete;
      device_impl&
      operator= (const device_impl&)
          = delete;
      device_impl&
      operator= (device_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~device_impl () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      virtual bool
      do_is_opened (void) override;

      virtual int
      do_vopen (const char* path, int oflag, std::va_list arguments)
          = 0;

      virtual int
      do_vioctl (int request, std::va_list arguments)
          = 0;

      virtual void
      do_sync (void)
          = 0;

      // ----------------------------------------------------------------------

      int
      open_count (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      int open_count_ = 0;

      /**
       * @endcond
       */
    };

#pragma GCC diagnostic pop

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    inline const char*
    device::name (void) const
    {
      return name_;
    }

    inline device_impl&
    device::impl (void) const
    {
      return static_cast<device_impl&> (impl_);
    }

    inline const char*
    device::device_prefix (void)
    {
      return MICRO_OS_PLUS_STRING_POSIX_DEVICE_PREFIX;
    }

    // ========================================================================

    inline int
    device_impl::open_count (void)
    {
      return open_count_;
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_BLOCK_H_

// ----------------------------------------------------------------------------
