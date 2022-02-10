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

// ----------------------------------------------------------------------------

#if defined(MICRO_OS_PLUS_INCLUDE_CONFIG_H)
#include <micro-os-plus/config.h>
#endif // MICRO_OS_PLUS_INCLUDE_CONFIG_H

#include <micro-os-plus/posix-io/block-device-partition.h>

#include <micro-os-plus/diag/trace.h>

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

    block_device_partition::block_device_partition (block_device_impl& impl,
                                                    const char* name)
        : block_device{ impl, name }
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s(\"%s\")=@%p\n", __func__,
                     name_, this);
#endif
    }

    block_device_partition::~block_device_partition ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s() @%p %s\n", __func__, this,
                     name_);
#endif
    }

    // ------------------------------------------------------------------------

    void
    block_device_partition::configure (blknum_t offset, blknum_t nblocks)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);
#endif

      impl ().configure (offset, nblocks);
    }

    // ========================================================================

    block_device_partition_impl::block_device_partition_impl (
        block_device& parent)
        : parent_ (parent)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s()=@%p\n", __func__,
                     this);
#endif
    }

    block_device_partition_impl::~block_device_partition_impl ()
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__,
                     this);
#endif
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    block_device_partition_impl::do_vioctl (int request,
                                            std::va_list arguments)
    {
      errno = ENOSYS;
      return -1;
    }

#pragma GCC diagnostic pop

    void
    block_device_partition_impl::configure (blknum_t offset, blknum_t nblocks)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(%u,%u) @%p\n", __func__,
                     offset, nblocks, this);
#endif

      partition_offset_blocks_ = offset;
      assert (nblocks > 0);
      num_blocks_ = nblocks;

      // Inherit from parent.
      block_logical_size_bytes_ = parent_.block_logical_size_bytes ();
      block_physical_size_bytes_ = parent_.block_physical_size_bytes ();
    }

    int
    block_device_partition_impl::do_vopen (const char* path, int oflag,
                                           std::va_list arguments)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(%d) @%p\n", __func__,
                     oflag, this);
#endif

      return parent_.vopen (path, oflag, arguments);
    }

    ssize_t
    block_device_partition_impl::do_read_block (void* buf, blknum_t blknum,
                                                std::size_t nblocks)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);
#endif

      return parent_.read_block (buf, blknum + partition_offset_blocks_,
                                 nblocks);
    }

    ssize_t
    block_device_partition_impl::do_write_block (const void* buf,
                                                 blknum_t blknum,
                                                 std::size_t nblocks)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s(0x%X, %u, %u) @%p\n",
                     __func__, buf, blknum, nblocks, this);
#endif

      return parent_.write_block (buf, blknum + partition_offset_blocks_,
                                  nblocks);
    }

    void
    block_device_partition_impl::do_sync (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__,
                     this);
#endif

      return parent_.sync ();
    }

    int
    block_device_partition_impl::do_close (void)
    {
#if defined(MICRO_OS_PLUS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
      trace::printf ("block_device_partition_impl::%s() @%p\n", __func__,
                     this);
#endif

      return parent_.close ();
    }

    // ==========================================================================
  } // namespace posix
} // namespace micro_os_plus

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
