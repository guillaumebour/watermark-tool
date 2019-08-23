#ifndef OPTIONS_H
#define OPTIONS_H

#include <cxxopts.hpp>
#include "log.h"
#include "../base/watermark.h"

namespace myoptions {
    void parseArgs(int argc, char ** argv);
}

#endif