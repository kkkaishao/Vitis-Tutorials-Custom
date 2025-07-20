#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "adf.h"
#include "kernel.h"

class rtpgraph : public adf::graph {
private:
    adf::kernel k;

public:
    adf::input_plio plio_in;
    adf::output_plio plio_out;
    adf::port<adf::direction::in> value;

    rtpgraph() {
        k = adf::kernel::create(vect_add<256>);
        plio_in = adf::input_plio::create("din0", adf::plio_32_bits, "data/input0.csv");
        plio_out = adf::output_plio::create("dout0", adf::plio_32_bits, "data/output0.txt");
        adf::connect<adf::parameter>(value, adf::sync(k.in[1]));

        adf::connect<>(plio_in.out[0], k.in[0]);
        adf::connect<>(k.out[0], plio_out.in[0]);
        adf::source(k) = "vect_add.cc";
        adf::runtime<adf::ratio>(k) = 0.9;
	}
};

#endif // __GRAPH_H__