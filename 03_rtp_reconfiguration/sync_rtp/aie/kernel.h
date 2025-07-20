#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "adf.h"

template <int32 num>
void vect_add(
    adf::input_buffer<int32, adf::extents<num>>& __restrict in,
    adf::output_buffer<int32, adf::extents<num>>& __restrict out, int32 value
);

#endif // __KERNEL_H__
