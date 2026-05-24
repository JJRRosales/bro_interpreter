#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <filesystem>

#include "lexer/lexer.h"

#ifdef _WIN32
#include <windows.h>
#endif
// ─────────────────────────────────────────────
//  Version info
// ─────────────────────────────────────────────
static constexpr const char* BRO_VERSION  = "0.1.0";
static constexpr const char* BRO_CODENAME = "ChillDude";

// ─────────────────────────────────────────────
//  Helpers
// ─────────────────────────────────────────────
static void print_banner()
{
    
    std::cout <<
        "\n"
        "  ██████╗ ██████╗  ██████╗ \n"
        "  ██╔══██╗██╔══██╗██╔═══██╗\n"
        "  ██████╔╝██████╔╝██║   ██║\n"
        "  ██╔══██╗██╔══██╗██║   ██║\n"
        "  ██████╔╝██║  ██║╚██████╔╝\n"
        "  ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ \n"
        "\n"
        "  Bro Language v" << BRO_VERSION << " (\"" << BRO_CODENAME << "\")\n"
        "  Trust the process, bro.\n\n";
}

static void print_help(const char* argv0)
{
    print_banner();
    std::cout <<
        "USAGE:\n"
        "  " << argv0 << " <file.bro>            Run a .bro file like the chad you are\n"
        "  " << argv0 << " <file.bro> --verbose  Print tokens so you can feel smart\n"
        "  " << argv0 << " <file.bro> --trust-me Skip the file-extension check (bold move)\n"
        "  " << argv0 << " --no-cap              Print version info, no cap\n"
        "  " << argv0 << " --help                Show this help (you clearly need it)\n"
        "  " << argv0 << " --ghost               Exit immediately\n"
        "\n"
        "FLAGS (can be combined):\n"
        "  --verbose    Dump the token list after lexing\n"
        "  --dump-file  Dump the token list after lexing to a file\n"
        "  --trust-me   Allow any file extension, not just .bro\n"
        "  --dry-run    Lex only, do not execute\n"
        "\n"
        "EXAMPLES:\n"
        "  bro main.bro\n"
        "  bro main.bro --verbose\n"
        "  bro main.bro --dump-file\n"
        "  bro script.txt --trust-me\n"
        "\n";
}

static void print_version()
{
    std::cout << "Bro Language v" << BRO_VERSION
              << " (\"" << BRO_CODENAME << "\") -- no cap.\n";
}

// Returns true if the string ends with ".bro"
static bool has_bro_extension(const std::string& path)
{
    const std::string ext = ".bro";
    if (path.size() < ext.size()) return false;
    return path.compare(path.size() - ext.size(), ext.size(), ext) == 0;
}

// Read entire file into a string
static bool read_file(const std::string& path, std::string& out_source)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::ostringstream buf;
    buf << file.rdbuf();
    out_source = buf.str();
    return true;
}

// ─────────────────────────────────────────────
//  Argument bag
// ─────────────────────────────────────────────
struct Args {
    std::string  file_path;
    bool         verbose  = false;
    bool         dump_file = false;   // dump tokens to a file instead of console
    bool         trust_me = false;   // skip extension check
    bool         dry_run  = false;   // lex only, no execute
    bool         help     = false;
    bool         version  = false;
    bool         ghost    = false;   // exit immediately
};

static bool parse_args(int argc, char* argv[], Args& out)
{
    if (argc < 2) {
        return false; // no arguments at all
    }

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if      (arg == "--help")     { out.help      = true;}
        else if (arg == "--no-cap")   { out.version   = true;}
        else if (arg == "--ghost")    { out.ghost     = true;}
        else if (arg == "--verbose")  { out.verbose   = true;}
        else if (arg == "--dump-file"){ out.dump_file = true;}
        else if (arg == "--trust-me") { out.trust_me  = true;}
        else if (arg == "--dry-run")  { out.dry_run   = true;}
        else if (arg[0] != '-') {
            if (!out.file_path.empty()) {
                std::cerr << "[bro] Bro, one file at a time, chill.\n";
                return false;
            }
            out.file_path = arg;
        } else {
            std::cerr << "[bro] Unknown flag \"" << arg
                      << "\". Try --help for help, bestie.\n";
            return false;
        }
    }
    return true;
}

