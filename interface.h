#ifndef INTERFACE_H
#define INTERFACE_H


#include "seedfinder.h"

SearchOptions getInputFromUser();
void printProgressBar(long long current, long long total);
void hideCursor();
void showCursor();

#endif