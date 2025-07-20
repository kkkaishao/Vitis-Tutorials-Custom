/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <adf.h>

void vectorized_weighted_sum_with_margin(
    adf::input_buffer<int32, adf::extents<256>, adf::margin<8>>& restrict in,
    adf::output_buffer<int32, adf::extents<256>>& restrict out
);
#endif
