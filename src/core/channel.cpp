#include "pucch/core/channel.hpp"
#include <cmath>      
#include <stdexcept>

namespace pucch::core {

AWGNChannel::AWGNChannel(double snr_db)
    : rng_(std::random_device{}()), snr_db_(snr_db) {
    update_noise_variance();
}

void AWGNChannel::set_snr(double snr_db) {
    if (snr_db < -50.0 || snr_db > 50.0){
        throw std::invalid_argument("SNR value in dB is out of reasonable range.");
    }
    snr_db_ = snr_db;
    update_noise_variance();
}

std::vector<double> AWGNChannel::transmit(const std::vector<int8_t>& signal) const {
    std::vector<double> noisy_signal(signal.size());
    
    std::normal_distribution<double> noise_distribution(0.0, std::sqrt(noise_variance_));

    for (size_t i = 0; i < signal.size(); ++i) {
        
        double bpsk_symbol = signal[i] == 0 ? -1.0 : 1.0;
        
        noisy_signal[i] = bpsk_symbol + noise_distribution(rng_);
    }

    return noisy_signal;
}

void AWGNChannel::update_noise_variance() {
    
    double snr_linear = std::pow(10.0, snr_db_ / 10.0);
    
    noise_variance_ = 1.0 / (2.0 * snr_linear);
}

} 