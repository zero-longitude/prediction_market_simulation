#include "forecaster.h"
#include "scoring_rule.h"
// angled brackets tells compiler "look in the cpp standard library"
#include <vector> // dynamic, resizable vectors
#include <memory> // smart pointers (std::unique_ptr)
#include <random>
#include <iostream> 


int main() {
    // real underlying probability
    const double theta = .3;

    // random number generation
    std::mt19937 rng(42);
    std::bernoulli_distribution coin(theta);

    // scoring rules 
    BrierScore brier;
    LogScore log_score;

    // dynamic array of forecasters
    // std::vector<type of element, allocating type> is just a dynamic array 
    std::vector<std::unique_ptr<Forecaster>> forecasters; // smart pointer ⟹ when forecasters vector is destroyed, so will the object in memory
    forecasters.push_back(std::make_unique<StaticForecaster>("Static(0.5)", 0.5));
    forecasters.push_back(std::make_unique<BayesianForecaster>("Bayes(1, 1)", 1, 1));
    forecasters.push_back(std::make_unique<MLEForecaster>("MLE"));
}