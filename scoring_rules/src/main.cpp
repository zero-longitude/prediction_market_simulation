#include "config.h"
#include "forecaster.h"
#include "scoring_rule.h"
#include "calibration_tracker.h"
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <fstream>

/* 
We have a Bernoulli RV with underlying parameter θ. Three forecasters——static, Bayesian, and
MLE——make predictions over n trials. 

They are scored accoring to Brier and negated log scores. 
*/

int main() {
    // generate random coin
    std::mt19937 rng(RNG_SEED);
    std::bernoulli_distribution coin(THETA);

    // scoring rules 
    BrierScore brier;
    LogScore log_score;

    std::vector<std::unique_ptr<Forecaster>> forecasters;
    forecasters.push_back(std::make_unique<StaticForecaster>(STATIC_BELIEF));
    forecasters.push_back(std::make_unique<BayesianForecaster>(BAYES_ALPHA, BAYES_BETA));
    forecasters.push_back(std::make_unique<BayesianForecaster>(9.0, 1.0));
    forecasters.push_back(std::make_unique<MLEForecaster>());

    const int n = N_TRIALS;

    // create a vector of doubles called cumulative_brier, set size to size of forecasters, initialize all values as 0
    std::vector<double> cumulative_brier(forecasters.size(), 0.0);
    std::vector<double> cumulative_log(forecasters.size(), 0.0);

    std::vector<CalibrationTracker> trackers(forecasters.size());

    // create an output stream
    std::ofstream csv("results.csv"); 

    // header row
    csv << "t,outcome";
    for (int i = 0; i < (int)forecasters.size(); i++) {
        std::string n = forecasters[i]->name();
        csv << ",\"" << n << "_belief\""
        << ",\"" << n << "_brier\""
        << ",\"" << n << "_log\"";
    }
    csv << "\n";

    for (int t = 0; t < n; t++) {
        int outcome = coin(rng);
        csv << t << "," << outcome;

        for (int i = 0; i < forecasters.size(); i++) {
            double p = forecasters[i]->belief();
            double b = brier.score(p, outcome);
            cumulative_brier[i] += b;
            double l = log_score.score(p, outcome);
            cumulative_log[i] += l; 

            csv << "," << p << "," << b << "," << l;

            trackers[i].record(p, outcome);
            forecasters[i]->update(outcome);
        }
        csv << "\n";
    }
    csv.close();

    // write calibration data: one row per (forecaster, bin)
    std::ofstream cal("calibration.csv");
    cal << "forecaster,bin,mean_forecast,observed_freq,count\n";
    for (int i = 0; i < (int)forecasters.size(); i++) {
        auto mf = trackers[i].bin_mean_forecast();
        auto of = trackers[i].bin_observed_freq();
        auto bc = trackers[i].bin_counts();
        for (int b = 0; b < (int)mf.size(); b++) {
            cal << "\"" << forecasters[i]->name() << "\""
                << "," << b
                << "," << mf[b]
                << "," << of[b]
                << "," << bc[b]
                << "\n";
        }
    }
    cal.close();

    for (int i = 0; i < (int)forecasters.size(); i++) {
        std::cout << "n = " << n << "\n"
                  << forecasters[i]->name()
                  << "  total Brier:       " << cumulative_brier[i] << "\n"
                  << forecasters[i]->name()
                  << "  total log score:   " << cumulative_log[i] << "\n"
                  << forecasters[i]->name()
                  << "  reliability:       " << trackers[i].reliability() << "\n"
                  << forecasters[i]->name()
                  << "  resolution:        " << trackers[i].resolution() << "\n"
                  << forecasters[i]->name()
                  << "  uncertainty:       " << trackers[i].uncertainty() << "\n\n";
    }
}