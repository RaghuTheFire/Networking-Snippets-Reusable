#include <iostream>
#include <vector>
#include <cmath>

// Simple anomaly detection based on deviation from mean
bool detect_anomaly(const std::vector<float>& network_data, float threshold) 
{
    float sum = 0, sum_sq = 0;
    for (float val : network_data) 
    {
        sum += val;
        sum_sq += val * val;
    }

    float mean = sum / network_data.size();
    float variance = (sum_sq / network_data.size()) - (mean * mean);
    float std_dev = std::sqrt(variance);

    for (float val : network_data) 
    {
        if (std::abs(val - mean) > threshold * std_dev) 
        {
            return true; // Anomaly detected
        }
    }
    return false;
}

int main() 
{
    // Simulated network packet sizes (in KB)
    std::vector<float> packet_sizes = {20.0, 21.0, 22.5, 18.0, 500.0, 19.5};

    if (detect_anomaly(packet_sizes, 3.0)) 
    {
        std::cout << "⚠️ Anomaly Detected in Network Traffic!\n";
    } 
    else 
    {
        std::cout << "✅ Network Traffic is Normal.\n";
    }

    return 0;
}
