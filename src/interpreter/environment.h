#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "runtime_value.h"

class Environment {
public:
    Environment(std::shared_ptr<Environment> enclosing = nullptr);

    // Define a new variable in the current scope
    void define(const std::string& name, const RuntimeValue& value);

    // Assign a new value to an existing variable
    // Returns true if successful, false if the variable doesn't exist in any scope
    bool assign(const std::string& name, const RuntimeValue& value);

    // Retrieve the value of a variable
    // Returns true and sets out_value if found, false otherwise
    bool get(const std::string& name, RuntimeValue& out_value) const;

    // Retrieve the enclosing scope
    std::shared_ptr<Environment> get_enclosing() const { return enclosing; }

    // Dump all global variables (useful for final state inspection)
    void dump_globals() const;

private:
    std::unordered_map<std::string, RuntimeValue> values;
    std::shared_ptr<Environment> enclosing;
};
