#include "runtime_value.h"

#include <cmath>
#include <sstream>

std::string RuntimeValue::to_string() const {
    if (is_null()) {
        return "ghosted";
    }
    if (is_int()) {
        return std::to_string(as_int());
    }
    if (is_float()) {
        double val = as_float();
        // Remove trailing zeros for a cleaner output
        std::string str = std::to_string(val);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') {
            str.push_back('0');
        }
        return str;
    }
    if (is_bool()) {
        return as_bool() ? "legit" : "fake";
    }
    if (is_string()) {
        return as_string();
    }
    return "unknown";
}

bool RuntimeValue::is_truthy() const {
    if (is_null()) return false;
    if (is_bool()) return as_bool();
    if (is_int()) return as_int() != 0;
    if (is_float()) return as_float() != 0.0;
    if (is_string()) return !as_string().empty();
    return false;
}
