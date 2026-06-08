#pragma once
#include <string>
#include <sstream>

// public = code inside or outside can access 
// private = only class; not eve subclasses
// protected = the class and it's subclasses
class Forecaster {
    public: 
        Forecaster(std::string name, double initial_belief);
        virtual void update(int outcome) = 0; // = 0 ⟹ pure virtual function; must be implemented by each child class 
        double belief() const; // const after parameters means that we cannot alter non-static class vars
        std::string name() const;
        virtual ~Forecaster() = default;

    protected: // why are name and belief protected variables?
        std::string name_; // trailing _ for class variables to distinguish from local vars
        double belief_;
}; 


class StaticForecaster : public Forecaster {
    public:
        StaticForecaster(std::string name, double belief);
        explicit StaticForecaster(double belief);
        void update(int outcome) override;
};


class BayesianForecaster : public Forecaster {
    public:
        BayesianForecaster(std::string name, double alpha, double beta);
        BayesianForecaster(double alpha, double beta);
        void update(int outcome) override;

    private:
        double alpha_;
        double beta_;
};

class MLEForecaster : public Forecaster {
    public:
        MLEForecaster(std::string name);
        MLEForecaster();
        void update(int outcome) override;

    private:
        int ones_;
        int total_;
};

// helpers to build display names from parameters
inline std::string static_name(double belief) {
    std::ostringstream s;
    s << "Static(" << belief << ")";
    return s.str();
}

inline std::string bayes_name(double alpha, double beta) {
    std::ostringstream s;
    s << "Bayes(" << alpha << "," << beta << ")";
    return s.str();
}