#include "pucch/core/orchestrator.hpp"
#include <fstream>
#include <random>
#include <iostream>
#include <stdexcept>
#include <omp.h>

namespace pucch::core {

bool Orchestrator::SimulationConfig::validate() const {
    if (snr_values.empty()) {
        return false;
    }
    if (num_frames_per_snr == 0) {
        return false;
    }
    if (rate < 2 || rate > GeneratorMatrix::MAX_COLS) {
        return false;
    }
    return true;
}

Orchestrator::Orchestrator(const SimulationConfig& config) : config_(config) {
    if (!config_.validate()) {
        throw std::invalid_argument("Invalid simulation configuration.");
    }

    matrix_ = std::make_unique<GeneratorMatrix>(config_.rate);
    encoder_ = std::make_unique<Encoder>(*matrix_);  
    decoder_ = std::make_unique<Decoder>(*matrix_);
    channel_ = std::make_unique<AWGNChannel>(0.0); 
    statistics_ = std::make_unique<utils::Statistics>();
}

Orchestrator::SimulationResults Orchestrator::run_simulation() {
    SimulationResults results;
    results.snr_values = config_.snr_values;
    results.ber_values.reserve(config_.snr_values.size());

    if (config_.measure_time) {
        results.decoding_times.reserve(config_.snr_values.size());
    }

    for (double snr : config_.snr_values) {
        channel_->set_snr(snr);
        statistics_->reset();
        double total_decoding_time = 0.0;
        
#pragma omp parallel for reduction(+:total_decoding_time) 
        for (size_t frame = 0; frame < config_.num_frames_per_snr; ++frame) {
            
            std::vector<int8_t> data = generate_random_data();
            std::vector<int8_t> encoded_data = encoder_->encode(data);
            std::vector<double> received_signal = channel_->transmit(encoded_data);
            
            if (config_.measure_time) {
                auto start_time = std::chrono::high_resolution_clock::now();
                decoder_->decode(received_signal);
                auto end_time = std::chrono::high_resolution_clock::now();
                total_decoding_time += std::chrono::duration<double, std::milli>(end_time - start_time).count();
            } else {
                decoder_->decode(received_signal);
            }
            #pragma omp critical
            {
               statistics_->add_result(data, decoder_->decode(received_signal));
            }

        }

        results.ber_values.push_back(statistics_->get_ber());

        if (config_.measure_time) {
            results.decoding_times.push_back(total_decoding_time / static_cast<double>(config_.num_frames_per_snr));
        }
    }
    return results;
}

void Orchestrator::save_results(const SimulationResults& results, const std::string& file_prefix) {
     
    std::ofstream ber_file(file_prefix + "_ber.bin", std::ios::binary);
    if (!ber_file.is_open()) {
        throw std::runtime_error("Could not open BER file for writing.");
    }
    ber_file.write(reinterpret_cast<const char*>(results.ber_values.data()),
                    results.ber_values.size() * sizeof(double));
    ber_file.close();

    if (!results.decoding_times.empty()) {
        std::ofstream time_file(file_prefix + "_time.bin", std::ios::binary);
        if (!time_file.is_open()) {
            throw std::runtime_error("Could not open time file for writing.");
        }
        time_file.write(reinterpret_cast<const char*>(results.decoding_times.data()),
                        results.decoding_times.size() * sizeof(double));
        time_file.close();
    }

    std::cout << "Results saved to: " << file_prefix << "_ber.bin"
              << (!results.decoding_times.empty() ? " and " + file_prefix + "_time.bin" : "")
              << std::endl;
}

std::vector<int8_t> Orchestrator::generate_random_data() const {
    
    static std::mt19937 generator(std::random_device{}()); 
    static std::uniform_int_distribution<int8_t> distribution(0, 1); 

    std::vector<int8_t> data(config_.rate);
    for (int8_t& bit : data) {
        bit = distribution(generator);
    }
    return data;
}

double Orchestrator::measure_decoding_time(const std::vector<double>& received_signal) const {
    auto start_time = std::chrono::high_resolution_clock::now();  
    decoder_->decode(received_signal); 
    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

} 