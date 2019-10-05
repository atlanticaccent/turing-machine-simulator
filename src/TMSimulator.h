#include <vector>
#include "TuringMachine.h"

class TMSimulator {
    public:
    static void simulateTM(TuringMachine&, std::vector<char>&);
    static void terminate(bool, long, std::vector<char>&);
    static void simulatNonDetTM(TuringMachine&, std::vector<char>&);
    static char findTapeChar(long, TapeNode&, std::vector<char>&);
};
