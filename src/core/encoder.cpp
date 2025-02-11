#include "pucch/core/encoder.hpp"

namespace pucch::core {

Encoder::Encoder(const GeneratorMatrix& matrix) : matrix_(matrix) {}

std::vector<int8_t> Encoder::encode(const std::vector<int8_t>& data) const { 
    if (data.size() != matrix_.rate()) {
        throw std::invalid_argument("Input data size does not match the generator matrix rate.");
    }

    std::vector<int8_t> encoded_data(matrix_.rows(), 0);

    for (size_t i = 0; i < matrix_.rows(); ++i) {
        for (size_t j = 0; j < matrix_.rate(); ++j) {
            
            encoded_data[i] ^= static_cast<int8_t>(matrix_.at(i, j) & data[j]); 
        }
    }

    return encoded_data;
}

} 