#include "Interpret.h"

void Interpret::debugOutput() const {
    // Вывод регистров
    std::cout << "Registers: ";
    for (size_t i = 0; i < registers.size(); ++i) {
        std::cout << "R" << i << ":" << registers[i] << " ";
    }
    std::cout << "\nMemory: ";
    // Вывод первых 16 ячеек памяти
    for (size_t i = 0; i < 16; ++i) {
        std::cout << "M" << i << ":" << memory[i] << " ";
    }
    std::cout << "\n\n";
}

template<int N>
std::bitset<N> Interpret::readBitsetFromBinaryFile(std::istream &is) {
    std::bitset<N> code;
    for (int i = 0; i < N / 8; i++) {
        unsigned char byte;
        if (is.read(reinterpret_cast<char *>(&byte), sizeof(byte))) {
            code |= std::bitset<N>(byte) << (i * 8);
        }
    }
    return code;
}

// Функция для декодирования loadConstant из считанного битсета
void Interpret::loadConstantFromCode(const std::bitset<32> code) {
    const unsigned int A = (code >> 0).to_ulong() & 0b1111111; // 7 бит для A
    const unsigned int B = (code >> 7).to_ulong() & ((1 << 22) - 1); // 22 бита для B
    const unsigned int C = (code >> 29).to_ulong() & 0b111; // 3 бита для C
    const int64_t new_B = B <= MAX_SIZE ? static_cast<int>(B) : static_cast<int>(B) - MAX_SIZE * 2;
    if (debug)
        std::cout << "LOAD_CONSTANT: A=" << A << ", B=" << B << ", C=" << C << "\n";

    registers[C] = new_B;
}

// Функция для декодирования memoryRead из считанного битсета
void Interpret::memoryReadFromCode(const std::bitset<40> code) {
    const unsigned int A = (code >> 0).to_ulong() & 0b1111111; // 7 бит для A
    const unsigned int B = (code >> 7).to_ulong() & 0b111; // 3 бита для B
    const unsigned int C = (code >> 10).to_ulong() & ((1 << 27) - 1); // 27 бит для C

    // Проверяем, что адрес в пределах памяти
    if (C < memory.size()) {
        registers[B] = memory[C];
    }

    if (debug)
        std::cout << "MEMORY_READ: A=" << A << ", B=" << B << ", C=" << C << "\n";
}

// Функция для декодирования memoryWrite из считанного битсета
void Interpret::memoryWriteFromCode(const std::bitset<32> code) {
    const unsigned int A = (code >> 0).to_ulong() & 0b1111111; // 7 бит для A
    const unsigned int B = (code >> 7).to_ulong() & 0b111; // 3 бита для B
    const unsigned int C = (code >> 10).to_ulong() & ((1 << 16) - 1); // 16 бит для C (смещение)
    const unsigned int D = (code >> 26).to_ulong() & 0b111; // 3 бита для D

    if (D < registers.size() && (registers[B] + C) < memory.size()) {
        memory[registers[B] + C] = registers[D];
    }

    if (debug)
        std::cout << "MEMORY_WRITE: A=" << A << ", B=" << B << ", C=" << C << ", D=" << D << "\n";
}

// Функция для декодирования abs из считанного битсета
void Interpret::abs(const std::bitset<16> code) {
    const unsigned int A = (code >> 0).to_ulong() & 0b1111111; // 7 бит для A
    const unsigned int B = (code >> 7).to_ulong() & 0b111; // 3 бита для B
    const unsigned int C = (code >> 10).to_ulong() & 0b111; // 3 бита для C
    // Проверка наличия регистров по адресам B и C
    if (C < registers.size() && B < registers.size()) {
        registers[B] = std::abs(memory[registers[C]]);
    }

    if (debug)
        std::cout << "ABSOLUTE_VALUE: A=" << A << ", B=" << B << ", C=" << C << ", Result=" << registers[B] << "\n";
}

Interpret::Interpret(const std::string &binaryFile, const std::string &resultFile, size_t start, size_t end,
                     const bool debug) : debug(debug) {
    std::ifstream infile(binaryFile, std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening binary file." << std::endl;
        return;
    }

    while (infile.peek() != EOF) {
        // Чтение первых 3 бит для определения команды
        std::bitset<8> commandType = readBitsetFromBinaryFile<8>(infile);
        const unsigned int A = commandType.to_ulong() & 0b1111111;

        // Возврат назад на 1 байт для полного считывания команды
        infile.seekg(-1, std::ios::cur);

        try {
            if (A == 98 || A == 17) {
                const std::bitset<32> code = readBitsetFromBinaryFile<32>(infile);
                switch (A) {
                    case 98:
                        loadConstantFromCode(code);
                        break;
                    case 17:
                        memoryWriteFromCode(code);
                        break;
                }
            } else if (A == 109) {
                // Команды на 32 бит (memoryWrite, bitwiseOr и abs)
                const std::bitset<40> code = readBitsetFromBinaryFile<40>(infile);
                memoryReadFromCode(code);
            } else if (A == 57) {
                const std::bitset<16> code = readBitsetFromBinaryFile<16>(infile);
                abs(code);
            } else {
                std::cerr << "Invalid command type: " << A << std::endl;
                return;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error processing command: " << e.what() << std::endl;
        }

        // Отладочный вывод после каждой команды
        if (debug) {
            debugOutput();
        }
    }

    // Сохранение памяти в CSV после выполнения всех команд
  saveMemoryToXML(resultFile, start, end);
}

void Interpret::saveMemoryToXML(const std::string &resultFile, const size_t start, const size_t end)const {
    std::ofstream out(resultFile);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<Memory>\n";
    for (size_t i = start; i <= end && i < memory.size(); ++i) {
        out << "  <Cell id=\"" << i << "\">" << memory[i] << "</Cell>\n";
    }
    out << "</Memory>\n";
}
