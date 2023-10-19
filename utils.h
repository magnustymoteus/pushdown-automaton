#ifndef PDA_UTILS_H
#define PDA_UTILS_H

#include <iostream>
#include <set>
#include <vector>

std::string makeVariable(const std::string &from, const std::string &stack, const std::string &to);

std::set<std::string> getAllVariables(const std::set<std::string> &states,
                                      const std::set<std::string> &stackAlphabet);
void permutateSymbols(int symbolsLeft,
                      const std::set<std::string> &alphabet, std::vector<std::vector<std::string>> &arr,
                      const std::vector<std::string> &str={});

bool stringIsInSet(const std::string &str, const std::set<std::string> &set);

void cerrAndSetValidity(const bool &condition, bool &validRef, const std::string &message);

#endif //PDA_UTILS_H
