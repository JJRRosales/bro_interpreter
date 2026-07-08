#pragma once

#include <variant>
#include <string>

// The underlying types for our runtime values
using RuntimeVariant = std::variant<
    std::monostate, // ghosted (null)
    long long,      // V_INT, V_HEX, V_BINARY
    double,         // V_FLOAT, V_SCI
    std::string,    // V_STRING
    bool            // legit / fake
>;

struct RuntimeValue {
    RuntimeVariant value;

    // Constructors
    RuntimeValue() : value(std::monostate{}) {}
    RuntimeValue(long long v) : value(v) {}
    RuntimeValue(double v) : value(v) {}
    RuntimeValue(const std::string& v) : value(v) {}
    RuntimeValue(bool v) : value(v) {}

    // Helpers
    std::string to_string() const;
    bool is_truthy() const;
    
    // Type checkers
    bool is_null() const { return std::holds_alternative<std::monostate>(value); }
    bool is_int() const { return std::holds_alternative<long long>(value); }
    bool is_float() const { return std::holds_alternative<double>(value); }
    bool is_string() const { return std::holds_alternative<std::string>(value); }
    bool is_bool() const { return std::holds_alternative<bool>(value); }

    // Getters
    long long as_int() const { return std::get<long long>(value); }
    double as_float() const { return std::get<double>(value); }
    std::string as_string() const { return std::get<std::string>(value); }
    bool as_bool() const { return std::get<bool>(value); }
};
