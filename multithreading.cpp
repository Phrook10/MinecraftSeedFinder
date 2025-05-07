#include "multithreading.h"

ParallelTasks::ParallelTasks(int threadCount) : threadCount(threadCount), shouldStop(false) {}

ParallelTasks::~ParallelTasks() {
    shouldStop = true;
    waitForCompletion();
}

void ParallelTasks::waitForCompletion() {
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
}