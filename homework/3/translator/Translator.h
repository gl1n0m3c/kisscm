#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <yaml-cpp/yaml.h>

class Translator {
public:
    Translator(std::string inputFile, std::string outputFile);
    void parseInput();
    void generateConfig() const;

private:
    static std::string parseComment(const std::string &comment);
    std::string parseConstant(const std::string &name, const YAML::Node &value);
    std::string parseExpression(const std::string &expression);
    int evaluateExpression(const std::vector<std::string> &tokens);

    // Вспомогательные методы для операций
    int evaluateOperation(const std::string &op, int a, int b = 0);

    // Обработка ошибок
    static void handleError(const std::string &errorMsg);

    std::string inputFile;
    std::string outputFile;
    std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, std::vector<int>> arrays;
    std::vector<std::string> configLines;
};

#endif // TRANSLATOR_H