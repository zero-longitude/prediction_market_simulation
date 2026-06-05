cmake_minimum_required(VERSION 3.15)

project(scoring_rules)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(scoring_rules
    src/main.cpp
    src/scoring_rule.cpp
    src/forecaster.cpp
    src/event.cpp
    src/calibration_tracker.cpp
)

target_include_directories(scoring_rules PRIVATE include)