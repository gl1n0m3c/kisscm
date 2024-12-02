#include "Translator.h"

Translator::Translator(std::string inputFile, std::string outputFile) :
    inputFile(std::move(inputFile)), outputFile(std::move(outputFile)) {}

void Translator::parseInput() {
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        handleError("Cannot open input file: " + inputFile);
    }

    YAML::Node config;
    try {
        config = YAML::Load(file);
    } catch (const YAML::ParserException &e) {
        handleError("YAML Parsing Error: " + std::string(e.what()));
    }

    // Парсинг комментария
    if (config["comment"]) {
        configLines.push_back(parseComment(config["comment"].as<std::string>()));
    }

    // Парсинг переменных
    if (config["variables"] && config["variables"].IsSequence()) {
        for (const auto &var: config["variables"]) {
            auto name = var["name"].as<std::string>();
            YAML::Node value = var["value"];
            configLines.push_back(parseConstant(name, value));
        }
    }

    // Парсинг выражений
    if (config["expressions"] && config["expressions"].IsSequence()) {
        for (const auto &expr: config["expressions"]) {
            configLines.push_back(parseExpression(expr["expression"].as<std::string>()));
        }
    }
}

void Translator::generateConfig() const {
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        handleError("Cannot open output file: " + outputFile);
    }

    for (const auto &line: configLines) {
        outFile << line << "\n";
    }
    outFile.close();
}

std::string Translator::parseComment(const std::string &comment) { return "' " + comment; }

std::string Translator::parseConstant(const std::string &name, const YAML::Node &value) {
    std::stringstream ss;
    ss << "set " << name << " = ";

    if (value.IsScalar()) {
        int constValue = value.as<int>();
        constants[name] = constValue;
        ss << constValue;
    } else if (value.IsSequence()) {
        std::vector<int> arrayValues;
        ss << "{ ";
        for (size_t i = 0; i < value.size(); ++i) {
            int element = value[i].as<int>();
            arrayValues.push_back(element);
            ss << element;
            if (i < value.size() - 1) {
                ss << ", ";
            }
        }
        arrays[name] = arrayValues;
        ss << " }";
    } else {
        handleError("Unsupported constant value type for variable " + name);
    }

    return ss.str();
}

std::string Translator::parseExpression(const std::string &expression) {
    if (expression.size() < 3 || expression.substr(0, 2) != "@(" || expression.back() != ')') {
        handleError("Invalid expression format: " + expression);
        return "";
    }

    std::string innerExpression = expression.substr(2, expression.size() - 3);

    std::vector<std::string> tokens;
    std::istringstream exprStream(innerExpression);
    std::string token;
    while (exprStream >> token) {
        tokens.push_back(token);
    }

    int result = evaluateExpression(tokens);

    return "@(" + innerExpression + ") = " + std::to_string(result);
}

int Translator::evaluateExpression(const std::vector<std::string> &tokens) {
    std::stack<int> stack;

    for (const auto &token: tokens) {
        if (token == "+") {
            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();
            stack.push(a + b);
        } else if (token == "max") {
            std::vector<int> args;
            while (!stack.empty()) {
                args.push_back(stack.top());
                stack.pop();
            }
            if (args.empty()) {
                handleError("max() requires at least one argument.");
            }
            stack.push(*std::max_element(args.begin(), args.end()));
        } else if (token == "pow") {
            int b = stack.top();
            stack.pop();
            int a = stack.top();
            stack.pop();
            stack.push(static_cast<int>(std::pow(a, b)));
        } else if (constants.count(token) > 0) {
            stack.push(constants[token]);
        } else if (arrays.count(token) > 0) {
            // Для массива: берем максимальное значение массива
            int maxValue = *std::max_element(arrays[token].begin(), arrays[token].end());
            stack.push(maxValue);
        } else {
            try {
                int value = std::stoi(token);
                stack.push(value);
            } catch (...) {
                handleError("Unknown token: " + token);
            }
        }
    }

    if (stack.size() != 1) {
        handleError("Invalid expression.");
    }

    return stack.top();
}

void Translator::handleError(const std::string &errorMsg) {
    std::cerr << "Error: " << errorMsg << std::endl;
    exit(1);
}