#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"
#include "adf.h"

using namespace adf;
alignas(aie::vector_decl_align) int32 weights[8] = {1, 2, 3, 4, 5, 6, 7, 8};

void vectorized_weighted_sum_with_margin(
    input_buffer<int32, extents<256>, margin<8>>& restrict in, output_buffer<int32, extents<256>>& restrict out
) {
    auto in_it = aie::begin_vector<8>(in);
    auto out_it = aie::begin_vector<8>(out);
    aie::vector<int32, 8> coeffs = aie::load_v<8>(weights);

    aie::vector<int32, 16> data;
    aie::vector<int32, 8> data_out;
    data.insert(0, *in_it++);

    for (unsigned i = 0; i < 256 / 16; i++)
        // clang-format off
        chess_prepare_for_pipelining 
		chess_loop_range(4, 32)
        // clang-format on
        {
            data.insert(1, *in_it++);
            auto acc = aie::sliding_mul<4, 8>(coeffs, 0, data, 1);
            data_out.insert(0, acc.to_vector<int32>());
            acc = aie::sliding_mul<4, 8>(coeffs, 0, data, 5);
            data_out.insert(1, acc.to_vector<int32>());
            *out_it++ = data_out;

            data.insert(0, *in_it++);
            acc = aie::sliding_mul<4, 8>(coeffs, 0, data, 9);
            data_out.insert(0, acc.to_vector<int32>());
            acc = aie::sliding_mul<4, 8>(coeffs, 0, data, 13);
            data_out.insert(1, acc.to_vector<int32>());
            *out_it++ = data_out;
        }
}
