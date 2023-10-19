//
// Created by gruzi on 18/10/2023.
//

#include "PDA.h"
#include <fstream>
#include "json.hpp"

#include "utils.h"

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

bool PDA::validate() const {
    bool isValid = true;
    cerrAndSetValidity(!stringIsInSet(startState, states), isValid,
                          "Starting state not in state set!");
    cerrAndSetValidity(!stringIsInSet(startStack, stackAlphabet), isValid,
                          "Starting stack symbol not in stack alphabet!");

    for(const auto &currentTransition : transitions) {
        cerrAndSetValidity(!stringIsInSet(currentTransition.first.from, states), isValid,
                              "There exists a 'from' state in transition that is not in states set!");
        cerrAndSetValidity(!stringIsInSet(currentTransition.first.input, alphabet) &&
        !currentTransition.first.input.empty(),isValid,
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


CFG PDA::toCFG() const {
    // to do: permutate symbols only once by finding the transition with longest stack replacement (optimization)
    std::set<std::string> variables = getAllVariables(states, stackAlphabet);

    std::map<std::string, std::vector<std::vector<std::string>>> productionRules;
    const std::string startSymbol = "S";
    const std::set<std::string> terminals = alphabet;
    // production rules for S
    for(const std::string &currentState : states) {
        const std::string &variable = makeVariable(startState, startStack, currentState);
        productionRules["S"].push_back({variable});
    }

    for(const auto& currentTransition : transitions) {
        const std::vector<std::string> &stackReplacement = currentTransition.second.stackReplacement;
        if(stackReplacement.empty()) {
            const std::string variableName = makeVariable(
                    currentTransition.first.from, currentTransition.first.stackTop, currentTransition.second.to);
            std::vector<std::string> body = {currentTransition.first.input};
            productionRules[variableName].push_back(body);
        }
        else {
            std::vector<std::vector<std::string>> permutations;
            permutateSymbols(static_cast<int>(stackReplacement.size()),
                             states, permutations);

            for(std::vector<std::string> &currentPermutation : permutations) {
                std::vector<std::string> result = {currentTransition.first.input};
                for (int i = 0; i < stackReplacement.size(); i++) {
                    const std::string &prevState = (!i) ? currentTransition.first.from : currentPermutation[i-1];
                    const std::string &variable =
                            makeVariable(prevState, stackReplacement[i], currentPermutation[i]);
                    result.push_back(variable);
                }
                const std::string &head = makeVariable(
                        currentTransition.first.from, currentTransition.first.stackTop, currentPermutation.back());
                productionRules[head].push_back(result);
            }
        }
    }
    return CFG {variables, terminals, productionRules, startSymbol};
}
