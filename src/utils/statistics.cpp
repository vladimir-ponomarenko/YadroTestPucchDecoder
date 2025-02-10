#include "pucch/utils/statistics.hpp"
#include <numeric>
#include <algorithm>
#include <stdexcept> 

namespace pucch::utils {

Statistics::Statistics() = default;

void Statistics::add_result(const std::vector<int8_t>& original, const std::vector<int8_t>& decoded) {
    if (original.size() != decoded.size()) {
        throw std::invalid_argument("Original and decoded bit vectors must have the same size.");
    }
    total_bits_ += original.size();

    error_bits_ += std::inner_product(
        original.begin(), original.end(),
        decoded.begin(),
        0ULL,
        std::plus<>(),
        std::not_equal_to<>()
    );
}

double Statistics::get_ber() const {
    return (total_bits_ > 0) ? static_cast<double>(error_bits_) / static_cast<double>(total_bits_) : 0.0; 
}

void Statistics::reset() {
    total_bits_ = 0;
    error_bits_ = 0;
}

} 