#include "log.h"

const std::string ANSI_RESET = "\x1b[0m";
const std::string log_text[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
const std::string console_colour_t[] = {"\x1b[31m", "\x1b[33m", "\x1b[34m", "\x1b[35m"};

int verbosity_level  = logger::VERBOSITY_NORMAL;

void logger::set_verbosity_level(int level) {
    verbosity_level = level;
}

void logger::vlog(int type, std::string str, ...) {
    if(verbosity_level <= VERBOSITY_QUIET)
        return;
    if((type == LOG_DEBUG && verbosity_level >= VERBOSITY_DEBUG) || (type >= 0 && type < 3))
    {
        va_list argptr;
        va_start(argptr, str);
        fprintf(stderr, "%s%s%s%s", console_colour_t[type].c_str(), "[", log_text[type].c_str(), "]: ");
        vfprintf(stderr, str.c_str(), argptr);
        fprintf(stderr, "%s\n", ANSI_RESET.c_str());
        va_end(argptr);
    }
}
