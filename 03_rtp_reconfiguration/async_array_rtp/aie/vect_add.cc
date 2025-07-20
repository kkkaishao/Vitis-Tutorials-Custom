#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
#include "adf.h"

using namespace adf;

const int n_elem = 16;
template <int32 num>
void vect_add(
    adf::input_buffer<int32, adf::extents<num>>& __restrict in,
    adf::output_buffer<int32, adf::extents<num>>& __restrict out, const int32 (&value)[num]
) {
    auto in_it = aie::begin_vector<n_elem>(in);
    auto out_it = aie::begin_vector<n_elem>(out);
    auto v_it = aie::begin_vector<n_elem>(value);

    for (int i = 0; i < num / n_elem; i++)
        // clang-format off
        chess_prepare_for_pipelining
        // clang-format on
        {
            aie::vector<int32, n_elem> vdata = *in_it++;
            aie::vector<int32, n_elem> vv = *v_it++;
            aie::vector<int32, n_elem> vout = aie::add(vdata, vv);
            *out_it++ = vout;
        }
}
