#include "calc_elapsed_time.hpp"
#include <chrono>

double TimeManager::get_delta_time() {
    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = current_time - last_time;
    last_time = current_time;

    return elapsed.count();
}

double TimeManager::get_total_time() {
    static const auto start_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> total = current_time - start_time;
    return total.count();
}