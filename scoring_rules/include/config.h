#pragma once

// Simulation parameters
constexpr int    N_TRIALS        = 1000;
constexpr int    RNG_SEED        = 42;
constexpr double THETA           = 0.5;

// StaticForecaster prior
constexpr double STATIC_BELIEF   = 0.5;

// BayesianForecaster Beta prior
constexpr double BAYES_ALPHA     = 1.0;
constexpr double BAYES_BETA      = 1.0;

// Calibration tracker
constexpr int NUM_BINS = 10; 
