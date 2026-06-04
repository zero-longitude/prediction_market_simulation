#pragma once
#include <stdexcept>
#include <cmath>


// let p = the true probability of an event happening
// a scoring rule incentivizes truth if, when differentiated w/r/t your guess q
    // ⟹ q = p 
class ScoringRule {
    public: // non-child classes can use this part
        // A pure virtual function is the equivalent of an abstract class; you cannot 
        // instantiate it directly. You can only create child classes which inherit from it
        virtual double score(double p, int outcome) const = 0;
        virtual ~ScoringRule() = default; // declares a destructor function

    protected: // only child classes can use this
        void validate(double p) const;
};


class BrierScore : public ScoringRule {
    public:
        double score(double p, int outcome) const override;
};


class LogScore : public ScoringRule {
    public:
        double score(double p, int outcome) const override;
};
