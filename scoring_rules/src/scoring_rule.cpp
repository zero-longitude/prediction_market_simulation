#include "scoring_rule.h"
#include <cmath>
#include <stdexcept>


double BrierScore::score(double p, int outcome) const {
    if (p < 0.0 || p > 1.0)
        throw std::domain_error("Probability must be within [0, 1]");
    double diff = outcome - p;
    return 1.0 - diff * diff;
}

double LogScore::score(double p, int outcome) const {
    if (p <= 0.0 || p >= 1.0)
        throw std::domain_error("Probability must be within (0, 1)");
    return outcome * std::log(p) + (1 - outcome) * std::log(1 - p);

}
