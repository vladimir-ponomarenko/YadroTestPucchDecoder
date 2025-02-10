#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <span>

namespace pucch::core {  

class GeneratorMatrix final { 
public:
    static constexpr size_t ROWS = 20;
    static constexpr size_t MAX_COLS = 13;  

    constexpr GeneratorMatrix() = default;
    
    explicit GeneratorMatrix(size_t rate);
    
    int8_t at(size_t row, size_t col) const noexcept(false); 
    
    size_t rows() const noexcept { return ROWS; } 
    size_t cols() const noexcept { return rate_; }
    size_t rate() const noexcept { return rate_; }

private:
    static constexpr std::array<std::array<int8_t, MAX_COLS>, ROWS> GENERATOR_MATRIX = {{
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
        {1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1},
        {1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1},
        {1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1},
        {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},
        {1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1},
        {1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1},
        {1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1},
        {1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
        {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0}
    }};
    size_t rate_{0};  
};

} 