#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
#include "adf.h"

using namespace adf;

const int n_elem = 16;
template <int32 num>
void vect_add(
    adf::input_buffer<int32, adf::extents<num>>& __restrict in,
    adf::output_buffer<int32, adf::extents<num>>& __restrict out, int32 value
) {
    auto in_it = aie::begin_vector<n_elem>(in);
    auto out_it = aie::begin_vector<n_elem>(out);
    aie::vector<int32, n_elem> v = aie::broadcast<int32>(value);

    for (int i = 0; i < num / n_elem; i++)
        // clang-format off
        chess_prepare_for_pipelining
        // clang-format on
        {
            aie::vector<int32, n_elem> vdata = *in_it++;
            aie::vector<int32, n_elem> vout = aie::add(vdata, v);
            *out_it++ = vout;
        }
}
