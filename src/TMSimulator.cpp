#include <fstream>
#include "TMSimulator.h"
#include "TuringMachine.h"
#include "FileParser.h"
#include "BadFormatError.h"

int main(int argc, char* argv[]) {
    bool nonDeterministic = (argc > 1) && (strcmp(argv[1], "-n") == 0);
    if ((nonDeterministic && argc < 3) || (!nonDeterministic && argc < 2)) {
        std::cerr << "Insufficient arguments." << std::endl;
        std::cout << "input error" << std::endl;
        exit(1);
    } else if ((nonDeterministic && argc > 4) || (!nonDeterministic && argc > 3)) {
        std::cerr << "Too many arguments." << std::endl;
        std::cout << "input error" << std::endl;
        exit(1);
    }
    
    std::string tmFilename;
    std::string tapeFilename("");
    if (nonDeterministic) {
        std::cout << "Running in non-deterministic mode" << std::endl;      //debug
        tmFilename = argv[2];
        if (argc == 4) tapeFilename = argv[3];
    } else {
        tmFilename = argv[1];
        if (argc == 3) tapeFilename = argv[2];
    }

    TuringMachine tm;
    std::vector<char>tape;
    try {
        std::ifstream tmDescription(tmFilename);
        if (!tmDescription.is_open()) throw std::runtime_error("Error opening TM file.");
        else {
            tm = FileParser::createTM(tmDescription, !nonDeterministic);

            if (!tapeFilename.empty()) {
                std::ifstream tapeDescription(tapeFilename);
                if (!tapeDescription.is_open()) throw std::runtime_error("Error opening tape file.");
                else FileParser::createTape(tapeDescription, tm.alphabet, tape);
            } else {
                tape.push_back('_');
            }
        }
    } catch (const BadFormatError& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "input error" << std::endl;
        exit(2);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(3);
    }

    TMSimulator::simulateTM(tm, tape);

    return 0;
}

void TMSimulator::simulateTM(TuringMachine& tm, std::vector<char>& tape) {
    std::string currentState = tm.startState;

    long steps = -1, index = 0;
    Transition transition;
    while (steps < 500000000) {
        if (currentState == tm.acceptState) {
            TMSimulator::terminate(true, steps, tape);
        } else if (currentState == tm.rejectState) {
            TMSimulator::terminate(false, steps, tape);
        }

        if (index == tape.size()) tape.push_back('_');

        // auto start = std::chrono::high_resolution_clock::now();     //debug
        auto tempIter = tm.transitions.find(std::make_pair(currentState, tape[index]));
        if (tempIter == tm.transitions.end()) TMSimulator::terminate(false, steps + 1, tape);
        // std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() << "ns\n";
        transition = tempIter -> second[0];
        // std::cout << currentState << " " << tape[index] << " -> " << transition.nextState << " " << transition.writeOut << " " << (transition.moveLeft ? "L\n" : "R\n");
        currentState = transition.nextState;
        tape[index] = transition.writeOut;
        if (transition.moveLeft) {
            if (index > 0) index--;
        } else index++;

        steps++;
    }

    TMSimulator::terminate(false, steps, tape);
}

void TMSimulator::terminate(bool accepted, long steps, std::vector<char>& tape) {
    std::cout << (accepted ? "" : "not ") << "accepted" << std::endl;
    std::cout << steps << std::endl;
    
    std::string tapeString(tape.begin(), tape.end());
    if (tapeString.find_first_not_of("_") == std::string::npos) tapeString = "_";
    else if (tapeString.back() == '_') {
        tapeString.erase(tapeString.find_last_not_of("_") + 1);
    }
    std::cout << tapeString << std::endl;
    exit(accepted ? 0 : 1);
}

void TMSimulator::simulatNonDetTM(TuringMachine& tm, std::vector<char>& tape) {
    std::vector<NonDetTMState> machines;
    std::string currentState = tm.startState;

    TapeNode root { root, -1, 0 };

    long index = 0;
    machines.push_back(NonDetTMState { tm.startState, index, root });

    long steps = -1;
    std::vector<Transition> transitions;
    char tapeChar;
    while (steps < 500000000) {
        for (NonDetTMState machine : machines) {
            if (machine.currentState == tm.acceptState) {
                TMSimulator::terminate(true, steps, tape);
            } else if (currentState == tm.rejectState) {
                machines.erase(std::find(machines.begin(), machines.end(), machine));
                if (machines.size() == 0) TMSimulator::terminate(false, steps, tape);
            }

            tapeChar = findTapeChar(machine.tapeIndex, machine.tapeNode, tape);

            if (tapeChar == -1) {
                if (machine.tapeIndex >= tape.size()) {
                    machine.tapeNode = { machine.tapeNode, machine.tapeIndex, '_' };
                } else {
                    tapeChar = tape[machine.tapeIndex];
                }
            }

            auto tempIter = tm.transitions.find(std::make_pair(machine.currentState, machine.tapeNode.edit));
            if (tempIter == tm.transitions.end()) {
                machines.erase(std::find(machines.begin(), machines.end(), machine));
                if (machines.size() == 0) TMSimulator::terminate(false, steps + 1, tape);
            }

            transitions = tempIter -> second;
            
            for (auto& transition : transitions) {
                int newIndex;
                if (transition.moveLeft) {
                    if (index > 0) newIndex = machine.tapeIndex - 1;
                } else newIndex = machine.tapeIndex + 1;

                if (transition.writeOut == tapeChar) {
                    machines.push_back(NonDetTMState {
                        transition.nextState,
                        newIndex,
                        machine.tapeNode
                    });
                } else {
                    TapeNode newNode { machine.tapeNode, machine.tapeIndex, transition.writeOut };
                    machines.push_back(NonDetTMState {
                        transition.nextState,
                        newIndex,
                        newNode
                    });
                }
            }
        }

        steps++;
    }
    TMSimulator::terminate(false, steps, tape);
}

char TMSimulator::findTapeChar(long index, TapeNode& node, std::vector<char>& tape) {
    if (node.index == -1) return -1;
    else if (node.index == index) return node.edit;
    else return TMSimulator::findTapeChar(index, node.parent, tape);
}
