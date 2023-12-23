#pragma once

#include <chrono>

// Function to get the current time point
std::chrono::high_resolution_clock::time_point getCurrentTime() ;

// Function to calculate the duration between two time points
std::chrono::milliseconds calculateDurationMili(const std::chrono::high_resolution_clock::time_point& start,
                                            const std::chrono::high_resolution_clock::time_point& end) ;

// Function to calculate time in micro seconds
std::chrono::microseconds calculateDurationMicro(const std::chrono::high_resolution_clock::time_point& start,
                                            const std::chrono::high_resolution_clock::time_point& end) ;
