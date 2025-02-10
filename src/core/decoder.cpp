

#include "pucch/core/decoder.hpp"
#include <limits> 
#include <cmath>  

namespace pucch::core {

Decoder::Decoder(const GeneratorMatrix& matrix) : matrix_(matrix) {
    precompute_codewords(); 
}

std::vector<int8_t> Decoder::decode(const std::vector<double>& llr_values) const {
    if (llr_values.size() != matrix_.rows()) {
      throw std::invalid_argument("LLR values size does not match generator matrix rows.");
    }
    const size_t k = matrix_.rate(); 
    std::vector<int8_t> best_sequence(k);
    double best_metric = std::numeric_limits<double>::lowest(); 

    
    for (size_t i = 0; i < precomputed_codewords_.size(); ++i) {

        
        double metric = compute_metric(precomputed_codewords_[i], llr_values);

        
        if (metric > best_metric) {
            best_metric = metric;
            
            std::vector<int8_t> candidate_sequence(k);
            for(size_t j = 0; j<k; ++j){
                candidate_sequence[j] = (i >> j) & 1;
            }
            best_sequence = candidate_sequence;
        }
    }

    return best_sequence;
}












double Decoder::compute_metric(const std::vector<int8_t>& encoded,
                             const std::vector<double>& received) const {
    double metric = 0.0;
    for (size_t i = 0; i < encoded.size(); ++i) {
        
        double bpsk_symbol = encoded[i] == 0 ? -1.0 : 1.0;
        metric += received[i] * bpsk_symbol; 
    }
    return metric;
}
void Decoder::precompute_codewords()
{
    const size_t k = matrix_.rate();
    const size_t num_sequences = 1ULL << k;
    precomputed_codewords_.resize(num_sequences);

     for (size_t i = 0; i < num_sequences; ++i) {
        std::vector<int8_t> candidate_sequence(k);
        
        for (size_t j = 0; j < k; ++j) {
            candidate_sequence[j] = (i >> j) & 1; 
        }
        
        std::vector<int8_t> encoded(matrix_.rows(), 0);
        for (size_t row = 0; row < matrix_.rows(); ++row) {
            for (size_t col = 0; col < matrix_.rate(); ++col) {
                encoded[row] ^= static_cast<int8_t>(matrix_.at(row, col) & candidate_sequence[col]);
            }
        }
        precomputed_codewords_[i] = encoded;
    }
}

} 