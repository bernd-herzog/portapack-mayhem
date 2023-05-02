#pragma once

const double PI = 3.14159265358979323846;
#include <cmath>
#include <numeric>
#include <algorithm>
#include <vector>

#include "dsp_window.hpp"

// namespace gr {
// namespace filter {

// template<gr::fft::win_type_t window_type>
// std::vector<float> low_pass(const double gain,
//                             const double sampling_freq,
//                             const double cutoff_freq,      // Hz center of transition band
//                             const double transition_width, // Hz width of transition band
//                             const double param);

// }
// }

namespace gr {
namespace filter {

template<gr::fft::win_type_t type>
constexpr double max_attenuation()
{
    switch (type) {
    case (gr::fft::win_type::WIN_HAMMING):
        return 53;
    case (gr::fft::win_type::WIN_HANN):
        return 44;
    case (gr::fft::win_type::WIN_BLACKMAN):
        return 74;
    case (gr::fft::win_type::WIN_RECTANGULAR):
        return 21;
    case (gr::fft::win_type::WIN_BLACKMAN_hARRIS):
        return 92;
    case (gr::fft::win_type::WIN_BARTLETT):
        return 27;
    case (gr::fft::win_type::WIN_FLATTOP):
        return 93;
    case gr::fft::win_type::WIN_NUTTALL:
        return 114;
    case gr::fft::win_type::WIN_NUTTALL_CFD:
        return 112;
    case gr::fft::win_type::WIN_WELCH:
        return 31;
    case gr::fft::win_type::WIN_PARZEN:
        return 56;
    case gr::fft::win_type::WIN_EXPONENTIAL:
        // varies slightly depending on the decay factor, but this is a safe return value
        return 26;
    case gr::fft::win_type::WIN_RIEMANN:
        return 39;
    case gr::fft::win_type::WIN_GAUSSIAN:
        // value not meaningful for gaussian windows, but return something reasonable
        return 100;
    default:
        return 100;
    }
}

template<gr::fft::win_type_t window_type, uint32_t sampling_freq, uint32_t transition_width>
constexpr int compute_ntaps() {
    constexpr double a = max_attenuation<window_type>();
    int ntaps = (int)(a * sampling_freq / (22.0 * transition_width));
    if ((ntaps & 1) == 0) // if even...
        ntaps++;          // ...make odd

    return ntaps;
}

template<gr::fft::win_type_t window_type, size_t sampling_freq, uint32_t transition_width>
std::vector<float> low_pass(const double gain, const double cutoff_freq)
{
    constexpr int ntaps = compute_ntaps<window_type, sampling_freq, transition_width>();
    // construct the truncated ideal impulse response
    // [sin(x)/x for the low pass case]

    std::vector<float> taps(ntaps);
    std::vector<float> w = gr::fft::dsp_window::build<window_type, ntaps>();

    int M = (ntaps - 1) / 2;
    double fwT0 = 2 * PI * cutoff_freq / sampling_freq;

    for (int n = -M; n <= M; n++) {
        if (n == 0)
            taps[n + M] = fwT0 / PI * w[n + M];
        else {
            // a little algebra gets this into the more familiar sin(x)/x form
            taps[n + M] = sin(n * fwT0) / (n * PI) * w[n + M];
        }
    }

    // find the factor to normalize the gain, fmax.
    // For low-pass, gain @ zero freq = 1.0

    double fmax = taps[0 + M];
    for (int n = 1; n <= M; n++)
        fmax += 2 * taps[n + M];

    auto gain_normalized = gain / fmax; // normalize

    for (int i = 0; i < ntaps; i++)
        taps[i] *= gain_normalized;

    return taps;
}
}
}