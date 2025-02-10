#pragma once

#include <vector>
#include <span>  

#include "pucch/core/matrix.hpp"

namespace pucch::core {

class Encoder final { 
public:
    
    explicit Encoder(const GeneratorMatrix& matrix);

    
    std::vector<int8_t> encode(const std::vector<int8_t>& data) const;

private:
    const GeneratorMatrix& matrix_; 
};

} 