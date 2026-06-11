#include "calibration_tracker.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>



CalibrationTracker::CalibrationTracker(int num_bins)
    : num_bins_(num_bins), 
    bin_forecast_sum_(num_bins, 0.0), 
    bin_outcome_sum_(num_bins, 0.0), 
    bin_count_(num_bins, 0), 
    total_(0),
    outcome_sum_(0.0) {
        if (num_bins <= 0)
            throw std::invalid_argument("num_bins must be strictly positive");
    }

int CalibrationTracker::bin_index(double p) const {
    // clamp to [0, num_bins - 1] so p = 1.0 doesn't overflow
    return std::min((int)(p * num_bins_), num_bins_ - 1);
}


void CalibrationTracker::record(double forecast, int outcome) {
    int b = std::min((int)(forecast * num_bins_), num_bins_ - 1);
    bin_forecast_sum_[b] += forecast;
    bin_outcome_sum_[b]  += outcome;
    bin_count_[b]        += 1;
    total_               += 1;
    outcome_sum_         += outcome;
}

std::vector<double> CalibrationTracker::bin_mean_forecast() const {
    std::vector<double> result(num_bins_, 0.0);
    for (int b = 0; b < num_bins_; b++) {
        if (bin_count_[b] > 0)
            result[b] = bin_forecast_sum_[b] / bin_count_[b];
        else
            // empty bin: use bin midpoint as placeholder
            result[b] = (b + 0.5) / num_bins_;
    }
    return result;
}

std::vector<double> CalibrationTracker::bin_observed_freq() const {
    std::vector<double> result(num_bins_, 0.0);
    for (int b = 0; b < num_bins_; b++) {
        if (bin_count_[b] > 0)
            result[b] = bin_outcome_sum_[b] / bin_count_[b];
        // empty bin stays 0.0 — Python should ignore bins with count == 0
    }
    return result;
}

std::vector<int> CalibrationTracker::bin_counts() const {
    return bin_count_;
}

double CalibrationTracker::base_rate() const {
    if (total_ == 0) return 0.0;
    return outcome_sum_ / total_;
}

int CalibrationTracker::total() const {
    return total_;
}

// Reliability = (1/N) * sum_b [ n_b * (p_bar_b - o_bar_b)^2 ]
// Measures how far stated probabilities are from observed frequencies.
// Lower is better. Zero = perfectly calibrated.
double CalibrationTracker::reliability() const {
    if (total_ == 0) return 0.0;
    double sum = 0.0;
    for (int b = 0; b < num_bins_; b++) {
        if (bin_count_[b] == 0) continue;
        double p_bar = bin_forecast_sum_[b] / bin_count_[b];
        double o_bar = bin_outcome_sum_[b]  / bin_count_[b];
        double diff  = p_bar - o_bar;
        sum += bin_count_[b] * diff * diff;
    }
    return sum / total_;
}

// Resolution = (1/N) * sum_b [ n_b * (o_bar_b - o_bar)^2 ]
// Measures how much bin frequencies vary from the base rate.
// Higher is better. Zero = forecaster never deviates from base rate.
double CalibrationTracker::resolution() const {
    if (total_ == 0) return 0.0;
    double o_bar = base_rate();
    double sum   = 0.0;
    for (int b = 0; b < num_bins_; b++) {
        if (bin_count_[b] == 0) continue;
        double o_bar_b = bin_outcome_sum_[b] / bin_count_[b];
        double diff    = o_bar_b - o_bar;
        sum += bin_count_[b] * diff * diff;
    }
    return sum / total_;
}

// Uncertainty = o_bar * (1 - o_bar)
// Variance of the outcomes — fixed given the event set.
// Independent of the forecaster, measures intrinsic difficulty.
double CalibrationTracker::uncertainty() const {
    double o_bar = base_rate();
    return o_bar * (1.0 - o_bar);
}