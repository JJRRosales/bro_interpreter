#include "symbol_table.h"
#include <iostream>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
//  String helpers
// ─────────────────────────────────────────────────────────────────────────────
std::string symbol_kind_to_string(SymbolKind k) {
    switch (k) {
        case SymbolKind::Variable:  return "var";
        case SymbolKind::Constant:  return "const";
        case SymbolKind::Function:  return "func";
        case SymbolKind::Parameter: return "param";
        default:                    return "?";
    }
}

std::string value_type_to_string(ValueType t) {
    switch (t) {
        case ValueType::Int:       return "int";
        case ValueType::Float:     return "float";
        case ValueType::String:    return "string";
        case ValueType::Bool:      return "bool";
        case ValueType::Imaginary: return "imaginary";
        case ValueType::Hex:       return "hex";
        case ValueType::Binary:    return "binary";
        case ValueType::Sci:       return "sci";
        case ValueType::Null:      return "null";
        case ValueType::Unknown:   return "unknown";
        default:                   return "?";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  SymbolTable implementation
// ─────────────────────────────────────────────────────────────────────────────
void SymbolTable::push_scope() {
    m_scopes.emplace_back();
}

void SymbolTable::pop_scope() {
    if (!m_scopes.empty()) m_scopes.pop_back();
}

bool SymbolTable::declare(const Symbol& s) {
    if (m_scopes.empty()) {
        throw std::logic_error("SymbolTable::declare called with no active scope");
    }
    auto& current = m_scopes.back();
    if (current.count(s.name)) return false; // already declared in this scope
    current[s.name] = s;
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) return &found->second;
    }
    return nullptr;
}

Symbol* SymbolTable::lookup_current(const std::string& name) {
    if (m_scopes.empty()) return nullptr;
    auto& current = m_scopes.back();
    auto found = current.find(name);
    return (found != current.end()) ? &found->second : nullptr;
}

void SymbolTable::dump() const {
    std::cout << "\n  ┌──────────────────────────────────────────────────────────┐\n";
    std::cout <<   "  │                    SYMBOL TABLE                          │\n";
    std::cout <<   "  ├──────────────────────────────────────────────────────────┤\n";

    if (m_scopes.empty()) {
        std::cout << "  │  (empty)                                                 │\n";
    } else {
        for (int d = 0; d < (int)m_scopes.size(); ++d) {
            const auto& scope = m_scopes[d];
            std::string scope_label = (d == 0) ? "Global scope" : "Scope depth " + std::to_string(d);
            std::cout << "  │  [" << scope_label << "]\n";

            if (scope.empty()) {
                std::cout << "  │    (no symbols)\n";
            } else {
                for (const auto& [name, sym] : scope) {
                    std::string kind_str = symbol_kind_to_string(sym.kind);
                    std::string type_str = value_type_to_string(sym.type);
                    std::string extra = "";
                    if (sym.kind == SymbolKind::Function) {
                        extra = "  (" + std::to_string(sym.param_count) + " params)";
                    }
                    std::cout << "  │    " << name
                              << "  [" << kind_str << " / " << type_str << "]"
                              << extra
                              << "  (line " << sym.decl_line << ")\n";
                }
            }
        }
    }

    std::cout << "  └──────────────────────────────────────────────────────────┘\n\n";
}
