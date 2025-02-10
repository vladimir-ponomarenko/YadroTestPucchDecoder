
#pragma once

#include <vector>
#include "pucch/core/matrix.hpp"

namespace pucch::core {

class Decoder final { 
public:
    explicit Decoder(const GeneratorMatrix& matrix);

    
    std::vector<int8_t> decode(const std::vector<double>& llr_values) const;

private:
    const GeneratorMatrix& matrix_;
    
    std::vector<std::vector<int8_t>> precomputed_codewords_;
    
    double compute_metric(const std::vector<int8_t>& encoded, const std::vector<double>& received) const;
    
    void precompute_codewords();
};

} 