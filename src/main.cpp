#include "utils/options.h"
#include "base/watermark.h"

int main(int argc, char** argv) {    
    myoptions::parseArgs(argc, argv);
    watermark::processSingleImage(watermark::getInputImage());
    return 0;
}