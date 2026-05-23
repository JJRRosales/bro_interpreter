#pragma once
#include <vector>
#include <unordered_map>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
//  TransitionMatrix
//
//  Preprocesses the raw transition table at construction time into three
//  fast structures per state:
//
//    1. exact_map   : char  → next_state          O(1)
//    2. ranges      : {lo, hi, next_state}         O(k), k ≈ 2–3
//    3. wildcard    : next_state or -1             O(1)
//
//  lookup() never allocates and never parses strings at runtime.
// ─────────────────────────────────────────────────────────────────────────────

class TransitionMatrix
{
public:
    TransitionMatrix();   // builds the preprocessed table from raw data

    // Returns the next state index for (state_index, c), or -1 on no match.
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