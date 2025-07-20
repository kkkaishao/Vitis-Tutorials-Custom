#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "adf.h"
#include "kernel.h"

class mygraph : public adf::graph {
private:
	adf::kernel k_m;

public:
	adf::input_gmio gmio_in;
	adf::output_gmio gmio_out;

	mygraph() {
		k_m = adf::kernel::create(vectorized_weighted_sum_with_margin);
		gmio_in = adf::input_gmio::create("gmioIn", 64, 1000);
		gmio_out = adf::output_gmio::create("gmioOut", 64, 1000);

		adf::connect<>(gmio_in.out[0], k_m.in[0]);
		adf::connect<>(k_m.out[0], gmio_out.in[0]);
		adf::source(k_m) = "weighted_sum.cc";
		adf::runtime<adf::ratio>(k_m) = 0.9;
	}
};

#endif // __GRAPH_H__