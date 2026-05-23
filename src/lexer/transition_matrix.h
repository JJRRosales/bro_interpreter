#pragma once

#include <vector>
#include <unordered_map>
#include <string>

class TransitionMatrix
{
public:
    TransitionMatrix();  

    int lookup(int state_index, char c) const;

private:
    struct State {
        std::unordered_map<char, int> matches;
        int                           wildcard = -1;
    };
    using RawState = std::unordered_map<std::string, int>;
    using RawMatrix = std::vector<RawState>;

    void preprocess(const RawMatrix& raw);

    std::vector<State> processed_states;
    static const RawMatrix k_raw_matrix;
};