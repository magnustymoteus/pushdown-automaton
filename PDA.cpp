//
// Created by gruzi on 18/10/2023.
//

#include "PDA.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

using nlohmann::json;

PDA::PDA(const std::string &jsonPath) {
    std::ifstream input(jsonPath);
    json j = json::parse(input);

    states = j["States"];
    alphabet = j["Alphabet"];
    stackAlphabet = j["StackAlphabet"];
    startState = j["StartState"];
    startStack = j["StartStack"];

    stack.push(startStack);

    for(const json & currentTransition : j["Transitions"]) {
        PDATransitionDomain domain(
                currentTransition["from"], currentTransition["input"], currentTransition["stacktop"]);
        PDATransitionImage image(
                currentTransition["to"], currentTransition["replacement"]
                );
        transitions.insert({domain, image});
    }
    validate();
}

bool stringIsInSet(const std::string &str, const std::set<std::string> &set) {
    return set.find(str) != set.end();
}
void cerrAndSetValidity(const bool &condition, bool &validRef, const std::string &message) {
    if(condition) std::cerr << message << std::endl;
    if(validRef && condition) validRef = false;
}

bool PDA::validate() const {
    bool isValid = true;
    cerrAndSetValidity(!stringIsInSet(startState, states), isValid,
                          "Starting state not in state set!");
    cerrAndSetValidity(!stringIsInSet(startStack, stackAlphabet), isValid,
                          "Starting stack symbol not in stack alphabet!");

    for(const auto &currentTransition : transitions) {
        cerrAndSetValidity(!stringIsInSet(currentTransition.first.from, states), isValid,
                              "There exists a 'from' state in transition that is not in states set!");
        cerrAndSetValidity(!stringIsInSet(currentTransition.first.input, alphabet),isValid,
                              "There exists an input in transition that is not in alphabet!");
        cerrAndSetValidity(!stringIsInSet(currentTransition.first.stackTop, stackAlphabet), isValid,
                              "A stack symbol in transition is not in stack alphabet!");
        cerrAndSetValidity(!stringIsInSet(currentTransition.second.to, states), isValid,
                              "There exists a 'to' state in transition that is not in states set!");
        for(const std::string &currentReplacementSymbol : currentTransition.second.stackReplacement) {
            cerrAndSetValidity(!stringIsInSet(currentReplacementSymbol, stackAlphabet), isValid,
                               "There exists a replacement symbol not in stack alphabet!");
        }
    }

    return isValid;
}
