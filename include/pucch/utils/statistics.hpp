#pragma once

#include <vector>
#include <cstdint>  

namespace pucch::utils {

class Statistics final {
public:
    Statistics();
    
    void add_result(const std::vector<int8_t>& original, const std::vector<int8_t>& decoded);
    double get_ber() const;
    void reset();

private:
    size_t total_bits_{0};   
    size_t error_bits_{0};   
};

} 