#include "pucch/core/matrix.hpp"

namespace pucch::core {

GeneratorMatrix::GeneratorMatrix(size_t rate) : rate_(rate) {
    if (rate_ < 2 || rate_ > MAX_COLS) {
        throw std::invalid_argument("Rate must be between 2 and " + std::to_string(MAX_COLS));
    }
}

int8_t GeneratorMatrix::at(size_t row, size_t col) const {
    if (row >= ROWS || col >= rate_) {
        throw std::out_of_range("Matrix indices out of range.");
    }
    return GENERATOR_MATRIX[row][col];
}

}