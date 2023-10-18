//
// Created by gruzi on 18/10/2023.
//

#ifndef PDA_PDA_H
#define PDA_PDA_H

#include <set>
#include <string>
#include <stack>
#include <map>
#include <list>

struct PDATransitionDomain {
    std::string from;
    std::string input;
    std::string stackTop;
    PDATransitionDomain(std::string from, std::string input, std::string stackTop) :
    from(std::move(from)), input(std::move(input)), stackTop(std::move(stackTop)) {}
    bool operator<(const PDATransitionDomain& other) const
    {
        return from < other.from;
    }
};

struct PDATransitionImage {
    std::string to;
    std::list<std::string> stackReplacement;
    PDATransitionImage(std::string to, std::list<std::string> stackReplacement) :
    to(std::move(to)), stackReplacement(std::move(stackReplacement)) {}
};

class PDA {
private:
    std::stack<std::string> stack;
public:
    std::set<std::string> states;
    std::set<std::string> alphabet;
    std::set<std::string> stackAlphabet;

    std::multimap<PDATransitionDomain, PDATransitionImage> transitions;

    std::string startState;
    std::string startStack;

    explicit PDA(const std::string &jsonPath);

    bool validate() const;
};


#endif //PDA_PDA_H
