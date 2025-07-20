#include <stdlib.h>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"

const int n_iter = 4;
const int n_elem_per_iter = 256;
const int output_size = n_iter * n_elem_per_iter * sizeof(int);

int run(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <xclbin file>" << std::endl;
        return EXIT_FAILURE;
    }

    int value[n_elem_per_iter];
    for (int i = 0; i < n_elem_per_iter; i++)
        value[i] = i;

    char* xclbinFilename = argv[1];

    auto device = xrt::device(0);
    auto uuid = device.load_xclbin(xclbinFilename);

    // s2mm & datagen kernels
    auto s2mm = xrt::kernel(device, uuid, "s2mm");
    auto datagen = xrt::kernel(device, uuid, "datagen");

    // output memory
    auto out_bo = xrt::bo(device, output_size, s2mm.group_id(0));
    auto host_out = out_bo.map<int*>();

    // kernel run
    auto s2mm_run = s2mm(out_bo, nullptr, n_elem_per_iter * n_iter);
    auto datagen_run = datagen(nullptr, n_elem_per_iter * n_iter, 0);

    auto ghdl = xrt::graph(device, uuid, "gr");
    ghdl.run(n_iter);
    ghdl.update("gr.k.in[1]", value);
    ghdl.end();

    s2mm_run.wait();
    out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    bool unmatch = 0;

    for (int i = 0; i < n_iter * n_elem_per_iter; i++) {
        if (host_out[i] != value[i % n_elem_per_iter] + i)
            unmatch = 1;
    }

    return unmatch;
}

int main(int argc, char** argv) {
    try {
        auto unmatch = run(argc, argv);
        std::cout << "Test " << (unmatch ? "failed" : "passed") << std::endl;
        return unmatch ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        std::cout << "Test failed due to exception." << std::endl;
        return EXIT_FAILURE;
    }
}
