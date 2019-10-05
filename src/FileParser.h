#include <iostream>
#include <iterator>
#include "TuringMachine.h"

class FileParser {
    public:
        static TuringMachine createTM(std::istream&, bool);

        static void createTape(std::istream&, std::unordered_set<char>&, std::vector<char>&);
};
