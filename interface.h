#ifndef INTERFACE_H
#define INTERFACE_H


#include "seedfinder.h"

// user input related functions
SearchOptions getInputFromUser(SearchOptions &searchOptions);                                         // gets input from user and returns a SearchOptions vector

// Progress bar related functions
void printProgressBar(long long current, long long total);                                            // visual feedback, displays the progress of the search
void hideCursor();                                                                                    // hides the cursor to prevent flickering during progress bar updates
void showCursor();                                                                                    // unhides the cursor once the search is complete

#endif