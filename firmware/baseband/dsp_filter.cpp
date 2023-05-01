#include "dsp_filter.hpp"
#include "dsp_window.hpp"


namespace gr {
namespace filter {

double max_attenuation(gr::fft::dsp_window::win_type type, double param)
{
    switch (type) {
    case (gr::fft::dsp_window::win_type::WIN_HAMMING):
        return 53;
    case (gr::fft::dsp_window::win_type::WIN_HANN):
        return 44;
    case (gr::fft::dsp_window::win_type::WIN_BLACKMAN):
        return 74;
    case (gr::fft::dsp_window::win_type::WIN_RECTANGULAR):
        return 21;
    case (gr::fft::dsp_window::win_type::WIN_KAISER):
        // linear approximation
        return (param / 0.1102 + 8.7);
    case (gr::fft::dsp_window::win_type::WIN_BLACKMAN_hARRIS):
        return 92;
    case (gr::fft::dsp_window::win_type::WIN_BARTLETT):
        return 27;
    case (gr::fft::dsp_window::win_type::WIN_FLATTOP):
        return 93;
    case gr::fft::dsp_window::win_type::WIN_NUTTALL:
        return 114;
    case gr::fft::dsp_window::win_type::WIN_NUTTALL_CFD:
        return 112;
    case gr::fft::dsp_window::win_type::WIN_WELCH:
        return 31;
    case gr::fft::dsp_window::win_type::WIN_PARZEN:
        return 56;
    case gr::fft::dsp_window::win_type::WIN_EXPONENTIAL:
        // varies slightly depending on the decay factor, but this is a safe return value
        return 26;
    case gr::fft::dsp_window::win_type::WIN_RIEMANN:
        return 39;
    case gr::fft::dsp_window::win_type::WIN_GAUSSIAN:
        // value not meaningful for gaussian windows, but return something reasonable
        return 100;
    case gr::fft::dsp_window::win_type::WIN_TUKEY:
        // low end is a rectangular window, attenuation exponentially approaches Hann
        // piecewise linear estimate, determined empirically via curve fitting, median
        // error is less than 0.5dB and maximum error is 2.5dB; the returned value will
        // never be less than expected attenuation to ensure that window designed filters
        // are never below expected quality.
        if (param > 0.9)
            return ((param - 0.9) * 135 + 30.5);
        else if (param > 0.7)
            return ((param - 0.6) * 20 + 24);
        return (param * 5 + 21);
    default:
        return 100;
    }
}

int compute_ntaps(double sampling_freq,
                          double transition_width,
                          gr::fft::dsp_window::win_type window_type,
                          double param)
{
    double a = max_attenuation(window_type, param);
    int ntaps = (int)(a * sampling_freq / (22.0 * transition_width));
    if ((ntaps & 1) == 0) // if even...
        ntaps++;          // ...make odd

    return ntaps;
}

std::vector<float> low_pass(double gain,
                               double sampling_freq,
                               double cutoff_freq,      // Hz center of transition band
                               double transition_width, // Hz width of transition band
                               gr::fft::dsp_window::win_type window_type,
                               double param) // used with Kaiser, Exp., Gaussian, Tukey
{

    int ntaps = compute_ntaps(sampling_freq, transition_width, window_type, param);
    // construct the truncated ideal impulse response
    // [sin(x)/x for the low pass case]

    std::vector<float> taps(ntaps);
    std::vector<float> w = gr::fft::dsp_window::build(window_type, ntaps, param, false);//window(window_type, ntaps, param);

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

    gain /= fmax; // normalize

    for (int i = 0; i < ntaps; i++)
        taps[i] *= gain;

    return taps;
}
}
}