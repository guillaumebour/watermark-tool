#ifndef LOG_H
#define LOG_H

#include <string>
#include <stdarg.h>
#include <stdlib.h>

namespace logger {

const int VERBOSITY_QUIET  = 0;
const int VERBOSITY_NORMAL = 1;
const int VERBOSITY_DEBUG  = 2;

const int LOG_ERROR   = 0;
const int LOG_WARNING = 1;
const int LOG_INFO    = 2;
const int LOG_DEBUG   = 3;

void vlog(int type, std::string str, ...);
void set_verbosity_level(int level);

}

#endif
