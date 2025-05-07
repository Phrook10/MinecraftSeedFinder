#ifndef MULTITHREADING_H
#define MULTITHREADING_H

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

// Helper class for parallel task execution
class ParallelTasks {
public:
    ParallelTasks(int threadCount);
    ~ParallelTasks();
    
    // Execute a function in parallel across a range of values
    // func should accept (startValue, endValue, threadIndex)
    template<typename T, typename Func>
    void executeRange(T startValue, T endValue, Func func);
    
    // Wait for all tasks to complete
    void waitForCompletion();
    
private:
    int threadCount;
    std::vector<std::thread> threads;
    std::atomic<bool> shouldStop;
};

// Template implementation
template<typename T, typename Func>
void ParallelTasks::executeRange(T startValue, T endValue, Func func) {
    // Calculate values per thread
    T totalRange = endValue - startValue;
    T valuesPerThread = totalRange / threadCount;
    
    // Start threads with appropriate ranges
    for (int i = 0; i < threadCount; i++) {
        T threadStartValue = startValue + (i * valuesPerThread);
        T threadEndValue;
        
        if (i == threadCount - 1) {
            threadEndValue = endValue;
        } else {
            threadEndValue = threadStartValue + valuesPerThread;
        }
        
        threads.push_back(std::thread(func, threadStartValue, threadEndValue, i));
    }
}

#endif