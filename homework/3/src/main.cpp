#include <fstream>
#include <iostream>
#include "Translator.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_yaml_file> <output_config_file>\n";
        return 1;
    }
    Translator translator(argv[1], argv[2]);
    translator.parseInput();
    translator.generateConfig();
    return 0;
}
