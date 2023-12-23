
#include "time.h"
#include <chrono>
std::chrono::high_resolution_clock::time_point getCurrentTime(){
    return std::chrono::high_resolution_clock::now();
}

std::chrono::milliseconds calculateDurationMili(const std::chrono::high_resolution_clock::time_point &start,
                                            const std::chrono::high_resolution_clock::time_point &end){
    return std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
}

std::chrono::microseconds calculateDurationMicro(const std::chrono::high_resolution_clock::time_point &start,
                                            const std::chrono::high_resolution_clock::time_point &end){
    return std::chrono::duration_cast<std::chrono::microseconds>(end-start);
}
