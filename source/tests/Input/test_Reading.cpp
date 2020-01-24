#include <gtest/gtest.h>

#include <Input/Reading.h>
#include <Input/InputInfo.h>

using namespace PREW::Input;

//------------------------------------------------------------------------------
// Tests specific for functions in Reading

TEST(TestReading, RKReadingExceptionOnWrongInfoType) {
  /** Check that exception is thrown on invalid file path.
  **/
  InputInfo i {"../testdata/RK_examplefile_500_250_2018_04_03.root"}; // path OK
  Data::PredDistrVec *dummy_ptr_1; 
  Data::CoefDistrVec *dummy_ptr_2;
  ASSERT_THROW(
    Reading::read_RK_file(&i, dummy_ptr_1, dummy_ptr_2), 
    std::invalid_argument 
  );
}

//------------------------------------------------------------------------------