// ─────────────────────────────────────────────
//  Entry point
// ─────────────────────────────────────────────
int main(int argc, char* argv[])
{
    #ifdef _WIN32
        // Set console output codepage to UTF-8
        SetConsoleOutputCP(65001);
    #endif
    
    // ── Parse arguments ──────────────────────
    Args args;
    if (!parse_args(argc, argv, args)) {
        std::cerr << "Usage: " << argv[0]
                  << " <file.bro> [--verbose] [--trust-me] [--dry-run]\n"
                  << "       " << argv[0]
                  << " --help      (help)\n";
        return 1;
    }

    // ── Instant-exit flags ───────────────────
    if (args.ghost) {
        std::cout << "[bro] You ghosted the interpreter. Respect.\n";
        return 0;
    }
    if (args.version) {
        print_version();
        return 0;
    }
    if (args.help) {
        print_help(argv[0]);
        return 0;
    }

    // ── Need a file ──────────────────────────
    if (args.file_path.empty()) {
        std::cerr << "[bro] Bro... you forgot to give me a file. Seriously.\n";
        print_help(argv[0]);
        return 1;
    }

    // ── Extension check ──────────────────────
    if (!args.trust_me && !has_bro_extension(args.file_path)) {
        std::cerr << "[bro] That file doesn't end in .bro. "
                     "Are you even a bro?\n"
                     "      Use --trust-me to override (at your own risk, bro).\n";
        return 1;
    }

    // ── Read source file ─────────────────────
    std::string source;
    if (!read_file(args.file_path, source)) {
        std::cerr << "[bro] Can't open \"" << args.file_path
                  << "\". Did it ghost you too?\n";
        return 1;
    }

    if (source.empty()) {
        std::cerr << "[bro] The file is empty, bro. "
                     "Even your ideas have more substance than this.\n";
        return 1;
    }

    // ── Lex ──────────────────────────────────
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    // ── Verbose dump ─────────────────────────
    if (args.verbose) {
        std::cout << "[bro] Token dump (you asked for this):\n";
        std::cout << "  ┌─────────────────────────────────────────┐\n";
        for (const auto& tok : tokens) {
            std::cout << "  │  " << tok.to_string() << "\n";
        }
        std::cout << "  └─────────────────────────────────────────┘\n\n";
    }

    // ── file dump ─────────────────────────
    if (args.dump_file) {
        std::filesystem::path path(args.file_path);
        std::string dump_filename = path.stem().string() + "_tokens.txt";
        
        std::ofstream dump_out(dump_filename);
        if (!dump_out.is_open()) {
            std::cerr << "[bro] Couldn't dump tokens to \"" << dump_filename 
                      << "\". Guess the OS is rejecting you too.\n";
            return 1;
        }

        dump_out << "[bro] Token dump for: " << args.file_path << 
        (lexer.had_error()? " (errors encountered)" : " (error free Nice!!)") << "\n";

        dump_out << "┌─────────────────────────────────────────┐\n";
        for (const auto& tok : tokens) {
            dump_out << "│  " << tok.to_string() << "\n";
        }
        dump_out << "└─────────────────────────────────────────┘\n";
        
        std::cout << "[bro] Successfully dumped your tokens to " << dump_filename << "\n";
    }

    if (lexer.had_error()) {
        std::cerr << "[bro] Lexer found issues. Fix your code, bro.\n";
        return 1;
    }

    // ── Dry run ──────────────────────────────
    if (args.dry_run) {
        std::cout << "[bro] Dry run complete. "
                  << tokens.size() << " token(s) produced. Big W.\n";
        return 0;
    }

    // ── Hand off to interpreter (TODO: your code here) ──
    //
    //   Parser parser(tokens);
    //   auto ast = parser.parse();
    //   Interpreter interp;
    //   interp.execute(ast);
    //
    std::cout << "[bro] Interpreter not hooked up yet. "
                 "Lexer ran successfully though, so that's a W.\n";

    return 0;
}
