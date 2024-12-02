#include <fstream>
#include <gtest/gtest.h>
#include "Translator.h"

class TranslatorTest : public ::testing::Test {
protected:
    // Убедимся, что файлы корректно записываются в рамках каждого теста.
    void SetUp() override {
        // Создаем входной YAML файл
        std::ofstream inputFile("../files/test_in.yaml");
        inputFile << "comment: \"abobis\"\n"
                  << "variables:\n"
                  << "  - name: MY_CONSTANT\n"
                  << "    value: 42\n"
                  << "  - name: ANOTHER_CONSTANT\n"
                  << "    value: [1, 2, 3]\n"
                  << "expressions:\n"
                  << "  - expression: \"@(MY_CONSTANT 1 +)\"\n"
                  << "  - expression: \"@(ANOTHER_CONSTANT max)\"\n";
        inputFile.close();
    }

    // Чтение выходного файла для сравнения
    std::string readOutputFile(const std::string &filename) {
        std::ifstream outputFile(filename);
        std::stringstream buffer;
        buffer << outputFile.rdbuf();
        return buffer.str();
    }
};

// Тестируем трансляцию YAML в конфигурацию
TEST_F(TranslatorTest, TestTranslation) {
    // Инициализация и вызов транслятора
    Translator translator("../files/test_in.yaml", "../files/test_out.txt");
    translator.parseInput();
    translator.generateConfig();
    // Проверка, что выходной файл содержит ожидаемое содержимое
    std::string output = readOutputFile("../files/test_out.txt");

    // Ожидаемое содержимое выходного файла
    std::string expectedOutput =
        "' abobis\n"
        "set MY_CONSTANT = 42\n"
        "set ANOTHER_CONSTANT = { 1, 2, 3 }\n"
        "@(MY_CONSTANT 1 +) = 43\n"
        "@(ANOTHER_CONSTANT max) = 3\n";

    // Сравнение результатов
    EXPECT_EQ(output, expectedOutput);
}