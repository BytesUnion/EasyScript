#include <iostream>
#include "Process/Lexer.h"
#include "Process/Interpreter.h"
#include <sys/stat.h>
#include <fstream>

bool fileExists(const std::string& fileName) {
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

int main(int argc, char* argv[]) {
    if (argc < 3 || (std::string(argv[1]) != "lex" && std::string(argv[1]) != "run")) {
        std::cerr << "[Easy++ Error]" << std::endl;
        std::cerr << "-> Usage: easypp <lex|run> <script_file>" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    std::string fileName = argv[2];

    if (!fileExists(fileName)) {
        std::cerr << "[Easy++ Error]" << std::endl;
        std::cerr << "Error: Script file '" << fileName << "' not found." << std::endl;
        return 1;
    }

    try {
        if (command == "lex") {
            std::ifstream inputFile(fileName);
            if (!inputFile) {
                throw std::runtime_error("Error opening file");
            }

            std::string script((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

            Lexer lexer(script);
            std::vector<Token> tokens = lexer.TokenizeAll();

            for (const auto& token : tokens) {
                std::cout << token.ToString() << std::endl;
            }
        }
        else if (command == "run") {
            std::ifstream inputFile(fileName);
            if (!inputFile) {
                throw std::runtime_error("Error opening file");
            }

            std::string script((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

            Interpreter interpreter(fileName);
            interpreter.Execute(script);
            return 0;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "[Easy++ Error]" << std::endl;
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}