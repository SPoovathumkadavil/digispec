#include "digispec/digitokenizer.hpp"
#include "digispec/digiparser.hpp"
#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

TEST(LibTests, FileExistsTest)
{
  ASSERT_TRUE(std::filesystem::exists("resources/test.digi"));
}

TEST(LibTests, TokenizeTest)
{
  std::ifstream file("resources/test.digi");
  ASSERT_TRUE(file.is_open());

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  auto tokens = digispec::digi_tokenizer::tokenize(content);
  ASSERT_FALSE(tokens.empty());

  // dump tokens to console
  for (const auto& token : tokens) {
      std::cout << "Token: " << token.value << ", Type: " << static_cast<int>(token.type) << std::endl;
  }
}

TEST(LibTests, ParseTest)
{
  std::ifstream file("resources/test.digi");
  ASSERT_TRUE(file.is_open());

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();

  auto tokens = digispec::digi_tokenizer::tokenize(content);
  ASSERT_FALSE(tokens.empty());

  digispec::digiparser parser;
  auto validation = parser.parse(tokens);
  
  ASSERT_TRUE(validation.valid) << "Parsing failed: " << validation.detail;
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

