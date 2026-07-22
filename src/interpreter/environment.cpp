#include "environment.h"
#include <iostream>
#include <iomanip>

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(std::move(enclosing))
{
}

void Environment::define(const std::string& name, const RuntimeValue& value) {
    values[name] = value;
}

bool Environment::assign(const std::string& name, const RuntimeValue& value) {
    if (values.find(name) != values.end()) {
        values[name] = value;
        return true;
    }

    if (enclosing) {
        return enclosing->assign(name, value);
    }

    return false;
}

bool Environment::get(const std::string& name, RuntimeValue& out_value) const {
    auto it = values.find(name);
    if (it != values.end()) {
        out_value = it->second;
        return true;
    }

    if (enclosing) {
        return enclosing->get(name, out_value);
    }

    return false;
}

void Environment::dump_globals() const {
    // If this environment has an enclosing one, it's not the global environment.
    // However, to be safe, we just print the current environment's values.
    // The interpreter will call this specifically on the global environment.
    std::cout << "\n[bro] --- Final Global Variables State ---\n";
    if (values.empty()) {
        std::cout << "  (no global variables)\n";
        return;
    }

    for (const auto& pair : values) {
        std::cout << "  " << std::left << std::setw(15) << pair.first 
                  << " = " << pair.second.to_string() << "\n";
    }
    std::cout << "------------------------------------------\n";
}
