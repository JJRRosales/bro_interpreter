#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Grammar {
    extern const std::vector<std::vector<std::string>> grammar;

    extern const std::unordered_map<
        std::string,
        std::unordered_map<std::string, int>
    > parsing_table;
}