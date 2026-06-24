#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
//  Type system
// ─────────────────────────────────────────────────────────────────────────────
enum class SymbolKind {
    Variable,   // id = expr
    Constant,   // lockedIn id = expr
    Function,   // letHimCook id(...)
    Parameter   // function parameter
};

enum class ValueType {
    Int,
    Float,
    String,
    Bool,
    Imaginary,
    Hex,
    Binary,
    Sci,
    Null,       // ghosted
    Unknown     // type couldn't be inferred statically
};

std::string symbol_kind_to_string(SymbolKind k);
std::string value_type_to_string(ValueType t);

// ─────────────────────────────────────────────────────────────────────────────
//  Symbol descriptor
// ─────────────────────────────────────────────────────────────────────────────
struct Symbol {
    std::string name;
    SymbolKind  kind       = SymbolKind::Variable;
    ValueType   type       = ValueType::Unknown;
    int         param_count = 0;  // only meaningful for Function symbols
    int         decl_line  = 0;
    int         decl_col   = 0;
};

// ─────────────────────────────────────────────────────────────────────────────
//  Symbol table — stack of scopes
//
//  Each scope is a hash map from name → Symbol.
//  Lookups walk the stack from innermost to outermost scope.
// ─────────────────────────────────────────────────────────────────────────────
class SymbolTable {
public:
    // Open / close a scope (push/pop the scope stack)
    void push_scope();
    void pop_scope();

    // Declare a symbol in the current (innermost) scope.
    // Returns false if a symbol with the same name already exists
    // in the *current* scope (i.e., a redeclaration error).
    bool declare(const Symbol& s);

    // Look up a symbol by name across all scopes (inner → outer).
    // Returns nullptr if not found.
    Symbol* lookup(const std::string& name);

    // Look up only in the current (innermost) scope.
    // Returns nullptr if not found in the current scope.
    Symbol* lookup_current(const std::string& name);

    // Current nesting depth (0 = no scopes pushed yet)
    int depth() const { return static_cast<int>(m_scopes.size()); }

    // Print the contents of all active scopes to stdout.
    void dump() const;

private:
    std::vector<std::unordered_map<std::string, Symbol>> m_scopes;
};
