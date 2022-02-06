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

#ifndef MICRO_OS_PLUS_POSIX_IO_IO_H_
#define MICRO_OS_PLUS_POSIX_IO_IO_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_HAS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_HAS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/types.h>
#include <micro-os-plus/diag/trace.h>

#include <cstddef>
#include <cstdarg>

// Needed for ssize_t
#include <sys/types.h>

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

struct iovec;

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class io_impl;

    class file_system;
    class socket;

    /**
     * @ingroup micro-os-plus-posix-io-function
     * @{
     */

    // ------------------------------------------------------------------------
    io*
    open (const char* path, int oflag, ...);

    io*
    vopen (const char* path, int oflag, std::va_list arguments);

    /**
     * @}
     */

    // ========================================================================
    /**
     * @brief Base I/O class.
     * @headerfile io.h <micro-os-plus/posix-io/io.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class io
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class file_system;
      friend class file_descriptors_manager;

      friend io*
      vopen (const char* path, int oflag, std::va_list arguments);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
      friend class socket*
      socket (int domain, int type, int protocol);
#pragma GCC diagnostic pop

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    public:
      /**
       * @name Types & Constants
       * @{
       */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

      using type_t = unsigned int;
      enum type : type_t
      {
        unknown = 0,
        not_set = 1 << 0,
        char_device = 1 << 1,
        block_device = 1 << 2,
        tty = 1 << 3,
        file = 1 << 4,
        socket = 1 << 5
      };

#pragma GCC diagnostic pop

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    protected:
      io (io_impl& impl, type t);

      /**
       * @cond ignore
       */

      // The rule of five.
      io (const io&) = delete;
      io (io&&) = delete;
      io&
      operator= (const io&)
          = delete;
      io&
      operator= (io&&)
          = delete;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    public:
      virtual ~io ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      virtual int
      close (void);

      virtual ssize_t
      read (void* buf, std::size_t nbyte);

      virtual ssize_t
      write (const void* buf, std::size_t nbyte);

      virtual ssize_t
      writev (const iovec* iov, int iovcnt);

      int
      fcntl (int cmd, ...);

      virtual int
      vfcntl (int cmd, std::va_list arguments);

      int
      isatty (void);

      virtual int
      fstat (stat* buf);

      virtual off_t
      lseek (off_t offset, int whence);

      // ----------------------------------------------------------------------
      // Support functions.

      type_t
      get_type (void) const;

      file_descriptor_t
      file_descriptor (void) const;

      bool
      is_opened (void);

      io_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Functions
       * @{
       */

    protected:
      // ----------------------------------------------------------------------
      // Support functions.

#if 0
      // Is called at the end of close, to release objects
      // acquired from a pool.
      virtual void
      do_release (void);
#endif

      void
      file_descriptor (file_descriptor_t fildes);

      void
      clear_file_descriptor (void);

      io*
      alloc_file_descriptor (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      io_impl& impl_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      type_t type_ = type::not_set;

      file_descriptor_t file_descriptor_ = no_file_descriptor;

      /**
       * @endcond
       */
    };

    // ========================================================================

    class io_impl
    {
      // ----------------------------------------------------------------------

      friend class io;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      io_impl (void);

      /**
       * @cond ignore
       */

      // The rule of five.
      io_impl (const io_impl&) = delete;
      io_impl (io_impl&&) = delete;
      io_impl&
      operator= (const io_impl&)
          = delete;
      io_impl&
      operator= (io_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~io_impl ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // Implementations

      virtual void
      do_deallocate (void);

      virtual bool
      do_is_opened (void)
          = 0;

      virtual bool
      do_is_connected (void);

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte)
          = 0;

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte)
          = 0;

      virtual ssize_t
      do_writev (const iovec* iov, int iovcnt);

      virtual int
      do_vfcntl (int cmd, std::va_list arguments);

      virtual int
      do_isatty (void);

      virtual int
      do_fstat (stat* buf);

      virtual off_t
      do_lseek (off_t offset, int whence)
          = 0;

      virtual int
      do_close (void)
          = 0;

      // ----------------------------------------------------------------------
      // Support functions.

      off_t
      offset (void);

      void
      offset (off_t offset);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      off_t offset_ = 0;

      /**
       * @endcond
       */
    };

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    inline io::type_t
    io::get_type (void) const
    {
      return type_;
    }

    inline void
    io::file_descriptor (file_descriptor_t fildes)
    {
      file_descriptor_ = fildes;
    }

    inline void
    io::clear_file_descriptor (void)
    {
      file_descriptor_ = no_file_descriptor;
    }

    inline file_descriptor_t
    io::file_descriptor (void) const
    {
      return file_descriptor_;
    }

    inline bool
    io::is_opened (void)
    {
      return impl ().do_is_opened ();
    }

    inline io_impl&
    io::impl (void) const
    {
      return impl_;
    }

    // ========================================================================

    inline off_t
    io_impl::offset (void)
    {
      return offset_;
    }

    inline void
    io_impl::offset (off_t offset)
    {
      offset_ = offset;
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_IO_H_

// ----------------------------------------------------------------------------
