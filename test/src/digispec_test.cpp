#include "digispec/digitokenizer.hpp"
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
      std::cout << "Token: " << token.getValue() << ", Type: " << static_cast<int>(token.getType()) << std::endl;
  }
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

