#include "hls_stream.h"
#include "ap_axi_sdata.h"

extern "C" {

void datagen(hls::stream<ap_axis<32, 0, 0, 0>>& s, int size, int init_val = 0) {
#pragma HLS INTERFACE axis port = s
#pragma HLS INTERFACE s_axilite port = size bundle = control
#pragma HLS INTERFACE s_axilite port = init_val bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control
#pragma HLS INTERFACE ap_none port = size
#pragma HLS INTERFACE ap_none port = init_val
#pragma HLS INTERFACE ap_ctrl_hs port = return
    for (int i = 0; i < size; i++) {
        ap_axis<32, 0, 0, 0> x;
        x.data = init_val + i;
        x.keep = -1; // All bytes are valid
        x.last = 0;
        s.write(x);
	}
}
}