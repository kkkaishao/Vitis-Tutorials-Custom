#include "adf.h"
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"
#include "xrt/xrt_aie.h"

using namespace adf;

void ref_func(int32* din, int32* dout, int32 c[8], int size) {
    static int32 tmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 7; j++) {
            tmp[j] = tmp[j + 1];
        }
        tmp[7] = din[i];
        int32 acc = 0;
        for (int j = 0; j < 8; j++) {
            acc += tmp[j] * c[j];
        }
        dout[i] = acc;
    }
}

const int n_iter = 4;
const int block_size = n_iter * 256 * sizeof(int);

int main(int argc, char** argv) {
    // Create XRT device handle for ADF API
    char* xclbinFilename = argv[1];
    // Open xclbin
    auto device = xrt::device(0); // device index=0
    auto uuid = device.load_xclbin(xclbinFilename);

    auto din_buffer = xrt::aie::bo(
        device, block_size, xrt::bo::flags::normal, /*memory group*/ 0
    ); // Only non-cacheable buffer is supported
    int* din = din_buffer.map<int*>();
    auto dout_buffer = xrt::aie::bo(
        device, block_size, xrt::bo::flags::normal, /*memory group*/ 0
    ); // Only non-cacheable buffer is supported
    int* dout = dout_buffer.map<int*>();
    int32* dref = (int32*)malloc(block_size);
    std::cout << "GMIO::malloc completed" << std::endl;

    int n_err = 0;
    int32 coeffs[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    for (int i = 0; i < n_iter * 256; i++)
        din[i] = i;

    auto ghdl = xrt::graph(device, uuid, "gr");
    din_buffer.async("gr.gmio_in", XCL_BO_SYNC_BO_GMIO_TO_AIE, block_size, /*offset*/ 0);
    ghdl.run(n_iter);
    auto dout_buffer_run = dout_buffer.async("gr.gmio_out", XCL_BO_SYNC_BO_AIE_TO_GMIO, block_size, /*offset*/ 0);
    // PS can do other tasks here when data is transferring
    dout_buffer_run.wait(); // Wait for gmioOut to complete

    ref_func(din, dref, coeffs, n_iter * 256);
    for (int i = 0; i < n_iter * 256; i++) {
        if (dout[i] != dref[i]) {
            std::cout << "ERROR: dout[" << i << "]=" << dout[i] << ", gold=" << dref[i] << std::endl;
            n_err++;
        }
    }

    std::cout << "GMIO transactions finished" << std::endl;

    ghdl.end();
    if (n_err == 0)
        std::cout << "TEST PASSED!" << std::endl;
    else
        std::cout << "ERROR!" << std::endl;

    return n_err;
};
