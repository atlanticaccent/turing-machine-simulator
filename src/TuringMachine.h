#ifndef TURING_MACHINE
#define TURING_MACHINE

#include <vector>
#include <unordered_map>
#include <boost/unordered_map.hpp>
#include <unordered_set>

typedef struct Transition {
    std::string nextState;
    char writeOut;
    bool moveLeft;
} Transition;

class TuringMachine {
public:
    std::string startState;
    std::string acceptState;
    std::string rejectState;
    std::unordered_set<char> alphabet;
    boost::unordered_map<std::pair<std::string, char>, std::vector<Transition>> transitions;

    TuringMachine();
};

typedef struct TapeNode {
    struct TapeNode& parent;
    long index;
    char edit;

    inline bool operator==(const TapeNode& other) const {
        return (
            parent == other.parent &&
            index == other.index &&
            edit == other.edit
        );
    }

    inline struct TapeNode& operator=(const TapeNode& other) {
        parent = other.parent;
        index = other.index;
        edit = other.edit;
        return *this;
    }
} TapeNode;

typedef struct NonDetTMState {
    std::string currentState;
    long tapeIndex;
    TapeNode& tapeNode;

    inline bool operator==(const NonDetTMState& other) const {
        return (
            currentState == other.currentState &&
            tapeIndex == other.tapeIndex &&
            tapeNode == other.tapeNode
        );
    }

    inline struct NonDetTMState& operator=(const NonDetTMState& other) {
        currentState = other.currentState;
        tapeIndex = other.tapeIndex;
        tapeNode = other.tapeNode;
        return *this;
    }
} NonDetTMState;

#endif