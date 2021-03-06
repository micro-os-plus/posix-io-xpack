/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017 Liviu Ionescu.
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

#ifndef MICRO_OS_PLUS_POSIX_IO_TTY_H_
#define MICRO_OS_PLUS_POSIX_IO_TTY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(HAVE_MICRO_OS_PLUS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // HAVE_MICRO_OS_PLUS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/char-device.h>
#include <micro-os-plus/posix/termios.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

namespace micro_os_plus
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class tty_impl;

    // ========================================================================

    class tty : public char_device
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      tty (tty_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      tty (const tty&) = delete;
      tty (tty&&) = delete;
      tty&
      operator= (const tty&)
          = delete;
      tty&
      operator= (tty&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~tty () noexcept;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:
      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcgetattr.html
      virtual int
      tcgetattr (termios* ptio);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsetattr.html
      virtual int
      tcsetattr (int options, const termios* ptio);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcflush.html
      virtual int
      tcflush (int queue_selector);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsendbreak.html
      virtual int
      tcsendbreak (int duration);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcdrain.html
      virtual int
      tcdrain (void);

      // ----------------------------------------------------------------------
      // Support functions.

      tty_impl&
      impl (void) const;
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class tty_impl : public char_device_impl
    {
      // ----------------------------------------------------------------------

      friend class tty;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      tty_impl (void);

      /**
       * @cond ignore
       */

      // The rule of five.
      tty_impl (const tty_impl&) = delete;
      tty_impl (tty_impl&&) = delete;
      tty_impl&
      operator= (const tty_impl&)
          = delete;
      tty_impl&
      operator= (tty_impl&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~tty_impl () override;

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
      do_tcgetattr (termios* ptio)
          = 0;

      virtual int
      do_tcsetattr (int options, const termios* ptio)
          = 0;

      virtual int
      do_tcflush (int queue_selector)
          = 0;

      virtual int
      do_tcsendbreak (int duration)
          = 0;

      virtual int
      do_tcdrain (void)
          = 0;

      virtual int
      do_isatty (void) override final;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template <typename T>
    class tty_implementable : public tty
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
      tty_implementable (const char* name, Args&&... arguments);

      /**
       * @cond ignore
       */

      // The rule of five.
      tty_implementable (const tty_implementable&) = delete;
      tty_implementable (tty_implementable&&) = delete;
      tty_implementable&
      operator= (const tty_implementable&)
          = delete;
      tty_implementable&
      operator= (tty_implementable&&)
          = delete;

      /**
       * @endcond
       */

      virtual ~tty_implementable ();

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

    inline tty_impl&
    tty::impl (void) const
    {
      return static_cast<tty_impl&> (impl_);
    }

    // ========================================================================

    template <typename T>
    template <typename... Args>
    tty_implementable<T>::tty_implementable (const char* name,
                                             Args&&... arguments)
        : tty{ impl_instance_, name }, //
          impl_instance_{ std::forward<Args> (arguments)... }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_implementable::%s(\"%s\")=@%p\n", __func__, name_,
                     this);
#endif
    }

    template <typename T>
    tty_implementable<T>::~tty_implementable ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_implementable::%s() @%p %s\n", __func__, this,
                     name_);
#endif
    }

    template <typename T>
    typename tty_implementable<T>::value_type&
    tty_implementable<T>::impl (void) const
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

#endif // MICRO_OS_PLUS_POSIX_IO_TTY_H_

// ----------------------------------------------------------------------------
