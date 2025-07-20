#include "adf.h"
#include "graph.h"

using namespace adf;

mygraph gr;

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
const int block_size = n_iter * 256 * sizeof(int32);


#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char** argv) {
    gr.init();

    int32* din = (int32*)GMIO::malloc(block_size);
    int32* dout = (int32*)GMIO::malloc(block_size);
    int32* dref = (int32*)malloc(block_size);

    std::cout << "GMIO::malloc completed" << std::endl;

    int n_error = 0;
    int32 coeffs[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    for (int i = 0; i < n_iter * 256; i++)
        din[i] = i;

    gr.gmio_in.gm2aie_nb(din, block_size);
    gr.run(n_iter);
    gr.gmio_out.aie2gm_nb(dout, block_size);
    gr.gmio_out.wait();

    ref_func(din, dref, coeffs, n_iter * 256);

    for (int i = 0; i < n_iter * 256; i++) {
        if (dout[i] != dref[i]) {
            std::cout << "ERROR: dout[" << i << "]=" << dout[i] << ", gold=" << dref[i] << std::endl;
            n_error++;
        }
    }

    std::cout << "GMIO transactions completed." << std::endl;

    GMIO::free(din);
    GMIO::free(dout);
    free(dref);
    gr.end();

    if (!n_error)
        std::cout << "PASSED!" << std::endl;
    else
        std::cout << "FAILED with " << n_error << " errors!" << std::endl;

    return n_error;
}
#endif
