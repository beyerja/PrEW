#include <gtest/gtest.h>

// Inputs from PrEW
#include <CppUtils/Num.h>
#include <Input/CSVMetadata.h>

// Standard library
#include <string>
#include <vector>

using namespace PrEW::CppUtils;
using namespace PrEW::Input;

//------------------------------------------------------------------------------
// Tests for CSVMetadata class

TEST(TestCSVMetadata, TestDataReading) {
  // Try reading the metadata from the test file
  CSVMetadata metadata;
  auto csv_reader = metadata.strip_metadata("../testdata/test.csv");

  // Same file as above without header
  auto csv_reader_check = csv::CSVReader("../testdata/test_without_header.csv");

  ASSERT_EQ(csv_reader.size(), csv_reader_check.size());
  ASSERT_EQ(csv_reader.get_col_names(), csv_reader_check.get_col_names());

  ASSERT_STREQ(metadata.get<std::string>("name").c_str(), "test");
  ASSERT_EQ(metadata.get<int>("energy"), 250);
  ASSERT_EQ(metadata.get<int>("e- chirality"), -1);
  ASSERT_EQ(metadata.get<int>("e+ chirality"), 1);
  ASSERT_TRUE(Num::equal_to_eps(metadata.get<double>("Coef|Test1"), 0.001));
  ASSERT_TRUE(Num::equal_to_eps(metadata.get<double>("Coef|Test2"), -50.5));

  // Check that all available keys were read
  std::vector<std::string> pred_keys{"Coef|Test1",   "Coef|Test2",
                                     "e+ chirality", "e- chirality",
                                     "energy",       "name"};
  std::vector<std::string> pred_coef_keys{"Coef|Test1",   "Coef|Test2"};
  ASSERT_EQ(metadata.keys("all"), pred_keys);
  ASSERT_EQ(metadata.keys("coefs"), pred_coef_keys);
}

//------------------------------------------------------------------------------

TEST(TestCSVMetadata, ThrowExceptionFaultyFiles) {
  std::string non_existant{"./blablabla"};
  std::string not_conform{"../testdata/RK_examplefile_500_250_2018_04_03.root"};

  CSVMetadata metadata;
  ASSERT_THROW(metadata.strip_metadata(non_existant), std::invalid_argument);
  ASSERT_THROW(metadata.strip_metadata(not_conform), std::invalid_argument);
}

//------------------------------------------------------------------------------
