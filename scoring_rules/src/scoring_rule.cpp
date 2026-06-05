#include "scoring_rule.h"
#include <cmath>
#include <stdexcept>

void ScoringRule::validate(double p) const {
    if (p <= 0 || p >= 1)
        throw std::domain_error("Probability must be ∈ (0, 1)");

}

double BrierScore::score(double p, int outcome) const {
    validate(p);
    double diff = outcome - p;
    return 1.0 - diff * diff;
}

double LogScore::score(double p, int outcome) const {
    validate(p);
    return outcome * std::log(p) + (1 - outcome) * std::log(1 - p);

}
