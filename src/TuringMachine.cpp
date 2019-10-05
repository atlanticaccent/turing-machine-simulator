#include <iostream>
#include <iomanip>
#include <boost/functional/hash.hpp>
#include "TuringMachine.h"

TuringMachine::TuringMachine() {
    startState = acceptState = rejectState = "";
    alphabet = std::unordered_set<char>();
    transitions = boost::unordered_map<std::pair<std::string, char>, std::vector<Transition>, boost::hash<std::pair<std::string, char>>>();
}
