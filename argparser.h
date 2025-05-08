#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include "seedfinder.h"
#include "config.h"
#include <string>

int parseThreadCount(int argc, char* argv[]);
MCVersion parseVersionArg(int argc, char* argv[]);
bool parseJSONFileArg(int argc, char* argv[], SearchOptions& options);

#endif