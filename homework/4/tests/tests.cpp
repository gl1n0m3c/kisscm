#include "Assemble.h"
#include "Interpret.h"
#include <gtest/gtest.h>

TEST(Assemble, Assembler_load) {
  std::ofstream input("test_input.asm");
  input << "LOAD_CONSTANT R3, #825";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0xe2);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x9c);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x01);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x60);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Read) {
  std::ofstream input("test_input.asm");
  input << "MEMORY_READ R2, [742]";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x6d);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x99);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x0b);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_Write) {
  std::ofstream input("test_input.asm");
  input << "MEMORY_WRITE [R5 + 843], R0";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x91);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x2e);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x0d);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x00);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}

TEST(Assemble, Assembler_abs) {
  std::ofstream input("test_input.asm");
  input << "ABS [R2], R3";
  input.close();
  Assemble("test_input.asm", "test_output.bin", "test_log.csv");
  std::ifstream output("test_output.bin");
  unsigned char byte;

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x39);

  output.read(reinterpret_cast<char *>(&byte), sizeof(byte));
  EXPECT_EQ(byte, 0x0d);

  std::remove("test_input.asm");
  std::remove("test_output.bin");
  std::remove("test_log.csv");
}