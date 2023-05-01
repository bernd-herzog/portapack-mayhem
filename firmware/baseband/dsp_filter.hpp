#pragma once

const double PI = 3.14159265358979323846;
#include <cmath>
#include <numeric>
#include <algorithm>
#include <vector>

#include "dsp_window.hpp"

namespace gr {
namespace filter {

std::vector<float> low_pass(double gain,
                               double sampling_freq,
                               double cutoff_freq,      // Hz center of transition band
                               double transition_width, // Hz width of transition band
                               gr::fft::dsp_window::win_type window_type,
                               double param);

}
}