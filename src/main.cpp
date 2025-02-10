#include <iostream>
#include <vector>
#include <iomanip>
#include "pucch/core/orchestrator.hpp"

int main() {
    try {
        pucch::core::Orchestrator::SimulationConfig config;
        config.snr_values = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
        config.num_frames_per_snr = 20000;   
        config.measure_time = true;       

        std::vector<size_t> rates = {2, 4, 6, 8, 11};

        for (size_t rate : rates) {
            config.rate = rate;
            std::cout << "Simulating PUCCH Format 2 with rate " << rate << "..." << std::endl;
            pucch::core::Orchestrator orchestrator(config);
            auto results = orchestrator.run_simulation();
             
            std::string file_prefix = "data/pucch_rate_" + std::to_string(rate);

            pucch::core::Orchestrator::save_results(results, file_prefix);
            
            std::cout << "SNR (dB)     BER            Decoding Time (ms)" << std::endl;
            std::cout << "---------------------------------------------" << std::endl;
            for (size_t i = 0; i < results.snr_values.size(); ++i) {
                std::cout << std::fixed << std::setprecision(2) << std::setw(8) << results.snr_values[i]
                          << "     " << std::scientific << std::setprecision(6) << std::setw(15) << results.ber_values[i];
                if (config.measure_time) {
                    std::cout << "     " << std::fixed << std::setprecision(4) << std::setw(18)
                              << results.decoding_times[i];
                }
                std::cout << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; 
        return 1; 
    }
    return 0;
}