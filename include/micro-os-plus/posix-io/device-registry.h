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

#ifndef MICRO_OS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_
#define MICRO_OS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(HAVE_MICRO_OS_PLUS_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // HAVE_MICRO_OS_PLUS_CONFIG_H

// ----------------------------------------------------------------------------

#include <micro-os-plus/posix-io/device.h>
#include <micro-os-plus/diag/trace.h>

#include <cstddef>
#include <cassert>
#include <cstring>

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

    /**
     * @brief Devices registry static class.
     * @headerfile device-registry.h <micro-os-plus/posix-io/device-registry.h>
     * @ingroup micro-os-plus-posix-io-base
     */
    template <typename T>
    class device_registry
    {
    public:
      using value_type = T;
      using pointer = T*;
      using reference = T&;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:
      // Do not allow to create instances of this class.
      device_registry () = delete;

      /**
       * @cond ignore
       */

      // The rule of five.
      device_registry (const value_type&) = delete;
      device_registry (value_type&&) = delete;
      value_type&
      operator= (const value_type&)
          = delete;
      value_type&
      operator= (value_type&&)
          = delete;

      /**
       * @endcond
       */

      ~device_registry () = delete;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Static Member Functions
       * @{
       */

    public:
      static void
      link (value_type* device);

      static value_type*
      identify_device (const char* path);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    private:
      /**
       * @cond ignore
       */

      // Since devices may be constructed statically, so may ask
      // to be linked here at any time, this list must be initialised
      // before any static constructor.
      // With the order of static constructors unknown, this means it
      // must be allocated in the BSS and will be initialised to 0 by
      // the startup code.
      using device_list
          = utils::intrusive_list<device, utils::double_list_links,
                                  &device::registry_links_, T>;
      static device_list registry_list__;

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

    template <typename T>
    void
    device_registry<T>::link (value_type* device)
    {
#if defined(DEBUG)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
      for (auto&& d : registry_list__)
        {
          // Validate the device name by checking duplicates.
          if (std::strcmp (device->name (), d.name ()) == 0)
            {
              trace::puts ("Duplicate device name. Abort.");
              std::abort ();
            }
        }
#pragma GCC diagnostic pop
#endif // DEBUG

      registry_list__.link (*device);

      trace::printf ("Device '%s%s' linked.\n", value_type::device_prefix (),
                     device->name ());
    }

    /**
     * return pointer to device or nullptr if not found.
     */
    template <typename T>
    T*
    device_registry<T>::identify_device (const char* path)
    {
      assert (path != nullptr);

      auto prefix = value_type::device_prefix ();
      if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
        {
          // The device prefix does not match, not a device.
          return nullptr;
        }

      // The prefix was identified; try to match the rest of the path.
      auto name = path + std::strlen (prefix);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"
      for (auto&& p : registry_list__)
        {
          if (p.match_name (name))
            {
              return static_cast<value_type*> (&p);
            }
        }
#pragma GCC diagnostic pop

      // Not a known device.
      return nullptr;
    }

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

    // Initialised to 0 by BSS.
    template <typename T>
    typename device_registry<T>::device_list
        device_registry<T>::registry_list__;

#pragma GCC diagnostic pop

    /**
     * @endcond
     */

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif // __cplusplus

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_

// ----------------------------------------------------------------------------
