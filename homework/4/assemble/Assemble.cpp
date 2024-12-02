#include "Assemble.h"

template<int N>
void Assemble::writeBitsetToBinaryFile(const std::bitset<N> &code) {
    for (int i = 0; i < N / 8; i++) {
        unsigned char byte = (code >> (i * 8)).to_ulong() & 0xFF;
        os.write(reinterpret_cast<const char *>(&byte), sizeof(byte));
    }
}

void Assemble::loadConstant(const unsigned int B, const unsigned int C) {
    std::bitset<32> code;
    constexpr unsigned int A = 98;

    for (int i = 0; i <= 6; i++)
        code[i] = (A >> i) & 1;
    for (int i = 7; i <= 28; i++)
        code[i] = (B >> (i - 7)) & 1;
    for (int i = 29; i <= 31; i++)
        code[i] = (C >> (i - 29)) & 1;

    writeBitsetToBinaryFile<32>(code);
}

void Assemble::memoryRead(const unsigned int B, const unsigned int C) {
    std::bitset<40> code;
    constexpr unsigned int A = 109;

    for (int i = 0; i <= 6; i++)
        code[i] = (A >> i) & 1;
    for (int i = 7; i <= 9; i++)
        code[i] = (B >> (i - 7)) & 1;
    for (int i = 10; i <= 36; i++)
        code[i] = (C >> (i - 10)) & 1;

    writeBitsetToBinaryFile<40>(code);
}

void Assemble::memoryWrite(const unsigned int B, const unsigned int C, const unsigned int D) {
    std::bitset<32> code;
    constexpr unsigned int A = 17;

    for (int i = 0; i <= 6; i++)
        code[i] = (A >> i) & 1;
    for (int i = 7; i <= 9; i++)
        code[i] = (B >> (i - 7)) & 1;
    for (int i = 10; i <= 25; i++)
        code[i] = (C >> (i - 10)) & 1;
    for (int i = 26; i <= 28; i++)
        code[i] = (D >> (i - 26)) & 1;

    writeBitsetToBinaryFile<32>(code);
}

void Assemble::abs(const unsigned int B, const unsigned int C) {
    std::bitset<16> code;
    constexpr unsigned int A = 57;

    for (int i = 0; i <= 6; i++)
        code[i] = (A >> i) & 1;
    for (int i = 7; i <= 9; i++)
        code[i] = (B >> (i - 7)) & 1;
    for (int i = 10; i <= 12; i++)
        code[i] = (C >> (i - 10)) & 1;

    writeBitsetToBinaryFile<16>(code);
}

