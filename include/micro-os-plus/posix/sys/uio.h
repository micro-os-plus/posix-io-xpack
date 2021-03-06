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

#ifndef POSIX_IO_SYS_UIO_H_
#define POSIX_IO_SYS_UIO_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif
#include_next <sys/uio.h>
#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

  struct iovec
  {
    void* iov_base; // Base address of a memory region for input or output.
    size_t iov_len; // The size of the memory pointed to by iov_base.
  };

  ssize_t
  writev (int fildes, const struct iovec* iov, int iovcnt);

  // --------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // defined(_POSIX_VERSION)

// ----------------------------------------------------------------------------

#endif // POSIX_IO_SYS_UIO_H_

// ----------------------------------------------------------------------------
