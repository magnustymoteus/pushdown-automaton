#include "CFG.h"
#include "utils.h"


bool stringIsInSet(const std::string &str, const std::set<std::string> &set) {
    return set.find(str) != set.end();
}
void cerrAndSetValidity(const bool &condition, bool &validRef, const std::string &message) {
    if(condition) std::cerr << message << std::endl;
    if(validRef && condition) validRef = false;
}


std::string makeVariable(const std::string &from, const std::string &stack, const std::string &to) {
    return "["+from+","+stack+","+to+"]";
}

// uses recursion to get all possible strings made from symbols in alphabet (repetitive permutation)
void permutateSymbols(const int symbolsLeft,
                      const std::set<std::string> &alphabet, std::vector<std::vector<std::string>> &arr,
                      const std::vector<std::string> &str) {
    if(symbolsLeft) {
        for(const std::string &currentStr : alphabet) {
            std::vector<std::string> newStr = str;
            newStr.push_back(currentStr);
            permutateSymbols(symbolsLeft-1, alphabet, arr, newStr);
        }
    }
    else arr.push_back(str);
}
std::set<std::string> getAllVariables(const std::set<std::string> &states,
                                      const std::set<std::string> &stackAlphabet) {
    std::vector<std::vector<std::string>> statePermutationPairs;
    permutateSymbols(2, states, statePermutationPairs);
    std::set<std::string> result = {"S"};
    for(const std::vector<std::string>& statePermutationPair : statePermutationPairs) {
        for(const std::string & currentStackSymbol : stackAlphabet) {
            const std::string &variable =
                    makeVariable(statePermutationPair[0], currentStackSymbol, statePermutationPair[1]);
            result.insert(variable);
        }
    }
    return result;
}