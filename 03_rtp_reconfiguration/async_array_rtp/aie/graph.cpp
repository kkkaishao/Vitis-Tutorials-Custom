#include "graph.h"

using namespace adf;

rtpgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char** argv) {
    int value[256];
    for (int i = 0; i < 256; i++)
        value[i] = i;
    gr.init();
    gr.run(4);
    gr.update(gr.value, value, 256);
    gr.end();
    return 0;
}
#endif