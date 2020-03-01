#include <Output/Printer.h>

#include <gtest/gtest.h>

using namespace PREW::Output;

//------------------------------------------------------------------------------
// Tests for the Printer output class

TEST(TestPrinter, ExceptionOnInaccessibleFile) {
  ASSERT_THROW(Printer(""), std::invalid_argument);
}

TEST(TestPrinter, TrivialConstructor) {
  // Test trivial constructor on accessible file (but never write to file)
  ASSERT_STREQ(Printer("../build/Makefile").get_current_output().c_str(), "");
}

//------------------------------------------------------------------------------