#include "graph.h"

using namespace adf;

rtpgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)
int main(int argc, char** argv) {
    gr.init();
    gr.run(4);
    gr.update(gr.value, 10);
    gr.end();
    return 0;
}
#endif