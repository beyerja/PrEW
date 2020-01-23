#include <gtest/gtest.h>

#include <Input/DataReader.h>
#include <Input/InfoRKFile.h>

using namespace PREW::Input;

//------------------------------------------------------------------------------
// Tests for DataReader class

TEST(TestDataReader, ExceptionOnWrongFile) {
  /** Check that exception is thrown on invalid file path.
  **/
  InfoRKFile i {"./obviously_wrong_file_path.root", 250};
  ASSERT_THROW(DataReader(&i, "RK"), std::invalid_argument);
}

TEST(TestDataReader, ExceptionOnWrongInputStyle) {
  /** Check that exception is thrown if file exists but input style not known.
  **/
  InfoRKFile i {"../testdata/RK_examplefile_500_250_2018_04_03.root", 250};
  DataReader reader(&i, "cheese");
  ASSERT_THROW(reader.read_file(), std::invalid_argument);
}

TEST(TestDataReader, TestRKFileReading) {
  /** Check that reading of an RK example file runs and gives right number of 
      distributions.
  **/
  InfoRKFile i {"../testdata/RK_examplefile_500_250_2018_04_03.root", 250};
  DataReader reader(&i, "RK");
  reader.read_file();
  auto measured_distributions = reader.get_meas_distrs();
  auto predicted_distributions = reader.get_pred_distrs();
  ASSERT_EQ(measured_distributions.size(), 0); // RK files don't contain measurement
  ASSERT_EQ(predicted_distributions.size(), 24);
}

//------------------------------------------------------------------------------