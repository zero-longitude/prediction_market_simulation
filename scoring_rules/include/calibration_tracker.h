#pragma once
#include <vector>
#include "config.h"


/* 
A forecaster is *calibrated* if the stated probability is roughly the same as the observed frequency. 

For each bin b
    1) split [0, 1] up into 10 bins
    2) compute the mean forecast in that bin (p_b)
    3) compute the observed frequency (o_b)

Plot should be roughly X = Y: 
    X-axis = p_b
    Y-axis = o_b

If overconfident or underconfident, it will bow away from X = Y. `


The Brier score decomposes into: [avg squared err] - [avg weighted variance] + [uncertainty]
                                 [ reliability   ] - [     resolution      ] + [uncertainty]
*/
class CalibrationTracker {
    public:
        CalibrationTracker(int num_bins = NUM_BINS); // constructor
        void record(double forecast, int outcome);

        // per bin output
        std::vector<double> bin_mean_forecast() const; // const after ⟹ this won't change the calibrationTracker object
        std::vector<double> bin_observed_freq() const;
        std::vector<int> bin_counts() const;

        // decomposition 
        double reliability() const; 
        double resolution() const;
        double uncertainty() const; 

        // overall base rate o_bar
        double base_rate() const;

        int total() const;

    private:
        int num_bins_; 
        std::vector<double> bin_forecast_sum_;
        std::vector<double> bin_outcome_sum_;
        std::vector<int>    bin_count_;
        int total_;
        double outcome_sum_;

        // helper: which bin does forecast p fall into?
        int bin_index(double p) const;
        
};