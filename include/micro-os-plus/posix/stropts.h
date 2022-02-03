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

#ifndef POSIX_STROPTS_H_
#define POSIX_STROPTS_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)
#include_next <stropts.h>
// #include <sys/ioctl.h>
#else

#ifdef __cplusplus
extern "C"
{
#endif

  // --------------------------------------------------------------------------

  int
  ioctl (int fildes, int request, ...);

  // --------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // defined(_POSIX_VERSION)

// ----------------------------------------------------------------------------

#endif // POSIX_STROPTS_H_

// ----------------------------------------------------------------------------