Assemble::Assemble(const std::string &inputFile, const std::string &outputFile, const std::string &logFile) {
    std::ifstream infile(inputFile);
    os = std::ofstream(outputFile, std::ios::binary);
    std::ofstream logfile(logFile);
    if (!infile || !os || !logfile) {
        logfile << "Error opening files." << std::endl;
        return;
    }
    logfile << "<LOGS>\n";

    std::string input, reg, operand;
    std::string plus, shift;
    unsigned int regAddr, value, addr, offset;

    while (infile >> input) {
        if (input == "LOAD_CONSTANT") {
            try {
                infile >> reg >> operand;
                if (reg[reg.size() - 1] != ',') {
                    throw std::invalid_argument("Missing comma");
                }
                if (reg.empty() || reg[0] != 'R') {
                    throw std::invalid_argument("Invalid register");
                }
                if (operand.empty() || operand[0] != '#') {
                    throw std::invalid_argument("Invalid operand");
                }
                regAddr = std::stoi(reg.substr(1, reg.size() - 1));
                value = std::stoi(operand.substr(1));

                loadConstant(value, regAddr);
                logfile << "<Operation type=\"LOAD_CONSTANT\">\n"
                        << "  <A>98</A>\n"
                        << "  <Constant>" << value << "</Constant>\n"
                        << "  <Register>" << regAddr << "</Register>\n"
                        << "</Operation>\n";
            } catch (std::invalid_argument &e) {
                std::cerr << "Syntax error. Load constant failed. " << inputFile << "\n" << e.what() << std::endl;
                std::cerr << "input: '" << input << "', register: '" << reg << "', operand: '" << operand << "'"
                          << std::endl;
                return;
            } catch (...) {
                std::cerr << "Syntax error. Load constant failed. " << inputFile << std::endl;
                std::cerr << "input: '" << input << "', register: '" << reg << "', operand: '" << operand << "'"
                          << std::endl;
                return;
            }
        } else if (input == "MEMORY_READ") {
            try {
                infile >> reg >> operand;
                if (reg[reg.size() - 1] != ',') {
                    throw std::invalid_argument("Missing comma");
                }
                if (reg.empty() || reg[0] != 'R') {
                    throw std::invalid_argument("Invalid register");
                }
                if (operand.empty() || operand[0] != '[' || operand[operand.size() - 1] != ']') {
                    throw std::invalid_argument("Invalid operand");
                }
                regAddr = std::stoi(reg.substr(1, reg.size() - 1));
                addr = std::stoi(operand.substr(1, operand.size() - 2));

                memoryRead(regAddr, addr);
                logfile << "<Operation type=\"MEMORY_READ\">\n"
                        << "  <A>109</A>\n"
                        << "  <Register>" << regAddr << "</Register>\n"
                        << "  <Address>" << addr << "</Address>\n"
                        << "</Operation>\n";
            } catch (std::invalid_argument &e) {
                std::cerr << "Syntax error. Memory read failed. " << inputFile << "\n" << e.what() << std::endl;
                std::cerr << "input: '" << input << "', register: '" << reg << "', operand: '" << operand << "'"
                          << std::endl;
                return;
            } catch (...) {
                std::cerr << "Syntax error. Memory read failed " << inputFile << std::endl;
                std::cerr << "input: '" << input << "', register: '" << reg << "', operand: '" << operand << "'"
                          << std::endl;
                return;
            }

        } else if (input == "MEMORY_WRITE") {
            try {
                infile >> operand >> plus >> shift >> reg;
                if (shift[shift.size() - 1] != ',') {
                    throw std::invalid_argument("Missing comma");
                }
                if (plus != "+") {
                    throw std::invalid_argument("Missing plus");
                }
                if (reg.empty() || reg[0] != 'R') {
                    throw std::invalid_argument("Invalid register");
                }
                if (operand[0] != '[' || shift[shift.size() - 2] != ']' || operand[1] != 'R') {
                    throw std::invalid_argument("Invalid operand");
                }

                regAddr = std::stoi(operand.substr(2));
                offset = std::stoi(shift.substr(0, shift.size() - 2));
                value = std::stoi(reg.substr(1));

                memoryWrite(regAddr, offset, value);
                logfile << "<Operation type=\"MEMORY_WRITE\">\n"
                        << "  <A>17</A>\n"
                        << "  <Register>" << regAddr << "</Register>\n"
                        << "  <Offset>" << offset << "</Offset>\n"
                        << "  <Value>" << value << "</Value>\n"
                        << "</Operation>\n";
            } catch (std::invalid_argument &e) {
                std::cerr << "Syntax error. Memory write failed. " << inputFile << "\n" << e.what() << std::endl;
                std::cerr << "input: '" << input << "', operand: '" << operand << " " << plus << " " << shift
                          << "', reg: '" << reg << "'" << std::endl;
                return;
            } catch (...) {
                std::cerr << "Syntax error. Memory write failed. " << inputFile << std::endl;
                std::cerr << "input: '" << input << "', operand: '" << operand << " " << plus << " " << shift
                          << "', reg: '" << reg << "'" << std::endl;
            }

        } else if (input == "ABS") {
            try {
                infile >> operand >> reg;
                if (operand[operand.size() - 1] != ',') {
                    throw std::invalid_argument("Missing comma");
                }
                if (reg.empty() || reg[0] != 'R') {
                    throw std::invalid_argument("Invalid register");
                }
                if (operand[0] != '[' || operand[operand.size() - 2] != ']' || operand[1] != 'R') {
                    throw std::invalid_argument("Invalid operand");
                }
                regAddr = std::stoi(reg.substr(1, reg.size() - 1));
                addr = std::stoi(operand.substr(2, operand.size() - 4));

                abs(addr, regAddr);
                logfile << "<Operation type=\"ABS\">\n"
                        << "  <A>57</A>\n"
                        << "  <Address>" << addr << "</Address>\n"
                        << "  <Register>" << regAddr << "</Register>\n"
                        << "</Operation>\n";
            } catch (std::invalid_argument &e) {
                std::cerr << "Syntax error. Abs or failed. " << inputFile << "\n" << e.what() << std::endl;
                std::cerr << "input: '" << input << "', operand: '" << operand << "', reg: '" << reg << "'"
                          << std::endl;
                return;
            } catch (...) {
                std::cerr << "Syntax error. Abs or failed. " << inputFile << std::endl;
                std::cerr << "input: '" << input << "', operand: '" << operand << "', reg: '" << reg << "'"
                          << std::endl;
            }
        }
    }
    logfile << "</LOGS>";
    infile.close();
    logfile.close();
}

Assemble::~Assemble() { os.close(); }
