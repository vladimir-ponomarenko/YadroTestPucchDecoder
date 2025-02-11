#pragma once

#include <vector>
#include <string>
#include <memory>   
#include <chrono>  

#include "pucch/core/encoder.hpp"
#include "pucch/core/decoder.hpp"
#include "pucch/core/channel.hpp"
#include "pucch/utils/statistics.hpp"

namespace pucch::core {

class Orchestrator final {
public:
    
    struct SimulationConfig {
        std::vector<double> snr_values;    
        size_t num_frames_per_snr = 1000;  
        size_t rate = 2;                   
        bool measure_time = false;          

        bool validate() const;
    };

    struct SimulationResults {
        std::vector<double> snr_values;
        std::vector<double> ber_values;
        std::vector<double> decoding_times; 
    };
  
    explicit Orchestrator(const SimulationConfig& config);
    SimulationResults run_simulation();
    static void save_results(const SimulationResults& results, const std::string& file_prefix);

private:
    SimulationConfig config_;
    std::unique_ptr<GeneratorMatrix> matrix_;
    std::unique_ptr<Encoder> encoder_;
    std::unique_ptr<Decoder> decoder_;
    std::unique_ptr<AWGNChannel> channel_;
    std::unique_ptr<utils::Statistics> statistics_;
    
    std::vector<int8_t> generate_random_data() const;
    
    double measure_decoding_time(const std::vector<double>& received_signal) const;
};

} 