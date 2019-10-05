#include <limits>
#include <boost/tokenizer.hpp>
#include "FileParser.h"
#include "BadFormatError.h"

TuringMachine FileParser::createTM(std::istream& in, bool deterministic) {
    TuringMachine tm;

    typedef boost::tokenizer<boost::char_separator<char>> tokeniser;
    boost::char_separator<char> whitespace (" \t\r\n");                         // Defines a series of whitespace characters that most instances of the tokeniser should break strings on

    in.ignore(7, ' ');              // Skip the word 'states'
    int totalStates;
    in >> totalStates;              // Extract the number of states from the stream. 
    if (in.fail()) throw BadFormatError("Error reading number of states.");                         // >> and streams don't typlically throw exceptions, so need to explicitly check and throw error.
    if (totalStates <= 0) throw BadFormatError("Number of states is less than or equal to 0.");

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');                                   // Ignore remaining whitespace
    
    // Begin processing state definitions. Store into a hash set for speed compared to a vector, which would require sorting and binary seaches later.
    std::string line;
    std::string stateName;
    std::unordered_set<std::string> states;
    while (std::getline(in, line) && line.compare(0, 8, "alphabet") != 0) {                         // Keep looping until either the stream errors out (ie: EOF, which is handled slightly later if it occurs here) or we reach the alphabet line.
        if (line.length() == 0) continue;                           // Account for empty lines, either between states or the state block and alphabet line
        tokeniser tokens(line, whitespace);                         // Create an iterator for this line
        tokeniser::iterator tokenIterator = tokens.begin();
        stateName = *tokenIterator;

        if (std::distance(tokens.begin(), tokens.end()) > 2) throw BadFormatError("Too many tokens in state description.");
        if (std::distance(tokens.begin(), tokens.end()) == 2) {
            tokenIterator++;
            if (*tokenIterator == "+") {
                if (!tm.acceptState.empty()) throw BadFormatError("TM description contains more than one accept state.");
                else tm.acceptState = stateName;
            } else if (*tokenIterator == "-") {
                if (!tm.rejectState.empty()) throw BadFormatError("TM description contains more than one reject state.");
                else tm.rejectState = stateName;
            } else {
                throw BadFormatError("Bad state description.");
            }
        }

        if (tm.startState.empty()) tm.startState = stateName;               // Set starting state

        if (!states.insert(stateName).second) throw BadFormatError("Duplicate state in states block of TM description.");           // Insert and check succeeded
    }
    if (states.size() == 0) throw BadFormatError("No states provided in description.");
    if (states.size() != totalStates) throw BadFormatError("Stated number of states and number of states provided not equal.");

    // Get alphabet line (can only get here if previous loop exited)
    tokeniser tokens(line, whitespace);
    tokeniser::iterator tokenIterator = tokens.begin();
    int alphabetLength;
    try {
        alphabetLength = std::stoi(*++tokenIterator);               // Extractor increment on iterator to skip word 'alphabet'
    } catch (const std::invalid_argument& e) {
        throw BadFormatError("Alphabet length given was not a valid number.");
    }
    if (alphabetLength <= 0) throw BadFormatError("Alphabet must be finite and cannot be empty.");

    std::string letter;
    for (++tokenIterator; tokenIterator != tokens.end(); ++tokenIterator) {             // Iterate through all tokens generated from the alphabet line
        letter = *tokenIterator;
        if (--alphabetLength < 0) throw BadFormatError("Provided alphabet length does not match number of characters provided.");
        if (letter.size() > 1) throw BadFormatError("Alphabet description contains letters not of size 1. Current syntax does not support larger sizes.");
        if (letter == "_") throw BadFormatError("Alphabet description cannot contain the empty character.");
        if (!tm.alphabet.emplace(letter[0]).second) throw BadFormatError("Alphabet contains duplicate symbols.");
    }
    if (alphabetLength > 0) throw BadFormatError("Provided alphabet length does not match number of letters provided.");            // Insert and check succeeded

    tm.alphabet.emplace('_');                   // Once alphabet successfully parsed, insert empty character into alphabet for purposes of simulation
    
    // Iterate through remaining lines. If stream fails then it is because we have reached EOF (stream is automatically cast to inform us of this)
    while(std::getline(in, line)){
        if (line.length() == 0) continue;

        tokeniser tokens(line, whitespace);
        if (std::distance(tokens.begin(), tokens.end()) != 5) throw BadFormatError("Transition description is malformed.");
        tokeniser::iterator tokenIterator = tokens.begin();

        // Manually shred tokens into string representations. Saves including another tokenisation implementation and constantly standing up/tearind down a vector
        std::string oldState, inputChar, newState, outputChar, direction;
        oldState = *tokenIterator;
        inputChar = *++tokenIterator;
        newState = *++tokenIterator;
        outputChar = *++tokenIterator;
        direction = *++tokenIterator;

        if (states.count(oldState) == 0) throw BadFormatError("Could not find transition starting state.");
        if (states.count(newState) == 0) throw BadFormatError("Could not find transition end state.");
        if (inputChar.length() != 1) throw BadFormatError("Input character is not a single character.");
        if (outputChar.length() != 1) throw BadFormatError("Output character is not a single character.");
        if (tm.alphabet.count(inputChar[0]) == 0) throw BadFormatError("Could not find transition input character in alphabet.");
        if (tm.alphabet.count(outputChar[0]) == 0) throw BadFormatError("Could not find transition output character in alphabet.");
        if (direction[0] != 'L' & direction[0] != 'R') throw BadFormatError("Tape direction for transition is invalid (not 'L' or 'R').");

        // Check transition with this key does not already exist. Likely conflicts with nondeterministic machines and their descriptions
        Transition newTransition { newState, outputChar[0], direction == "L" };
        auto tempIter = tm.transitions.find(std::make_pair(oldState, inputChar[0]));
        if (tempIter == tm.transitions.end()) {
            std::vector<Transition> tempVec;
            tempVec.push_back(newTransition);
            tm.transitions.emplace(std::make_pair(std::make_pair(oldState, inputChar[0]), tempVec));
        } else if (!deterministic) {
            tempIter -> second.push_back(newTransition);
        } else {
            throw BadFormatError("Transition already exists with that input and state combination.");
        }
    }

    return tm;
}

// Parses a tape file from a given file inputstream. Needs a reference to the TM object to access the TM's alphabet for verification.
void FileParser::createTape(std::istream& in, std::unordered_set<char>& alphabet, std::vector<char>& tape) {
    tape = std::vector<char>();

    std::string line;
    while (in >> line) {                        // Continue until EOF - '>>' returns the stream + stream is cast automatically to correct value in case of failure
        for(auto&& tapeChar : line) {
            if (alphabet.count(tapeChar) == 0) throw BadFormatError("Could not find character on tape in TM's alphabet.");
        }
        
        tape.insert(tape.end(), line.begin(), line.end());
    }
}
