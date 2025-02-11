#pragma once

#include <vector>
#include <random>
#include <cstdint>

namespace pucch::core {

class AWGNChannel final { 
public:
    
    explicit AWGNChannel(double snr_db);
    void set_snr(double snr_db);
    std::vector<double> transmit(const std::vector<int8_t>& signal) const;
    double get_noise_variance() const noexcept { return noise_variance_; }

private:
    mutable std::mt19937 rng_;        
    double snr_db_{0.0};           
    double noise_variance_{1.0}; 
    void update_noise_variance();
};

} 