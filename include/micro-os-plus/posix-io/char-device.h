/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MICRO_OS_PLUS_POSIX_IO_CHAR_DEVICE_H_
#define MICRO_OS_PLUS_POSIX_IO_CHAR_DEVICE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(HAVE_MICRO_OS_PLUS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // HAVE_MICRO_OS_PLUS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/device.h>

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

    class char_device_impl;

    // ========================================================================

    /**
     * @brief Char device class.
     * @headerfile char-device.h <micro-os-plus/posix-io/char-device.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    class char_device : public device
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      char_device (char_device_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      char_device (const char_device&) = delete;
      char_device (char_device&&) = delete;
      char_device&
      operator= (const char_device&)
          = delete;
      char_device&
      operator= (char_device&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~char_device ();

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

      char_device_impl&
      impl (void) const;

      /**
       * @}
       */
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class char_device_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      friend class char_device;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      char_device_impl (void);

      /**
       * @cond ignore
       */

      // The rule of five.
      char_device_impl (const char_device_impl&) = delete;
      char_device_impl (char_device_impl&&) = delete;
      char_device_impl&
      operator= (const char_device_impl&)
          = delete;
      char_device_impl&
      operator= (char_device_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~char_device_impl () override;

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

      virtual off_t
      do_lseek (off_t offset, int whence) override;

      virtual void
      do_sync (void) override;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template <typename T>
    class char_device_implementable : public char_device
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
      char_device_implementable (const char* name, Args&&... arguments);

      /**
       * @cond ignore
       */

      // The rule of five.
      char_device_implementable (const char_device_implementable&) = delete;
      char_device_implementable (char_device_implementable&&) = delete;
      char_device_implementable&
      operator= (const char_device_implementable&)
          = delete;
      char_device_implementable&
      operator= (char_device_implementable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~char_device_implementable ();

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

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

// ===== Inline & template implementations ====================================

namespace micro_os_plus
{
  namespace posix
  {
    // ========================================================================

    inline char_device_impl&
    char_device::impl (void) const
    {
      return static_cast<char_device_impl&> (impl_);
    }

    // ========================================================================

    template <typename T>
    template <typename... Args>
    char_device_implementable<T>::char_device_implementable (
        const char* name, Args&&... arguments)
        : char_device{ impl_instance_, name }, //
          impl_instance_{ std::forward<Args> (arguments)... }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device_implementable::%s(\"%s\")=@%p\n", __func__,
                     name_, this);
#endif
    }

    template <typename T>
    char_device_implementable<T>::~char_device_implementable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_CHAR_DEVICE)
      trace::printf ("char_device_implementable::%s() @%p %s\n", __func__,
                     this, name_);
#endif
    }

    template <typename T>
    typename char_device_implementable<T>::value_type&
    char_device_implementable<T>::impl (void) const
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

#endif // MICRO_OS_PLUS_POSIX_IO_CHAR_DEVICE_H_

// ----------------------------------------------------------------------------
