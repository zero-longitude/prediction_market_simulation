#include "forecaster.h"
#include "scoring_rule.h"
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
    // real underlying probability
    const double THETA = .5;

    // random number generation
    std::mt19937 rng(42);
    std::bernoulli_distribution coin(THETA);

    // scoring rules 
    BrierScore brier;
    LogScore log_score;

    std::vector<std::unique_ptr<Forecaster>> forecasters; 
    forecasters.push_back(std::make_unique<StaticForecaster>("Static(0.5)", .5));
    forecasters.push_back(std::make_unique<BayesianForecaster>("Bayes(1,1)", 1, 1));
    forecasters.push_back(std::make_unique<MLEForecaster>("MLE"));

    // simulation
    int n = 1000;

    // create a vector of doubles called cumulative_brier, set size to size of forecasters, initialize all values as 0
    std::vector<double> cumulative_brier(forecasters.size(), 0.0);
    std::vector<double> cumulative_log(forecasters.size(), 0.0);

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


            forecasters[i]->update(outcome);
        }
        csv << "\n";
    }
    csv.close();

    for (int i = 0; i < forecasters.size(); i++) {
        std::cout << "n = "
                  << n << "\n"
                  << forecasters[i]->name()
                  << " total Brier: "
                  << cumulative_brier[i] << "\n"
                  << forecasters[i]->name()
                  << " total negated log score: "
                  << -cumulative_log[i] << "\n";
    }
}