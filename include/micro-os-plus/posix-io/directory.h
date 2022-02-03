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

#ifndef MICRO_OS_PLUS_POSIX_IO_DIRECTORY_H_
#define MICRO_OS_PLUS_POSIX_IO_DIRECTORY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(HAVE_MICRO_OS_PLUS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // HAVE_MICRO_OS_PLUS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/utils/lists.h>
#include <micro-os-plus/posix/dirent.h>

#include <micro-os-plus/diag/trace.h>

#include <mutex>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#if __GNUC__ >= 10
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class directory;
    class directory_impl;
    class file_system;

    // ------------------------------------------------------------------------

    // ========================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Directory class.
     * @headerfile directory.h <micro-os-plus/posix-io/directory.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class directory
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class file_system;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      directory (directory_impl& impl);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory (const directory&) = delete;
      directory (directory&&) = delete;
      directory&
      operator= (const directory&)
          = delete;
      directory&
      operator= (directory&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~directory ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html
      virtual dirent*
      read (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
      virtual void
      rewind (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
      virtual int
      close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      dirent*
      dir_entry (void);

      class file_system&
      file_system (void) const;

      directory_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    public:
      /**
       * @cond ignore
       */

      // Intrusive node used to link this device to the deferred
      // deallocation list. Must be public.
      utils::double_list_links deferred_links_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      directory_impl& impl_;

      /**
       * @endcond
       */
    };

    // ========================================================================

    class directory_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class directory;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      directory_impl (class file_system& fs);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory_impl (const directory_impl&) = delete;
      directory_impl (directory_impl&&) = delete;
      directory_impl&
      operator= (const directory_impl&)
          = delete;
      directory_impl&
      operator= (directory_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~directory_impl ();

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

      /**
       * @return object if successful, otherwise nullptr and errno.
       */
      virtual dirent*
      do_read (void)
          = 0;

      virtual void
      do_rewind (void)
          = 0;

      virtual int
      do_close (void)
          = 0;

      // ----------------------------------------------------------------------
      // Support functions.

      class file_system&
      file_system (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:
      /**
       * @cond ignore
       */

      // This also solves the readdir() re-entrancy issue.
      dirent dir_entry_;

      class file_system& file_system_;

      /**
       * @endcond
       */
    };

    // ========================================================================

    template <typename T>
    class directory_implementable : public directory
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
      directory_implementable (class file_system& fs);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory_implementable (const directory_implementable&) = delete;
      directory_implementable (directory_implementable&&) = delete;
      directory_implementable&
      operator= (const directory_implementable&)
          = delete;
      directory_implementable&
      operator= (directory_implementable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~directory_implementable ();

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
    class directory_lockable : public directory
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
      directory_lockable (class file_system& fs, lockable_type& locker);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory_lockable (const directory_lockable&) = delete;
      directory_lockable (directory_lockable&&) = delete;
      directory_lockable&
      operator= (const directory_lockable&)
          = delete;
      directory_lockable&
      operator= (directory_lockable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~directory_lockable () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // opendir() uses the file system lock.

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html
      virtual dirent*
      read (void) override;

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
      virtual void
      rewind (void) override;

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
      virtual int
      close (void) override;

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

      lockable_type& locker_;

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

    inline file_system&
    directory::file_system (void) const
    {
      return impl ().file_system ();
    }

    inline dirent*
    directory::dir_entry (void)
    {
      return &(impl ().dir_entry_);
    }

    inline directory_impl&
    directory::impl (void) const
    {
      return impl_;
    }

    // ========================================================================

    inline file_system&
    directory_impl::file_system (void) const
    {
      return file_system_;
    }

    // ========================================================================

    template <typename T>
    directory_implementable<T>::directory_implementable (class file_system& fs)
        : directory{ impl_instance_ }, //
          impl_instance_{ fs }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_implementable::%s()=@%p\n", __func__, this);
#endif
    }

    template <typename T>
    directory_implementable<T>::~directory_implementable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_implementable::%s() @%p\n", __func__, this);
#endif
    }

    template <typename T>
    typename directory_implementable<T>::value_type&
    directory_implementable<T>::impl (void) const
    {
      return static_cast<value_type&> (impl_);
    }

    // ========================================================================

    template <typename T, typename L>
    directory_lockable<T, L>::directory_lockable (class file_system& fs,
                                                  lockable_type& locker)
        : directory{ impl_instance_ }, //
          impl_instance_{ fs }, //
          locker_ (locker)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_lockable::%s()=@%p\n", __func__, this);
#endif
    }

    template <typename T, typename L>
    directory_lockable<T, L>::~directory_lockable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif
    }

    // ------------------------------------------------------------------------

    template <typename T, typename L>
    dirent*
    directory_lockable<T, L>::read (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

      std::lock_guard<L> lock{ locker_ };

      return directory::read ();
    }

    template <typename T, typename L>
    void
    directory_lockable<T, L>::rewind (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

      std::lock_guard<L> lock{ locker_ };

      return directory::rewind ();
    }

    template <typename T, typename L>
    int
    directory_lockable<T, L>::close (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_DIRECTORY)
      trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

      std::lock_guard<L> lock{ locker_ };

      return directory::close ();
    }

    template <typename T, typename L>
    typename directory_lockable<T, L>::value_type&
    directory_lockable<T, L>::impl (void) const
    {
      return static_cast<value_type&> (impl_);
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_DIRECTORY_H_

// ----------------------------------------------------------------------------
