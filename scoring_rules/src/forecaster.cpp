#include "forecaster.h"
#include <algorithm>

/* 
Three traders (forecasters)
    1) Static: [NAIVE] does not update belief_ (which is a protected class variable of Forecaster)
    2) Bayesian: totals 1's (alpha) and 0's (beta) and updates belief := alpha / (alpha + beta)
         ⟹ alpha and beta are priors 
    3) MLEForecaster: [NAIVE] a point estimator; computes ratio ones / total 
        ⟹ no priors
*/

Forecaster::Forecaster(std::string name, double initial_belief)
    : name_(name), belief_(initial_belief) {}

double Forecaster::belief() const { return belief_; }
std::string Forecaster::name() const { return name_; }

// static forecaster
StaticForecaster::StaticForecaster(std::string name, double belief)
    : Forecaster(name, belief) {}

void StaticForecaster::update(int outcome) {}


// Bayesian forecaster
    // α = number of 1's at time step t
    // β = number of 0's at time step t
    // bayesian forecaster ⟶ E[θ | α, β ] = α / (α + β)
BayesianForecaster::BayesianForecaster(std::string name, double alpha, double beta)
    : Forecaster(name, alpha / (alpha + beta)), 
    alpha_(alpha), beta_(beta) {}

void BayesianForecaster::update(int outcome) {
    if (outcome == 1) 
        alpha_ += 1.0;
    else 
        beta_ += 1.0;
    belief_ = alpha_ / (alpha_ + beta_);
} 


// MLE forecaster
MLEForecaster::MLEForecaster(std::string name)
    : Forecaster(name, 0.5), ones_(0), total_(0) {}

void MLEForecaster::update(int outcome) {
    total_ += 1;
    if (outcome == 1) 
        ones_ += 1;
    double raw = static_cast<double>(ones_) / total_;
    belief_ = std::clamp(raw, 1e-6, 1.0 - 1e-6);  // belief CANNOT BE ZERO (div by 0 err)
}


